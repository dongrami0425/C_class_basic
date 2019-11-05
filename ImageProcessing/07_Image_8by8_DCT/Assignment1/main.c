//DSPE_2012706067_이동민_7_Assignment1
// < 2-D block-based Discrete Cosine Transform >
// - DCT :
//		- type-II DCT가 가장 널리 사용됨
//		- Non-Peoriodic signal을 sampling 및 peoriodic discrete하여 Peodiodic signal로 변환후 DCT 수식을 이용해
//		  spatial domain에서 frequency domain으로 변환하여 신호에 대한 정보를 파악하는 것.
//		- DCT가 적용된 영상 분석시 Low Frequency에 대다수의 데이터 정보가 형성되어 있으며 high frequency range에 적은 수의 값들이 형성된다. 
//		  이를 이용해 JPEG, MPEG과 같은 데이터 압축에서 양자화과정을 거쳐 함께 사용된다.
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define WIDTH 512
#define HEIGHT 512
#define DCT_BlockSize 8 // Definition for DCT macro block size
#define PI 3.141592653589793238462
typedef unsigned char BYTE;

BYTE** MemAlloc_2D(int width, int height);  //2D memory allocation
void MemFree_2D(BYTE** arr, int height); // 2D memory free
void FileRead(char* filename, BYTE** img_in, int width, int height); //read data from a file
void FileWrite(char* filename, BYTE** img_out, int width, int height); // write data to a file
float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height); //PSNR calculation

