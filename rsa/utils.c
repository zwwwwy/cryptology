#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG 0

// 打印2048位整数的有效位
int print_uint2048(const uint2048_t* num)
{
	if (num == NULL)
		return -1;

	int flag = 0;
	printf("0x");
	for (int i = 0; i < 64; ++i)
	{
		if (num->data[i])
			flag = 1;
		if (flag)
			printf("%.8x", num->data[i]);
	}

	return 0;
}

// 填充2048位整数的低1024位为随机数
int rand1024_in_2048(uint2048_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 32; i < 64; ++i)
		num->data[i] = rand() & 0xFFFFFFFF;

	return 0;
}

// 2048位整数置0
int set0_uint2048(uint2048_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 0; i < 64; ++i)
		num->data[i] = 0;

	return 0;
}

// 4090位整数置0
int set0_uint4096(uint4096_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 0; i < 128; ++i)
		num->data[i] = 0;

	return 0;
}

// 左移
int shl_uint2048(uint2048_t* num, unsigned shift)
{
	unsigned tmp  = 0;
	unsigned last = 0;

	int zero_blocks = shift / 32;
	shift %= 32;
	if (shift)
	{
		for (int i = 63; i >= zero_blocks; --i)
		{
			tmp = num->data[i];
			num->data[i] <<= shift;
			num->data[i] |= last;
			last = tmp >> (32 - shift);
		}
	}

	if (zero_blocks)
	{
		for (int i = 0; i < 64 - zero_blocks; ++i)
			num->data[i] = num->data[i + zero_blocks];
		for (int i = 0; i < zero_blocks; ++i)
			num->data[63 - i] = 0;
	}
	return 1;
}

// 右移
int shr_uint2048(uint2048_t* num, unsigned shift)
{
	unsigned tmp  = 0;
	unsigned last = 0;

	int zero_blocks = shift / 32;
	shift %= 32;
	if (shift)
	{
		for (int i = 0; i < 64 - zero_blocks; ++i)
		{
			tmp = num->data[i];
			num->data[i] >>= shift;
			num->data[i] |= last;
			last = tmp << (32 - shift);
		}
	}

	if (zero_blocks)
	{
		for (int i = 63; i >= zero_blocks; --i)
			num->data[i] = num->data[i - zero_blocks];
		for (int i = 0; i < zero_blocks; ++i)
			num->data[i] = 0;
	}
	return 1;
}

// 比大小
int isbig_uint2048(const uint2048_t* a, const uint2048_t* b)
{
	for (int i = 0; i < 64; ++i)
	{
		if (a->data[i] == b->data[i])
			continue;
		else if ((unsigned)a->data[i] > (unsigned)b->data[i])
			return 1;
		else
			return 0;
	}
	return 0;
}

// 是否相等
int isequal_uint2048(const uint2048_t* a, const uint2048_t* b)
{
	for (int i = 63; i >= 0; --i)
		if (a->data[i] != b->data[i])
			return 0;
	return 1;
}

// 判0
int iszero_uint2048(const uint2048_t* num)
{
	for (int i = 63; i >= 0; --i)
	{
		if (num->data[i])
			return 0;
	}
	return 1;
}

// 计算2048位整数的比特数
int countbit_uint2048(const uint2048_t* num)
{
	int zero_blocks;
	int zero_counts;
	for (zero_blocks = 0; zero_blocks < 64; ++zero_blocks)
		if (num->data[zero_blocks])
			break;
	for (zero_counts = 0; zero_counts < 32; ++zero_counts)
		if (num->data[zero_blocks] & (1 << (31 - zero_counts)))
			break;

	return 2048 - zero_blocks * 32 - zero_counts;
}

// 复制
int cpy_uint2048(uint2048_t* dest, const uint2048_t* src)
{
	for (int i = 0; i < 64; ++i)
		dest->data[i] = src->data[i];
	return 1;
}

