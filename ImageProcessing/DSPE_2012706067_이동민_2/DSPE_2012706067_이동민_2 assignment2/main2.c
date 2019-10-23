//DSPE_2012706067_이동민_2주차
// 앞의 assignment1과 중복되는 주석들은 빼고 작성하였습니다.
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define WIDTH 512
#define HEIGH 512

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

	img_in = (BYTE**)malloc(sizeof(BYTE*) * (HEIGH + 6)); //이미지 처리를 위해 메모리를 할당, 7x7의경우 (HEIGH + 6) , 5x5의경우 (HEIGH + 4)
	for (i = 0; i < HEIGH + 6; i++)
	{
		img_in[i] = (BYTE*)malloc(sizeof(BYTE) * (WIDTH + 6));  //이미지 처리를 위해 메모리를 할당, 7x7의경우 (WIDTH + 6) , 5x5의경우 (WIDTH + 4)
	}
	for (i = 0; i < HEIGH; i++) {
		fread(img_in[i], sizeof(BYTE), WIDTH, fp_in);
	}
	/////////////////////////////////////////////////////////////////
	for (i = 0; i < HEIGH; i++) //  이미지를 부여한 할당메모리에 맞게 패딩
	{

		for (j = 0; j < 6; j++) //7x7의경우(j>6), 5x5의경우(j>4) ,세로방향패딩
		{
			img_in[i][WIDTH + j] = img_in[i][WIDTH - 1];
		}
	}

	for (j = 0; j < WIDTH; j++)
	{

		for (i = 0; i < 6; i++) //7x7의경우(i>6), 5x5의경우(i>4), 가로방향 패딩
		{
			img_in[HEIGH + i][j] = img_in[HEIGH - 1][j];
		}
	}

	for (i = 0; i < 6; i++) //7x7의경우(i>6), 5x5의경우(i>4), 오른쪽아래부분 패딩
	{

		for (j = 0; j < 6; j++)
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

	for (i = 0; i < HEIGH; i++) // 7x7 average연산과정
	{

		for (j = 0; j < WIDTH; j++)
		{
			temp = 0;
			for (m = 0; m < 7; m++) //7x7의경우(m > 7), 5x5의경우(m > 5)
			{

				for (n = 0; n < 7; n++) //7x7의경우(n > 7), 5x5의경우(n > 5)
				{
					temp += img_in[i + m][j + n];

				}
			}
			img_out[i][j] = (BYTE)(temp / 49); //7x7의경우 (temp / 49), 5x5의경우(temp / 25)
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