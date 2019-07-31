#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"

int sum(int i, int j);
float sum(float i, float j);

void main() {
	int i = 10, j = 20, k;
	float a = 3.2, b = 4.5, c;

	k = sum(i, j);
	c = sum(a, b);
	printf("int sum : %d, float sum : %f \n", k, c);

}

int sum(int i, int j) {
	int k;
	k = i + j;
	return k;
}

float sum(float i, float j) {
	float n;
	n = i + j;
	return n;
}