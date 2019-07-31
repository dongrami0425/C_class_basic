#define _CRT_SECURE_NO_WARNINGS
//#define CHARGE 3000

#include "stdio.h"
/*
void main()
{
   char a;
   printf("입력하세요 : ");
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
//tydef => 예약어 설정. 키워드
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
	printf("radius 반지름 값을 입력하세요 :");
	scanf("%d", &radius);
	printf("2*radius*PI = %f", 2*radius*PI);

}
*/

/*
//#define CHARGE 3000 => 전역변수 설정.
void main() {
	int time, rate;
	printf("사용시간입력 : ");
	scanf("%d", &time);
	rate = time * CHARGE;
	printf("금액은 %d 원 입니다",rate);
}
*/


// < 문자/문자열 입출력. >
//문자열 입/출력 gets/puts
//문자에 대한 것은 printf scanf 이 있고 putchar, getchar는 버퍼를 사용하는 문자입출력이다. 
//또한 gets, puts는 문자열 입출력에 사용된다.

/* / 애매한 실습, 찾아서 공부
void main() {
	char i, j;
	printf("Enter any char:");
	i=getchar();
	printf("입력한 값은 :");
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
	printf("문자 입력:");
	gets(str);
	printf("출력:");
	puts(str);
}
*/

