//DSPE_2012706067_이동민_9
// <Intra Predition Part2>
// - intra predictor DC, horizontal, vertical, diagonal Left, diagonal Right 실습
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

#define WIDTH 512
#define HEIGHT 512
#define BLOCK_SIZE 8
#define NMODE 5

typedef unsigned char BYTE;

void MemFree_2D(BYTE** arr, int height);
void MemFree_2D_int(int** arr, int height);
void FileRead(char* filename, BYTE** img_in, int width, int height);
void FileWrite(char* filename, BYTE** img_out, int width, int height);
void Encode(BYTE** img_ori, BYTE** img_pred, int** img_resi, BYTE** img_recon);

BYTE** MemAlloc_2D(int width, int height);
int** MemAlloc_2D_int(int width, int height);

int intra_prediction(BYTE* ori, BYTE* ref, BYTE(pred)[NMODE][BLOCK_SIZE*BLOCK_SIZE], int(resi)[NMODE][BLOCK_SIZE*BLOCK_SIZE], BYTE(recon)[NMODE][BLOCK_SIZE*BLOCK_SIZE]);
int intra_dc(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE* recon);
int intra_hor(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE* recon);
int intra_ver(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon);
int intra_DL(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon);
int intra_DR(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon);

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height);


int main()
{
	BYTE **img_ori, **img_pred, **img_recon, **img_in_R, **img_in_G, **img_in_B, **img_pred_R, **img_pred_G, **img_pred_B;
	int **img_resi_R, **img_resi_G, **img_resi_B;

	int i;

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

	///////Intra Prediction Processing ////////

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
	static BYTE ref[BLOCK_SIZE * 3 + 1];
	static BYTE pred[NMODE][BLOCK_SIZE*BLOCK_SIZE];
	static BYTE recon[NMODE][BLOCK_SIZE*BLOCK_SIZE];
	static int resi[NMODE][BLOCK_SIZE*BLOCK_SIZE];

	BYTE** img_padding = MemAlloc_2D(WIDTH + 1, HEIGHT + 1);

	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			img_padding[i + 1][j + 1] = img_ori[i][j];
		}
	}

	for (i = 0; i < HEIGHT; i++)
	{
		img_padding[i + 1][0] = 128;
	}


	for (i = 0; i < WIDTH; i++)
	{
		img_padding[0][i] = 128;
	}

	//intra prediction loop
	for (i = 0; i < HEIGHT; i += BLOCK_SIZE)
	{
		for (j = 0; j < WIDTH; j += BLOCK_SIZE)
		{
			//get original block
			for (m = 0; m < BLOCK_SIZE; m++)
			{
				for (n = 0; n < BLOCK_SIZE; n++)
				{
					ori[m*BLOCK_SIZE + n] = img_ori[i + m][j + n];
				}
			}

			//get reference samples

			if (j != WIDTH - BLOCK_SIZE)
			{
				for (m = 0; m < 2 * BLOCK_SIZE + 1; m++)
				{
					ref[m] = img_padding[i][j + m];
				}
			}
			else
			{
				for (m = 0; m < BLOCK_SIZE + 1; m++)
				{
					ref[m] = img_padding[i][j + m];
				}
				for (m = BLOCK_SIZE + 1; m < 2 * BLOCK_SIZE + 1; m++)
				{
					ref[m] = 128;
				}
			}


			if (j != 0)
			{
				for (m = 0; m < BLOCK_SIZE; m++)
				{

					ref[m + (2 * BLOCK_SIZE) + 1] = img_padding[(i + 1) + m][j];

				}
			}
			else
			{
				for (m = 0; m < BLOCK_SIZE; m++)
				{
					ref[m + (2 * BLOCK_SIZE) + 1] = 128;
				}
			}
						
			//search for best mode
			best_mode = intra_prediction(ori, ref, //input
				pred, resi, recon); //output

			for (m = 0; m < BLOCK_SIZE; m++)
			{

				for (n = 0; n < BLOCK_SIZE; n++)
				{

					img_pred[i + m][j + n] = pred[best_mode][m*BLOCK_SIZE + n];
					img_resi[i + m][j + n] = resi[best_mode][m*BLOCK_SIZE + n];
					img_recon[i + m][j + n] = recon[best_mode][m*BLOCK_SIZE + n];

				}

			}
		}
	}
	MemFree_2D(img_padding, HEIGHT + 1);
}

