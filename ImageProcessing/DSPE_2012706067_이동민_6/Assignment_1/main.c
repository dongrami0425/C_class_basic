//DSPE_2012706067_¿ÃµøπŒ_6

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define WIDTH 512
#define HEIGHT 512


typedef unsigned char BYTE;

unsigned char** MemAlloc_2D(int width, int height);
void MemFree_2D(BYTE** arr, int height);
void FileRead(char* filename, BYTE** img_in, int width, int height);
void FileWrite(char* filename, BYTE** img_out, int width, int height);



void ContraHarmonicMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, float Q, int width, int height);
float GetPSNR(unsigned char** img_ori, unsigned char** img_dist, int width, int height);

int main() {
	
	BYTE **img_ori, **img_in_S, **img_in_P, **img_res;
	float Q;
	char filename_out[100];

	img_ori = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_S = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_P = MemAlloc_2D(WIDTH, HEIGHT);

	FileRead("Lena(512x512).raw", img_ori, WIDTH, HEIGHT);
	FileRead("[Salt_Noise]Lena(512x512).raw", img_in_S, WIDTH, HEIGHT);
	FileRead("[Pepper_Noise]Lena(512x512).raw", img_in_P, WIDTH, HEIGHT);

	img_res = MemAlloc_2D(WIDTH, HEIGHT);

	///////////////// ContraHarmonicMeanFilter(3x3) on Salt noise ///////////////////
	printf("Salt noise PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_in_S, WIDTH, HEIGHT));

	for (Q = -2; Q <= 2; Q++) {
		ContraHarmonicMeanFilter(img_in_S, img_res, 3, Q, WIDTH, HEIGHT); //contraharmonicmean filtering
		printf("3x3 ContraHarmonic Mean filter(Q = %.1f) PSNR : %.2f dB\n", Q, GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
		sprintf(filename_out, "[Salt_CHMF_3x3_%.1f]Lena(512x512).raw", Q);
		FileWrite(filename_out, img_res, WIDTH, HEIGHT);                  //output file write

	}
	printf("\n\n");

	///////////////// ContraHarmonicMeanFilter(3x3) on pepper noise ///////////////////
	printf("Pepper noise PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_in_S, WIDTH, HEIGHT));

	for (Q = -2; Q <= 2; Q++) {
		ContraHarmonicMeanFilter(img_in_P, img_res, 3, Q, WIDTH, HEIGHT); //contraharmonicmean filtering
		printf("3x3 ContraHarmonic Mean filter(Q = %.1f) PSNR : %.2f dB\n", Q, GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
		sprintf(filename_out, "[Pepper_CHMF_3x3_%.1f]Lena(512x512).raw", Q);
		FileWrite(filename_out, img_res, WIDTH, HEIGHT);                 //output file write

	}
	printf("\n\n");


	//2D memory free
	MemFree_2D(img_ori, HEIGHT);
	MemFree_2D(img_in_P, HEIGHT);
	MemFree_2D(img_in_S, HEIGHT);
	MemFree_2D(img_res, HEIGHT);

	return 0;
}

void ContraHarmonicMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, float Q, int width, int height)
{
	int padding = mask_size / 2;
	float temp1, temp2;
	float q = Q;
	int h, w, i, j;

	BYTE** img_in_padding = MemAlloc_2D(width + 2 * padding, height + 2 * padding);


	for (i = 0; i < height; i++) //Data copy
	{
		for (j = 0; j < width; j++)
		{
			img_in_padding[i + padding][j + padding] = img_in[i][j];
		}
	}

	//padding
	for (i = padding; i < height + padding; i++) {
		for (j = 0; j < padding; j++) {
			img_in_padding[i][j] = img_in_padding[i][padding];
			img_in_padding[i][width + padding + j] = img_in_padding[i][width + padding - 1];
		}
	}

	for (j = 0; j < width + padding * 2; j++) {
		for (i = 0; i < padding; i++) {
			img_in_padding[i][j] = img_in_padding[padding][j];
			img_in_padding[height + padding + i][j] = img_in_padding[height + padding - 1][j];
		}
	}


	
	for (i = 0; i < height; i++) { ////contraharmonicmean filtering
		for (j = 0; j < width; j++) {
			temp1 = 0;
			temp2 = 0;
			for (h = 0; h < mask_size; h++) {
				for (w = 0; w < mask_size; w++) {
					
						temp1 += pow(img_in_padding[i + h][j + w], q);
						temp2 += pow(img_in_padding[i + h][j + w], q + 1.0);
					
				}
			}			
			img_out[i][j] = (unsigned char)(temp2 / temp1);				
		}
	}



	MemFree_2D(img_in_padding, height + 2 * padding);

}

unsigned char** MemAlloc_2D(int width, int height)
{

	BYTE **arr = 0;
	int i;

	arr = (BYTE **)malloc(sizeof(BYTE*) * (height));
	for (i = 0; i < height; i++)
	{
		arr[i] = (BYTE *)malloc(sizeof(BYTE) * (width));
	}

	return arr;
}

void MemFree_2D(BYTE** arr, int height)
{
	for (int i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

void FileRead(char* filename, BYTE** img_in, int width, int height)
{
	FILE *fp_in;
	int i;
	fp_in = fopen(filename, "rb");
	for (i = 0; i < height; i++)
	{
		fread(img_in[i], sizeof(BYTE), width, fp_in);
	}
	fclose(fp_in);
}

void FileWrite(char* filename, BYTE** img_out, int width, int height)
{
	FILE *fp_out;
	int i;

	fp_out = fopen(filename, "wb");
	for (i = 0; i < height; i++)
	{
		fwrite(img_out[i], sizeof(BYTE), width, fp_out);
	}
	fclose(fp_out);
}

float GetPSNR(unsigned char** img_ori, unsigned char** img_dist, int width, int height)
{

	int i, j;

	float s = 0, temp = 0;
	float m = 0;
	float PSNR;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{

			temp = img_ori[i][j] - img_dist[i][j];
			s = (float)pow(temp, 2);
			m += s / (float)(height * width);
		}
	}

	PSNR = 10.0 * log10(pow(255, 2) / m);

	return PSNR;
}