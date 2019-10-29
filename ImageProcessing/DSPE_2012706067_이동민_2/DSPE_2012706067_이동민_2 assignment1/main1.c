//DSPE_2012706067_�̵���_2����
// < 2-D Digital Data Operation >
// - Imgage file I/O and Memory alloacation
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //�������
#include<stdlib.h>

#define WIDTH 512 //�̹��� ������ => �Լ� ��ũ�� ���.(�Լ�ó�� ��ũ�ΰ� ������ ������ ��.)
#define HEIGH 512

typedef unsigned char BYTE;

int main()
{
	FILE *fp_in = 0, *fp_out = 0; //����������
	BYTE **img_in = 0, **img_out = 0; 
	int i = 0 , j = 0; //int�� temp ���� ����
	 
	fp_in = fopen("Lena(512x512).raw", "rb"); //fopen�� ����Ͽ� ������ �а� fp_in�� ����. 
	if (fp_in == NULL)
	{
		printf("File open failed\n");
	}

	img_in = (BYTE**)malloc(sizeof(BYTE*)*HEIGH); //�̹��������ŭ�� �޸𸮸� �����Ҵ�
	for (i = 0; i < HEIGH; i++)
	{
		img_in[i] = (BYTE*)malloc(sizeof(BYTE)*WIDTH);
	}
	for (i = 0; i < HEIGH; i++){                   //fp_in���� �̹��������ŭ �а� img_in�� ����
		fread(img_in[i], sizeof(BYTE), WIDTH, fp_in);
	}

	img_out = (BYTE **)malloc(sizeof(BYTE*)*HEIGH);  //�̹��������ŭ�� �޸𸮸� ���img_out���� �����Ҵ�
	for (i = 0; i < HEIGH; i++) 
	{
		img_out[i] = (BYTE*)malloc(sizeof(BYTE)*WIDTH);  //�̹��������ŭ�� �޸𸮸� ���img_out���� �����Ҵ�
	}
	for (i = 0; i < HEIGH; i++) // img_in�� �̹��������� img_out������
	{
		for (j = 0; j < WIDTH; j++)
		{
			img_out[i][j] = img_in[i][j];
		}
	}
	fp_out = fopen("[Output]Lena(512x512).raw", "wb"); //���̳ʸ��������� output.raw�� ����
	if (fp_out == NULL)
	{
		printf("File open failed\n");
	}
	for (i = 0; i < HEIGH; i++)
	{
		fwrite(img_out[i], sizeof(BYTE), WIDTH, fp_out);
	}

	for (i = 0; i < HEIGH; i++) //�Ҵ��ߴ� �޸� ����
	{
		free(img_in[i]);
		free(img_out[i]);
	}
	free(img_in); //�Ҵ��ߴ� �޸� ����
	free(img_out);

	fclose(fp_in); //fopen���� �� ���� close
	fclose(fp_out);

	return 0;

	
}