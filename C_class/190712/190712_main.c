#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"


// < 변수의 종류 >
// - 지역변수 : 함수 내, 할당 된 코드블럭 내에서 사용하는 변수, 함수 종료와 함께 소멸
// - 전역변수 : 전체 소스코드/프로그램 에서 사용 , 다른 함수에서도 사용 가능
// - 정적변수 : 함수가 종결되어도 계속 사용 가능, 선언과 동시에 메모리 주소가 할당되고 코드 종료시 주소 반환
// - 외부변수 : 다른 프로그램에서 선언된 변수. export 를 사용해서 다른 파일에 있는 변수/함수를 불러와서 사용가능.
//			**HEAP : 동적할당/반드시 포인터로 접근.
//			  STACK : 지역변수/함수의 파라미터로 접근.

// STACK 실습
/*
void sum(int a, ...);

void main() {
	int i = 10;
	int   j = 21;
	sum(i, j, NULL);
}
void sum(int a, ...) {
	int* i, b;
	int sum = a;
	i = &a;
	while (*i != NULL) {
		i = i + 1;
		b = *i;
		sum += b;
	}

	printf("sum = %d\n", sum);
	//   return 0;

}
*/

// 전역변수, 지역변수
/*
void var_test(int local_var1); // 함수원형

int global_var1; // 전역변수

void main() {
	int local_var1; // 지역변수
	global_var1 = 10;
	local_var1 = 10;
	
	printf("[1]global variable : %d, local variable : %d \n", global_var1, local_var1);

	var_test(local_var1);
	printf("[3]global variable : %d, local variable : %d \n", global_var1, local_var1);

}

void var_test(int local_var1) {
    local_var1 = 20;

	printf("[2]global variable : %d, local variable : %d \n", global_var1, local_var1);
}
// [ 결과 ]
//[1]global variable : 10, local variable : 10
//[2]global variable : 10, local variable : 20
//[3]global variable : 10, local variable : 10

*/


//정적변수
/*
void static_test();

void main() {
	static_test();
	static_test();
	static_test();
}
void static_test() {
	int sum = 0;				//지역변수.
	static int static_sum = 0;	//정적변수. // 프로그램이 시작될 때 정적 변수가 생성되고 초기화됨
											// 다시 함수가 호출될 때는 값 초기화 무시
	sum += 10;
	static_sum += 10;
	printf("Sum = %d, static_sum = %d \n", sum, static_sum);

}
// [ 결과 ]
//	Sum = 10, static_sum = 10
//	Sum = 10, static_sum = 20
//	Sum = 10, static_sum = 30

*/


//외부변수 
/*
extern int i, j; // 다른 소스 파일(외부)에 선언된 전역 변수를 사용할 때는 extern 키워드를 사용합니다.
extern void sum(void);


void main() {
	printf("정수 입력 : ");
	scanf("%d", &i);
	printf("두번 째 정수 입력 : ");
	scanf("%d", &j);

	sum(i, j);
		
}
*/

// < 배열 >
//	동일한 데이터 타입이 여럿 있을 떄 일일히 변수를 선언하지 않고
//	하나늬 배열로 만들어서 처리한다. 
//	각 요소에 접근할 때 에는 인덱스 번호를 사용한다.
//	항상 0부터 시작하므로 요소 추출에 주의 한다.
//	int a[3] 식으로 쓴느데 자료형 배열명[배열크기] 이다.

/*
void main() {
	int a[3];
	printf("a address =%x \n", &a);
	printf("a[0] address =%x \n", &a[0]);
	printf("a[1] address =%x \n", &a[1]);
	printf("a[2] address =%x \n", &a[2]);

	printf("Size of a =%d \n", sizeof(a));


}

// [ 결과 ]
// a address = effdf8
// a[0] address = effdf8
// a[1] address = effdfc
// a[2] address = effe00
// Size of a = 12 => int = 4byte 따라서 4x3=12
*/



// 문자열 복사하기. stringcopy
/*
void string_copy(char* str1, char* srt2); // function prototype

void main() {
	int i;
	char countury[10] = "korea";
	char nation[10];

	string_copy(countury, nation); //function call

	printf("countury[] : %s \n", countury);
	printf("nation[] : %s \n", nation);
	printf("nation[] : %x \n", nation); //배열의 주소

}

void string_copy(char* str1, char* str2) { // function body
	
	//for (int i = 0; i < 10; i++)
	//{
	//	str2[i] = str1[i];		
	//}
	
	int j=0;
	while(str1[j] != NULL) {
		str2[j] = str1[j];
		j++;
	}
	str2[j] = NULL; // 문자열의 끝에 NULL(= \0)은 문장이 끝났다는 것을 말한다. 
					//할당된 데이터에 딱맞게 문자열이 들어갈 경우 마지막에 항상 NULL이 붙는다.
}

*/

