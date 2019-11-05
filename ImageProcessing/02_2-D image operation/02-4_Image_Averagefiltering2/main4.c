//DSPE_2012706067_이동민_2주차
// < 2-D Digital Data Operation >
// - 7x7 average filter
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define WIDTH 512
#define HEIGH 512
#define mask_size 7 // 5x5 마스크 사용시 5로 수정

typedef unsigned char BYTE;

int main()
{
	FILE *fp_in = 0, *fp_out = 0;
	BYTE **img_in = 0, **img_out = 0;
	float temp = 0;
	int i = 0, j = 0, m = 0, n = 0;

	FILE* fp_test;

	fp_in = fopen("Lena(512x512).raw", "rb");
	if (fp_in == NULL)
	{
		printf("File open failed\n");
	}

	img_in = (BYTE**)malloc(sizeof(BYTE*) * (HEIGH + mask_size-1));
	for (i = 0; i < HEIGH + mask_size-1; i++)
	{
		img_in[i] = (BYTE*)malloc(sizeof(BYTE) * (WIDTH + mask_size-1));
	}
	for (i = 0; i < HEIGH; i++) {
		fread(img_in[i], sizeof(BYTE), WIDTH, fp_in);
	}
	/////////////////////////////////////////////////////////////////
	for (i = 0; i < HEIGH; i++) //padding
	{

		for (j = 0; j < mask_size-1; j++)
		{
			img_in[i+][WIDTH + j] = img_in[i][WIDTH - 1];
		}
	}

	for (j = 0; j < WIDTH; j++)
	{

		for (i = 0; i < mask_size-1; i++)
		{
			img_in[HEIGH + i][j] = img_in[HEIGH - 1][j];
		}
	}

	for (i = 0; i < mask_size-1; i++)
	{

		for (j = 0; j < mask_size-1; j++)
		{
			img_in[HEIGH + i][WIDTH + j] = img_in[HEIGH + i][WIDTH - 1];
		}
	}
	//////////////////////////////////
	//			processing			//

	img_out = (BYTE **)malloc(sizeof(BYTE*)*HEIGH);
	for (i = 0; i < HEIGH; i++)
	{
		img_out[i] = (BYTE*)malloc(sizeof(BYTE)*WIDTH);
	}

	for (i = 0; i < HEIGH; i++)
	{

		for (j = 0; j < WIDTH; j++)
		{
			temp = 0;
			for (m = 0; m < mask_size; m++)
			{

				for (n = 0; n < mask_size; n++)
				{
					temp += img_in[i + m][j + n];

				}
			}
			img_out[i][j] = (BYTE)(temp / (mask_size*mask_size));
		}
	}

	fp_out = fopen("[Output_ave7x7]Lena(512x512).raw", "wb");
	if (fp_out == NULL)
	{
		printf("File open failed\n");
	}
	for (i = 0; i < HEIGH; i++)
	{
		fwrite(img_out[i], sizeof(BYTE), WIDTH, fp_out);
	}

	for (i = 0; i < HEIGH; i++)
	{
		free(img_in[i]);
		if (i < HEIGH)
			free(img_out[i]);

	}
	free(img_in);
	free(img_out);

	fclose(fp_in);
	fclose(fp_out);

	return 0;
}