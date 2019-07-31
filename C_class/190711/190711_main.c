#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"


// < 메모 >
//** Arduino를 통한 조작등에서는 python이 필요하고 RasberryPie를 통한 조작등에서는 java가 필요하다.
//	 Arduino의 Embeded들 위해서는 C를 사용한다.
//** Python으로 BigData, Machine Learning, R을 통한 데이터 분석에도 사용된다.


/*
// < 연산자 >
// a+=3 == a=a+3 => cpu의 처리속도가 전자쪽이 더 빠르다. +=, -=, *=, /= 등이 있다.
void main() {
	int a = 10, b=20, c;
	printf("a+b=%d \n", a + b);
	printf("a%%b=%d \n", a%b); // %를 기호(문자)로 넣을 때는 %% 로 써준다.
	printf("-a = %d", -a);
}
*/

/*
// - 논리 연산자 : AND(&&) OR(||) NOT(!)
// - 비교 연산자 : A==B(A==B가 같다.), > , < , >=, <= , !=
// - 증감 연산자 : a++, a-- (a를 먼저 대입한뒤 1을 덧/뺄셈한다.), --n, ++n (a를 먼저 1 덧/뺄셈하고 값을 대입한다.)
void main() {

	int a = 10, b = 10, c, d;
	c = --a;
	d = a--;
	
	printf("a=%d, b=%d, c=%d, d=%d", a,b,c,d);
	//결과 a=8 b=10 c=9 d=9(a가 먼저 대입되고 --가 수행) 

}
*/

/*
// - 비트 연산자 : AND(&), OR(|), XOR(^ : 카렛 ), NOT(~ : 틸드)
// - 쉬프트 연산자 : 비트연산자의 일종으로 볼 수 있는데 좌측과 우측 shift연산이 있다. 
//					 a<<N : a의 비트를 N자리 만큼 왼쪽으로 이동 ,a>>N : a의 비트를 N자리 만큼 오른쪽으로 이동
void main() {
	int i = 1, j = 8, k, z;
	k = i << 1;
	z = j >> 3;
	printf("k = %d, z = %d", k, z);

}
*/

// - 콤마 연산자 : 왼쪽을 오른쪽으로 연결해주는 역할을 한다.
// - 삼항 연산자 : 조건?항목1:항목2  => true면 항목1 false면 항목2를 수행.
// - 형변환 연산자 : 연산을 수행 할 때 형식을 지정(변환)해주는 것.
/*
void main() {
	int i, j, a=7, b, c;
	float d = 3.5, k = 0.0;

	// 삼항연산자
	printf("입력 숫자 i j를 입력하시오.");
	scanf("%d %d", &i, &j);
	i > j ? printf("\n i가 더크다.") : printf("\n j가 더 크다");

	//형변환
	b = a + d;
	c = a * (int)d;
	k = (float)a + d;
	printf(" \n %d %d %f", b, c, k);
	return 0;
}
*/


// - 크기연산자 : 어느 데이터나 자료형의 크기를 표시하는데 sizeof() => int=4 float=4 char=1 long=4 double=8
// - 연산자의 우선순위 : 동등할때에는 좌측에서 우측으로 , 
//						 쉬프트 > 비교 > 논리 > 삼행 > 대입연산자 순으로 연산자가 적용 된다. 


// < 제어문 >
//		조건문 : if, switch-case,
//		반복문 : while(조건이 참인동안 실행) do ~while(조건은 나중에 따지기 때문에 한번은 꼭 실행됨), for, break, 
//				

/*
// if문
void main() {
	int a;
	printf("점수를 입력 : ");
	scanf("%d", &a);
	if (a >= 90)
		printf("\n GRADE : A ");
	else if (a>=80)
		printf("\n GRADE : B");
	else
		printf("\n GRADE : C");
}
*/

// switch-case 문
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

// while 문
/*
#include <Windows.h>
void main() {
	int a=5;

	while (a>=0) // a가 10보다 작은 동안 실행해라.
	{
		printf("CMD가 %d초 뒤에 종료됩니다 \n", a);
		Sleep(1000);
		--a;
	}
	
}
*/

