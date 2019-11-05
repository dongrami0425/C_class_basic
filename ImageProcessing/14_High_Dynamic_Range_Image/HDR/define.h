#pragma once

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8   uint8;

#define WIDTH   1920
#define HEIGHT  1080
#define CHANNEL 3

#define BITDEPTH 10

#define SDR_MAX  1000.0  //nits => SDR_max:SDR_min = 1000:1
#define SDR_MIN  SDR_MAX/1000.0    //nits
#define HDR_MAX  10000.0           //nits
#define HDR_MIN  HDR_MAX/100000.0  //nits	