//DSPE_2012706067_¿ÃµøπŒ_8
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define WIDTH 512
#define HEIGHT 512
#define BLOCK_SIZE 4
#define NMODE 3

typedef unsigned char BYTE;

void MemFree_2D(BYTE** arr, int height); // 2D memory allocation for unsigned char
void MemFree_2D_int(int** arr, int height);
void FileRead(char* filename, BYTE** img_in, int width, int height);
void FileWrite(char* filename, BYTE** img_out, int width, int height);
void Encode(BYTE** img_ori, BYTE** img_pred, int** img_resi, BYTE** img_recon);

// image file management 
BYTE** MemAlloc_2D(int width, int height);
int** MemAlloc_2D_int(int width, int height);

int intra_prediction(BYTE* ori, BYTE* ref, BYTE(pred)[3][16], int(resi)[3][16], BYTE(recon)[3][16]);
int intra_dc(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE* recon);
int intra_hor(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE* recon);
int intra_ver(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon);

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height);


int main()
{
	BYTE **img_ori, **img_pred, **img_recon, **img_in_R, **img_in_G, **img_in_B, **img_pred_R, **img_pred_G, **img_pred_B;
	int **img_resi_R, **img_resi_G, **img_resi_B;

	int i, j;

	img_ori = MemAlloc_2D(WIDTH, HEIGHT * 3);
	img_pred = MemAlloc_2D(WIDTH, HEIGHT * 3);
	img_recon = MemAlloc_2D(WIDTH, HEIGHT * 3);


	img_in_R = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_G = MemAlloc_2D(WIDTH, HEIGHT);
	img_in_B = MemAlloc_2D(WIDTH, HEIGHT);


	img_pred_R = MemAlloc_2D(WIDTH, HEIGHT);
	img_pred_G = MemAlloc_2D(WIDTH, HEIGHT);
	img_pred_B = MemAlloc_2D(WIDTH, HEIGHT);


	img_resi_R = MemAlloc_2D_int(WIDTH, HEIGHT);
	img_resi_G = MemAlloc_2D_int(WIDTH, HEIGHT);
	img_resi_B = MemAlloc_2D_int(WIDTH, HEIGHT);


	FileRead("Lena(512x512).RGB", img_ori, WIDTH, HEIGHT * 3);


	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_in_R[i], img_ori[i], sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_in_G[i], img_ori[i + HEIGHT], sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_in_B[i], img_ori[i + (HEIGHT << 1)], sizeof(BYTE)*WIDTH);
	}

	//////////Intra Prediction Processing //////////////

	Encode(img_in_R, img_pred_R, img_resi_R, &img_recon[0]);
	Encode(img_in_G, img_pred_G, img_resi_G, &img_recon[HEIGHT]);
	Encode(img_in_B, img_pred_B, img_resi_B, &img_recon[HEIGHT * 2]);

	//merging result image
	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_pred[i], img_pred_R[i], sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_pred[i + HEIGHT], img_pred_G[i], sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGHT; i++)
	{
		memcpy(img_pred[i + HEIGHT * 2], img_pred_B[i], sizeof(BYTE)*WIDTH);
	}

	//get psnr

	printf("PREDICTION VS ORIGINAL PSNR : %.2f\n", GetPSNR(img_ori, img_pred, WIDTH, HEIGHT * 3));
	printf("RECON VS ORIGINAL PSNR : %.2f\n", GetPSNR(img_ori, img_recon, WIDTH, HEIGHT * 3));

	//get result
	FileWrite("[Intra]Lena(512x512).RGB", img_pred, WIDTH, HEIGHT * 3);
	FileWrite("[Recon]Lena(512x512).RGB", img_recon, WIDTH, HEIGHT * 3);

	MemFree_2D(img_in_R, HEIGHT);
	MemFree_2D(img_in_G, HEIGHT);
	MemFree_2D(img_in_B, HEIGHT);
	MemFree_2D(img_pred_R, HEIGHT);
	MemFree_2D(img_pred_G, HEIGHT);
	MemFree_2D(img_pred_B, HEIGHT);
	MemFree_2D(img_ori, HEIGHT * 3);
	MemFree_2D(img_pred, HEIGHT * 3);
	MemFree_2D_int(img_resi_R, HEIGHT);
	MemFree_2D_int(img_resi_G, HEIGHT);
	MemFree_2D_int(img_resi_B, HEIGHT);

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
int** MemAlloc_2D_int(int width, int height)
{
	int** arr;
	int i;
	arr = (int**)malloc(sizeof(int*)*height);
	for (i = 0; i < height; i++)
	{
		arr[i] = (int*)malloc(sizeof(int)*width);
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
void MemFree_2D_int(int** arr, int height)
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
	float mse = 0;
	int i, j;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mse += ((img_ori[i][j] - img_dist[i][j])*(img_ori[i][j] - img_dist[i][j])) / (float)(width*height);
		}
	}
	return 10 * (float)log10((255 * 255) / mse);
}

