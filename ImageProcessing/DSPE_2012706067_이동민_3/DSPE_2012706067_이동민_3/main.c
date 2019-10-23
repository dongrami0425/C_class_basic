#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  512
#define HEIGHT  512

typedef unsigned char BYTE;

unsigned char** MenAlloc_2D(int width, int height);
void MenFree_2D(unsigned char** arr, int height);
void GaussianFilter_9x9(unsigned char** img_in, unsigned char** img_out, int width, int height);
void GaussianFilter_5x5(unsigned char** img_in, unsigned char** img_out, int width, int height);
void SobelFilter_3x3_Gx(unsigned char** img_in, unsigned char** img_out, int width, int height);
void SobelFilter_3x3_Gy(unsigned char** img_in, unsigned char** img_out, int width, int height);
void LaplacianMask(unsigned char** img_in, unsigned char** img_out, int width, int height);
void SharpeningMask(unsigned char** img_in, unsigned char** img_out, int width, int height);


int main()
{
	FILE *fp_in = 0, *fp_out = 0;
	BYTE **img_in = 0, **img_out = 0;
	int i = 0;
	int n = 0;
	
	fp_in = fopen("Lena(512x512).raw", "rb");


	if (fp_in == NULL) {
		printf("File open failed\n");
		return -1;
	}

	printf(" 1번, 9x9가우시안\n 2번, 5x5가우시안\n 3번, Sobel_Vertical\n 4번, Sobel_Horizontal\n 5번, 라플라시안\n 6번, 샤프닝\n 샤프닝(빼기)\n숫자를 입력하시오: ");
	scanf("%d", &n);


	img_in = MenAlloc_2D(WIDTH, HEIGHT);
	img_out = MenAlloc_2D(WIDTH, HEIGHT);

	for (i = 0; i < HEIGHT; i++) {
		fread(img_in[i], sizeof(BYTE), WIDTH, fp_in);
	}

	
	if (n == 1) {
		GaussianFilter_9x9(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Gaussian_9x9]Lena(512x512).raw", "wb");
		printf("Gaussian_9x9 완료\n");
	}

	else if (n == 2) {
		GaussianFilter_5x5(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Gaussian_5x5]Lena(512x512).raw", "wb");
		printf("Gaussian_5x5 완료\n");
	}

	else if (n == 3) {
		SobelFilter_3x3_Gx(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Vertical_edge]Lena(512x512).raw", "wb");
		printf("Sobel_Gx_3x3 완료\n");
	}

	else if (n == 4) {
		SobelFilter_3x3_Gy(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Horizontal_edge]Lena(512x512).raw", "wb");
		printf("Sobel_Gy_3x3 완료\n");
	}
	
	else if (n == 5) {
		LaplacianMask(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Laplacian]Lena(512x512).raw", "wb");
		printf("Laplacian_3x3 완료\n");
	}

	else if (n == 6) {
		SharpeningMask(img_in, img_out, WIDTH, HEIGHT);
		fp_out = fopen("[Sharpening]Lena(512x512).raw", "wb");
		printf("Sharpening_3x3 완료\n");
	}
		
	else {
		printf("Not valid mask size\n");
		return -1;
	}
	


	if (fp_out == NULL) {
		printf("File open failed\n");
		return -1;
	}

	for (i = 0; i<HEIGHT; i++) {
		fwrite(img_out[i], sizeof(BYTE), WIDTH, fp_out);
	}

	MenFree_2D(img_in, HEIGHT);
	MenFree_2D(img_out, HEIGHT);
	fclose(fp_in);
	fclose(fp_out);


	return 0;

}


unsigned char** MenAlloc_2D(int width, int height)
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

void MenFree_2D(unsigned char** arr, int height)
{
	int i = 0;

	for (i = 0; i < height; i++)
		free(arr[i]);
}


