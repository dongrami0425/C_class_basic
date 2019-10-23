//DSPE_2012706067_이동민_13
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


#define WIDTH 352
#define HEIGHT 288

#define BLOCK_SIZE 4
#define SR 16 

#define cWIDTH (WIDTH>>1) //Chroma frame size
#define cHEIGHT (HEIGHT>>1) //Chroma frame size


#define cBLOCK_SIZE (BLOCK_SIZE>>1) //Chroma prediction block size
#define cSR (SR>>1) //Chroma search Range

#define Clip(x) (x<0?0:(x>255?255:x))

typedef unsigned char BYTE;

typedef struct MV //motion vector structure
{
	int x, y;
}MV;

BYTE** MemAlloc_2D(int width, int height); // in : RGB, out : YUV, RGB
void MemFree_2D(BYTE** arr, int height); //동적 메모리 할당 해제

float GetPSNR(BYTE** img_ori, BYTE** img_dist, int width, int height); // PSNR value

int Read_Frame(FILE *fp_in, BYTE** img_in, int width, int height); //1 frame read from input file
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height); //1frame write on output file
void RGB_to_YUV(BYTE** img_in, BYTE** img_out, int width, int height); //image color conversion RGB to YUV
void YUV_to_RGB(BYTE** img_in, BYTE** img_out, int width, int height);  //image coloe consersion YUV to RGB


void YUV444_to_420(BYTE** img_in, BYTE** img_Y, BYTE** img_U420, BYTE** img_V420, int width, int height);   //Chroma sampling 4:4:4 -> 4:2:0
void YUV420_to_444(BYTE** img_Y, BYTE** img_U420, BYTE** img_V420, BYTE** img_out, int width, int height);  //Chroma sampling 4:2:0 -> 4:4:4

void InterPrediction(BYTE** img_ori, BYTE** img_ref, BYTE** img_pred, BYTE** img_resi, BYTE** img_recon, int width, int height, int block_size, int search_range); //