//  < 포인터 (pointer) >
//  컴퓨터의 메모리 주소를 지시하는 기법으로 "상대주소"와 "절대주소"가 있다.
//	어느 메모리 주소에 할당 된 값을 보려면, 변수를 이용한다.
//	int a = 10; 이라면 a를 위한 주소를 CPU가 메모리에 할당 해주고, a를 통해 해당 주소로 가서 10을 가져오는 것이다. 

/*
void main() {
	int a;
	printf("a 가 저장된 주소 : %p",&a); //  %p 포인터값(메모리 어드레스 값)을 16진수로 출력
}
*/


//	포인터 : C에서 "메모리 어드레스"를 이르는 말. 이 메모리 어드레스를 저장하고 있는 변수를 "포인터 변수"라 한다.
//			 따라서 포인터 변수의 크기는 모두 같다.
//			 변수가 할당 된 주소를 알아보기 위해서 => pointer("%d", *변수명)
//			 포인터에서는 주소 값이나 실제 저장된 값을 나타내기 위해서 &를 써서 주소을 표시, *는 포인터 변수가 가리키는 곳에 저장된값.

/*
void main() {
	int a = 10; //일반 변수
	int* pa;	//포인터 변수 선언
	pa = &a;	//a의 주소 값을 pa에 대입.

	printf("a = %d \n", a);		//a의 값.
	printf("*pa = %d \n", *pa);	//포인터 pa가 가리키고있는 주소에 저장된 값.
	printf("&a = %p \n", &a);	//a의 주소.
	printf("&pa = %p \n", &pa);	//pa의 주소.
	printf("pa = %p \n", pa);	//pa가 가리키는 주소.
	printf("&*pa=%p \n",&*pa);	//pa가 가리키는 주소.

}

// 정리
// 변수 a에 저장된 값은 a, *pa
// 변수 a의 주소는 &a, pa &*pa
// 포인터 변수 pa의 주소는 &pa

*/

// 포인터 연산.
/*
void main() {
	int a[5] = { 10,20,30,40,50 }; // 크기 5의 배열 a 선언 5x4byte(int) = 20byte
	int *pa;					   //포인터 변수 선언
	pa = a;						   //a를 포인터 변수에 대입, 배열의 주소는 배열이름으로 적어주면됨.
	printf("pa의 값 : %d \n", *pa);
	pa++; // pa = a(배열의 시작 주소)에 1(4byte)을 더해줌으로서 다음 요소로 넘어감.
	printf("pa++의 값 : %d \n", *pa);

	pa--;
	printf("a의 사이즈 %d \n", sizeof(a));			 //sizeof()는 byte단위로 크기를 출력해줌.
	for (int i = 0; i < sizeof(a)/sizeof(int); i++)  //sizeof(a)는 a가 int이므로 한칸에 4바이트이다. 따라서 sizeof(int)로 나눠 배열의 크기를 표현한것.
	{
		printf("pa의 값 : %d \n", *pa);
		pa++;
	}

}
*/

// - 이중 포인터 
// 이중포인터는 포인터의 메모리주소를 저장하는 포인터.
// 하나의 함수내에서, 다른함수에서 선언된 변수로 접근할 때 사용된다.

/*
int main() {
	int num1 = 10, num2 = 20; //일반 지역변수.
	
	int* pt1 = &num1;		//포인터 변수. // 일반 지역변수의 주소를 가리키는 것.
	int* pt2 = &num2;
	
	int** dpt1 = &pt1;		//이중 포인터 변수. // 포인터의 주소를 가리키는 것.
	int** dpt2 = &pt2;

	int sum = num1 + num2;
	printf("num1+num2 = %d\n", sum);
	
	sum = *pt1 + *pt2;					// 두 포인터 변수가 가르키는 곳에 들어 있는 값을 더함.
	printf("*pt1 + *pt2 = %d\n", sum);
	sum = **dpt1 + **dpt2;				// 두 이중 포인터 변수가 가르키는 곳에 들어있는 값을 더함.
	printf("**pt1 + **pt2 = %d\n", sum);


}
*/
// num1 + num2 = 30
// *pt1 + *pt2 = 30
// **pt1 + **pt2 = 30
/*
void swap(int** pt1, int** pt2);
void main() {
	int num1 = 10, num2 = 20;
	int* pt1;
	int* pt2;

	pt1 = &num1;
	pt2 = &num2;

	printf("before swap pt1 : %d pt2 : %d\n", *pt1, *pt2);
	swap(&pt1, &pt2);
	printf("after swap pt1 : %d pt2 : %d\n", *pt1, *pt2);
	printf("num1 : %d num2: %d \n",num1,num2);

}

void swap(int** pt1, int** pt2) {
	int temp = *pt1; //pt1이 가리키는 곳의 값을 temp에 대입
	*pt1 = *pt2;	 //pt2가 가리키는 곳의 값을 *pt1에 대입
	*pt2 = temp;
}
*/