void GaussianFilter_9x9(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double Gauss_9x9[9][9] =           //9x9 Gaussian mask
	{
		{ 0.0033,0.0033,0.0033,0.0067,0.0067,0.0067,0.0033,0.0033,0.0033 },
		{ 0.0033,0.0033,0.0067,0.0067,0.0134,0.0067,0.0067,0.0033,0.0033 },
		{ 0.0033,0.0067,0.0067,0.0134,0.0268,0.0134,0.0067,0.0067,0.0033 },
		{ 0.0067,0.0067,0.0134,0.0268,0.0535,0.0268,0.0134,0.0067,0.0067 },
		{ 0.0067,0.0134,0.0268,0.0535,0.1070,0.0535,0.0268,0.0134,0.0067 },
		{ 0.0067,0.0067,0.0134,0.0268,0.0535,0.0268,0.0134,0.0067,0.0067 },
		{ 0.0033,0.0067,0.0067,0.0134,0.0268,0.0134,0.0067,0.0067,0.0033 },
		{ 0.0033,0.0033,0.0067,0.0067,0.0134,0.0067,0.0067,0.0033,0.0033 },
		{ 0.0033,0.0033,0.0033,0.0067,0.0067,0.0067,0.0033,0.0033,0.0033 },
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 8)); //Memory allocation
	for (i = 0; i < height + 8; i++)
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char)*(width + 8));


	for (i = 0; i < height; i++) { //data copy
		for (j = 0; j < width; j++) {
			img_in_pad[i + 4][j + 4] = img_in[i][j];
		}
	}
	for (i = 4; i < height + 4; i++) { //padding
		for (j = 0; j < 4; j++) {
			img_in_pad[i][j] = img_in_pad[i][4];
			img_in_pad[i][width + 4 + j] = img_in_pad[i][width + 4 - 1];
		}
	}

	for (j = 4; j < width + 4; j++) {
		for (i = 0; i < 4; i++) {
			img_in_pad[i][j] = img_in_pad[4][j];
			img_in_pad[height + 4 + i][j] = img_in_pad[height + 4 - 1][j];
		}
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			img_in_pad[i][j] = img_in_pad[4][4];
			img_in_pad[i][width + 4 + j] = img_in_pad[4][width + 4 - 1];
			img_in_pad[height + 4 + i][j] = img_in_pad[height + 4 - 1][4];
			img_in_pad[height + 4 + i][width + 4 + j] = img_in_pad[height + 4 - 1][width + 4 - 1];
		}
	}


	for (i = 0; i < height; i++) { //2-D Gaussian filtering
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 9; m++) {
				for (n = 0; n < 9; n++) {
					temp += img_in_pad[i + m][j + n] * Gauss_9x9[m][n];
				}
			}
			img_out[i][j] = (unsigned char)floor(temp + 0.5);
		}
	}

	for (i = 0; i < height + 8; i++) //memory free
		free(img_in_pad[i]);
	free(img_in_pad);
}

