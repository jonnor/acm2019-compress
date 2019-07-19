#pragma once

// Bit fields making up IEE754 floats
typedef union {
  float f;
  struct {
    uint32_t mantissa:23;
    uint32_t exp:8;
    uint32_t sign:1;
  } b;
} split_float32;

typedef union {
  double d;
  struct {
    uint64_t mantissa:52;
    uint64_t exp:11;
    uint64_t sign:1;
  } b;
} split_float64;


// A 64 byte cacheline whose values can be accessed
// in whatever format is convenient
//
// cacheline cl;
// cl.float32[4].f = 3.14f;
// printf("%x\n", cl.float32[4].b.mantissa);
typedef union {
  uint8_t byte[64];
  uint16_t word[32];
  uint32_t dword[16];
  uint32_t qword[8];
  split_float32 float32[16];
  split_float64 float64[8];
} cacheline;

