//DSPE_2012706067_이동민_12
// < Video Subsampling(Chroma subsampling) >
// - RGB to YUV Conversion
// - YUV 444,422, 420 conversion
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //헤더파일 선언
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define WIDTH 352 //CIF 규격
#define HEIGHT 288

#define Clip(x) (x<0?0:(x>255?255:x)) //클리핑

typedef unsigned char BYTE; //unsigned char -> BYTE

BYTE** MemAlloc_2D(int width, int height); // in : RGB, out : YUV, RGB
void MemFree_2D(BYTE** arr, int height); //동적 메모리 할당 해제

int Read_Frame(FILE *fp_in, BYTE** img_in, int width, int height);  //in file
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height); //output yuv file
void RGB_to_YUV(BYTE** img_in, BYTE** img_out, int width, int height); //image color conversion RGB to YUV
void YUV_to_RGB(BYTE** img_in, BYTE** img_out, int width, int height); //image coloe consersion YUV to RGB

void YUV444_to_420(BYTE** img_in, BYTE** img_Y, BYTE** img_U420, BYTE** img_V420, int width, int height); //Chroma sampling 4:4:4 -> 4:2:0
void YUV420_to_444(BYTE** img_Y, BYTE** img_U420, BYTE** img_V420, BYTE** img_out, int width, int height); //Chroma sampling 4:2:0 -> 4:4:4
void YUV444_to_422(BYTE** img_in, BYTE** img_Y, BYTE** img_U422, BYTE** img_V422, int width, int height); //Chroma sampling 4:4:4: -> 4:2:2
void YUV422_to_444(BYTE** img_Y, BYTE** img_U422, BYTE** img_V422, BYTE** img_out, int width, int height); //Chroma sampling 4:2:2 -> 4:4:4

int main()
{
	FILE *fp_in = fopen("Suzie_CIF_150_30.rgb", "rb"); //in RGB file
	FILE *fp_out0 = fopen("[YUV444]Suzie_CIF_150_30.yuv", "wb"); //output yuv 444 file
	FILE *fp_out1 = fopen("[YUV420]Suzie_CIF_150_30.yuv", "wb"); //output yuv 420 file
	FILE *fp_out2 = fopen("[YUV422]Suzie_CIF_150_30.yuv", "wb"); //output yuv 422 file
	FILE *fp_out3 = fopen("[Recon_420]Suzie_CIF_150_30.rgb", "wb"); //recon RGB file
	FILE *fp_out4 = fopen("[Recon_422]Suzie_CIF_150_30.rgb", "wb"); //recon RGB file

	BYTE **img_YUV444, **img_RGB; //in : RGB444 out: YUV444,YUV420, YUV422, recon RGB
	BYTE **img_Y, **img_U420, **img_V420; //420 memory pointer
	BYTE **img_U422, **img_V422; // 422 memory pointer

	int size = 1; //loop condition

	img_YUV444 = MemAlloc_2D(WIDTH, HEIGHT * 3); //YUV444 memory
	img_RGB = MemAlloc_2D(WIDTH, HEIGHT * 3); //RGB memory

	//YUV 420 ememory
	img_Y = MemAlloc_2D(WIDTH, HEIGHT);
	img_U420 = MemAlloc_2D(WIDTH >> 1, HEIGHT >> 1);
	img_V420 = MemAlloc_2D(WIDTH >> 1, HEIGHT >> 1);

	//YUV 422 ememory
	img_U422 = MemAlloc_2D(WIDTH >> 1, HEIGHT);
	img_V422 = MemAlloc_2D(WIDTH >> 1, HEIGHT);

	/////////Processing Loop////////////

	while (size = Read_Frame(fp_in, img_RGB, WIDTH, HEIGHT * 3))
	{
		RGB_to_YUV(img_RGB, img_YUV444, WIDTH, HEIGHT);
		Write_Frame(fp_out0, img_YUV444, WIDTH, HEIGHT * 3);

		YUV444_to_420(img_YUV444, img_Y, img_U420, img_V420, WIDTH, HEIGHT);
		YUV444_to_422(img_YUV444, img_Y, img_U422, img_V422, WIDTH, HEIGHT);

		Write_Frame(fp_out1, img_Y, WIDTH, HEIGHT);
		Write_Frame(fp_out1, img_U420, WIDTH >> 1, HEIGHT >> 1);
		Write_Frame(fp_out1, img_V420, WIDTH >> 1, HEIGHT >> 1);

		Write_Frame(fp_out2, img_Y, WIDTH, HEIGHT);
		Write_Frame(fp_out2, img_U422, WIDTH >> 1, HEIGHT);
		Write_Frame(fp_out2, img_V422, WIDTH >> 1, HEIGHT);

		YUV420_to_444(img_Y, img_U420, img_V420, img_YUV444, WIDTH, HEIGHT);
		YUV_to_RGB(img_YUV444, img_RGB, WIDTH, HEIGHT);
		Write_Frame(fp_out3, img_RGB, WIDTH, HEIGHT * 3);

		YUV422_to_444(img_Y, img_U422, img_V422, img_YUV444, WIDTH, HEIGHT);
		YUV_to_RGB(img_YUV444, img_RGB, WIDTH, HEIGHT);
		Write_Frame(fp_out4, img_RGB, WIDTH, HEIGHT * 3);
	}

	MemFree_2D(img_YUV444, HEIGHT * 3);
	MemFree_2D(img_RGB, HEIGHT * 3);

	MemFree_2D(img_Y, HEIGHT);
	MemFree_2D(img_U420, HEIGHT >> 1);
	MemFree_2D(img_V420, HEIGHT >> 1);

	MemFree_2D(img_U422, HEIGHT);
	MemFree_2D(img_V422, HEIGHT);

	fcloseall();

	return 0;
}