int main()
{
	FILE *fp_in0 = fopen("Suzie_CIF_1.rgb", "rb");  // in frame number 1  RGB file
	FILE *fp_in1 = fopen("Suzie_CIF_13.rgb", "rb"); // in frame number 13 RGB file
	FILE *fp_out0 = fopen("[predc]Suzie_CIF_13.rgb", "wb"); //Predictor RGB file
	FILE *fp_out1 = fopen("[Resid]Suzie_CIF_13.rgb", "wb"); //Residual RGB file
	FILE *fp_out2 = fopen("[Recon]Suzie_CIF_13.rgb", "wb"); //recon RGB file

	BYTE **img_YUV444, **img_RGB; //input original RGB, YUV444
	BYTE **img_ref_Y, **img_ref_U, **img_ref_V; //input reference YUV420
	BYTE **img_ori_Y, **img_ori_U, **img_ori_V; //input original YUV420

	BYTE **img_recon_Y, **img_recon_U, **img_recon_V; //recon pointer
	BYTE **img_pred_Y, **img_pred_U, **img_pred_V;    //prediction pointer
	BYTE **img_resi_Y, **img_resi_U, **img_resi_V;    //residual pointer

	img_YUV444 = MemAlloc_2D(WIDTH, HEIGHT * 3); //YUV 444 memory
	img_RGB = MemAlloc_2D(WIDTH, HEIGHT * 3);    //RGB memory

	img_pred_Y = MemAlloc_2D(WIDTH, HEIGHT);  // Y component memory
	img_recon_Y = MemAlloc_2D(WIDTH, HEIGHT); // Y component memory
	img_resi_Y = MemAlloc_2D(WIDTH, HEIGHT);  // Y component memory

	img_pred_U = MemAlloc_2D(cWIDTH, cHEIGHT);  // U component memory
	img_recon_U = MemAlloc_2D(cWIDTH, cHEIGHT); // U component memory
	img_resi_U = MemAlloc_2D(cWIDTH, cHEIGHT);  // U component memory

	img_pred_V = MemAlloc_2D(cWIDTH, cHEIGHT);  // V component memory
	img_recon_V = MemAlloc_2D(cWIDTH, cHEIGHT); // V component memory
	img_resi_V = MemAlloc_2D(cWIDTH, cHEIGHT);  // V component memory

	//YUV 420 memory
	img_ref_Y = MemAlloc_2D(WIDTH, HEIGHT); //reference picture memory
	img_ref_U = MemAlloc_2D(cWIDTH, cHEIGHT); //reference picture memory
	img_ref_V = MemAlloc_2D(cWIDTH, cHEIGHT); //reference picture memory

	img_ori_Y = MemAlloc_2D(WIDTH, HEIGHT); //original picture memory
	img_ori_U = MemAlloc_2D(cWIDTH, cHEIGHT); //original picture memory
	img_ori_V = MemAlloc_2D(cWIDTH, cHEIGHT); //original picture memory

	///////////////////////////////////////////
	Read_Frame(fp_in0, img_RGB, WIDTH, HEIGHT * 3); //read reference picture
	RGB_to_YUV(img_RGB, img_YUV444, WIDTH, HEIGHT); //color conversion
	YUV444_to_420(img_YUV444, img_ref_Y, img_ref_U, img_ref_V, WIDTH, HEIGHT); //input reference data

	Read_Frame(fp_in1, img_RGB, WIDTH, HEIGHT * 3); //read original picture
	RGB_to_YUV(img_RGB, img_YUV444, WIDTH, HEIGHT); //color conversion
	YUV444_to_420(img_YUV444, img_ori_Y, img_ori_U, img_ori_V, WIDTH, HEIGHT); //input original data

	//Inter-Prediction of the Y conponent
	InterPrediction(img_ori_Y, img_ref_Y, img_pred_Y, img_resi_Y, img_recon_Y, WIDTH, HEIGHT, BLOCK_SIZE, SR);
	//Inter-Prediction of the U conponent
	InterPrediction(img_ori_U, img_ref_U, img_pred_U, img_resi_U, img_recon_U, cWIDTH, cHEIGHT, cBLOCK_SIZE, cSR);
	//Inter-Prediction of the V conponent
	InterPrediction(img_ori_V, img_ref_V, img_pred_V, img_resi_V, img_recon_V, cWIDTH, cHEIGHT, cBLOCK_SIZE, cSR);

	printf("Predicted Y conponent PSNR value : %.3f\n", GetPSNR(img_ori_Y, img_pred_Y, WIDTH, HEIGHT));
	printf("Predicted U conponent PSNR value : %.3f\n", GetPSNR(img_ori_U, img_pred_U, cWIDTH, cHEIGHT));
	printf("Predicted V conponent PSNR value : %.3f\n\n", GetPSNR(img_ori_V, img_pred_V, cWIDTH, cHEIGHT));

	printf("Reconstructed Y conponent PSNR value : %.3f\n", GetPSNR(img_ori_Y, img_recon_Y, WIDTH, HEIGHT));
	printf("Reconstructed U conponent PSNR value : %.3f\n", GetPSNR(img_ori_U, img_recon_U, cWIDTH, cHEIGHT));
	printf("Reconstructed V conponent PSNR value : %.3f\n\n", GetPSNR(img_ori_V, img_recon_V, cWIDTH, cHEIGHT));

	YUV420_to_444(img_pred_Y, img_pred_U, img_pred_V, img_YUV444, WIDTH, HEIGHT); //upsampling & write file
	YUV_to_RGB(img_YUV444, img_RGB, WIDTH, HEIGHT);
	Write_Frame(fp_out0, img_RGB, WIDTH, HEIGHT * 3);

	YUV420_to_444(img_resi_Y, img_resi_U, img_resi_V, img_YUV444, WIDTH, HEIGHT);
	YUV_to_RGB(img_YUV444, img_RGB, WIDTH, HEIGHT);
	Write_Frame(fp_out1, img_RGB, WIDTH, HEIGHT * 3);

	YUV420_to_444(img_recon_Y, img_recon_U, img_recon_V, img_YUV444, WIDTH, HEIGHT);
	YUV_to_RGB(img_YUV444, img_RGB, WIDTH, HEIGHT);
	Write_Frame(fp_out2, img_RGB, WIDTH, HEIGHT * 3);

	//////////////////////////////////////////////////

	//memory free
	MemFree_2D(img_YUV444, HEIGHT * 3);
	MemFree_2D(img_RGB, HEIGHT * 3);

	MemFree_2D(img_ref_Y, HEIGHT);
	MemFree_2D(img_ref_U, cHEIGHT);
	MemFree_2D(img_ref_V, cHEIGHT);

	MemFree_2D(img_ori_Y, HEIGHT);
	MemFree_2D(img_ori_U, cHEIGHT);
	MemFree_2D(img_ori_V, cHEIGHT);

	MemFree_2D(img_pred_Y, HEIGHT);
	MemFree_2D(img_pred_U, cHEIGHT);
	MemFree_2D(img_pred_V, cHEIGHT);

	MemFree_2D(img_resi_Y, HEIGHT);
	MemFree_2D(img_resi_U, cHEIGHT);
	MemFree_2D(img_resi_V, cHEIGHT);

	MemFree_2D(img_recon_Y, HEIGHT);
	MemFree_2D(img_recon_U, cHEIGHT);
	MemFree_2D(img_recon_V, cHEIGHT);

	_fcloseall(); //file close

	return 0;
}