void Encode(BYTE** img_ori, BYTE** img_pred, int** img_resi, BYTE** img_recon)
{
	int i, j, m, n;
	int best_mode;
	int min_SAD, temp_SAD;

	static BYTE ori[BLOCK_SIZE*BLOCK_SIZE];
	static BYTE ref[BLOCK_SIZE * 2 + 1];
	static BYTE pred[3][BLOCK_SIZE*BLOCK_SIZE];
	static BYTE recon[3][BLOCK_SIZE*BLOCK_SIZE];
	static int resi[3][BLOCK_SIZE*BLOCK_SIZE];

	BYTE** img_padding = MemAlloc_2D(WIDTH + 1, HEIGHT + 1);

	for (i = 0; i < HEIGHT; i++)	{

		for (j = 0; j < WIDTH; j++)	{

			img_padding[i + 1][j + 1] = img_ori[i][j];

		}
	}

	for (i = 0; i < HEIGHT; i++)	{
		img_padding[i + 1][0] = 128;
	}


	for (i = 0; i < WIDTH; i++)	{
		img_padding[0][i] = 128;
	}

	//intra prediction loop
	for (i = 0; i < HEIGHT; i += BLOCK_SIZE){

		for (j = 0; j < WIDTH; j += BLOCK_SIZE)	{

			//get original block
			for (m = 0; m < BLOCK_SIZE; m++){

				for (n = 0; n < BLOCK_SIZE; n++){

					ori[m*BLOCK_SIZE + n] = img_ori[i + m][j + n];
				}

			}

			//get reference samples
			for (m = 0; m < BLOCK_SIZE; m++){

				ref[m] = img_padding[i][j + m];

			}
			for (m = 0; m < BLOCK_SIZE; m++){

				ref[5 + m] = img_padding[(i + 1) + m][j];

			}

			//search for best mode
			best_mode = intra_prediction(ori, ref, //input
				pred, resi, recon); //output

			for (m = 0; m < BLOCK_SIZE; m++){

				for (n = 0; n < BLOCK_SIZE; n++){

					img_pred[i + m][j + n] = pred[best_mode][m*BLOCK_SIZE + n];
					img_resi[i + m][j + n] = resi[best_mode][m*BLOCK_SIZE + n];
					img_recon[i + m][j + n] = recon[best_mode][m*BLOCK_SIZE + n];
				}
			}
		}
	}
	MemFree_2D(img_padding, HEIGHT + 1);
}

int intra_prediction(BYTE* ori, BYTE* ref, BYTE(pred)[3][16], int(resi)[3][16], BYTE(recon)[3][16]) //output
{
	static int SAD[3];
	int min_SAD, best_mode, i, j, m, n;

	SAD[0] = intra_ver(ori, ref, pred[0], resi[0], recon[0]);
	SAD[1] = intra_hor(ori, ref, pred[1], resi[1], recon[1]);
	SAD[2] = intra_dc(ori, ref, pred[2], resi[2], recon[2]);

	best_mode = 0;
	min_SAD = SAD[0];

	for (n = 1; n < NMODE; n++){

		if (min_SAD > SAD[n]){

			min_SAD = SAD[n];
			best_mode = n;
		}
	}
	return best_mode;
}


int intra_dc(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int x, y;
	int temp1 = 0, temp2 = 0;
	int DCsum, sad = 0;;

	for (x = 0; x < BLOCK_SIZE * 2; x++){

		temp1 += ref[x];

	}
	temp1 += 4;

	DCsum = temp1 >> 3;

	for (x = 0; x < BLOCK_SIZE *BLOCK_SIZE; x++){


		pred[x] = DCsum;
		resi[x] = ori[x] - pred[x];
		recon[x] = pred[x] + resi[x];

		if (resi[x] < 0) {

			temp2 = resi[x] * (-1);

		}
		else

			temp2 = resi[x];

		sad += temp2;

	}



	return sad;
}

int intra_hor(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int x, y;
	int temp1 = 0;
	int sad = 0;;


	for (x = 0; x < BLOCK_SIZE; x++){


		for (y = 0; y < BLOCK_SIZE; y++){


			pred[4 * x + y] = ref[x + 5];

		}
	}

	for (x = 0; x < BLOCK_SIZE*BLOCK_SIZE; x++)
	{
		resi[x] = ori[x] - pred[x];
		recon[x] = pred[x] + resi[x];

		if (resi[x] < 0) {
			temp1 = resi[x] * (-1);

		}
		else
			temp1 = resi[x];

		sad += temp1;

	}

	return sad;
}

int intra_ver(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int x, y;
	int temp1 = 0, temp2 = 0;
	int sad = 0;;


	for (x = 0; x < BLOCK_SIZE; x++){


		for (y = 0; y < BLOCK_SIZE; y++){


			pred[4 * x + y] = ref[x + 1];


		}
	}

	for (x = 0; x < BLOCK_SIZE*BLOCK_SIZE; x++){

		resi[x] = ori[x] - pred[x];
		recon[x] = pred[x] + resi[x];

		if (resi[x] < 0) {

			temp2 = resi[x] * (-1);

		}
		else

			temp2 = resi[x];

		sad += temp2;

	}

	return sad;

}