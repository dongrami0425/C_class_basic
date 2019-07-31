#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"


// < �޸� >
//** Arduino�� ���� ���۵���� python�� �ʿ��ϰ� RasberryPie�� ���� ���۵���� java�� �ʿ��ϴ�.
//	 Arduino�� Embeded�� ���ؼ��� C�� ����Ѵ�.
//** Python���� BigData, Machine Learning, R�� ���� ������ �м����� ���ȴ�.


/*
// < ������ >
// a+=3 == a=a+3 => cpu�� ó���ӵ��� �������� �� ������. +=, -=, *=, /= ���� �ִ�.
void main() {
	int a = 10, b=20, c;
	printf("a+b=%d \n", a + b);
	printf("a%%b=%d \n", a%b); // %�� ��ȣ(����)�� ���� ���� %% �� ���ش�.
	printf("-a = %d", -a);
}
*/

/*
// - �� ������ : AND(&&) OR(||) NOT(!)
// - �� ������ : A==B(A==B�� ����.), > , < , >=, <= , !=
// - ���� ������ : a++, a-- (a�� ���� �����ѵ� 1�� ��/�����Ѵ�.), --n, ++n (a�� ���� 1 ��/�����ϰ� ���� �����Ѵ�.)
void main() {

	int a = 10, b = 10, c, d;
	c = --a;
	d = a--;
	
	printf("a=%d, b=%d, c=%d, d=%d", a,b,c,d);
	//��� a=8 b=10 c=9 d=9(a�� ���� ���Եǰ� --�� ����) 

}
*/

/*
// - ��Ʈ ������ : AND(&), OR(|), XOR(^ : ī�� ), NOT(~ : ƿ��)
// - ����Ʈ ������ : ��Ʈ�������� �������� �� �� �ִµ� ������ ���� shift������ �ִ�. 
//					 a<<N : a�� ��Ʈ�� N�ڸ� ��ŭ �������� �̵� ,a>>N : a�� ��Ʈ�� N�ڸ� ��ŭ ���������� �̵�
void main() {
	int i = 1, j = 8, k, z;
	k = i << 1;
	z = j >> 3;
	printf("k = %d, z = %d", k, z);

}
*/

// - �޸� ������ : ������ ���������� �������ִ� ������ �Ѵ�.
// - ���� ������ : ����?�׸�1:�׸�2  => true�� �׸�1 false�� �׸�2�� ����.
// - ����ȯ ������ : ������ ���� �� �� ������ ����(��ȯ)���ִ� ��.
/*
void main() {
	int i, j, a=7, b, c;
	float d = 3.5, k = 0.0;

	// ���׿�����
	printf("�Է� ���� i j�� �Է��Ͻÿ�.");
	scanf("%d %d", &i, &j);
	i > j ? printf("\n i�� ��ũ��.") : printf("\n j�� �� ũ��");

	//����ȯ
	b = a + d;
	c = a * (int)d;
	k = (float)a + d;
	printf(" \n %d %d %f", b, c, k);
	return 0;
}
*/


// - ũ�⿬���� : ��� �����ͳ� �ڷ����� ũ�⸦ ǥ���ϴµ� sizeof() => int=4 float=4 char=1 long=4 double=8
// - �������� �켱���� : �����Ҷ����� �������� �������� , 
//						 ����Ʈ > �� > �� > ���� > ���Կ����� ������ �����ڰ� ���� �ȴ�. 


// < ��� >
//		���ǹ� : if, switch-case,
//		�ݺ��� : while(������ ���ε��� ����) do ~while(������ ���߿� ������ ������ �ѹ��� �� �����), for, break, 
//				

/*
// if��
void main() {
	int a;
	printf("������ �Է� : ");
	scanf("%d", &a);
	if (a >= 90)
		printf("\n GRADE : A ");
	else if (a>=80)
		printf("\n GRADE : B");
	else
		printf("\n GRADE : C");
}
*/

// switch-case ��
/*
void main() {
	int a;
	printf("Enter any integer");
	scanf("%d", &a);

	switch (a/10)
	{ 
	case 10:
	case 9:
		printf("Grade : A");
		break;
	case 8:
		printf("Grade : B");
		break;
	default:
		printf("Grade : F");
		break;
	
	}
}
*/

// while ��
/*
#include <Windows.h>
void main() {
	int a=5;

	while (a>=0) // a�� 10���� ���� ���� �����ض�.
	{
		printf("CMD�� %d�� �ڿ� ����˴ϴ� \n", a);
		Sleep(1000);
		--a;
	}
	
}
*/

// do ~while ��
/*
void main() {
	int a = 0;
	do
	{
		++a;
		printf("While repetition executed %d times \n",a);
	} while (a < 10);
		
}
*/

// for�� 
/*
void main() {
	int num1 = 1;
	printf("2�� ���� ������ ���϶� : \n");
	for ( int i = 0; i <= 10; i++)
	{
		printf("2�� %d���� : %d \n", i, num1);
		num1 = num1 << 1;
	}
}
*/