float GetPSNR(BYTE ** img_ori, BYTE ** img_dist, int width, int height) //psnr calculation
{
	float mse = 0;
	int i, j;

	for (i = 0; i < height; i++) //mse calculation
	{
		for (j = 0; j < width; j++)
		{
			mse += ((img_ori[i][j] - img_dist[i][j])*(img_ori[i][j] - img_dist[i][j])) / (float)(width*height);
		}
	}
	return 10 * (float)log10((255 * 255) / mse); //get psnr
}


BYTE ** MemAlloc_2D(int width, int height) //2d memory allocation
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

void MemFree_2D(BYTE ** arr, int height) //2d memory free
{
	int i;
	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

// 1frame read from input file
int Read_Frame(FILE * fp_in, BYTE ** img_in, int width, int height)
{
	int i, size = 0;
	for (i = 0; i < height; i++)
	{
		size += fread(img_in[i], sizeof(BYTE), width, fp_in); //accululate the reading size
	}
	return size;
}

// 1framme write on output file
void Write_Frame(FILE * fp_out, BYTE ** img_in, int width, int height)
{
	int i;
	for (i = 0; i < height; i++)
	{
		fwrite(img_in[i], sizeof(BYTE), width, fp_out); //write on the output file
	}
}

void RGB_to_YUV(BYTE ** img_in, BYTE ** img_out, int width, int height)
{
	int i, j;
	int w[9] = { 66,129,25,-38,-74,112,112,-94,-18 }; //weight
	int temp[3] = { 0, };

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			temp[0] = w[0] * img_in[i][j] + w[1] * img_in[i + height][j] + w[2] * img_in[i + height * 2][j] + 128;
			temp[1] = w[3] * img_in[i][j] + w[4] * img_in[i + height][j] + w[5] * img_in[i + height * 2][j] + 128;
			temp[2] = w[6] * img_in[i][j] + w[7] * img_in[i + height][j] + w[8] * img_in[i + height * 2][j] + 128;

			img_out[i][j] = (BYTE)(temp[0] >> 8) + 16;
			img_out[i + height][j] = (BYTE)(temp[1] >> 8) + 128;
			img_out[i + 2 * height][j] = (BYTE)(temp[2] >> 8) + 128;
		}
	}
}

void YUV_to_RGB(BYTE ** img_in, BYTE ** img_out, int width, int height)
{
	int i, j;
	int w[5] = { 298,409,-100,-208,516 }; //weight
	int temp[3] = { 0, };
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			temp[0] = w[0] * (img_in[i][j] - 16) + w[1] * (img_in[i + height * 2][j] - 128) + 128;
			temp[1] = w[0] * (img_in[i][j] - 16) + w[2] * (img_in[i + height][j] - 128) + w[3] * (img_in[i + 2 * height][j] - 128) + 128;
			temp[2] = w[0] * (img_in[i][j] - 16) + w[4] * (img_in[i + height][j] - 128) + 128;

			img_out[i][j] = (BYTE)Clip((temp[0] >> 8));
			img_out[i + height][j] = (BYTE)Clip((temp[1] >> 8));
			img_out[i + 2 * height][j] = (BYTE)Clip((temp[2] >> 8));
		}
	}
}

//yuv444 -> yuv420
void YUV444_to_420(BYTE ** img_in, BYTE ** img_Y, BYTE ** img_U420, BYTE ** img_V420, int width, int height)
{
	int i, j; //loop index

	//Y component copy
	for (i = 0; i < height; i++)
	{
		memcpy(img_Y[i], img_in[i], sizeof(BYTE)*width);
	}

	//chroma sub sampling
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			img_U420[i >> 1][j >> 1] = (BYTE)((img_in[i + height][j] + img_in[i + height + 1][j]) / 2); //Cb calculation
			img_V420[i >> 1][j >> 1] = (BYTE)((img_in[i + height * 2][j] + img_in[i + height * 2 + 1][j]) / 2); //Cr calculation
		}
	}
}


