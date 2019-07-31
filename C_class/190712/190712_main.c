#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"


// < ������ ���� >
// - �������� : �Լ� ��, �Ҵ� �� �ڵ�� ������ ����ϴ� ����, �Լ� ����� �Բ� �Ҹ�
// - �������� : ��ü �ҽ��ڵ�/���α׷� ���� ��� , �ٸ� �Լ������� ��� ����
// - �������� : �Լ��� ����Ǿ ��� ��� ����, ����� ���ÿ� �޸� �ּҰ� �Ҵ�ǰ� �ڵ� ����� �ּ� ��ȯ
// - �ܺκ��� : �ٸ� ���α׷����� ����� ����. export �� ����ؼ� �ٸ� ���Ͽ� �ִ� ����/�Լ��� �ҷ��ͼ� ��밡��.
//			**HEAP : �����Ҵ�/�ݵ�� �����ͷ� ����.
//			  STACK : ��������/�Լ��� �Ķ���ͷ� ����.

// STACK �ǽ�
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

// ��������, ��������
/*
void var_test(int local_var1); // �Լ�����

int global_var1; // ��������

void main() {
	int local_var1; // ��������
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
// [ ��� ]
//[1]global variable : 10, local variable : 10
//[2]global variable : 10, local variable : 20
//[3]global variable : 10, local variable : 10

*/


//��������
/*
void static_test();

void main() {
	static_test();
	static_test();
	static_test();
}
void static_test() {
	int sum = 0;				//��������.
	static int static_sum = 0;	//��������. // ���α׷��� ���۵� �� ���� ������ �����ǰ� �ʱ�ȭ��
											// �ٽ� �Լ��� ȣ��� ���� �� �ʱ�ȭ ����
	sum += 10;
	static_sum += 10;
	printf("Sum = %d, static_sum = %d \n", sum, static_sum);

}
// [ ��� ]
//	Sum = 10, static_sum = 10
//	Sum = 10, static_sum = 20
//	Sum = 10, static_sum = 30

*/


//�ܺκ��� 
/*
extern int i, j; // �ٸ� �ҽ� ����(�ܺ�)�� ����� ���� ������ ����� ���� extern Ű���带 ����մϴ�.
extern void sum(void);


void main() {
	printf("���� �Է� : ");
	scanf("%d", &i);
	printf("�ι� ° ���� �Է� : ");
	scanf("%d", &j);

	sum(i, j);
		
}
*/

// < �迭 >
//	������ ������ Ÿ���� ���� ���� �� ������ ������ �������� �ʰ�
//	�ϳ��� �迭�� ���� ó���Ѵ�. 
//	�� ��ҿ� ������ �� ���� �ε��� ��ȣ�� ����Ѵ�.
//	�׻� 0���� �����ϹǷ� ��� ���⿡ ���� �Ѵ�.
//	int a[3] ������ ������ �ڷ��� �迭��[�迭ũ��] �̴�.

/*
void main() {
	int a[3];
	printf("a address =%x \n", &a);
	printf("a[0] address =%x \n", &a[0]);
	printf("a[1] address =%x \n", &a[1]);
	printf("a[2] address =%x \n", &a[2]);

	printf("Size of a =%d \n", sizeof(a));


}

// [ ��� ]
// a address = effdf8
// a[0] address = effdf8
// a[1] address = effdfc
// a[2] address = effe00
// Size of a = 12 => int = 4byte ���� 4x3=12
*/



// ���ڿ� �����ϱ�. stringcopy
/*
void string_copy(char* str1, char* srt2); // function prototype

void main() {
	int i;
	char countury[10] = "korea";
	char nation[10];

	string_copy(countury, nation); //function call

	printf("countury[] : %s \n", countury);
	printf("nation[] : %s \n", nation);
	printf("nation[] : %x \n", nation); //�迭�� �ּ�

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
	str2[j] = NULL; // ���ڿ��� ���� NULL(= \0)�� ������ �����ٴ� ���� ���Ѵ�. 
					//�Ҵ�� �����Ϳ� ���°� ���ڿ��� �� ��� �������� �׻� NULL�� �ٴ´�.
}

*/

