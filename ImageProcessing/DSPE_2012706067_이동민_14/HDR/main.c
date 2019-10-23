#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "hdr_function.h"
#include "define.h"

int main()
{
	FILE *fp_SDR, *fp_HDR;

	fp_SDR = fopen("SDR_444_1920x1080.rgb", "wb");
	fp_HDR = fopen("HDR_444_1920x1080.rgb", "wb");

	float   *src;
	uint16  *dst_HDR= (uint16*)malloc(sizeof(uint16) * HEIGHT*WIDTH*CHANNEL);
	uint16  *dst_SDR = (uint16*)malloc(sizeof(uint16) * HEIGHT*WIDTH*CHANNEL);

	src = HDRread("input.exr");

	convertLight2Video(src, dst_HDR, "HDR");
	convertLight2Video(src, dst_SDR, "SDR");
	
	fwrite(dst_HDR, sizeof(uint16), HEIGHT*WIDTH*CHANNEL, fp_HDR);
	fwrite(dst_SDR, sizeof(uint16), HEIGHT*WIDTH*CHANNEL, fp_SDR);

	fclose(fp_SDR);
	fclose(fp_HDR);

	return 0;
}