//DSPE_2012706067_이동민_2주차
// 앞의 assignment1과 중복되는 주석들은 빼고 작성하였습니다.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>

#define WIDTH  512
#define HEIGHT  512

typedef unsigned char BYTE;

int main()
{
	FILE *fp_in1 = 0, *fp_in2 = 0, *fp_in3 = 0, *fp_in4 = 0, *fp_out_1 = 0, *fp_out_2 = 0, *fp_out_3, *fp_out_4;
	//*fp_in1  *fp_in2 *fp_out_1 *fp_out_2 => Lena이미지 입출력 변수
	//*fp_in3  *fp_in4 *fp_out_3 *fp_out_4 => Mit이미지 입출력 변수 
	BYTE **img_in1 = 0, **img_in2 = 0, **img_out_1 = 0, **img_out_2 = 0, **img_out_3 = 0, **img_out_4 = 0;
	//**img_in1  **img_out_1 **img_out_2 => Lena이미지 메모리할당 변수
	//**img_in2  **img_out_3 **img_out_4 => Mit이미지 메모리할당 변수
	int i = 0;
	int j = 0;
	int temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0;

	//파일 오픈
	fp_in1 = fopen("Lena(512x512).raw", "rb");
	fp_in2 = fopen("Mit(512x512).raw", "rb");
	if (fp_in1 == NULL) {
		printf("File open failed\n");
	}

	//2D 할당
	img_in1 = (BYTE **)malloc(sizeof(BYTE*) * (HEIGHT + 1));  //패딩을 위해 +1
	img_in2 = (BYTE **)malloc(sizeof(BYTE*) * (HEIGHT + 1));
	for (i = 0; i < HEIGHT + 1; i++) {
		img_in1[i] = (BYTE *)malloc(sizeof(BYTE) * (WIDTH + 1));
		img_in2[i] = (BYTE *)malloc(sizeof(BYTE) * (WIDTH + 1));
	}

	img_out_1 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);   //원본사이즈로 할당
	img_out_2 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);
	img_out_3 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);   //원본사이즈로 할당
	img_out_4 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);
	for (i = 0; i < HEIGHT; i++) {
		img_out_1[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_2[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_3[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_4[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
	}

	////fopen을 사용하여 파일을 읽고 fp_in에 저장.
	for (i = 0; i < HEIGHT; i++) {
		fread(img_in1[i], sizeof(BYTE), WIDTH, fp_in1);
		fread(img_in2[i], sizeof(BYTE), WIDTH, fp_in2);
	}

	////  이미지를 부여한 할당메모리에 맞게 패딩
	for (i = 0; i < HEIGHT; i++) {
		img_in1[i][WIDTH] = img_in1[i][WIDTH - 1];
		img_in2[i][WIDTH] = img_in2[i][WIDTH - 1];
	}

	for (i = 0; i < WIDTH + 1; i++) {
		img_in1[HEIGHT][i] = img_in1[HEIGHT - 1][i];
		img_in2[HEIGHT][i] = img_in2[HEIGHT - 1][i];
	}


	// 프로세싱

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			temp_1 = img_in1[i][j] - img_in1[i][j + 1];            //f(x,y) - f(x+1,y) 연산수행, 엣지검출시 사용하는 수식, 1차미분
			temp_2 = img_in1[i][j] - img_in1[i + 1][j];            //f(x,y) - f(x,y+1) 연산수행
			temp_3 = img_in2[i][j] - img_in2[i][j + 1];            //f(x,y) - f(x+1,y) 연산수행
			temp_4 = img_in2[i][j] - img_in2[i + 1][j];            //f(x,y) - f(x,y+1) 연산수행

			img_out_1[i][j] = (BYTE)floor((((temp_1 + 255) / 2.0) + 45)); //for dynamic range
			img_out_2[i][j] = (BYTE)floor((((temp_2 + 255) / 2.0) + 45)); //for dynamic range
			img_out_3[i][j] = (BYTE)floor((((temp_3 + 255) / 2.0) + 45)); //for dynamic range
			img_out_4[i][j] = (BYTE)floor((((temp_4 + 255) / 2.0) + 45)); //for dynamic range

			
		}
	}


	fp_out_1 = fopen("[output_vertical]Lena(512x512).raw", "wb"); // output 생성
	fp_out_2 = fopen("[output_horizontal]Lena(512x512).raw", "wb");
	fp_out_3 = fopen("[output_vertical]Mit(512x512).raw", "wb"); // output 생성
	fp_out_4 = fopen("[output_horizontal]Mit(512x512).raw", "wb");


	for (i = 0; i < HEIGHT; i++)
	{
		fwrite(img_out_1[i], sizeof(BYTE), WIDTH, fp_out_1);
		fwrite(img_out_2[i], sizeof(BYTE), WIDTH, fp_out_2);
		fwrite(img_out_3[i], sizeof(BYTE), WIDTH, fp_out_3);
		fwrite(img_out_4[i], sizeof(BYTE), WIDTH, fp_out_4);
	}

	

	return 0;

}