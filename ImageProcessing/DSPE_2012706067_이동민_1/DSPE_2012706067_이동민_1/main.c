//1����_2012706067_�̵���
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> //Header file
#include<stdlib.h> //malloc, free�� �������� Header

#define N 512 //N=512�� ����

int main()
{
	FILE *fp_in = 0, *fp_out1 = 0, *fp_out2 = 0, *fp_out3 = 0; //FILE �����ʹ� ������ �ּҸ� ����Ŵ
	unsigned char *idata = 0, *odata1 = 0, *odata2 = 0; //unsigned char������ ������ ���� ����
	int temp = 0; //int�� temp ���� ����
	int idx = 0; //int�� idx ���� ����

	fp_in = fopen("input1.raw", "rb"); //fopen�� ����Ͽ� ������ �а� fp_in�� ����. 
	//fopen("�����̸�","���̳ʸ� ���� �б� �������� ����")

	idata = (unsigned char *)malloc(sizeof(unsigned char)*N); //Nũ�⸸ŭ�� �޸𸮸� �����Ҵ�

	/*�޸��� �����Ҵ� malloc
	���α׷� ���� �� �ʿ��Ѹ�ŭ�� �޸𸮸� �Ҵ� �� ���� �� �����ϴ�
	�����Ҵ��̴�. free�� �ݵ�� �޸𸮸� ��������� �Ѵ�.
	���� �����ʴ� ���ʿ��� �޸𸮰� ��ǻ�� �޸𸮸� �����Ͽ� ������ �� �� ����*/

	fread(idata, sizeof(unsigned char), N, fp_in);
	//fp_in���� unsigned char������ N�׸���� �а� idata�� ����

	odata2 = (unsigned char *)malloc(sizeof(unsigned char)*(N - 1));
	//idata�� ���������� �����޸� �Ҵ�. ũ��� N-1��ŭ�̴�.

	for (idx = 0; idx < N - 1; idx++) //odata2�迭�� f(x)-f(x+1)���� ����� ����
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


	fp_out1 = fopen("output.raw", "wb"); //���̳ʸ��������� output.raw�� ����
	fwrite(odata2, sizeof(unsigned char), N - 1, fp_out1);

	fp_out2 = fopen("output.xls", "wt"); //odata2�迭�� ��� �������Ϸ� ����
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

	fp_out3 = fopen("input.xls", "wt"); //input1.xls������ ��� ���� ������� ����
	for (idx = 0; idx < N - 1; idx++)
	{
		fprintf(fp_out3, "%u\n", odata1[idx]);
	}

	free(idata); //�Ҵ��ߴ� �޸� ����
	free(odata2);
	free(odata1);
	fclose(fp_in); //fopen���� �� ���� close
	fclose(fp_out1);
	fclose(fp_out2);
	fclose(fp_out3);

	return 0;
}
