//DSPE_2012706067_¿ÃµøπŒ_14
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hdr_function.h"
#include "define.h"

unsigned int halfToFloat(uint16 value)
{
	int sign = (value >> 15) & 0x00000001;
	int exponent = (value >> 10) & 0x0000001f;
	int significand = value & 0x000003ff;

	// First handle all special cases (e.g. if exponent is 0 or 31)
	if (exponent == 0) {
		if (significand == 0) {
			return (sign << 31);
		}
		else {
			while (!(significand & 0x00000400)) {
				significand <<= 1;
				exponent -= 1;
			}
			significand &= ~0x00000400;
			exponent += 1;
		}
	}
	else if (exponent == 31) {
		if (significand == 0) {
			return ((sign << 31) | 0x7f800000);
		}
		else {
			return ((sign << 31) | 0x7f800000 | (significand << 13));
		}
	}

	// consider exponent bias (127 for single, 15 for half => 127 - 15 = 112)
	exponent += 112;
	// extend significand precision from 10 to 23 bits
	significand <<= 13;

	// Gather sign, exponent, and significand and reconstruct the number as single precision.
	return ((sign << 31) | (exponent << 23) | significand);
}

float* HDRread(const char* name)
{
	char buf[2000];
	FILE *fp_in;
	
	int num, cCount, y_coord, data_size;
	const int Frame_size = WIDTH * HEIGHT * CHANNEL;
	
	uint16 *rawdata = (uint16*)malloc(sizeof(uint16) * Frame_size * 3);
	uint32  *R, *G, *B;
	uint64 offset_table[1080];

	static float *out;

	out = (float*)malloc(sizeof(float) * Frame_size * 3);
	R = (uint32*)out;
	G = (uint32*)&out[WIDTH * HEIGHT];
	B = (uint32*)&out[2 * WIDTH * HEIGHT];

	fp_in = fopen(name, "rb");
	fread(&num, sizeof(int), 1, fp_in);
	fread(&num, sizeof(int), 1, fp_in);
	
	while (1) {
		cCount = 0;
		while (1) {
			fread(&buf[cCount], sizeof(char), 1, fp_in);
			cCount++;
			if (buf[cCount - 1] == 0) break;
		}

		if (cCount == 1) break;

		// read attribute type
		cCount = 0;
		while (1) {
			fread(&buf[cCount], sizeof(char), 1, fp_in);
			cCount++;
			if (buf[cCount - 1] == 0) break;
		}

		// read attribute size
		int attributeSize = 0;
		fread(&attributeSize, sizeof(int), 1, fp_in);
		fread(&buf[0], attributeSize, 1, fp_in);
	}
	for (int i = 0; i < 1080; i++)
	{
		fread(&offset_table[i], sizeof(uint64), 1, fp_in);
	}

	//fseek(fp_in, 8953, SEEK_SET);
	for (int h = 0; h < HEIGHT; h++)
	{
		fseek(fp_in, offset_table[h], SEEK_SET);
		fread(&y_coord, sizeof(int), 1, fp_in);
		fread(&data_size, sizeof(int), 1, fp_in);

		fread(&rawdata[h*WIDTH * 3], data_size, 1, fp_in);
	}

	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
			B[h*WIDTH + w] = halfToFloat(rawdata[h*WIDTH * 3 + w]);

		for (int w = 0; w < WIDTH; w++)
			G[h*WIDTH + w] = halfToFloat(rawdata[h*WIDTH * 3 + w + WIDTH]);

		for (int w = 0; w < WIDTH; w++)
			R[h*WIDTH + w] = halfToFloat(rawdata[h*WIDTH * 3 + w + (WIDTH * 2)]);
	}
	return out;
}

void convertLight2Video(float* src, uint16* dst, const char *mode)
{
	//Quantization factors
	double weight = (1 << (BITDEPTH - 8)) * 219.0;
	double offset = (1 << (BITDEPTH - 8)) * 16.0;
	int    maxPelValue = (1 << BITDEPTH) - 1;

	if (mode[0] == 'H')
	{
		//HDR Transfer function factor
		const double m1 = (2610.0) / (4096.0 * 4.0);
		const double m2 = (2523.0 * 128.0) / 4096.0;
		const double c1 = (3424.0) / 4096.0;
		const double c2 = (2413.0 *  32.0) / 4096.0;
		const double c3 = (2392.0 *  32.0) / 4096.0;
		const double m_normalFactor = HDR_MAX - HDR_MIN;

		int frame_size = HEIGHT*WIDTH*CHANNEL;
		for (int i = 0; i < frame_size; i++)
		{
			double tempValue;

			//Normalization
			tempValue = ((double)src[i] - HDR_MIN) / m_normalFactor;
			tempValue = (tempValue < 0.0) ? 0.0 : (tempValue > 1.0 ? 1.0 : (tempValue));

			//Transfer function(PQ)
			tempValue = pow(tempValue, m1);
			tempValue = (pow(((c2 *(tempValue)+c1) / (1.0 + c3 *(tempValue))), m2));

			//Quantization(float to integer)
			dst[i] = (uint16)(weight * tempValue + offset);
		}

	}
	else
	{
		//SDR function factor
		const double m_gamma = 2.4;
		const double m_inverseGamma = 1.0 / m_gamma;
		const double m_alpha = 0.055;
		const double m_beta  = 0.0031308;
		const double m_normalFactor = SDR_MAX - SDR_MIN;

		int frame_size = HEIGHT*WIDTH*CHANNEL;
		for (int i = 0; i < frame_size; i++)
		{
			double tempValue;

			//Normalization
			tempValue = (double)(src[i] - SDR_MIN) / m_normalFactor;
			tempValue = (tempValue < 0.0) ? 0.0 : (tempValue > 1.0 ? 1.0 : (tempValue)); // Normalized Linear light signal V

			//Transfer function(sRGB)
			
			/////////////////////////////
			/////   Assignment#1    /////
			///////////////////////////// 
			if (tempValue <= 0.0031308) {
				tempValue = 12.92 * tempValue; //save Video signal L

			}

			if (tempValue > 0.0031308){
				tempValue = pow(((1 + 0.055)*tempValue), 1/2.4) - 0.055; //save Video signal L
			}


			//Quantization
			dst[i] = (uint16)(weight *tempValue +offset);
		}
	}
}