// break ��ɾ�. => ���� �� �̻� �ݺ����� ����, �ٷ� for���̳� while���� ������
/*
void main() {
	int sum = 0;

	for (int i = 1; i <= 100; i++)
	{
		sum += i;
		printf("%d ������ ���� %d�̴� \n.",i,sum);
		if (sum >= 200)
			break;
					   
	}

}
*/

// Continue ��  - for���̳� while���� {}�ȿ��� continue ������ ���� ���� continue�� �Ʒ��� �ִ� �����ؾ� �ϴ� ������� �ǳ� �ٰ�, ���� �ݺ��� �����Ѵ�.
/*
void main() {
	int sum = 0;
	for (int i = 0; i <= 20; i++)
	{
		if (i % 2 == 0) // => ¦�� �϶� ����.
			continue; 

		sum += i;
		printf("%d������ Ȧ��(ODD Number)�� ���� %d�̴�. \n",i,sum);
	}
}
*/

// go to �� => ���α׷��� �帧�� ���ϴ� ��ġ�� �̵���Ű�� ��.
/*
void main() {
	int a;
	while (1) {
		printf("Enter any number");
		scanf("%d", &a);

		if (a<= 0)
			printf("���� or 0�̴�.");

		if (a != 0) {
			if (a % 2 == 0)
				printf("¦����");
			else
				printf("Ȧ����");
		}
	}
}
*/
// <ǥ���Լ� �������>
/* ǥ���Լ��� ������Ͽ���(.h ����) conio ctype firect dos 
									fontl io    malloc math 
									memory process signal stdio 
									stdlib string				 ���� �ִ�.

	����� ���� �Լ���   ��ȯ_Ÿ�� �Լ�_��(����1, ����2, .....) => �Լ� ������ ���� �ϴ� ��.
	
	void main(){
		������=�Լ���(����1, ����2...) // �Լ� ȣ��
	}
		��ȯ�� �Լ���(����1,����2,...){

		...
		// �Լ� �ٵ� �κ� //
		...

		��ȯ ���� ����.
	}
*/

/*
float avg(int i, int j); //function prototype

void main() {
	int a, b;
	float c;
	printf("First value");
	scanf("%d", &a);
	printf("\nSecond value");
	scanf("%d", &b);

	c = avg(a, b); // function call
	printf("\n%d and %d�� ��հ��� : %f", a, b, c);

}

// Define function
float avg(int i, int j) {
	float k;
	k = (i + j) / 2.0;
	return k;
}

*/

//�ڹ��� �����ε�, �������̵��� C������ �������� �ʴ´�. ���� �Լ������� �ٲٰ� �ڵ带 C++���� �ϸ� �����.
/*
int int_sum(int i, int j);

float float_sum(float i, float j);

void main() {
	int i = 10, j = 20, k;
	float a = 3.2, b = 4.5, c;

	k = int_sum(i, j);
	c = float_sum(a, b);
	printf("int sum : %d, float sum : %f \n", k, c);
		   
}

int int_sum(int i, int j) {
	int k;
	k = i + j;
	return k;
}

float float_sum(float i, float j) {
	float n;
	n = i + j;
	return n;
}
*/


// ���� ���� �Լ� 
/*
int sum(int a, ...);
void main() {
	int i = 12;
	int j = 18;
	int k = 20;
	sum(i, j, k);

}
int sum(int a, ...) {
	int *i, b;
	i = &a;
	a = *i;
	i = i + 1;
	b = *i;
	printf("%d + %d = %d",a,b,a+b);
}
*/







// < ������ ����� �޸� �ּҰ�. >
/*
int a = 10, b;

void main() {
	int c = 5, k=0xfc12;
	const char* d = "STRING";
	static int e;

		// �� ������ �޸� �ּ� ���� ���� ��.
		// 16���� ��¹�� %x
		
		//%8x   => 8�ڸ��� ����ϵ� �������� ������ ä��
		//%.8x  => 8�ڸ��� ���鿡 �ش��ϴ� �ڸ��� 0���� ���
		//%08x  => 8�ڸ��� ����ϵ�, ������ 0���� �е��Ͽ� ���
		//%8.5x => 8�ڸ��� ����ϵ�, 5�ڸ������� ����� 0���� �е��ϰ� �������� ����.
		
	printf("k :%x\n", k);
	printf("k :%8x\n", k);
	printf("k :%.8x\n", k);
	printf("k :%08x\n", k);
	printf("k :%8.5x\n", k);

	// ������ ���������� 
	printf("a :  %8x\n", &a); // =>&���� : ������ �ּ� �ٷ� �� ���, �ּҴ� ���� 16������ ����.
	printf("b :  %8x\n", &b);
	printf("c :  %8x\n", &c);
	printf("d :  %8x\n", d);
	printf("e :  %8x\n", &e);	   
}
*/

