#include "NumberUtils.h"

#include <string.h>

using namespace math;

uint32_t NumberUtils::floatToIntBits (float value) {
    uint32_t y;
    memcpy(&y, &value, sizeof(uint32_t));
    return y;
}

int NumberUtils::floatToRawIntBits (float value) {
    union {
        int i;
        float f;
    } u;
    u.f = (float)value;
    return u.i;
}

float NumberUtils::intBitsToFloat (int value) {
    union {
        float f;  // assuming 32-bit IEEE 754 single-precision
        int i;    // assuming 32-bit 2's complement int
    } u;

    u.i = value;
    return u.f;
}

long NumberUtils::doubleToLongBits (double value) {
  union { double f; long i; } u;
  u.f = value;
  return u.i;
}

double NumberUtils::longBitsToDouble (long value) {
  union { double f; long i; } u;
  u.i = value;
  return u.i;
}