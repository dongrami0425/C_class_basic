//DSPE_2012706067_이동민_11
// < Color Conversion >
// - Color Conversion [RGB to YUV] or [YUV to RGB]
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define WIDTH 352
#define HEIGHT 288

#define Clip(x) (x<0?0:(x>255?255:x))

typedef unsigned char BYTE;

BYTE** MemAlloc_2D(int width, int height); // in : RGB, out : YUV, RGB
void MemFree_2D(BYTE** arr, int height);

int Read_Frame(FILE* fp_in, BYTE** img_in, int width, int height);
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height);
void RGB_to_YUV(BYTE** img_in, BYTE** img_out, int width, int height);
void YUV_to_RGB(BYTE** img_in, BYTE** img_out, int width, int height);

int main()
{
	FILE *fp_in = fopen("Suzie_CIF_150_30.rgb", "rb"); //in file
	FILE *fp_out1 = fopen("[YUV]Suzie_CIF_150_30.yuv", "wb"); //output yuv file
	FILE *fp_out2 = fopen("[RGB]Suzie_CIF_150_30.rgb", "wb"); //output rgb file

	BYTE **img_out, **img_in; //in:RGB, out:YUV,RGB
	int size = 1; //loop condition

	img_out = MemAlloc_2D(WIDTH, HEIGHT * 3); // YUV memory
	img_in = MemAlloc_2D(WIDTH, HEIGHT * 3);  // RGB memory

	while (size = Read_Frame(fp_in, img_in, WIDTH, HEIGHT * 3)) // LOOP
	{
		RGB_to_YUV(img_in, img_out, WIDTH, HEIGHT);
		Write_Frame(fp_out1, img_out, WIDTH, HEIGHT * 3);

		YUV_to_RGB(img_out, img_in, WIDTH, HEIGHT);
		Write_Frame(fp_out2, img_in, WIDTH, HEIGHT * 3);
				
	}

	

	MemFree_2D(img_out, HEIGHT * 3);
	MemFree_2D(img_in, HEIGHT * 3);

	fcloseall();

	return 0;
}

BYTE** MemAlloc_2D(int width, int height)
{
	BYTE** arr;
	int i;

	arr = (BYTE**)malloc(sizeof(BYTE*)*height);
	for (i = 0; i < height; i++)
	{
		arr[i] = (BYTE*)malloc(sizeof(BYTE)*width);
	}
	return arr;
}
void MemFree_2D(BYTE** arr, int height) //2D MEMORY FREE
{
	int i;

	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

//1 frame read from input file
int Read_Frame(FILE *fp_in, BYTE** img_in, int width, int height)
{
	int i, size = 0;

	for (i = 0; i < height; i++)
	{
		size += fread(img_in[i], sizeof(BYTE), width, fp_in); // accumulate the reading size
	}

	return size;
}

// 1 frame write on output file
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height)
{
	int i, size = 0;

	for (i = 0; i < height; i++)
	{
		fwrite(img_in[i], sizeof(BYTE), width, fp_out); // write on the output file
	}
}



void RGB_to_YUV(BYTE** img_in, BYTE** img_out, int width, int height) // RGB파일을 YUV값으로 변환하여 저장하는 함수
{
	int i, j;
	int Y, U, V;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) // 변환식 PDF참조
		{
			Y = ((66 * img_in[i][j] + 129 * img_in[i + height][j] + 25 * img_in[i + 2 * height][j] + 128) >> 8) + 16;
			U = ((-38 * img_in[i][j] - 74 * img_in[i + height][j] + 112 * img_in[i + 2 * height][j] + 128) >> 8) + 128;
			V = ((112 * img_in[i][j] - 94 * img_in[i + height][j] - 18 * img_in[i + 2 * height][j] + 128) >> 8) + 128;

			img_out[i][j] = Y;
			img_out[i + height][j] = U;
			img_out[i + 2 * height][j] = V;
		}
	}

}

void YUV_to_RGB(BYTE** img_in, BYTE** img_out, int width, int height) // YUV파일을 RGB값으로 변환하여 저장하는 함수
{
	int i, j;
	int C, D, E;
	int R, G, B;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++) // 변환식 PDF참조
		{
			C = img_in[i][j] - 16;
			D = img_in[i + height][j] - 128;
			E = img_in[i + 2 * height][j] - 128;

			R = Clip(((298 * C + 409 * E + 128) >> 8)); // Clipping 과정
			G = Clip(((298 * C - 100 * D - 208 * E + 128) >> 8));
			B = Clip(((298 * C + 516 * D + 128) >> 8));

			img_out[i][j] = R;
			img_out[i + height][j] = G;
			img_out[i + 2 * height][j] = B;

		}
	}

}