//DSPE_2012706067_이동민_4주차
//< Image Upsampling >
// Interpolation method : Copy method, Average method, Interpolation Upsampling filter(8-tap interpolation filter 사용)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  256
#define HEIGHT  256

#define Padding_Scale 2

#define Clip(x) x<0?0:(x>255?255:x) // x<0이면 0, x>0일때 x>255이면 x=255 아니면 x값으로 대입 // Clipping 함수를 함수 매크로로 정의

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

	img_out_copy = MemAlloc_2D(WIDTH * Padding_Scale, HEIGHT * Padding_Scale);
	img_out_ave = MemAlloc_2D(WIDTH * Padding_Scale, HEIGHT * Padding_Scale);
	img_out_filter = MemAlloc_2D(WIDTH * Padding_Scale, HEIGHT * Padding_Scale);

	upsampling_copy(img_in, img_out_copy, WIDTH, HEIGHT);
	upsampling_ave(img_in, img_out_ave, WIDTH, HEIGHT);
	upsampling_filter(img_in, img_out_filter, WIDTH, HEIGHT);



	FileWrite("[upsampling_copy]Lena(512x512).raw", img_out_copy, WIDTH * Padding_Scale, HEIGHT * Padding_Scale);
	FileWrite("[upsampling_ave]Lena(512x512).raw", img_out_ave, WIDTH * Padding_Scale, HEIGHT * Padding_Scale);
	FileWrite("[upsampling_filter]Lena(512x512).raw", img_out_filter, WIDTH * Padding_Scale, HEIGHT * Padding_Scale);


	printf("Copy method PSNR : %.2F dB \n\n", GetPSNR(img_out_filter, img_out_copy, WIDTH * Padding_Scale, HEIGHT * Padding_Scale));
	printf("Average method PSNR : %.2F dB \n\n", GetPSNR(img_out_filter, img_out_ave, WIDTH * Padding_Scale, HEIGHT * Padding_Scale));

	MemFree_2D(img_in, HEIGHT); //메모리 free
	MemFree_2D(img_out_copy, HEIGHT * Padding_Scale);
	MemFree_2D(img_out_ave, HEIGHT * Padding_Scale);
	MemFree_2D(img_out_filter, HEIGHT * Padding_Scale);



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
			for (m = 0; m < Padding_Scale; m++) {
				for (n = 0; n < Padding_Scale; n++) {
					img_out[Padding_Scale * i + m][Padding_Scale * j + n] = img_in[i][j];
				}
			}
		}
	}

}

void upsampling_ave(BYTE** img_in, BYTE** img_out, int width, int height)
{
	BYTE** img_in_padding = MemAlloc_2D(width + 1, height + 1);
	BYTE** img_out_padding = MemAlloc_2D((width + 1) * Padding_Scale, (height + 1) * Padding_Scale);

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
	for (i = 0; i < height * Padding_Scale; i++)
	{
		for (j = 0; j < width * Padding_Scale; j++)
		{
			img_out[i][j] = img_out_padding[i][j];
		}
	}


	MemFree_2D(img_in_padding, height + 1);
	MemFree_2D(img_out_padding, (height + 1) * Padding_Scale);

}


void upsampling_filter(BYTE** img_in, BYTE** img_out, int width, int height)
{
	int interpol_filter[8] = { -1, 4, -11, 40, 40, -11, 4, -1 };
	int sum = 64;
	float temp;

	int i, j, m;

	BYTE** img_in_padding = MemAlloc_2D(width + 7, height + 7); // 원본 이미지를 처리하기위한 패딩된 메모리 공간 할당 
	BYTE** img_out_padding = MemAlloc_2D((width + 7) * Padding_Scale, (height + 7) * Padding_Scale); // 필터적용을 위한 패딩 및 2배로 이미지 확장하여 할당.


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


	//padding image upsampling
	for (i = 0; i < height + 7; i++)
	{
		for (j = 0; j < width + 7; j++)
		{
			img_out_padding[i * Padding_Scale][j * Padding_Scale] = img_in_padding[i][j]; // 패딩된 이미지를 2배로 확장한 메모리공간에 저장.
		}
	}



	//interpolation 세로방향으로
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height + 7; i++)
		{
			temp = 0;
			for (m = 0; m < 8; m++)
			{
				temp += (interpol_filter[m] * img_out_padding[i * Padding_Scale][j * Padding_Scale + m * Padding_Scale]) / sum;
			}

			img_out_padding[i * Padding_Scale][j * Padding_Scale + 7] = temp;
		}
	}



	//interpolation 가로방향으로
	for (i = 0; i < height; i++)
	{
		for (j = 6; j < width * Padding_Scale + 6; j++)
		{
			temp = 0;
			for (m = 0; m < 8; m++)
			{
				temp += (interpol_filter[m] * img_out_padding[i * Padding_Scale + m * Padding_Scale][j]) / sum;
			}

			img_out_padding[i * Padding_Scale + 7][j] = temp;
		}
	}


	//패딩된 부분을 버리고 이미지 원 사이즈를 취함
	for (i = 0; i < height * Padding_Scale; i++)
	{
		for (j = 0; j < width * Padding_Scale; j++)
		{
			img_out[i][j] = img_out_padding[i + 6][j + 6];
		}
	}



	MemFree_2D(img_in_padding, height + 7);
	MemFree_2D(img_out_padding, (height + 7) * Padding_Scale);
}

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height) //PSNR을 취하기위한 계산 함수
// PSNR : 최대 신호 대 잡음비(Peak Signal-to-noise ratio, PSNR)는 신호가 가질 수 있는 최대 전력에 대한 잡음의 전력
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

	PSNR = 10.0 * log10(pow(255, 2) / m); // pow(255,2)는 이미지에서 가지는 (최대값-최소값) = MaxI, m(MSE) : 평균제곱오차.
										  // 무손실 영상의 경우 MSE가 0이기 때문에 PSNR이 정의되지 않음.
										  // PSNR 값이 클수록 손상된 이미지가 품질이좋다. 이미지손상이 덜하다는 의미.

	return PSNR;
}