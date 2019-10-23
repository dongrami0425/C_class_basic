//DSPE_2012706067_�̵���_2����
// ���� assignment1�� �ߺ��Ǵ� �ּ����� ���� �ۼ��Ͽ����ϴ�.
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
	//*fp_in1  *fp_in2 *fp_out_1 *fp_out_2 => Lena�̹��� ����� ����
	//*fp_in3  *fp_in4 *fp_out_3 *fp_out_4 => Mit�̹��� ����� ���� 
	BYTE **img_in1 = 0, **img_in2 = 0, **img_out_1 = 0, **img_out_2 = 0, **img_out_3 = 0, **img_out_4 = 0;
	//**img_in1  **img_out_1 **img_out_2 => Lena�̹��� �޸��Ҵ� ����
	//**img_in2  **img_out_3 **img_out_4 => Mit�̹��� �޸��Ҵ� ����
	int i = 0;
	int j = 0;
	int temp_1 = 0, temp_2 = 0, temp_3 = 0, temp_4 = 0;

	//���� ����
	fp_in1 = fopen("Lena(512x512).raw", "rb");
	fp_in2 = fopen("Mit(512x512).raw", "rb");
	if (fp_in1 == NULL) {
		printf("File open failed\n");
	}

	//2D �Ҵ�
	img_in1 = (BYTE **)malloc(sizeof(BYTE*) * (HEIGHT + 1));  //�е��� ���� +1
	img_in2 = (BYTE **)malloc(sizeof(BYTE*) * (HEIGHT + 1));
	for (i = 0; i < HEIGHT + 1; i++) {
		img_in1[i] = (BYTE *)malloc(sizeof(BYTE) * (WIDTH + 1));
		img_in2[i] = (BYTE *)malloc(sizeof(BYTE) * (WIDTH + 1));
	}

	img_out_1 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);   //����������� �Ҵ�
	img_out_2 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);
	img_out_3 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);   //����������� �Ҵ�
	img_out_4 = (BYTE **)malloc(sizeof(BYTE*) * HEIGHT);
	for (i = 0; i < HEIGHT; i++) {
		img_out_1[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_2[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_3[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
		img_out_4[i] = (BYTE*)malloc(sizeof(BYTE) * WIDTH);
	}

	////fopen�� ����Ͽ� ������ �а� fp_in�� ����.
	for (i = 0; i < HEIGHT; i++) {
		fread(img_in1[i], sizeof(BYTE), WIDTH, fp_in1);
		fread(img_in2[i], sizeof(BYTE), WIDTH, fp_in2);
	}

	////  �̹����� �ο��� �Ҵ�޸𸮿� �°� �е�
	for (i = 0; i < HEIGHT; i++) {
		img_in1[i][WIDTH] = img_in1[i][WIDTH - 1];
		img_in2[i][WIDTH] = img_in2[i][WIDTH - 1];
	}

	for (i = 0; i < WIDTH + 1; i++) {
		img_in1[HEIGHT][i] = img_in1[HEIGHT - 1][i];
		img_in2[HEIGHT][i] = img_in2[HEIGHT - 1][i];
	}


	// ���μ���

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			temp_1 = img_in1[i][j] - img_in1[i][j + 1];            //f(x,y) - f(x+1,y) �������, ��������� ����ϴ� ����, 1���̺�
			temp_2 = img_in1[i][j] - img_in1[i + 1][j];            //f(x,y) - f(x,y+1) �������
			temp_3 = img_in2[i][j] - img_in2[i][j + 1];            //f(x,y) - f(x+1,y) �������
			temp_4 = img_in2[i][j] - img_in2[i + 1][j];            //f(x,y) - f(x,y+1) �������

			img_out_1[i][j] = (BYTE)floor((((temp_1 + 255) / 2.0) + 45)); //for dynamic range
			img_out_2[i][j] = (BYTE)floor((((temp_2 + 255) / 2.0) + 45)); //for dynamic range
			img_out_3[i][j] = (BYTE)floor((((temp_3 + 255) / 2.0) + 45)); //for dynamic range
			img_out_4[i][j] = (BYTE)floor((((temp_4 + 255) / 2.0) + 45)); //for dynamic range

			
		}
	}


	fp_out_1 = fopen("[output_vertical]Lena(512x512).raw", "wb"); // output ����
	fp_out_2 = fopen("[output_horizontal]Lena(512x512).raw", "wb");
	fp_out_3 = fopen("[output_vertical]Mit(512x512).raw", "wb"); // output ����
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