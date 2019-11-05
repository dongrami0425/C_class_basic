//DSPE_2012706067_이동민_10
// < Video file I/O >
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define WIDTH 352 // CIF 프레임 사이즈
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
		Write_Frame(fp_cpy_out, img_in, WIDTH, HEIGHT * 3); // copy한 프레임을 쓴다

		Average_RGB(img_in, img_out, WIDTH, HEIGHT);  // rgb 값들의 평균을 구하고 값을 rgb에 다시 저장한다.
		Write_Frame(fp_avr_out, img_out, WIDTH, HEIGHT * 3);  // 평균값을 적용한 프레임을 써준다.

		if (first_frame == 1)     // 첫번째 프레임일 때 
		{
			cpy_frame(img_in, img_prev, WIDTH, HEIGHT * 3);  // 이전 프레임이 없으니까 (-)연산을 못하므로 첫번째 프레임은 저장해야한다. 따라서 첫프레임을 저장


			first_frame = 0;  // 조건문을 적용하기 때문에 다른 조건(else)으로 넘어가기 위해서 0을 넣어준다.
		}

		else{  // 첫 프레임이 아닐 때
		
			sub_frame(img_in, img_prev, img_out, WIDTH, HEIGHT * 3); // 현재 프레임에서 이전 프레임을 뺀다.
			Write_Frame(fp_sub_out, img_out, WIDTH, HEIGHT * 3);  // 연산후 프레임 값을 저장해준다.

			cpy_frame(img_in, img_prev, WIDTH, HEIGHT * 3);  // 이후 같은 연산작동을 위해 계산한 프레임을 카피한다.
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


void Average_RGB(BYTE**img_in, BYTE** img_avg, int width, int height)  // RGB의 평균을 구해주는 함수
{	int i, j;
	double average_rgb_temp;

	for (i = 0; i < height; i++){
		
		for (j = 0; j < width; j++){
			average_rgb_temp = (img_in[i][j] + img_in[i + height][j] + img_in[i + 2 * height][j]) / 3;  // 각각에 저장된 rgb값들을 취하여 평균연산

			// 각각의 평균값들을 r g b에 저장 해준다.
			img_avg[i][j] = average_rgb_temp;  
			img_avg[i + height][j] = average_rgb_temp; 
			img_avg[i + 2 * height][j] = average_rgb_temp;  
		}
	}
}


void cpy_frame(BYTE** img_src, BYTE** img_dst, int width, int height) // 이전프레임과 현재프레임의 연산이 끝난 후 다음 연산을 위해 프레임을 저장해두기 위한 함수
{
	int i;

	for (i = 0; i < height; i++)
	{
		memcpy(img_dst[i], img_src[i], sizeof(BYTE)* width);
	}
}
void sub_frame(BYTE** img_in, BYTE** img_prev, BYTE** img_out, int width, int height) // 현재에서 이전 프레임을 뺴주는 연산함수
{
	int i, j;
	double sub_frame_temp;


	for (i = 0; i < height; i++){ // CIF프레임의 height에는 HEIGHT *3(3개의 프레임)이 들어오기 때문에 R G B에 해당하는 픽셀값을 모두 구할 수 있다.
	
		for (j = 0; j < width; j++) {
			sub_frame_temp = img_in[i][j] - img_prev[i][j] + 128;   //T frame buffer - T-1 frame buffer + 128 = T frame buffer =(Write file)> T frame Output file

			if (sub_frame_temp > 255) {  // Clipping
				sub_frame_temp = 255;
			}
			else if (sub_frame_temp < 0) {
			sub_frame_temp = 0;
		    } 

			img_out[i][j] = sub_frame_temp;  // 픽셀 값을 img_out에 저장해준다.

		}
	}

}