// do ~while 문
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

// for문 
/*
void main() {
	int num1 = 1;
	printf("2의 제곱 값들을 구하라 : \n");
	for ( int i = 0; i <= 10; i++)
	{
		printf("2의 %d승은 : %d \n", i, num1);
		num1 = num1 << 1;
	}
}
*/

// break 명령어. => 이제 더 이상 반복하지 말고, 바로 for문이나 while문을 끝내라
/*
void main() {
	int sum = 0;

	for (int i = 1; i <= 100; i++)
	{
		sum += i;
		printf("%d 까지의 합은 %d이다 \n.",i,sum);
		if (sum >= 200)
			break;
					   
	}

}
*/

// Continue 문  - for문이나 while문의 {}안에서 continue 문장을 만난 순간 continue문 아래에 있는 실행해야 하는 문장들을 건너 뛰고, 다음 반복을 시작한다.
/*
void main() {
	int sum = 0;
	for (int i = 0; i <= 20; i++)
	{
		if (i % 2 == 0) // => 짝수 일때 실행.
			continue; 

		sum += i;
		printf("%d까지의 홀수(ODD Number)의 합은 %d이다. \n",i,sum);
	}
}
*/

// go to 문 => 프로그램의 흐름을 원하는 위치로 이동시키는 것.
/*
void main() {
	int a;
	while (1) {
		printf("Enter any number");
		scanf("%d", &a);

		if (a<= 0)
			printf("음수 or 0이다.");

		if (a != 0) {
			if (a % 2 == 0)
				printf("짝수임");
			else
				printf("홀수임");
		}
	}
}
*/
// <표준함수 헤더파일>
/* 표준함수의 헤더파일에는(.h 생략) conio ctype firect dos 
									fontl io    malloc math 
									memory process signal stdio 
									stdlib string				 등이 있다.

	사용자 정의 함수는   반환_타입 함수_명(인자1, 인자2, .....) => 함수 원형을 선언 하는 것.
	
	void main(){
		변수명=함수명(인자1, 인자2...) // 함수 호출
	}
		반환값 함수명(인자1,인자2,...){

		...
		// 함수 바디 부분 //
		...

		반환 값을 써줌.
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
	printf("\n%d and %d의 평균값은 : %f", a, b, c);

}

// Define function
float avg(int i, int j) {
	float k;
	k = (i + j) / 2.0;
	return k;
}

*/

//자바의 오버로딩, 오버라이딩은 C에서는 지원하지 않는다. 같은 함수명으로 바꾸고 코드를 C++에서 하면 실행됨.
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


// 가변 인자 함수 
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







// < 변수가 저장된 메모리 주소값. >
/*
int a = 10, b;

void main() {
	int c = 5, k=0xfc12;
	const char* d = "STRING";
	static int e;

		// 각 변수의 메모리 주소 값을 보는 것.
		// 16진수 출력방법 %x
		
		//%8x   => 8자리로 출력하되 공백으로 공간을 채움
		//%.8x  => 8자리중 공백에 해당하는 자리를 0으로 출력
		//%08x  => 8자리로 출력하되, 공백을 0으로 패딩하여 출력
		//%8.5x => 8자리로 출력하되, 5자리까지만 빈곳을 0으로 패딩하고 나머지는 공백.
		
	printf("k :%x\n", k);
	printf("k :%8x\n", k);
	printf("k :%.8x\n", k);
	printf("k :%08x\n", k);
	printf("k :%8.5x\n", k);

	// 변수의 종류에따라 
	printf("a :  %8x\n", &a); // =>&변수 : 변수의 주소 다룰 때 사용, 주소는 보통 16진수로 본다.
	printf("b :  %8x\n", &b);
	printf("c :  %8x\n", &c);
	printf("d :  %8x\n", d);
	printf("e :  %8x\n", &e);	   
}
*/

