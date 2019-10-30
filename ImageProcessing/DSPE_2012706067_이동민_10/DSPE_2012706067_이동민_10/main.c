//DSPE_2012706067_�̵���_10
// < Video file I/O >
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define WIDTH 352 // CIF ������ ������
#define HEIGHT 288

typedef unsigned char BYTE;

BYTE** MemAlloc_2D(int width, int height);
void MemFree_2D(BYTE** arr, int height);

int Read_Frame(FILE *fp_in, BYTE** img_in, int width, int height);
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height);

void Average_RGB(BYTE**img_in, BYTE** img_avg, int width, int height);

void cpy_frame(BYTE** img_src, BYTE** img_dst, int width, int height);
void sub_frame(BYTE** img_in, BYTE** img_prev, BYTE** img_out, int width, int height);

int main()
{
	FILE *fp_in = fopen("Suzie_CIF_150_30.rgb", "rb");
	FILE *fp_cpy_out = fopen("[Copy]Suzie_CIF.rgb", "wb");
	FILE *fp_avr_out = fopen("[Avr]Suzie_CIF.rgb", "wb");
	FILE *fp_sub_out = fopen("[Sub]Suzie_CIF.rgb", "wb");


	BYTE **img_out, **img_in, **img_prev;
	int read_size = 1, first_frame = 1;

	img_in = MemAlloc_2D(WIDTH, HEIGHT * 3);
	img_out = MemAlloc_2D(WIDTH, HEIGHT * 3);
	img_prev = MemAlloc_2D(WIDTH, HEIGHT * 3);

	//
	while (read_size = Read_Frame(fp_in, img_in, WIDTH, HEIGHT * 3))
	{
		Write_Frame(fp_cpy_out, img_in, WIDTH, HEIGHT * 3); // copy�� �������� ����

		Average_RGB(img_in, img_out, WIDTH, HEIGHT);  // rgb ������ ����� ���ϰ� ���� rgb�� �ٽ� �����Ѵ�.
		Write_Frame(fp_avr_out, img_out, WIDTH, HEIGHT * 3);  // ��հ��� ������ �������� ���ش�.

		if (first_frame == 1)     // ù��° �������� �� 
		{
			cpy_frame(img_in, img_prev, WIDTH, HEIGHT * 3);  // ���� �������� �����ϱ� (-)������ ���ϹǷ� ù��° �������� �����ؾ��Ѵ�. ���� ù�������� ����


			first_frame = 0;  // ���ǹ��� �����ϱ� ������ �ٸ� ����(else)���� �Ѿ�� ���ؼ� 0�� �־��ش�.
		}

		else{  // ù �������� �ƴ� ��
		
			sub_frame(img_in, img_prev, img_out, WIDTH, HEIGHT * 3); // ���� �����ӿ��� ���� �������� ����.
			Write_Frame(fp_sub_out, img_out, WIDTH, HEIGHT * 3);  // ������ ������ ���� �������ش�.

			cpy_frame(img_in, img_prev, WIDTH, HEIGHT * 3);  // ���� ���� �����۵��� ���� ����� �������� ī���Ѵ�.
		}


	}

	MemFree_2D(img_in, HEIGHT * 3);
	MemFree_2D(img_out, HEIGHT * 3);
	MemFree_2D(img_prev, HEIGHT * 3);

	fcloseall();

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
void MemFree_2D(BYTE** arr, int height)
{
	int i;

	for (i = 0; i < height; i++)
	{
		free(arr[i]);
	}
	free(arr);
}

int Read_Frame(FILE *fp_in, BYTE** img_in, int width, int height)
{
	int i, size = 0;

	for (i = 0; i < height; i++)
	{
		size += fread(img_in[i], sizeof(BYTE), width, fp_in);
	}

	return size;
}
void Write_Frame(FILE *fp_out, BYTE** img_in, int width, int height)
{
	int i, size = 0;

	for (i = 0; i < height; i++)
	{
		fwrite(img_in[i], sizeof(BYTE), width, fp_out);
	}
}


void Average_RGB(BYTE**img_in, BYTE** img_avg, int width, int height)  // RGB�� ����� �����ִ� �Լ�
{	int i, j;
	double average_rgb_temp;

	for (i = 0; i < height; i++){
		
		for (j = 0; j < width; j++){
			average_rgb_temp = (img_in[i][j] + img_in[i + height][j] + img_in[i + 2 * height][j]) / 3;  // ������ ����� rgb������ ���Ͽ� ��տ���

			// ������ ��հ����� r g b�� ���� ���ش�.
			img_avg[i][j] = average_rgb_temp;  
			img_avg[i + height][j] = average_rgb_temp; 
			img_avg[i + 2 * height][j] = average_rgb_temp;  
		}
	}
}


void cpy_frame(BYTE** img_src, BYTE** img_dst, int width, int height) // ���������Ӱ� ������������ ������ ���� �� ���� ������ ���� �������� �����صα� ���� �Լ�
{
	int i;

	for (i = 0; i < height; i++)
	{
		memcpy(img_dst[i], img_src[i], sizeof(BYTE)* width);
	}
}
void sub_frame(BYTE** img_in, BYTE** img_prev, BYTE** img_out, int width, int height) // ���翡�� ���� �������� ���ִ� �����Լ�
{
	int i, j;
	double sub_frame_temp;


	for (i = 0; i < height; i++){ // CIF�������� height���� HEIGHT *3(3���� ������)�� ������ ������ R G B�� �ش��ϴ� �ȼ����� ��� ���� �� �ִ�.
	
		for (j = 0; j < width; j++) {
			sub_frame_temp = img_in[i][j] - img_prev[i][j] + 128;   //T frame buffer - T-1 frame buffer + 128 = T frame buffer =(Write file)> T frame Output file

			if (sub_frame_temp > 255) {  // Clipping
				sub_frame_temp = 255;
			}
			else if (sub_frame_temp < 0) {
			sub_frame_temp = 0;
		    } 

			img_out[i][j] = sub_frame_temp;  // �ȼ� ���� img_out�� �������ش�.

		}
	}

}