//  < ������ (pointer) >
//  ��ǻ���� �޸� �ּҸ� �����ϴ� ������� "����ּ�"�� "�����ּ�"�� �ִ�.
//	��� �޸� �ּҿ� �Ҵ� �� ���� ������, ������ �̿��Ѵ�.
//	int a = 10; �̶�� a�� ���� �ּҸ� CPU�� �޸𸮿� �Ҵ� ���ְ�, a�� ���� �ش� �ּҷ� ���� 10�� �������� ���̴�. 

/*
void main() {
	int a;
	printf("a �� ����� �ּ� : %p",&a); //  %p �����Ͱ�(�޸� ��巹�� ��)�� 16������ ���
}
*/


//	������ : C���� "�޸� ��巹��"�� �̸��� ��. �� �޸� ��巹���� �����ϰ� �ִ� ������ "������ ����"�� �Ѵ�.
//			 ���� ������ ������ ũ��� ��� ����.
//			 ������ �Ҵ� �� �ּҸ� �˾ƺ��� ���ؼ� => pointer("%d", *������)
//			 �����Ϳ����� �ּ� ���̳� ���� ����� ���� ��Ÿ���� ���ؼ� &�� �Ἥ �ּ��� ǥ��, *�� ������ ������ ����Ű�� ���� ����Ȱ�.

/*
void main() {
	int a = 10; //�Ϲ� ����
	int* pa;	//������ ���� ����
	pa = &a;	//a�� �ּ� ���� pa�� ����.

	printf("a = %d \n", a);		//a�� ��.
	printf("*pa = %d \n", *pa);	//������ pa�� ����Ű���ִ� �ּҿ� ����� ��.
	printf("&a = %p \n", &a);	//a�� �ּ�.
	printf("&pa = %p \n", &pa);	//pa�� �ּ�.
	printf("pa = %p \n", pa);	//pa�� ����Ű�� �ּ�.
	printf("&*pa=%p \n",&*pa);	//pa�� ����Ű�� �ּ�.

}

// ����
// ���� a�� ����� ���� a, *pa
// ���� a�� �ּҴ� &a, pa &*pa
// ������ ���� pa�� �ּҴ� &pa

*/

// ������ ����.
/*
void main() {
	int a[5] = { 10,20,30,40,50 }; // ũ�� 5�� �迭 a ���� 5x4byte(int) = 20byte
	int *pa;					   //������ ���� ����
	pa = a;						   //a�� ������ ������ ����, �迭�� �ּҴ� �迭�̸����� �����ָ��.
	printf("pa�� �� : %d \n", *pa);
	pa++; // pa = a(�迭�� ���� �ּ�)�� 1(4byte)�� ���������μ� ���� ��ҷ� �Ѿ.
	printf("pa++�� �� : %d \n", *pa);

	pa--;
	printf("a�� ������ %d \n", sizeof(a));			 //sizeof()�� byte������ ũ�⸦ �������.
	for (int i = 0; i < sizeof(a)/sizeof(int); i++)  //sizeof(a)�� a�� int�̹Ƿ� ��ĭ�� 4����Ʈ�̴�. ���� sizeof(int)�� ���� �迭�� ũ�⸦ ǥ���Ѱ�.
	{
		printf("pa�� �� : %d \n", *pa);
		pa++;
	}

}
*/

// - ���� ������ 
// ���������ʹ� �������� �޸��ּҸ� �����ϴ� ������.
// �ϳ��� �Լ�������, �ٸ��Լ����� ����� ������ ������ �� ���ȴ�.

/*
int main() {
	int num1 = 10, num2 = 20; //�Ϲ� ��������.
	
	int* pt1 = &num1;		//������ ����. // �Ϲ� ���������� �ּҸ� ����Ű�� ��.
	int* pt2 = &num2;
	
	int** dpt1 = &pt1;		//���� ������ ����. // �������� �ּҸ� ����Ű�� ��.
	int** dpt2 = &pt2;

	int sum = num1 + num2;
	printf("num1+num2 = %d\n", sum);
	
	sum = *pt1 + *pt2;					// �� ������ ������ ����Ű�� ���� ��� �ִ� ���� ����.
	printf("*pt1 + *pt2 = %d\n", sum);
	sum = **dpt1 + **dpt2;				// �� ���� ������ ������ ����Ű�� ���� ����ִ� ���� ����.
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
	int temp = *pt1; //pt1�� ����Ű�� ���� ���� temp�� ����
	*pt1 = *pt2;	 //pt2�� ����Ű�� ���� ���� *pt1�� ����
	*pt2 = temp;
}
*/