int intra_prediction(BYTE* ori, BYTE* ref, BYTE(pred)[NMODE][BLOCK_SIZE*BLOCK_SIZE], int(resi)[NMODE][BLOCK_SIZE*BLOCK_SIZE], BYTE(recon)[NMODE][BLOCK_SIZE*BLOCK_SIZE]) //output
{
	static int SAD[NMODE];
	int min_SAD, best_mode, i, j, m, n;

	SAD[0] = intra_ver(ori, ref, pred[0], resi[0], recon[0]);
	SAD[1] = intra_hor(ori, ref, pred[1], resi[1], recon[1]);
	SAD[2] = intra_dc(ori, ref, pred[2], resi[2], recon[2]);
	SAD[3] = intra_DL(ori, ref, pred[3], resi[3], recon[3]);
	SAD[4] = intra_DR(ori, ref, pred[4], resi[4], recon[4]);

	best_mode = 0;
	min_SAD = SAD[0];

	for (n = 1; n < NMODE; n++)
	{
		if (min_SAD > SAD[n])
		{
			min_SAD = SAD[n];
			best_mode = n;
		}
	}
	return best_mode;
}


int intra_dc(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int m, n;
	int temp1 = 0, temp2 = 0;   
	int dc_sum;       // dc값을 저장
	int   sad = 0;;   // sad값을 저장

	for (m = 0; m < BLOCK_SIZE * BLOCK_SIZE; m++)
	{

		temp1 += ref[m];   // 평균 계산을 위해 전부 더함

	}
	dc_sum = temp1 / 64;  //  평균을 구한다.


	for (m = 0; m < BLOCK_SIZE *BLOCK_SIZE; m++)
	{

		pred[m] = dc_sum;   //prediction의 값을 dc로 저장.
		resi[m] = ori[m] - pred[m];   // 원본에서 prediction을 빼준 값
		recon[m] = pred[m] + resi[m];  // prediction과 residual더 하여  reconstruction 값들을 얻어냄.

		if (resi[m] < 0) 
		{

			temp2 = resi[m] * (-1);   //절대값을 얻기위해 음수는 -1를 곱해준다

		}

		else
			temp2 = resi[m];

		sad += temp2;   // residual의 절대값을 더해 SAD값을 얻는다

	}


	return sad; 
}

int intra_hor(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int u, v;
	int temp1 = 0;  
	int sad = 0;; // sad값을 저장해주는 변수


	for (u = 0; u < BLOCK_SIZE; u++)
	{

		for (v = 0; v < BLOCK_SIZE; v++)
		{

			pred[8 * u + v] = ref[u + (2 * BLOCK_SIZE + 1)];   // ref[2 * BLOCK_SIZE(=8) + 1]번 부터는 수평의 reference값

		}

	}

	for (u = 0; u < BLOCK_SIZE*BLOCK_SIZE; u++)
	{

		resi[u] = ori[u] - pred[u];   // 원본에서 prediction을 빼준 값
		recon[u] = pred[u] + resi[u];   // prediction과 residual더 하여  reconstruction 값들을 얻어냄.

		if (resi[u] < 0)
		{

			temp1 = resi[u] * (-1);  //절대값을 얻기위해 음수는 -1를 곱해준다


		}
		else
			temp1 = resi[u];

		sad += temp1; // residual의 절대값을 더해 SAD값을 얻는다 


	}

	return sad;  
}

