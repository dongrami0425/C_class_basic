//DSPE_2012706067_이동민_4
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  256
#define HEIGHT  256

#define Clip(x) x<0?0:(x>255?255:x)

typedef unsigned char BYTE;

BYTE** MemAlloc_2D(int width, int height);  //메모리 할당을 함수로 선언
void MemFree_2D(unsigned char** arr, int height);  // 메모리 free 함수 선언

void FileRead(char* filename, BYTE** img_in, int width, int height); // file 읽어오기 함수
void FileWrite(char* filename, BYTE** img_out, int width, int height);// file write 함수

void upsampling_copy(BYTE** img_in, BYTE** img_out, int width, int height); //interpolation method - copy
void upsampling_ave(BYTE** img_in, BYTE** img_out, int width, int height); //interpolation method - average
void upsampling_filter(BYTE** img_in, BYTE** img_out, int width, int height); //interpolation method - interpolation filter

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height); // PSNR 계산 함수



int main()
{

	BYTE **img_in = 0, **img_out_copy = 0, **img_out_ave = 0, **img_out_filter = 0;
	   
	img_in = MemAlloc_2D(WIDTH, HEIGHT);
	FileRead("Lena(256x256).raw", img_in, WIDTH, HEIGHT);

	img_out_copy = MemAlloc_2D(WIDTH * 2, HEIGHT * 2);
	img_out_ave = MemAlloc_2D(WIDTH * 2, HEIGHT * 2);
	img_out_filter = MemAlloc_2D(WIDTH * 2, HEIGHT * 2);

	upsampling_copy(img_in, img_out_copy, WIDTH, HEIGHT);
	upsampling_ave(img_in, img_out_ave, WIDTH, HEIGHT);
	upsampling_filter(img_in, img_out_filter, WIDTH, HEIGHT);



	FileWrite("[upsampling_copy]Lena(512x512).raw", img_out_copy, WIDTH * 2, HEIGHT * 2);
	FileWrite("[upsampling_ave]Lena(512x512).raw", img_out_ave, WIDTH * 2, HEIGHT * 2);
	FileWrite("[upsampling_filter]Lena(512x512).raw", img_out_filter, WIDTH * 2, HEIGHT * 2);


	printf("Copy method PSNR : %.2F dB \n\n", GetPSNR(img_out_filter, img_out_copy, WIDTH * 2, HEIGHT * 2));
	printf("Average method PSNR : %.2F dB \n\n", GetPSNR(img_out_filter, img_out_ave, WIDTH * 2, HEIGHT * 2));

	MemFree_2D(img_in, HEIGHT); //메모리 free
	MemFree_2D(img_out_copy, HEIGHT * 2);
	MemFree_2D(img_out_ave, HEIGHT * 2);
	MemFree_2D(img_out_filter, HEIGHT * 2);



	return 0;
}


unsigned char** MemAlloc_2D(int width, int height) //메모리 2D할당 함수
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


void MemFree_2D(unsigned char** arr, int height) // 메모리 free함수
{
	int i = 0;

	for (i = 0; i < height; i++)
		free(arr[i]);
	free(arr);
}

void FileRead(char* filename, BYTE** img_in, int width, int height) // 파일을 읽어 img_in을 fp_in에 저장
{
	FILE *fp_in;
	int i;

	fp_in = fopen(filename, "rb");
	for (i = 0; i < height; i++)
		fread(img_in[i], sizeof(BYTE), width, fp_in);
	fclose(fp_in);
}

void FileWrite(char* filename, BYTE** img_out, int width, int height) // 파일을 할당된 저장소에 fp_out에 저장
{
	FILE *fp_out;
	int i;

	fp_out = fopen(filename, "wb");
	for (i = 0; i < height; i++)
		fwrite(img_out[i], sizeof(BYTE), width, fp_out);
	fclose(fp_out);
}


void upsampling_copy(BYTE** img_in, BYTE** img_out, int width, int height)
{
	int i, j, m, n;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (m = 0; m < 2; m++) {
				for (n = 0; n < 2; n++) {
					img_out[2 * i + m][2 * j + n] = img_in[i][j];
				}
			}
		}
	}

}

