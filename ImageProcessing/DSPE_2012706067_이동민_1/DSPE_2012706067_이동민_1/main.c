//1주차_2012706067_이동민
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //Header file
#include<stdlib.h> //malloc, free를 쓰기위한 Header

#define N 512 //N=512로 정의

int main()
{
	FILE *fp_in = 0, *fp_out1 = 0, *fp_out2 = 0, *fp_out3 = 0; //FILE 포인터는 파일의 주소를 가리킴
	unsigned char *idata = 0, *odata1 = 0, *odata2 = 0; //unsigned char형식의 포인터 변수 선언
	int temp = 0; //int형 temp 변수 선언
	int idx = 0; //int형 idx 변수 선언

	fp_in = fopen("input1.raw", "rb"); //fopen을 사용하여 파일을 읽고 fp_in에 저장. 
	//fopen("파일이름","바이너리 파일 읽기 전용으로 열기")

	idata = (unsigned char *)malloc(sizeof(unsigned char)*N); //N크기만큼의 메모리를 동적할당

	/*메모리의 동적할당 malloc
	프로그램 실행 중 필요한만큼의 메모리를 할당 후 원할 때 해제하는
	동적할당이다. free로 반드시 메모리를 해제해줘야 한다.
	이후 쓰지않는 불필요한 메모리가 컴퓨터 메모리를 차지하여 문제가 될 수 있음*/

	fread(idata, sizeof(unsigned char), N, fp_in);
	//fp_in에서 unsigned char길이의 N항목까지 읽고 idata에 저장

	odata2 = (unsigned char *)malloc(sizeof(unsigned char)*(N - 1));
	//idata와 마찬가지로 동적메모리 할당. 크기는 N-1만큼이다.

	for (idx = 0; idx < N - 1; idx++) //odata2배열에 f(x)-f(x+1)연산 결과를 저장
	{
		temp = idata[idx] - idata[idx + 1];
		if (temp < 0)
		{
			temp = 0;
		}
		else if (temp > 255)
		{
			temp = 255;
		}
		odata2[idx] = (unsigned char)temp;
	}


	fp_out1 = fopen("output.raw", "wb"); //바이너리형식으로 output.raw로 저장
	fwrite(odata2, sizeof(unsigned char), N - 1, fp_out1);

	fp_out2 = fopen("output.xls", "wt"); //odata2배열의 결과 엑셀파일로 저장
	for (idx = 0; idx < N - 1; idx++)
	{
		fprintf(fp_out2, "%u\n", odata2[idx]);
	}

	odata1 = (unsigned char *)malloc(sizeof(unsigned char)*(N));

	for (idx = 0; idx < N; idx++)
	{
		temp = idata[idx];
		if (temp < 0)
		{
			temp = 0;
		}
		else if (temp > 255)
		{
			temp = 255;
		}
		odata1[idx] = (unsigned char)temp;
	}

	fp_out3 = fopen("input.xls", "wt"); //input1.xls파일이 없어서 같은 방식으로 생성
	for (idx = 0; idx < N - 1; idx++)
	{
		fprintf(fp_out3, "%u\n", odata1[idx]);
	}

	free(idata); //할당했던 메모리 해제
	free(odata2);
	free(odata1);
	fclose(fp_in); //fopen으로 연 파일 close
	fclose(fp_out1);
	fclose(fp_out2);
	fclose(fp_out3);

	return 0;
}