int intra_ver(BYTE* ori, BYTE* ref, //input
	BYTE* pred, int* resi, BYTE* recon) //output
{
	int x, y;
	int temp_1 = 0;
	int dc_sum, sad = 0;


	for (x = 0; x < BLOCK_SIZE; x++)
	{
		for (y = 0; y < BLOCK_SIZE; y++)
		{

			pred[8 * x + y] = ref[y + 1];  // ref[1]번 ~ ref[8]는 수직의 reference값

		}

	}

	for (x = 0; x < BLOCK_SIZE*BLOCK_SIZE; x++)
	{
		resi[x] = ori[x] - pred[x]; // 원본에서 prediction을 빼준 값
		recon[x] = pred[x] + resi[x];  // prediction과 residual더 하여  reconstruction 값들을 얻어냄.

		if (resi[x] < 0) {

			temp_1 = resi[x] * (-1);   //절대값을 얻기위해 음수는 -1를 곱해준다

		}
		else
			temp_1 = resi[x];

		sad += temp_1; // residual의 절대값을 더해 SAD값을 얻는다 

	}

	return sad; 

}

int intra_DL(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon) // Nake Predictors 에서 Diagonal Left Down
{

	int i, j;
	int temp1 = 0;
	int dc_sum, sad = 0;

	for (i = 0; i < BLOCK_SIZE*BLOCK_SIZE; i++) // ref값을 diagonal 방향으로 Prediction 변수 부분을 채워나감
	{

		if (i < BLOCK_SIZE)
		{

			pred[i] = ref[i + 2];

		}
		else if (i == 7)
		{

			pred[i] = ref[9];

		}
		else if (i == 15)
		{

			pred[i] = ref[10];

		}
		else if (i == 23)
		{

			pred[i] = ref[11];

		}
		else if (i == 31)
		{

			pred[i] = ref[12];

		}
		else if (i == 39)
		{

			pred[i] = ref[13];

		}
		else if (i == 47)
		{

			pred[i] = ref[14];

		}
		else if (i == 55)
		{

			pred[i] = ref[15];

		}
		else if (i == 63)
		{

			pred[i] = ref[16];

		}
		else
		{

			pred[i] = pred[i - 7];

		}

	}


	for (i = 0; i < BLOCK_SIZE*BLOCK_SIZE; i++)
	{
		resi[i] = ori[i] - pred[i]; // 원본에서 prediction을 빼준 값
		recon[i] = pred[i] + resi[i];  // prediction과 residual더 하여  reconstruction 값들을 얻어냄.

		if (resi[i] < 0)
		{

			temp1 = resi[i] * (-1);   //절대값을 얻기위해 음수는 -1를 곱해준다


		}
		else
			temp1 = resi[i];

		sad += temp1;  // residual의 절대값을 더해 SAD값을 얻는다 

	}


	return sad;   

}

int intra_DR(BYTE* ori, BYTE* ref, BYTE* pred, int* resi, BYTE*recon)
{
	int k, z;
	int temp1 = 0;
	int dc_sum, sad = 0;

	for (k = 0; k < BLOCK_SIZE*BLOCK_SIZE; k++) // ref값을 diagonal 방향으로 Prediction 변수 부분을 채워나감
	{
		if (k < BLOCK_SIZE)
		{

			pred[k] = ref[k];

		}
		else if (k == 8)
		{

			pred[k] = ref[17];

		}
		else if (k == 16)
		{

			pred[k] = ref[18];

		}
		else if (k == 24)
		{

			pred[k] = ref[19];

		}
		else if (k == 32)
		{

			pred[k] = ref[20];

		}
		else if (k == 40)
		{

			pred[k] = ref[21];

		}
		else if (k == 48)
		{

			pred[k] = ref[22];

		}
		else if (k == 56)
		{

			pred[k] = ref[23];

		}
		else
		{

			pred[k] = pred[k - 9];

		}


	}


	for (k = 0; k < BLOCK_SIZE*BLOCK_SIZE; k++)
	{
		resi[k] = ori[k] - pred[k]; // 원본에서 prediction을 빼준 값
		recon[k] = pred[k] + resi[k];  // prediction과 residual더 하여  reconstruction 값들을 얻어냄.

		if (resi[k] < 0) {
			temp1 = resi[k] * (-1);   //절대값을 얻기위해 음수는 -1를 곱해준다

		}
		else
			temp1 = resi[k];

		sad += temp1;  // residual의 절대값을 더해 SAD값을 얻는다 

	}


	return sad;

}