#define _CRT_SECURE_NO_WARNINGS
//#define CHARGE 3000

#include "stdio.h"
/*
void main()
{
   char a;
   printf("�Է��ϼ��� : ");
   scanf("%c", &a);
   printf("%c", a);

}
*/


/*
void main()
{
   printf("a=%d\n", 10);
   printf("b=%d\n", 20);
   printf("a=%d, b=%d\n", 10, 20);
}
*/


/*
void main() {
	int i = 0;
	short s = 1;
	long l = 2;
	float f = 3.14;
	double d = 4.13;

	printf("int : %d, short : %d, long : %ld, float : %f, double : %lf ", i,s,l,f,d);
}
*/


/*
void main() {
	char name[4] = "Lee";
	printf("name = %s\n", name);
	

}
*/

/*
//tydef => ����� ����. Ű����
typedef unsigned int unit;
typedef int integer;
void main()
{
	unit number1;
	integer number2;

	number1 = 100;
	number2 = 200;

	printf("number1 : %d,number2 : %d\n", number1, number2);

}
*/


/*
void main() {
	const float PI = 3.141592;
	int radius;
	printf("radius ������ ���� �Է��ϼ��� :");
	scanf("%d", &radius);
	printf("2*radius*PI = %f", 2*radius*PI);

}
*/

/*
//#define CHARGE 3000 => �������� ����.
void main() {
	int time, rate;
	printf("���ð��Է� : ");
	scanf("%d", &time);
	rate = time * CHARGE;
	printf("�ݾ��� %d �� �Դϴ�",rate);
}
*/


// < ����/���ڿ� �����. >
//���ڿ� ��/��� gets/puts
//���ڿ� ���� ���� printf scanf �� �ְ� putchar, getchar�� ���۸� ����ϴ� ����������̴�. 
//���� gets, puts�� ���ڿ� ����¿� ���ȴ�.

/* / �ָ��� �ǽ�, ã�Ƽ� ����
void main() {
	char i, j;
	printf("Enter any char:");
	i=getchar();
	printf("�Է��� ���� :");
	putchar(i);
	fflush(stdin);

	
	printf("Enter any char:");
	j=getchar();
	putchar(j);
	printf("what you entered is %c and %c, right?", i, j);
	
}
*/

/*
void main() {
	char str[31];
	printf("���� �Է�:");
	gets(str);
	printf("���:");
	puts(str);
}
*/