void FDCT(BYTE** img_in, double** img_coeffi, int blockSize, int height, int width);
void IDCT(double** img_coeffi, BYTE** img_recon, int blockSize, int lowFreqBlock, int highFreqBlock, int height, int width);
// lowFreqBlock Size : coefficient block size to remain low frequency
// highFreqBlock Size : coefficint block size to remain highfrequency
int main()
{
	BYTE** img_in, **img_recon;
	double** img_coeffi;

	int i, j;
	double temp; // variavles for operation
	BYTE data;

	FILE* fp_fdct_out = fopen("[8x8_DCT]Lena(512x512).raw", "wb"); // file stream to write DCT  coefficients

	img_in = MemAlloc_2D(WIDTH, HEIGHT);
	img_recon = MemAlloc_2D(WIDTH, HEIGHT);

	img_coeffi = (double**)malloc(sizeof(double)*WIDTH);
	for (i = 0; i < HEIGHT; i++)
	{
		img_coeffi[i] = (double*)malloc(sizeof(double)*WIDTH);
	}

	FileRead("Lena(512x512).raw", img_in, WIDTH, HEIGHT);
	FDCT(img_in, img_coeffi, DCT_BlockSize, WIDTH, HEIGHT); //Forward DCT

	for (i = 0; i < HEIGHT; i++) // DCT data save
	{
		for (j = 0; j < WIDTH; j++)
		{
			temp = img_coeffi[i][j];
			if (temp < 0)  // Clipping
			{
				temp = 0;
			}
			else if (temp > 255)
			{
				temp = 255;
			}
			data = (BYTE)floor(temp + 0.5);
			fwrite(&data, 1, 1, fp_fdct_out);
		}
	}

	IDCT(img_coeffi, img_recon, DCT_BlockSize, DCT_BlockSize, 0, HEIGHT, WIDTH); // inverse DCT by all coefficients
	FileWrite("[8x8_IDCT]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by all coefficients) : %fdB\n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT)); // printf the PSNR value

	IDCT(img_coeffi, img_recon, DCT_BlockSize, 6, 0, HEIGHT, WIDTH); // inverse DCT by low6x6 coefficients
	FileWrite("[8x8_IDCT_Low6x6]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by low6x6 coefficients) : %fdB\n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));

	IDCT(img_coeffi, img_recon, DCT_BlockSize, 2, 0, HEIGHT, WIDTH); // inverse DCT by low2x2 coefficients
	FileWrite("[8x8_IDCT_Low2x2]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by low2x2 coefficients) : %fdB\n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));

	IDCT(img_coeffi, img_recon, DCT_BlockSize, 0, 6, HEIGHT, WIDTH); // inverse DCT by high6x6 coefficients
	FileWrite("[8x8_IDCT_High6x6]Lena(512x512).raw", img_recon, WIDTH, HEIGHT);
	printf("PSNR (Reconstruction by high6x6 coefficients) : %fdB\n\n", GetPSNR(img_in, img_recon, WIDTH, HEIGHT));

	MemFree_2D(img_in, HEIGHT);
	MemFree_2D(img_recon, HEIGHT);

	for (i = 0; i < HEIGHT; i++)
	{
		free(img_coeffi[i]);
	}
	free(img_coeffi);

	fclose(fp_fdct_out);

	return 0;
}

BYTE** MemAlloc_2D(int width, int height)
{
	BYTE** arr;
	int i;
	arr = (double**)malloc(sizeof(BYTE*)*height);
	for (i = 0; i < height; i++)
	{
		arr[i] = (BYTE*)malloc(sizeof(BYTE)*width);
	}
	return arr;
}
void MemFree_2D(BYTE** arr, int height)
{
	int i;
	for (i = 0; i < height; i++)
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
float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height)
{
	float mse = 0.0;;
	float temp = 0.0;
	float square = 0.0;
	float psnr;
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			temp = img_ori[i][j] - img_dist[i][j];
			square = temp * temp;
			mse += square / (height*width);
		}
	}
	psnr = 10.0 * log10((255.0 * 255.0) / mse);

	return psnr;
}

void FDCT(BYTE** img_in, double** img_coeffi, int blockSize, int height, int width) // operating forward 
{
	int x, y, u, v, i, j;
	double coeffi, cn, cm;
	for (x = 0; x < height; x += blockSize) // (x,y) : left top position of current block on operation
	{
		for (y = 0; y < width; y += blockSize)
		{
			for (u = 0; u < blockSize; u++) // (u,v) : coefficients coordinates
			{
				for (v = 0; v < blockSize; v++)
				{
					coeffi = 0;
					for (i = 0; i < blockSize; i++) // (i,j) : image data coordinates
					{
						for (j = 0; j < blockSize; j++)
						{
							cn = u == 0 ? 1 / sqrt(2.0) : 1; //FDCT operation
							cm = v == 0 ? 1 / sqrt(2.0) : 1;
							coeffi += cn * cm*(2 / (double)blockSize)*(double)img_in[x + i][y + j] * cos(((2 * i + 1)*u*PI) / (double)(2 * blockSize))*cos(((2 * j + 1)*v*PI) / (double)(2 * blockSize));
						}
					}
					img_coeffi[x + u][y + v] = coeffi; //coefficient save
				}
			}
		}
	}
}

void IDCT(double** img_coeffi, BYTE** img_recon, int blockSize, int lowFreqBlockSize, int highFreqBlockSize, int height, int width)
{
	int x, y, u, v, i, j;
	double coeffi, cn, cm;

	if (highFreqBlockSize < lowFreqBlockSize)
	{
		for (i = 0; i < height; i += blockSize)
		{
			for (j = 0; j < width; j += blockSize)
			{
				for (u = 0; u < blockSize; u++)
				{
					for (v = 0; v < blockSize; v++)
					{
						coeffi = 0;
						for (x = 0; x < lowFreqBlockSize; x++)
						{
							for (y = 0; y < lowFreqBlockSize; y++)
							{
								cn = x == 0 ? 1 / sqrt(2.0) : 1;
								cm = y == 0 ? 1 / sqrt(2.0) : 1;
								coeffi += cn * cm*(2 / (double)blockSize)*(double)img_coeffi[x + i][y + j] * cos(((2 * u + 1)*x* PI) / (double)(2 * blockSize))*cos(((2 * v + 1)*y*PI) / (double)(2 * blockSize));
							}
						}
						img_recon[i + u][j + v] = coeffi;
					}
				}
			}
		}
	}
	else
	{
		for (i = 0; i < height; i += blockSize)
		{
			for (j = 0; j < width; j += blockSize)
			{
				for (u = 0; u < blockSize; u++)
				{
					for (v = 0; v < blockSize; v++)
					{
						coeffi = 0;
						for (x = blockSize - highFreqBlockSize; x < blockSize; x++)
						{
							for (y = blockSize - highFreqBlockSize; y < blockSize; y++)
							{
								cn = x == 0 ? 1 / sqrt(2.0) : 1;
								cm = y == 0 ? 1 / sqrt(2.0) : 1;
								coeffi += cn * cm*(2 / (double)blockSize)*(double)img_coeffi[x + i][y + j] * cos(((2 * u + 1)*x* PI) / (double)(2 * blockSize))*cos(((2 * v + 1)*y*PI) / (double)(2 * blockSize));
							}
						}
						img_recon[i + u][j + v] = coeffi;
					}
				}
			}
		}
	}

}

