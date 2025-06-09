#ifndef UTILS_H
#define UTILS_H

typedef struct uint1024
{
	unsigned int data[32];
} uint1024_t;

typedef struct uint2048
{
	unsigned int data[64];
} uint2048_t;

typedef struct uint4096
{
	unsigned int data[128];
} uint4096_t;

int print_uint2048(const uint2048_t* num);
int rand1024_in_2048(uint2048_t* num);
int set0_uint2048(uint2048_t* num);
int set0_uint4096(uint4096_t* num);
int shl_uint2048(uint2048_t* num, unsigned shift);
int shr_uint2048(uint2048_t* num, unsigned shift);
int isbig_uint2048(const uint2048_t* a, const uint2048_t* b);
int isequal_uint2048(const uint2048_t* a, const uint2048_t* b);
int iszero_uint2048(const uint2048_t* num);
int countbit_uint2048(const uint2048_t* num);
int cpy_uint2048(uint2048_t* dest, const uint2048_t* src);
int add_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b);
int sub_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b);
int mul_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b);
int div_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b);
int pow_uint2048(uint2048_t* result, const uint2048_t* base, const unsigned exp);
int mod_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b);
int mod_pow_uint2048(uint2048_t* result, uint2048_t* base, const uint2048_t* exp, const uint2048_t* mod);
int isprime_uint2048(const uint2048_t* num);

#endif
