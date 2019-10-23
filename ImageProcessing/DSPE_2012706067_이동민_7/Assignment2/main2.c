//DSPE_2012706067_¿ÃµøπŒ_Assignment2
#define _CRT_SECURE_NO_WARNINGS // Header file
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#define WIDTH 512 // image size
#define HEIGHT 512
#define BLOCK_SIZE 8 // DCT block size
#define PI 3.1415926535

typedef unsigned char BYTE;

BYTE** MemAlloc_UC_2D(const int width, const int height); // 2D memory allocation for unsigned char
double** MemAlloc_D_2D(const int width, const int height);// 2D memory allocation for double type
void MemFree_UC_2D(BYTE** arr, const int height); // 2D memory free for unsigned char type
void MemFree_D_2D(double** arr, const int height); // 2D memory free for double type

// image file management 
void ImgRead(const char* filename, BYTE** img_in, const int width, const int height); //image file read 
void ImgWrite(const char* filename, const BYTE** img_out, const int width, const int height); // image file write

// Discrete Cosine Transform (DCT)
// separable block-based forward DCT
void SeparableBlockFDCT_2D(const double** input, double** coeff, const int width, const int height, const int block_size);
// separable block-based inverse DCT
void SeparableBlockIDCT_2D(const double** coeff, double** output, const int width, const int height, const int block_size);

void FDCT_1D(const double* input, double* coeff, const int N); //N point 1D forward DCT
void IDCT_1D(const double* coeff, double* output, const int N); //N point 1D inverse DCT

// matrix opration
void MatTranspose(double **mat, const int size); //matrix transpose

// block based 2D forward DCT
void BlockFDCT_2D(const double** input, double** coeff, const int width, const int height, const int block_size);
// block based 2D inverse DCT
void BlockIDCT_2D(const double** coeff, double** output, const int width, const int height, const int block_size);



void main()
{
	int i, j, cnt;
	clock_t start;

	//memory allocation
	BYTE **img_in = MemAlloc_UC_2D(WIDTH, HEIGHT);
	BYTE **img_out_sep = MemAlloc_UC_2D(WIDTH, HEIGHT);
	BYTE **img_out_2D = MemAlloc_UC_2D(WIDTH, HEIGHT);
	double **input = MemAlloc_D_2D(WIDTH, HEIGHT);
	double **output = MemAlloc_D_2D(WIDTH, HEIGHT);
	double **coeff_sep = MemAlloc_D_2D(WIDTH, HEIGHT);
	double **coeff_2D = MemAlloc_D_2D(WIDTH, HEIGHT);

	//image read
	ImgRead("Lena(512x512).raw", img_in, WIDTH, HEIGHT);

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			input[i][j] = (double)img_in[i][j];
		}
	}

	// separable block-based forward DCT
	start = clock();
	SeparableBlockFDCT_2D(input, coeff_sep, WIDTH, HEIGHT, BLOCK_SIZE);
	printf("%d%d separable block-based FDCT to %d%d image : %.2f ms\n", BLOCK_SIZE, BLOCK_SIZE, WIDTH, HEIGHT, (double)1000 * (clock() - start) / CLOCKS_PER_SEC);

	//type casting and clipping
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			double temp = coeff_sep[i][j];
			temp = temp > 255 ? 255 : temp < 0 ? 0 : temp;
			img_out_sep[i][j] = (BYTE)floor(temp + 0.5);
		}
	}

	//image write
	ImgWrite("[SeparableFDCT]Lena(512x512).raw", img_out_sep, WIDTH, HEIGHT);

	// separable block_based inverse DCT
	start = clock();
	SeparableBlockIDCT_2D(coeff_sep, output, WIDTH, HEIGHT, BLOCK_SIZE);
	printf("%d%d separable block_based IDCT to %d%d image : %.2f ms\n", BLOCK_SIZE, BLOCK_SIZE, WIDTH, HEIGHT, (double)1000 * (clock() - start) / CLOCKS_PER_SEC);

	//type casting and clipping
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			double temp = output[i][j];
			temp = temp > 255 ? 255 : temp < 0 ? 0 : temp;
			img_out_sep[i][j] = (BYTE)floor(temp + 0.5);
		}
	}

	//image write
	ImgWrite("[SeparableIDCT]Lena(512x512).raw", img_out_sep, WIDTH, HEIGHT);

	// block based 2D forward DCT
	start = clock();
	BlockFDCT_2D(input, coeff_2D, WIDTH, HEIGHT, BLOCK_SIZE);
	printf("\n%d%d block-based 2-D FDCT to %d%d image : %.2f ms\n", BLOCK_SIZE, BLOCK_SIZE, WIDTH, HEIGHT, (double)1000 * (clock() - start) / CLOCKS_PER_SEC);

	//type casting and clipping
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			double temp = coeff_2D[i][j];
			temp = temp > 255 ? 255 : temp < 0 ? 0 : temp;
			img_out_2D[i][j] = (BYTE)floor(temp + 0.5);
		}
	}

	//image write
	ImgWrite("[BlockFDCT]Lena(512x512).raw", img_out_2D, WIDTH, HEIGHT);

	// block based 2D inverse DCT
	start = clock();
	BlockIDCT_2D(coeff_2D, output, WIDTH, HEIGHT, BLOCK_SIZE);
	printf("%d%d block_based 2-D IDCT to %d%d image : %.2f ms\n", BLOCK_SIZE, BLOCK_SIZE, WIDTH, HEIGHT, (double)1000 * (clock() - start) / CLOCKS_PER_SEC);

	//type casting and clipping
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			double temp = output[i][j];
			temp = temp > 255 ? 255 : temp < 0 ? 0 : temp;
			img_out_2D[i][j] = (BYTE)floor(temp + 0.5);
		}
	}

	//image write
	ImgWrite("[BlockIDCT]Lena(512x512).raw", img_out_2D, WIDTH, HEIGHT);




	// check whether two results are same
	cnt = 0;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (img_out_2D[i][j] != img_out_sep[i][j])
			{
				
				cnt++;
			}
		}
	}
	if (cnt == 0)
	{
		printf("\nSeparable-DCT and 2D-DCT are same\n");
	}
	else
	{
		printf("\nSeparable-DCT and 2D-DCT are different\n");
	}

	//memory free
	MemFree_UC_2D(img_in, HEIGHT);
	MemFree_UC_2D(img_out_sep, HEIGHT);
	MemFree_UC_2D(img_out_2D, HEIGHT);
	MemFree_D_2D(input, HEIGHT);
	MemFree_D_2D(output, HEIGHT);
	MemFree_D_2D(coeff_sep, HEIGHT);
	MemFree_D_2D(coeff_2D, HEIGHT);
}