// <포인터 와 배열>
//	포인터 배열 : 포인터들을 배열로 묶어서 사용하는 방법. 데이터들을 포인터를 이용해 배열로 묶어서 사용하는 것.
//	int* pa[5];
//	배열 포인터 : 배열을 가리키는 포인터 변수이다.
//	int (*pa)[5];

// < 정적/동적메모리할당 > 
//  - 동적 메모리 할당 : 실행과 동시에 메모리를 할당하지 않고 프로그램을 실행하는 도중 사용할 메모리 공간을 할당하는 기법이다.
//	 					 프로그램 실행이 종료되면 메모리를 반납하여 CPU가 다른 프로그램에게 메모리를 재할당하는 기법이다.

//	- 정적 메모리 할당 : 프로그램을 실행하는 순간(처음에)에 메모리의 크기를 고려하여 할당해주는 기법.
/*
void main(){
	int num1 = 10, num2 = 20, num3 = 30, num4 = 40; //일반변수선언

	int arr[2][2] = { {1,2},{3,4} };					//정수형 2차원 배열 arr 선언과 초기화
	int* pt_num[4] = { &num1,&num2,&num3,&num4 };	//포인터 배열선언과 변수의 주소로 초기화
	int(* pt_arr)[2] = arr;							//배열 포인트선언과 초기화로 포인터 변수 pt_arr에 arr배열의 주소가 입력됨, 2차원 배열에서 의미가 있음.
													//배열 포인터의 크기는 
	printf("pt_num : ");
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", *pt_num[i]);					//포인터 배열 출력 
	}
	printf("\n--------------------------------\n");

	printf("pt_arr : ");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("%d ", pt_arr[i][j]);	//배열 포인터 출력 pt_arr은 

		}

	}
	printf("\n--------------------------------\n");

}
*/
// [ 결과 ]
//	pt_num: 10 20 30 40
//	--------------------------------
//	pt_arr : 1 2 3 4
//	--------------------------------


// 메모리 동적할당. malloc
/*
#include <stdlib.h>
typedef unsigned int INT;



int main() {
	INT *pa;
	INT sum = 0, i, size;
	float avg;

	printf("Number of entering integer : ");
	scanf("%d", &size);

	pa = (INT*)malloc(sizeof(INT)*size);
	for ( i = 0; i < size; i++)
	{
		printf("%d번째 데이터 입력 : ", i + 1);
		scanf("%d", &pa[i]);
		sum += pa[i];
	}
	
	avg = sum / (float)size;
	printf("sum = %d", sum);
	printf("avg = %f", avg);
	free(pa);
	return 0;
}
*/

// < 구조체와 공용체 >
// 이들을 이해하기 위해서는 먼저 필드와 레코드를 이해해야 한다.
// char name[11]; 을 이름으로 사용, char tel[11]; 을 전화번호로 사용하게 했을 때
//	name와 tel을 필드라 부르고, 한쌍의 name과 tel을 묶어서 레코드라 부른다.

//	- 구조체.
//	구조체는 배열과 같은 변수들의 집합이다. 
//	배열과 다른 점은 배열은 동일한 데이터 타입으로 이뤄져 있지만
//					 구조체는 각기 다른 데이터 타입을 가지고 있을 수 있다. => char c[10]; 과 int i[5];를 묶어서 구조체로 사용.
//																			  struct ci[15]{ char c[10];
//																							 int i[5];
//																							 ...데이터를 나열...}		

// - 공용체
//	하나의 메모리에 서로다른 타입의 여러 변수를 공유시키는 기법이다. 
//	union anyaname{ char name;
//					int num;
//					...}

// 열거형은 변수가 가질 수 있는 데이터의 값을 선언해서 사용하는 기법으로 열거형에서 사용된 기호 상수만 변수로서 호출하여 사용 할 수 있다.
// enum COMPUTER(MONITOR, RAM, HDD, DVD)