//yuv 420 -> yuv 444
void YUV420_to_444(BYTE ** img_Y, BYTE ** img_U420, BYTE ** img_V420, BYTE ** img_out, int width, int height)
{
	int i, j, m, n;

	//Y component copy
	for (i = 0; i < height; i++)
	{
		memcpy(img_out[i], img_Y[i], sizeof(BYTE)*width);
	}

	//chroma recon
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			for (m = 0; m < 2; m++)
			{
				for (n = 0; n < 2; n++)
				{
					img_out[i + m + height][j + n] = img_U420[i >> 1][j >> 1]; //Cb copy interpolation
					img_out[i + m + height * 2][j + n] = img_V420[i >> 1][j >> 1]; //Cr copy interpolation
				}
			}
		}
	}
}

/*
Inter-prediction function
input : original image, reference image, image width & height, prediction block size, maximum search range
output : prediction image, residual image, reconstruction image
*/

void InterPrediction(BYTE ** img_ori, BYTE ** img_ref, BYTE ** img_pred, BYTE ** img_resi, BYTE ** img_recon, int width, int height, int block_size, int search_range)
{
	int i, j, m, n, x, y;                                     //Loop index
	int k, l;                                                 //motion vector position
	int SR_left = 0, SR_right = 0, SR_top = 0, SR_bottom = 0; //Search range variable
	int temp_resi;                                            //residual temporal memory

	float temp = 0;

	float min_MAE;  //memory for minimun value
	float temp_MAE; //MAE temporal memory

	MV mv[HEIGHT / BLOCK_SIZE][WIDTH / BLOCK_SIZE]; //motion vector memory

	//motion vector initialization
	for (i = 0; i < height; i += block_size) //BLOCK_SIZE=4  cBLOCK_SIZE=2
	{
		for (j = 0; j < width; j += block_size)
		{
			
			k = (int)(i / block_size);
			l = (int)(j / block_size);

			//Motion Vector (x,y)=(0,0)
			mv[k][l].x = 0; 
			mv[k][l].y = 0;

			//////Adaptive Search Range Decision & Motion Estimation Code
			SR_left = search_range; //SR=16 cSR=8
			SR_right = search_range;
			SR_top = search_range;
			SR_bottom = search_range;

			//SR영역이 이미지밖으로 될경우  SR영역을 이미지안으로 제한하는 코드
			if (j - SR_left < 0) //
				SR_left = j;

			if (j + block_size - 1 + SR_right > width)
				SR_right = width - j - block_size;

			if (i - SR_top < 0)
				SR_top = i;

			if (i + block_size - 1 + SR_bottom > height)
				SR_bottom = height - i - block_size;



			min_MAE = 1000;

			for (x = (-1) * SR_top; x <= SR_bottom; x++)
			{
				for (y = (-1) * SR_left; y <= SR_right; y++)
				{

					temp_MAE = 0;
					for (m = 0; m < block_size; m++)
					{
						for (n = 0; n < block_size; n++)
						{

							temp_MAE += (float)abs((img_ori[i + m][j + n] - img_ref[i + x + m][j + y + n])); //Search range내에서 원본과 ref를 빼준다.

						}
					}

					temp_MAE /= (float)(block_size * block_size);// block사이즈 만큼 평균내어 에러값을 찾는다.

					if (temp_MAE < min_MAE) //일정 수치만큼 제한한 min_MAE값이하라면 그값을 min_MAE에 저장한다.
					{
						min_MAE = temp_MAE;

						mv[k][l].x = x; //이때의 모션벡터값을 저장한다.
						mv[k][l].y = y;
					}


				}
			}
					   
			//Best prediction & reconstruction block copy
			for (m = 0; m < block_size; m++)
			{
				for (n = 0; n < block_size; n++)
				{
					img_pred[i + m][j + n] = img_ref[i + m + mv[k][l].x][j + n + mv[k][l].y];
					temp_resi = img_ori[i + m][j + n] - img_pred[i + m][j + n];
					img_recon[i + m][j + n] = temp_resi + img_pred[i + m][j + n];

					img_resi[i + m][j + n] = Clip(temp_resi + 128);
				}
			}
		}

	}
}