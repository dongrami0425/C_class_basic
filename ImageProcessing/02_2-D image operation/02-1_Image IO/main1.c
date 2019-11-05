//DSPE_2012706067_이동민_2주차
// < 2-D Digital Data Operation >
// - Imgage file I/O and Memory alloacation
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //헤더파일
#include<stdlib.h>

#define WIDTH 512 //이미지 사이즈 => 함수 매크로 사용.(함수처럼 매크로가 변수를 가지는 것.)
#define HEIGH 512

typedef unsigned char BYTE;

int main()
{
	FILE *fp_in = 0, *fp_out = 0; //파일포인터
	BYTE **img_in = 0, **img_out = 0; 
	int i = 0 , j = 0; //int형 temp 변수 선언
	 
	fp_in = fopen("Lena(512x512).raw", "rb"); //fopen을 사용하여 파일을 읽고 fp_in에 저장. 
	if (fp_in == NULL)
	{
		printf("File open failed\n");
	}

	img_in = (BYTE**)malloc(sizeof(BYTE*)*HEIGH); //이미지사이즈만큼의 메모리를 동적할당
	for (i = 0; i < HEIGH; i++)
	{
		img_in[i] = (BYTE*)malloc(sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGH; i++){                   //fp_in에서 이미지사이즈만큼 읽고 img_in에 저장
		fread(img_in[i], sizeof(BYTE), WIDTH, fp_in);
	}

	img_out = (BYTE **)malloc(sizeof(BYTE*)*HEIGH);  //이미지사이즈만큼의 메모리를 출력img_out으로 동적할당
	for (i = 0; i < HEIGH; i++) 
	{
		img_out[i] = (BYTE*)malloc(sizeof(BYTE)*WIDTH);  //이미지사이즈만큼의 메모리를 출력img_out으로 동적할당
	}
	for (i = 0; i < HEIGH; i++) // img_in의 이미지정보를 img_out에저장
	{
		for (j = 0; j < WIDTH; j++)
		{
			img_out[i][j] = img_in[i][j];
		}
	}
	fp_out = fopen("[Output]Lena(512x512).raw", "wb"); //바이너리형식으로 output.raw로 저장
	if (fp_out == NULL)
	{
		printf("File open failed\n");
	}
	for (i = 0; i < HEIGH; i++)
	{
		fwrite(img_out[i], sizeof(BYTE), WIDTH, fp_out);
	}

	for (i = 0; i < HEIGH; i++) //할당했던 메모리 해제
	{
		free(img_in[i]);
		free(img_out[i]);
	}
	free(img_in); //할당했던 메모리 해제
	free(img_out);

	fclose(fp_in); //fopen으로 연 파일 close
	fclose(fp_out);

	return 0;

	
}