BYTE ** MemAlloc_2D(int width, int height)
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

void MemFree_2D(BYTE ** arr, int height)
{
	int i;
	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

int Read_Frame(FILE * fp_in, BYTE ** img_in, int width, int height)
{
	int i, size = 0;
	for (i = 0; i < height; i++)
	{
		size += fread(img_in[i], sizeof(BYTE), width, fp_in);
	}
	return size;
}

void Write_Frame(FILE * fp_out, BYTE ** img_in, int width, int height)
{
	int i;
	for (i = 0; i < height; i++)
	{
		fwrite(img_in[i], sizeof(BYTE), width, fp_out);
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

void YUV444_to_420(BYTE ** img_in, BYTE ** img_Y, BYTE ** img_U420, BYTE ** img_V420, int width, int height) // YUV 4:4:4 => YUV 4:2:0 Subsampling
{
	int i, j;

	for (i = 0; i < height; i++) //YUV중 Y(4개 픽셀값)를 4개의 픽셀에 그대로 저장한다.
	{
		for (j = 0; j < width; j++)
		{
			img_Y[i][j] = img_in[i][j];
		}
	}

	for (i = 0; i < height / 2; i++)
	{
		for (j = 0; j < width / 2; j++)
		{

			//YUV중 U(4개 픽셀 값)를 1개의 픽셀로 저장한다. 이때 2개는 버리고 2개를 평균하여 저장한다.(PDF참조)
			img_U420[i][j] = (img_in[i + height][j * 2] + img_in[i * 2 + height][j * 2]) / 2;
			
			//YUV중 V(4개 픽셀 값)를 1개의 픽셀로 저장한다. 이때 2개는 버리고 2개를 평균하여 저장한다.(PDF참조)
			img_V420[i][j] = (img_in[i + 2 * height][j * 2] + img_in[i * 2 + 2 * height][j * 2]) / 2;
		}
	}

}

void YUV420_to_444(BYTE ** img_Y, BYTE ** img_U420, BYTE ** img_V420, BYTE ** img_out, int width, int height)  // YUV 4:4:4 <= YUV 4:2:0 Copy the neighboring pixel
{
	int i, j;

	for (i = 0; i < height; i++) //YUV중 Y(4개 픽셀값)를 4개의 픽셀에 그대로 저장한다.
	{
		for (j = 0; j < width; j++)
		{
			img_out[i][j] = img_Y[i][j];
		}
	}

	for (i = 0; i < height / 2; i++)
	{
		for (j = 0; j < width / 2; j++)
		{
			//YUV중 U(1개 픽셀 값)를 4개의 픽셀에 카피하여 저장한다.
			img_out[(i * 2) + height][j * 2] = img_U420[i][j]; 
			img_out[(i * 2) + 1 + height][j * 2] = img_U420[i][j];
			img_out[(i * 2) + height][(j * 2) + 1] = img_U420[i][j];
			img_out[(i * 2) + 1 + height][(j * 2) + 1] = img_U420[i][j];

			//YUV중 V(1개 픽셀 값)를 4개의 픽셀에 카피하여 저장한다.
			img_out[(i * 2) + (2 * height)][j * 2] = img_V420[i][j];
			img_out[(i * 2) + 1 + (2 * height)][j * 2] = img_V420[i][j];
			img_out[(i * 2) + (2 * height)][(j * 2) + 1] = img_V420[i][j];
			img_out[(i * 2) + 1 + (2 * height)][(j * 2) + 1] = img_V420[i][j];
		}
	}

}

void YUV444_to_422(BYTE** img_in, BYTE** img_Y, BYTE** img_U422, BYTE** img_V422, int width, int height)  // YUV 4:4:4 => YUV 4:2:2 Subsampling
{
	int i, j;

	for (i = 0; i < height; i++) //YUV중 Y(4개 픽셀값)를 4개의 픽셀에 그대로 저장한다.
	{
		for (j = 0; j < width; j++)
		{
			img_Y[i][j] = img_in[i][j];

		}
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width / 2; j++)
		{
			//YUV중 U(4개 픽셀 값)를 1개의 픽셀로 저장한다. 이때 2개를 저장한다.(PDF참조)
			img_U422[i][j] = img_in[i + height][j * 2];

			//YUV중 V(4개 픽셀 값)를 1개의 픽셀로 저장한다. 이때 2개를 저장한다.(PDF참조)
			img_V422[i][j] = img_in[i + height * 2][j * 2];
		}
	}

}

void YUV422_to_444(BYTE ** img_Y, BYTE ** img_U422, BYTE ** img_V422, BYTE ** img_out, int width, int height) // YUV 4:4:4 <= YUV 4:2:2 Copy the neighboring pixel
{
	int i, j;

	for (i = 0; i < height; i++) //YUV중 Y(4개 픽셀값)를 4개의 픽셀에 그대로 저장한다.
	{
		for (j = 0; j < width; j++)
		{
			img_out[i][j] = img_Y[i][j];
		}
	}

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width / 2; j++)
		{
			//YUV중 U(2개 픽셀 값)를 4개의 픽셀로 저장한다. 이때 2개를 그대로 카피하여 4개 픽셀에 저장한다.(PDF참조)
			img_out[i + height][j * 2] = img_U422[i][j];
			img_out[i + height][j * 2 + 1] = img_U422[i][j];

			//YUV중 V(2개 픽셀 값)를 4개의 픽셀로 저장한다. 이때 2개를 그대로 카피하여 4개 픽셀에 저장한다.(PDF참조)
			img_out[i + (2 * height)][j * 2] = img_V422[i][j];
			img_out[i + (2 * height)][j * 2 + 1] = img_V422[i][j];
		}
	}

}