void upsampling_ave(BYTE** img_in, BYTE** img_out, int width, int height)
{
	BYTE** img_in_padding = MemAlloc_2D(width + 1, height + 1);
	BYTE** img_out_padding = MemAlloc_2D((width + 1) * 2, (height + 1) * 2);

	int i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_in_padding[i][j] = img_in[i][j];
		}
	}
	//Padding
	for (i = 0; i < height; i++)
	{
		img_in_padding[i][width] = img_in[i][width - 1];
	}

	for (j = 0; j < width + 1; j++)
	{
		img_in_padding[height][j] = img_in_padding[height - 1][j];
	}


	//average
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_out_padding[i * 2][j * 2] = img_in_padding[i][j];

			img_out_padding[i * 2][j * 2 + 1] = (img_in_padding[i][j] + img_in_padding[i][j + 1]) / 2;
			img_out_padding[i * 2 + 1][j * 2] = (img_in_padding[i][j] + img_in_padding[i + 1][j]) / 2;
			img_out_padding[i * 2 + 1][j * 2 + 1] = (img_in_padding[i][j] + img_in_padding[i + 1][j + 1]) / 2;
		}
	}


	//패딩된 부분을 버리고 이미지 원 사이즈를 취함
	for (i = 0; i < height * 2; i++)
	{
		for (j = 0; j < width * 2; j++)
		{
			img_out[i][j] = img_out_padding[i][j];
		}
	}


	MemFree_2D(img_in_padding, height + 1);
	MemFree_2D(img_out_padding, (height + 1) * 2);

}


void upsampling_filter(BYTE** img_in, BYTE** img_out, int width, int height)
{
	int interpol_filter[8] = { -1, 4, -11, 40, 40, -11, 4, -1 };
	int sum = 64;
	float temp;

	int i, j, m;

	BYTE** img_in_padding = MemAlloc_2D(width + 7, height + 7);
	BYTE** img_out_padding = MemAlloc_2D((width + 7) * 2, (height + 7) * 2);


	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img_in_padding[i + 3][j + 3] = img_in[i][j];
		}
	}

	//img_in padding
	for (i = 3; i < height + 3; i++)
	{
		for (m = 0; m < 3; m++)
		{
			img_in_padding[i][m] = img_in_padding[i][3];
		}
	}

	for (i = 3; i < height + 3; i++)
	{
		for (m = 0; m < 4; m++)
		{
			img_in_padding[i][width + 3 + m] = img_in_padding[i][width - 1 + 3];
		}
	}

	for (j = 0; j < width + 7; j++)
	{
		for (m = 0; m < 3; m++)
		{
			img_in_padding[m][j] = img_in_padding[3][j];
		}
	}

	for (j = 0; j < width + 7; j++)
	{
		for (m = 0; m < 4; m++)
		{
			img_in_padding[height + 3 + m][j] = img_in_padding[height - 1 + 3][j];
		}
	}


	//upsampling
	for (i = 0; i < height + 7; i++)
	{
		for (j = 0; j < width + 7; j++)
		{
			img_out_padding[i * 2][j * 2] = img_in_padding[i][j];
		}
	}



	//interpolation
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height + 7; i++)
		{
			temp = 0;
			for (m = 0; m < 8; m++)
			{
				temp += (interpol_filter[m] * img_out_padding[i * 2][j * 2 + m * 2]) / sum;
			}

			img_out_padding[i * 2][j * 2 + 7] = temp;
		}
	}




	for (i = 0; i < height; i++)
	{
		for (j = 6; j < width * 2 + 6; j++)
		{
			temp = 0;
			for (m = 0; m < 8; m++)
			{
				temp += (interpol_filter[m] * img_out_padding[i * 2 + m * 2][j]) / sum;
			}

			img_out_padding[i * 2 + 7][j] = temp;
		}
	}


	//패딩된 부분을 버리고 이미지 원 사이즈를 취함
	for (i = 0; i < height * 2; i++)
	{
		for (j = 0; j < width * 2; j++)
		{
			img_out[i][j] = img_out_padding[i + 6][j + 6];
		}
	}



	MemFree_2D(img_in_padding, height + 7);
	MemFree_2D(img_out_padding, (height + 7) * 2);
}

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height) //PSNR을 취하기위한 계산 함수
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
			m += s / (float)(width * height);
		}
	}

	PSNR = 10.0 * log10(pow(255, 2) / m);

	return PSNR;
}