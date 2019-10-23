//DSPE_2012706067_이동민_5

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define WIDTH 512
#define HEIGHT 512
#define SWAP(a,b)  {int t; t = a; a=b; b=t;}


typedef unsigned char BYTE;

unsigned char** MemAlloc_2D(int width, int height);
void MemFree_2D(BYTE** arr, int height);

void FileRead(char* filename, BYTE** img_in, int width, int height);
void FileWrite(char* filename, BYTE** img_out, int width, int height);

void MedianMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height);

void ArithmeticMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height);

float GetPSNR(unsigned char** img_ori, unsigned char** img_dist, int width, int height);

int main()
{
	BYTE **img_ori, **img_in_Gaus, **img_in_SP, **img_res;

	img_ori = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_Gaus = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_SP = MemAlloc_2D(WIDTH, HEIGHT);

	FileRead("Lena(512x512).raw", img_ori, WIDTH, HEIGHT);
	FileRead("[G]Lena(512x512).raw", img_in_Gaus, WIDTH, HEIGHT);
	FileRead("[S&P]Lena(512x512).raw", img_in_SP, WIDTH, HEIGHT);

	img_res = MemAlloc_2D(WIDTH, HEIGHT);

	/////////////////arithmetic mean filtering(3x3, 5x5, 7x7, 9x9) on Gaussian noise ///////////////////
	printf("Gaussian noise PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_in_Gaus, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_Gaus, img_res, 3, WIDTH, HEIGHT); FileWrite("[AMF_3x3_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("3x3 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_Gaus, img_res, 5, WIDTH, HEIGHT); FileWrite("[AMF_5x5_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("5x5 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_Gaus, img_res, 7, WIDTH, HEIGHT); FileWrite("[AMF_7x7_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("7x7 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_Gaus, img_res, 9, WIDTH, HEIGHT); FileWrite("[AMF_9x9_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("9x9 arithmetic mean filter PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	
	/////////////////median mean filtering(3x3, 5x5, 7x7, 9x9) on Gaussian noise ///////////////////
	MedianMeanFilter(img_in_Gaus, img_res, 3, WIDTH, HEIGHT); FileWrite("[MM_3x3_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("3x3 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_Gaus, img_res, 5, WIDTH, HEIGHT); FileWrite("[MM_5x5_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("5x5 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_Gaus, img_res, 7, WIDTH, HEIGHT); FileWrite("[MM_7x7_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("7x7 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_Gaus, img_res, 9, WIDTH, HEIGHT); FileWrite("[MM_9x9_GN]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("9x9 median mean filter PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
	

	/////////////////arithmetic mean filtering(3x3, 5x5, 7x7, 9x9) on salt&pepper noise ///////////////////
	printf("Salt&Pepper noise PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_in_SP, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_SP, img_res, 3, WIDTH, HEIGHT); FileWrite("[AMF_3x3_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("3x3 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_SP, img_res, 5, WIDTH, HEIGHT); FileWrite("[AMF_5x5_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("5x5 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_SP, img_res, 7, WIDTH, HEIGHT); FileWrite("[AMF_7x7_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("7x7 arithmetic mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	ArithmeticMeanFilter(img_in_SP, img_res, 9, WIDTH, HEIGHT); FileWrite("[AMF_9x9_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("9x9 arithmetic mean filter PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
	/////////////////median mean filtering(3x3, 5x5, 7x7, 9x9) on salt&pepper noise ///////////////////
	

	
	//////////////////////median mean filtering (3x3, 5x5, 7x7, 9x9) on salt&pepper noise
	MedianMeanFilter(img_in_SP, img_res, 3, WIDTH, HEIGHT); FileWrite("[MM_3x3_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("3x3 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_SP, img_res, 5, WIDTH, HEIGHT); FileWrite("[MM_5x5_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("5x5 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_SP, img_res, 7, WIDTH, HEIGHT); FileWrite("[MM_7x7_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("7x7 median mean filter PSNR : %.2f dB\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));

	MedianMeanFilter(img_in_SP, img_res, 9, WIDTH, HEIGHT); FileWrite("[MM_9x9_SP]Lena(512x512).raw", img_res, WIDTH, HEIGHT);
	printf("9x9 median mean filter PSNR : %.2f dB\n\n", GetPSNR(img_ori, img_res, WIDTH, HEIGHT));
	

	//2D memory free
	MemFree_2D(img_ori, HEIGHT);
	MemFree_2D(img_in_Gaus, HEIGHT);
	MemFree_2D(img_in_SP, HEIGHT);
	MemFree_2D(img_res, HEIGHT);

	return 0;
}

void ArithmeticMeanFilter(unsigned char** img_in, unsigned char** img_out, int mask_size, int width, int height)
{
	int padding = mask_size / 2;
	float temp;
	int h, w, i, j;

	BYTE** img_in_padding = MemAlloc_2D(width + 2*padding, height + 2*padding);


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
	int idx , s;
	int h, w, i, j, n;

	float temp = 0;
	BYTE** img_in_padding;

	float* data = (float*)malloc(sizeof(float)*mask_size*mask_size);

	img_in_padding = MemAlloc_2D(width + 2*padding, height + 2*padding);
	
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

	

	


	for (i = 0; i < height; i++) { //spatial filtering : 2-D median mean filtering nxn
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
			for ( idx = 0; idx < n  ; idx++) // sorting 버블정렬 오름차순 계산
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