void GaussianFilter_5x5(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double Gauss_5x5[5][5] =           //5x5 Gaussian mask
	{
		{0.0037, 0.0147, 0.0256, 0.0147, 0.0037},
		{0.0147, 0.0586, 0.0952, 0.0586, 0.0147},
		{0.0256, 0.0952, 0.1502, 0.0952, 0.0256},
		{0.0147, 0.0586, 0.0952, 0.0586, 0.0147},
		{0.0037, 0.0147, 0.0256, 0.0147, 0.0037},
		
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 4)); //Memory allocation
	for (i = 0; i < height + 4; i++)
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char)*(width + 4));


	for (i = 0; i < height; i++) { //data copy
		for (j = 0; j < width; j++) {
			img_in_pad[i + 2][j + 2] = img_in[i][j];
		}
	}
	for (i = 2; i < height + 2; i++) { //padding
		for (j = 0; j < 2; j++) {
			img_in_pad[i][j] = img_in_pad[i][2];
			img_in_pad[i][width + 2 + j] = img_in_pad[i][width + 2 - 1];
		}
	}

	for (j = 2; j < width + 2; j++) {
		for (i = 0; i < 2; i++) {
			img_in_pad[i][j] = img_in_pad[2][j];
			img_in_pad[height + 2 + i][j] = img_in_pad[height + 2 - 1][j];
		}
	}

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			img_in_pad[i][j] = img_in_pad[2][2];
			img_in_pad[i][width + 2 + j] = img_in_pad[2][width + 2 - 1];
			img_in_pad[height + 2 + i][j] = img_in_pad[height + 2 - 1][2];
			img_in_pad[height + 2 + i][width + 2 + j] = img_in_pad[height + 2 - 1][width + 2 - 1];
		}
	}


	for (i = 0; i < height; i++) { //2-D Gaussian filtering
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 5; m++) {
				for (n = 0; n < 5; n++) {
					temp += img_in_pad[i + m][j + n] * Gauss_5x5[m][n];
				}
			}
			img_out[i][j] = (unsigned char)floor(temp + 0.5);
		}
	}

	for (i = 0; i < height + 4; i++) //memory free
		free(img_in_pad[i]);
	free(img_in_pad);
}

void SobelFilter_3x3_Gx(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double sobel_3x3[3][3] =
	{
		{ -1, -2, -1},
		{ 0, 0, 0 },
		{ 1, 2, 1 }
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 2));
	for (i = 0; i < height + 2; i++) {
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char) * (width + 2));
	}

	for (i = 0; i < height; i++) { //데이타 카피
		for (j = 0; j < width; j++) {
			img_in_pad[i + 1][j + 1] = img_in[i][j];
		}
	}
	for (i = 1; i < height + 1; i++) {  //패딩
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[i][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[i][width + 1 - 1];
		}
	}
	for (j = 1; j < width + 1; j++) {
		for (i = 0; i < 1; i++) {
			img_in_pad[i][j] = img_in_pad[1][j];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][j];
		}
	}
	for (i = 0; i < 1; i++) {
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[1][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[2][width + 1 - 1];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][1];
			img_in_pad[height + 1 + i][width + 1 + j] = img_in_pad[height + 1 - 1][width + 1 - 1];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 3;m++) {
				for (n = 0; n < 3; n++) {
					temp += img_in_pad[i + m][j + n] * sobel_3x3[m][n];


				}
			}
			temp = (temp / 8) + 128;

			if (temp < 0)
			{
				temp = 0;
			}
			else if (temp > 255)
			{
				temp = 255;
			}
			img_out[i][j] = (unsigned char)temp;
		}
	}

	for (i = 0; i < height + 2; i++)
		free(img_in_pad[i]);
	free(img_in_pad);
}

void SobelFilter_3x3_Gy(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double sobel_3x3[3][3] =
	{
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 2));
	for (i = 0; i < height + 2; i++) {
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char) * (width + 2));
	}

	for (i = 0; i < height; i++) { //데이타 카피
		for (j = 0; j < width; j++) {
			img_in_pad[i + 1][j + 1] = img_in[i][j];
		}
	}
	for (i = 1; i < height + 1; i++) {  //패딩
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[i][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[i][width + 1 - 1];
		}
	}
	for (j = 1; j < width + 1; j++) {
		for (i = 0; i < 1; i++) {
			img_in_pad[i][j] = img_in_pad[1][j];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][j];
		}
	}
	for (i = 0; i < 1; i++) {
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[1][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[2][width + 1 - 1];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][1];
			img_in_pad[height + 1 + i][width + 1 + j] = img_in_pad[height + 1 - 1][width + 1 - 1];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 3;m++) {
				for (n = 0; n < 3; n++) {
					temp += img_in_pad[i + m][j + n] * sobel_3x3[m][n];

				}
			}
			temp = (temp / 8)+128;
			
			if (temp < 0)
			{
				temp = 0;
			}
			else if (temp > 255)
			{
				temp = 255;
			}
			img_out[i][j] = (unsigned char)temp;
		}
	}

	for (i = 0; i < height + 2; i++)
		free(img_in_pad[i]);
	free(img_in_pad);
}