// <������ �� �迭>
//	������ �迭 : �����͵��� �迭�� ��� ����ϴ� ���. �����͵��� �����͸� �̿��� �迭�� ��� ����ϴ� ��.
//	int* pa[5];
//	�迭 ������ : �迭�� ����Ű�� ������ �����̴�.
//	int (*pa)[5];

// < ����/�����޸��Ҵ� > 
//  - ���� �޸� �Ҵ� : ����� ���ÿ� �޸𸮸� �Ҵ����� �ʰ� ���α׷��� �����ϴ� ���� ����� �޸� ������ �Ҵ��ϴ� ����̴�.
//	 					 ���α׷� ������ ����Ǹ� �޸𸮸� �ݳ��Ͽ� CPU�� �ٸ� ���α׷����� �޸𸮸� ���Ҵ��ϴ� ����̴�.

//	- ���� �޸� �Ҵ� : ���α׷��� �����ϴ� ����(ó����)�� �޸��� ũ�⸦ ����Ͽ� �Ҵ����ִ� ���.
/*
void main(){
	int num1 = 10, num2 = 20, num3 = 30, num4 = 40; //�Ϲݺ�������

	int arr[2][2] = { {1,2},{3,4} };					//������ 2���� �迭 arr ����� �ʱ�ȭ
	int* pt_num[4] = { &num1,&num2,&num3,&num4 };	//������ �迭����� ������ �ּҷ� �ʱ�ȭ
	int(* pt_arr)[2] = arr;							//�迭 ����Ʈ����� �ʱ�ȭ�� ������ ���� pt_arr�� arr�迭�� �ּҰ� �Էµ�, 2���� �迭���� �ǹ̰� ����.
													//�迭 �������� ũ��� 
	printf("pt_num : ");
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", *pt_num[i]);					//������ �迭 ��� 
	}
	printf("\n--------------------------------\n");

	printf("pt_arr : ");
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			printf("%d ", pt_arr[i][j]);	//�迭 ������ ��� pt_arr�� 

		}

	}
	printf("\n--------------------------------\n");

}
*/
// [ ��� ]
//	pt_num: 10 20 30 40
//	--------------------------------
//	pt_arr : 1 2 3 4
//	--------------------------------


// �޸� �����Ҵ�. malloc
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
		printf("%d��° ������ �Է� : ", i + 1);
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

// < ����ü�� ����ü >
// �̵��� �����ϱ� ���ؼ��� ���� �ʵ�� ���ڵ带 �����ؾ� �Ѵ�.
// char name[11]; �� �̸����� ���, char tel[11]; �� ��ȭ��ȣ�� ����ϰ� ���� ��
//	name�� tel�� �ʵ�� �θ���, �ѽ��� name�� tel�� ��� ���ڵ�� �θ���.

//	- ����ü.
//	����ü�� �迭�� ���� �������� �����̴�. 
//	�迭�� �ٸ� ���� �迭�� ������ ������ Ÿ������ �̷��� ������
//					 ����ü�� ���� �ٸ� ������ Ÿ���� ������ ���� �� �ִ�. => char c[10]; �� int i[5];�� ��� ����ü�� ���.
//																			  struct ci[15]{ char c[10];
//																							 int i[5];
//																							 ...�����͸� ����...}		

// - ����ü
//	�ϳ��� �޸𸮿� ���δٸ� Ÿ���� ���� ������ ������Ű�� ����̴�. 
//	union anyaname{ char name;
//					int num;
//					...}

// �������� ������ ���� �� �ִ� �������� ���� �����ؼ� ����ϴ� ������� ���������� ���� ��ȣ ����� �����μ� ȣ���Ͽ� ��� �� �� �ִ�.
// enum COMPUTER(MONITOR, RAM, HDD, DVD)






