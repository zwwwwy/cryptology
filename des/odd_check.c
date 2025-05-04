#include "odd_check.h"
#include <stdio.h>
#define HIGH_BIT 0x8000000000000000l

/*
 * 采用奇校验，校验位置于高位
 * 取无符号64位整数的前56位为密钥
 */
unsigned long key_generate(unsigned long key, unsigned long* ret)
{
	unsigned int count = 0;
	// printf("原始密钥:0x%.16lx\n", key);
	for (int i = 0; i < 64; ++i)
	{
		if ((i + 1) % 8 == 0)
		{
			if (count % 2 == 0)
				*ret |= HIGH_BIT;
			count = 0;
		}
		else
		{
			if ((1ul << (i - (i + 1) / 8)) & key)
			{
				*ret |= HIGH_BIT;
				++count;
			}
		}
		if (i != 63)
			*ret >>= 1;
	}
	// printf("含校验位:0x%.16lx\n", *ret);
	return 1;
}

unsigned long key_check(unsigned long key, unsigned long* ret)
{
	unsigned int count = 0;
	for (int i = 0; i < 64; ++i)
	{
		if ((1ul << i) & key)
			++count;
		if ((i + 1) % 8 == 0)
		{
			if (count % 2 == 0)
			{
				printf("密钥损坏\n");
				return 0;
			}
			count = 0;
		}
		else
		{
			if ((1ul << i) & key)
				*ret |= HIGH_BIT;
			*ret >>= 1;
		}
	}
	*ret >>= 7;
	// printf("复原密钥:0x%.16lx\n", *ret);
	// printf("密钥校验通过\n");
	return 1;
}