// 2D memory allocation for unsigned char type
BYTE** MemAlloc_UC_2D(const int width, const int height)
{
	BYTE** arr;
	int i;
	arr = (BYTE**)malloc(height * sizeof(BYTE*));
	for (i = 0; i < height; i++)
	{
		arr[i] = (BYTE*)malloc(width * sizeof(BYTE));
	}
	return arr;
}

//2D memory allocation for double type
double** MemAlloc_D_2D(const int width, const int height)
{
	double **arr;
	int i;
	arr = (double**)malloc(height * sizeof(BYTE*));
	for (i = 0; i < height; i++)
	{
		arr[i] = (double*)malloc(width * sizeof(double));
	}
	return arr;
}

//2D memory free for unsigned char type
void MemFree_UC_2D(BYTE** arr, const int height)
{
	int i;
	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

//2D memory free for double type
void MemFree_D_2D(double** arr, const int height)
{
	int i;
	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

//image file read
void ImgRead(const char* filename, BYTE** img_in, const int width, const int height)
{
	FILE* fp_in;
	int i;
	fopen_s(&fp_in, filename, "rb");
	for (i = 0; i < height; i++)
	{
		fread(img_in[i], sizeof(BYTE), width, fp_in);
	}
	fclose(fp_in);
}

//image file write
void ImgWrite(const char* filename, const BYTE** img_out, const int width, const int height)
{
	FILE* fp_out;
	int i;
	fopen_s(&fp_out, filename, "wb");
	for (i = 0; i < height; i++)
	{
		fwrite(img_out[i], sizeof(BYTE), width, fp_out);
	}
	fclose(fp_out);
}

// separable block-based forward DCT
void SeparableBlockFDCT_2D(const double** input, double** coeff, const int width, const int height, const int block_size)
{
	int i, j, m, n, s, t, x, y;
	double **temp_hor = MemAlloc_D_2D(block_size, block_size);
	double **temp_ver = MemAlloc_D_2D(block_size, block_size);

	double **temp_input = MemAlloc_D_2D(block_size, block_size);

	double *input_temp1;
	input_temp1 = (double**)malloc(block_size * sizeof(BYTE*));

	for (i = 0; i < height; i += block_size)
	{
		for (j = 0; j < width; j += block_size)
		{
			for (m = 0; m < block_size; m++)
			{
				for (n = 0; n < block_size; n++)
				{
					temp_input[m][n] = input[i + m][j + n];
				}

				input_temp1 = temp_input[m];

				FDCT_1D(input_temp1, temp_hor[m], block_size);

			}

			MatTranspose(temp_hor, block_size);


			for (s = 0; s < block_size; s++)
			{
				FDCT_1D(temp_hor[s], temp_ver[s], block_size);
			}

			MatTranspose(temp_ver, block_size);

			for (x = 0; x < block_size; x++)
			{
				for (y = 0; y < block_size; y++)
				{
					coeff[x + i][y + j] = temp_ver[x][y];
				}

			}

		}
	}

	


	MemFree_D_2D(temp_hor, block_size);
	MemFree_D_2D(temp_ver, block_size);
}

// separable block-based inverse DCT
void SeparableBlockIDCT_2D(const double** coeff, double** output, const int width, const int height, const int block_size)
{
	int i, j, m, n, s, t, x, y;
	double **temp_hor = MemAlloc_D_2D(block_size, block_size);
	double **temp_ver = MemAlloc_D_2D(block_size, block_size);

	double **temp_input = MemAlloc_D_2D(block_size, block_size);

	double *input_temp1;
	input_temp1 = (double**)malloc(block_size * sizeof(BYTE*));


	for (i = 0; i < height; i += block_size)
	{
		for (j = 0; j < width; j += block_size)
		{
			for (m = 0; m < block_size; m++)
			{
				for (n = 0; n < block_size; n++)
				{
					temp_input[m][n] = coeff[i + m][j + n];
				}

				input_temp1 = temp_input[m];

				IDCT_1D(input_temp1, temp_hor[m], block_size);
			}

			MatTranspose(temp_hor, block_size);


			for (s = 0; s < block_size; s++)
			{
				IDCT_1D(temp_hor[s], temp_ver[s], block_size);
			}

			MatTranspose(temp_ver, block_size);

			for (x = 0; x < block_size; x++)
			{
				for (y = 0; y < block_size; y++)
				{
					output[x + i][y + j] = temp_ver[x][y];
				}

			}

		}
	}

	MemFree_D_2D(temp_hor, block_size);
	MemFree_D_2D(temp_ver, block_size);
}

////N point 1D forward DCT
void FDCT_1D(const double* input, double* coeff, const int N)
{
	int n, k;


	for (k = 0; k < N; k++)
	{
		double beta = k == 0 ? 1 / sqrt(2.0) : 1;
		double temp = 0;


		for (n = 0; n < N; n++)
		{
			double basis = cos(((2 * n + 1)*PI*k) / (2.0*N));
			temp += input[n] * basis;
		}

		temp *= sqrt(2 / (double)N)*beta;

		coeff[k] = temp;
	}


}

//N point 1D inverse DCT
void IDCT_1D(const double* coeff, double* output, const int N)
{
	int n, k;
	for (n = 0; n < N; n++)
	{
		double temp = 0;
		for (k = 0; k < N; k++)
		{
			double beta = k == 0 ? 1 / sqrt(2.0) : 1;
			double basis = cos(((2 * n + 1)*PI*k) / (2.0*N));
			temp += beta * coeff[k] * basis;
		}
		temp *= sqrt(2 / (double)N);
		output[n] = temp;
	}
}

// matrix transpose
void MatTranspose(double **mat, const int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = i + 1; j < size; j++)
		{
			double temp = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = temp;
		}
	}

}
// block based 2D forward DCT
void BlockFDCT_2D(const double ** input, double ** coeff, const int width, const int height, const int block_size)
{
	int f, z, i, j, x, y;


	for (f = 0; f < width; f += block_size)
	{
		for (z = 0; z < height; z += block_size)
		{
			for (x = 0; x < block_size; x++)
			{
				double cx = x == 0 ? 1 / sqrt(2.0) : 1;

				for (y = 0; y < block_size; y++)
				{
					double cy = y == 0 ? 1 / sqrt(2.0) : 1;
					double temp = 0;

					for (i = 0; i < block_size; i++)
					{
						for (j = 0; j < block_size; j++)
						{
							double basis = cos(((2 * i + 1)*PI*x) / (2.0*block_size))*cos(((2 * j + 1)*PI*y) / (2.0*block_size));
							temp += input[f + i][z + j] * basis;
						}
					}
					temp *= (cx*cy) / (block_size / 2.0);
					coeff[f + x][z + y] = temp;
				}
			}
		}
	}

}

// block based 2D inverse DCT
void BlockIDCT_2D(const double ** coeff, double ** output, const int width, const int height, const int block_size)
{


	int i, j, u, v, x, y;
	double coeffi, cn, cm;


	for (x = 0; x < height; x += block_size) {
		for (y = 0; y < width; y += block_size) {

			for (u = 0; u < block_size; u++) {
				for (v = 0; v < block_size; v++) {

					coeffi = 0;

					for (i = 0; i < block_size; i++) {
						for (j = 0; j < block_size; j++) {
							cn = i == 0 ? 1 / sqrt(2.0) : 1;
							cm = j == 0 ? 1 / sqrt(2.0) : 1;

							coeffi += cn * cm * (2 / (double)block_size) * (double)coeff[x + i][y + j] * cos(((2 * u + 1) * i * PI) / (double)(2 * block_size)) * cos(((2 * v + 1) * j *PI) / (double)(2 * block_size));
						}
					}
					output[x + u][y + v] = coeffi;
				}
			}
		}
	}


}