void LaplacianMask(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double Laplacian_3x3[3][3] =
	{
		{ 1, 1, 1 },
	{ 1, -8, 1 },
	{ 1, 1, 1 }
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 2));
	for (i = 0; i < height + 2; i++) {
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char) * (width + 2));
	}

	for (i = 0; i < height; i++) { //데이타 카피
		for (j = 0; j < width; j++) {
			img_in_pad[i + 1][j + 1] = img_in[i][j];
		}
	}
	for (i = 1; i < height + 1; i++) {  //패딩
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[i][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[i][width + 1 - 1];
		}
	}
	for (j = 1; j < width + 1; j++) {
		for (i = 0; i < 1; i++) {
			img_in_pad[i][j] = img_in_pad[1][j];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][j];
		}
	}
	for (i = 0; i < 1; i++) {
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[1][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[2][width + 1 - 1];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][1];
			img_in_pad[height + 1 + i][width + 1 + j] = img_in_pad[height + 1 - 1][width + 1 - 1];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 3;m++) {
				for (n = 0; n < 3; n++) {
					temp += img_in_pad[i + m][j + n] * Laplacian_3x3[m][n];
				}
			}
			temp = temp + 128;
			if (temp < 0)
				temp = 0;
			else if (temp > 255)
				temp = 255;
			img_out[i][j] = (unsigned char)temp;
		}
	}

	for (i = 0; i < height + 2; i++)
		free(img_in_pad[i]);
	free(img_in_pad);
}

void SharpeningMask(unsigned char** img_in, unsigned char** img_out, int width, int height)
{
	int i, j, m, n;
	double temp;
	unsigned char **img_in_pad;
	double Sharpening_3x3[3][3] =
	{
		{ -1, -1, -1 },
		{ -1, 9, -1 },
		{ -1, -1, -1 }
	};

	img_in_pad = (unsigned char**)malloc(sizeof(unsigned char*) * (height + 2));
	for (i = 0; i < height + 2; i++) {
		img_in_pad[i] = (unsigned char*)malloc(sizeof(unsigned char) * (width + 2));
	}

	for (i = 0; i < height; i++) { //데이타 카피
		for (j = 0; j < width; j++) {
			img_in_pad[i + 1][j + 1] = img_in[i][j];
		}
	}
	for (i = 1; i < height + 1; i++) {  //패딩
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[i][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[i][width + 1 - 1];
		}
	}
	for (j = 1; j < width + 1; j++) {
		for (i = 0; i < 1; i++) {
			img_in_pad[i][j] = img_in_pad[1][j];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][j];
		}
	}
	for (i = 0; i < 1; i++) {
		for (j = 0; j < 1; j++) {
			img_in_pad[i][j] = img_in_pad[1][1];
			img_in_pad[i][width + 1 + j] = img_in_pad[2][width + 1 - 1];
			img_in_pad[height + 1 + i][j] = img_in_pad[height + 1 - 1][1];
			img_in_pad[height + 1 + i][width + 1 + j] = img_in_pad[height + 1 - 1][width + 1 - 1];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp = 0;
			for (m = 0; m < 3;m++) {
				for (n = 0; n < 3; n++) {
					temp += img_in_pad[i + m][j + n] * Sharpening_3x3[m][n];
				}
			}
			if (temp < 0)
				temp = 0;
			else if (temp > 255)
				temp = 255;
			img_out[i][j] = (unsigned char)(temp);
		}
	}

	for (i = 0; i < height + 2; i++)
		free(img_in_pad[i]);
	free(img_in_pad);
}