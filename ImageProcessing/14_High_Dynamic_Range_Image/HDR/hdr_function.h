#pragma once
#include "define.h"


unsigned int halfToFloat(uint16 value);
float*       HDRread(const char* name);
void         convertLight2Video(float* src, uint16* dst, const char *mode);
