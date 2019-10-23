//DSPE_2012706067_이동민_6

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define WIDTH 512
#define HEIGHT 512


typedef unsigned char BYTE;

unsigned char** MemAlloc_2D(int width, int height); //2D memory allocation
void MemFree_2D(BYTE** arr, int height);			//2D memory free
void FileRead(char* filename, unsigned char** img_in, int width, int height); // read data from a file
void FileWrite(char* filename, unsigned char** img_out, int width, int height); // write data to a file


void MedianMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height);					//MedianMeanFilter

void ArithmeticMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height);				//ArithmeticMeanFilter

void ContraHarmonicMeanFilter(unsigned char** img_in, unsigned char** img_out,int mask_size, float Q, int width, int height);	//ContraHarmonicMeanFilter

void AlphaTrimmedMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int d, int width, int height);		//AlpahTrimmedMeanFilter

float GetPSNR(unsigned char** img_ori, unsigned char** img_dist, int width, int height);										// PSNR 계산

int main() {

	BYTE **img_ori, **img_noise, **img_res;
	float Q;
	int d;
	char filename_out[100];
	img_ori = MemAlloc_2D(WIDTH, HEIGHT);
	img_noise = MemAlloc_2D(WIDTH, HEIGHT);
	FileRead("Lena(512x512).raw", img_ori, WIDTH, HEIGHT);
	FileRead("[Noise]Lena(512x512).raw", img_noise, WIDTH, HEIGHT);
	

	img_res = MemAlloc_2D(WIDTH, HEIGHT);

	ArithmeticMeanFilter(img_noise, img_res, 5, WIDTH, HEIGHT); //ArithmeticMeanFilter filtering
	printf("5x5 arithmetic mean filter PSNR : %.2f dB\n\n\n",GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
	sprintf(filename_out, "[AMF_5x5]Lena(512x512).raw");
	FileWrite(filename_out, img_res, WIDTH, HEIGHT);                  //output file write

	

	
	MedianMeanFilter(img_noise, img_res, 5, WIDTH, HEIGHT); //MedianMeanFilter filtering
	printf("5x5 median mean filter PSNR : %.2f dB\n\n\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
	sprintf(filename_out, "[MMF_5x5]Lena(512x512).raw");
	FileWrite(filename_out, img_res, WIDTH, HEIGHT);                  //output file write

	

	///////////////// ContraHarmonicMeanFilter(5x5) ///////////////////
	
	for (Q = -2; Q <= 2; Q++) {
		ContraHarmonicMeanFilter(img_noise, img_res, 5, Q, WIDTH, HEIGHT); //contraharmonicmean filtering
		printf("5x5 ContraHarmonic Mean filter(Q = %.1f) PSNR : %.2f dB\n", Q, GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
		sprintf(filename_out, "[CHMF_5x5_%.1f]Lena(512x512).raw", Q);
		FileWrite(filename_out, img_res, WIDTH, HEIGHT);                  //output file write

	}
	printf("\n\n");

	///////////////// AlphatrimmedMeanFilter(5x5) ///////////////////

	for (d = 0; d <= 24; d+=2) {
		AlphaTrimmedMeanFilter(img_noise, img_res, 5, d, WIDTH, HEIGHT); //AlphaTrimmedMean filtering
		printf("5x5 AlphaTrimmed Mean filter(d = %d) PSNR : %.2f dB\n", d, GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
		sprintf(filename_out, "[ATMF_5x5_%d]Lena(512x512).raw", d);
		FileWrite(filename_out, img_res, WIDTH, HEIGHT);                  //output file write

	}
	

	//2D memory free
	MemFree_2D(img_ori, HEIGHT);
	MemFree_2D(img_noise, HEIGHT);
	MemFree_2D(img_res, HEIGHT);

	return 0;
}


void ArithmeticMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height)
{
	int padding = mask_size / 2;
	float temp;
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


	for (i = 0; i < height; i++) { //spatial filtering : 2-D arithmetic mean filtering nxn
		for (j = 0; j < width; j++) {
			temp = 0;
			for (h = 0; h < mask_size; h++) {
				for (w = 0; w < mask_size; w++) {
					temp += img_in_padding[i + h][j + w];
				}
			}

			img_out[i][j] = (unsigned char)floor(temp / (mask_size*mask_size));
		}
	}


	MemFree_2D(img_in_padding, height + 2 * padding);

}


void MedianMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height)
{
	int padding = mask_size / 2;
	int idx, s;
	int h, w, i, j, n;

	float temp = 0;
	BYTE** img_in_padding;

	float* data = (float*)malloc(sizeof(float)*mask_size*mask_size);

	img_in_padding = MemAlloc_2D(width + 2 * padding, height + 2 * padding);

	for (i = 0; i < height; i++)
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






	for (i = 0; i < height; i++) { //spatial filtering : 2-D arithmetic mean filtering nxn
		for (j = 0; j < width; j++) {
			temp = 0;
			n = 0;

			for (h = 0; h < mask_size; h++) {  //1-D recorering
				for (w = 0; w < mask_size; w++) {
					data[n] = img_in_padding[i + h][j + w];
					n += 1;
				}
			}
			n = n - 1;
			for (idx = 0; idx < n; idx++) // sorting 버블정렬 오름차순 계산
			{
				for (s = 1; s < n + 1; s++)
				{
					if (data[idx] > data[s]) {
						temp = data[idx];
						data[idx] = data[s];
						data[s] = temp;
					}
				}
			}

			img_out[i][j] = data[(mask_size * mask_size) / 2];
		}
	}

	free(data);
	MemFree_2D(img_in_padding, height + 2 * padding);

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

void AlphaTrimmedMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int d, int width, int height)
{
	int padding = mask_size / 2;
	int idx, s, k;
	int h, w, i, j, n;
	int D = d;
	float temp1 = 0;
	float temp2 = 0;
	
	float* data = (float*)malloc(sizeof(float)*mask_size*mask_size);

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


	//alphatrimmedmean filtering
	for (i = 0; i < height; i++) 
	{ 
		for (j = 0; j < width; j++) 
		{
			temp1 = 0;
			temp2 = 0;
			n = 0;

			for (h = 0; h < mask_size; h++)
			{  //1-D recodering
				for (w = 0; w < mask_size; w++) {
					data[n] = img_in_padding[i + h][j + w];
					n += 1;
				}
			}
			n = n - 1; // n=25 -> 24
			for (idx = 0; idx < n; idx++) // sorting 버블정렬 오름차순 계산
			{
				for (s = 1; s < n + 1; s++)
				{
					if (data[idx] > data[s])
					{
						temp1 = data[idx];
						data[idx] = data[s];
						data[s] = temp1;
					}
				}
			}
			
			for ( k = D/2; k <= n-(D/2); k++) //D=4일때 => k=2 ~ 22 까지 data[2~22]값을 더함, data[0],data[1],data[23]data[24]빠짐
			{
				temp2 += data[k];
			}
			img_out[i][j] = (unsigned char)(temp2/((mask_size*mask_size)-D)); // (sumdata/mn-d)  값 계산
			
		}
	}
	
	free(data);
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