// 2048位整数加法
int add_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);
	unsigned long carry = 0;
	for (int i = 63; i >= 0; --i)
	{
		unsigned long sum = (unsigned long)a->data[i] + (unsigned long)b->data[i] + carry;
		tmp->data[i]	  = (unsigned)(sum & 0xFFFFFFFF);
		carry			  = sum >> 32;
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 2048位整数减法
int sub_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	if (isbig_uint2048(b, a))
	{
		printf("减法溢出\n");
		return 0;
	}

	unsigned long borrow = 0;
	for (int i = 63; i >= 0; --i)
	{
		unsigned long aval = (unsigned long)a->data[i];
		unsigned long bval = (unsigned long)b->data[i] + borrow;

		if (aval >= bval)
		{
			tmp->data[i] = (unsigned)(aval - bval);
			borrow		 = 0;
		}
		else
		{
			tmp->data[i] = (unsigned)(aval + 0x100000000UL - bval); // 加上 2^32 进行借位
			borrow		 = 1;
		}
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 1024位整数乘法
int mul_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);
	for (int i = 63; i >= 32; --i)
	{
		unsigned long carry = 0;
		for (int j = 63; j >= 32; --j)
		{
			int k = i + j - 63;
			if (k < 0 || k > 63)
				continue;
			unsigned long mul = (unsigned long)a->data[i] * b->data[j];
			unsigned long sum = (unsigned long)tmp->data[k] + mul + carry;
			tmp->data[k]	  = (unsigned)(sum & 0xFFFFFFFF);
			carry			  = sum >> 32;
		}
		int k = i + 32 - 63 - 1;
		if (k >= 0 && k <= 63)
			tmp->data[k] += (unsigned)carry;
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 二分法整数除法，要求b小于1024位
int div_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	if (iszero_uint2048(b))
	{
		printf("被除数为零\n");
		return 0;
	}

	if (countbit_uint2048(b) > 1024)
	{
		printf("除数过大\n");
		return 0;
	}

	if (isbig_uint2048(b, a))
	{
		set0_uint2048(result);
		return 1;
	}

	uint2048_t* x_min = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* x_max = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* x	  = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(x_min), set0_uint2048(x_max), set0_uint2048(x);

	// 位数之差一定小于2048
	int delta_bit	= countbit_uint2048(a) - countbit_uint2048(b);
	x_min->data[63] = 1;
	x_max->data[63] = 1;

	if (delta_bit == 0) // 位数相同说明a一定小于b的2倍
	{
		cpy_uint2048(result, x_min);
		return 1;
	}

	shl_uint2048(x_min, delta_bit - 1); // x_min = 2^(delta_bit-1)
	shl_uint2048(x_max, delta_bit + 1);

	uint2048_t one;
	set0_uint2048(&one);
	one.data[63]  = 1;
	int times_mul = 0;
	while (1)
	{
		sub_uint2048(x, x_max, x_min);
		if (!isbig_uint2048(x, &one)) // x_max-x_min <= 1
			break;

		// x = 1.5*(x_min+x_min)
		add_uint2048(x, x_min, x_max);
		shr_uint2048(x, 1);

		mul_uint2048(tmp, b, x);	// tmp = b*x
		if (isbig_uint2048(tmp, a)) // x过大
			cpy_uint2048(x_max, x);
		else if (isbig_uint2048(a, tmp)) // x过小
			cpy_uint2048(x_min, x);

		++times_mul;
	}

	cpy_uint2048(result, x_min);
	free(tmp), free(x_min), free(x_max), free(x);
	if (LOG)
		printf("作除共计搜索%d次\n", times_mul);
	return 1;
}

// 基于整数除法的求模
int mod_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	div_uint2048(tmp, a, b);
	mul_uint2048(tmp, tmp, b);
	sub_uint2048(result, a, tmp);
	return 1;
}

// 快速指数
int pow_uint2048(uint2048_t* result, const uint2048_t* base, const unsigned exp)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	tmp->data[63] = 1;
	for (int j = 31; j >= 0; --j)
	{
		mul_uint2048(tmp, tmp, tmp);
		if (exp & (1 << j))
			mul_uint2048(tmp, tmp, base);
	}
	cpy_uint2048(result, tmp);
	return 1;
}

// 快速幂指数求模
int mod_pow_uint2048(uint2048_t* result, uint2048_t* base, const uint2048_t* exp, const uint2048_t* mod)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	mod_uint2048(base, base, mod);
	tmp->data[63] = 1;

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 31; j >= 0; --j)
		{
			mul_uint2048(tmp, tmp, tmp);
			mod_uint2048(tmp, tmp, mod);
			if (exp->data[i] & (1 << j))
			{
				mul_uint2048(tmp, tmp, base);
				mod_uint2048(tmp, tmp, mod);
			}
		}
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// Miller–Rabin素性检验
int isprime_uint2048(const uint2048_t* num)
{
	uint2048_t const_value;
	set0_uint2048(&const_value);

	// num <= 2时的素性判定
	const_value.data[63] = 2;
	if (!isbig_uint2048(num, &const_value))
	{
		if (isequal_uint2048(num, &const_value))
			return 1;
		else
			return 0;
	}

	// num > 2时的素性判定

	return 1;
}
