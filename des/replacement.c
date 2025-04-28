#include "replacement.h"
#include <stdio.h>
#define TOP_BIT 0x8000000000000000
#define HIGH_6_IN_48 0xfc0000000000
#define HIGH_4_IN_32 0xf0000000

extern const unsigned char e_table[48];
extern const unsigned char s_box[8][4][16];
extern const unsigned char p_table[32];
extern const unsigned char PC1_table[56];
extern const unsigned char PC2_table[48];
extern const unsigned char P_table[32];

unsigned long IP_replacement(unsigned long key, unsigned long* ret, const unsigned char* table)
{
	for (int i = 0; i < 64; ++i)
	{
		if (key & (TOP_BIT >> (table[i] - 1)))
			*ret |= TOP_BIT >> i;
	}
	return 1;
}

unsigned long E_replacement(unsigned long key, unsigned long* ret)
{
	unsigned int tmp = 0xffffffff & key;
	for (int i = 0; i < 48; ++i)
	{
		if (tmp & (TOP_BIT >> (e_table[i] - 1 + 32)))
			*ret |= TOP_BIT >> i;
	}
	*ret >>= 16;
	return 1;
}

unsigned long S_replacement(unsigned long key, unsigned long* ret)
{
	unsigned char current_6_bits;
	unsigned char current_row;
	unsigned char current_col;
	unsigned char current_val;
	// printf("Si\trow\tcol\tret\n");
	*ret = 0;
	for (int i = 0; i < 8; ++i)
	{
		current_6_bits = 0;
		current_6_bits |= (key & (HIGH_6_IN_48 >> (6 * i))) >> (42 - 6 * i);
		current_row = (1 & current_6_bits) + (current_6_bits >> 5) * 2;
		current_col = (current_6_bits >> 1) & 0xf;
		current_val = s_box[i][current_row][current_col];

		*ret |= current_val << (28 - 4 * i);
		// printf("%.6b\t%d\t%d\t%.4b\n", current_6_bits, current_row, current_col, current_val);
	}
	*ret &= 0x00000000ffffffff;
	return 1;
}

unsigned long S_replacement_single(unsigned long key, unsigned long* ret, int i)
{
	unsigned char current_6_bits;
	unsigned char current_row;
	unsigned char current_col;
	unsigned char current_val;

	// printf("Si\trow\tcol\tret\n");
	current_6_bits = key;
	// current_6_bits |= (key & (HIGH_6_IN_48 >> (6 * i))) >> (42 - 6 * i);
	current_row = (1 & current_6_bits) + (current_6_bits >> 5) * 2;
	current_col = (current_6_bits >> 1) & 0xf;
	current_val = s_box[i][current_row][current_col];
	// printf("%.6b\t%d\t%d\t%.4b\n", current_6_bits, current_row, current_col, current_val);

	*ret = 0;
	*ret = current_val;
	return 1;
}

unsigned long PC1_replacement(unsigned long key, unsigned long* ret)
{
	for (int i = 0; i < 56; ++i)
	{
		if (key & (TOP_BIT >> (PC1_table[i]))) // key中校验位为高位，PC1认为校验位在低位
			*ret |= TOP_BIT >> i;
	}
	*ret >>= 8;
	*ret &= 0x00ffffffffffffff;
	return 1;
}

unsigned long PC2_replacement(unsigned long key, unsigned long* ret)
{
	for (int i = 0; i < 48; ++i)
	{
		if (key & (TOP_BIT >> (8 + PC2_table[i] - 1)))
			*ret |= TOP_BIT >> i;
	}
	*ret >>= 16;
	return 1;
}

unsigned long P_replacement(unsigned long key, unsigned long* ret)
{
	for (int i = 0; i < 32; ++i)
	{
		if (key & (TOP_BIT >> (32 + P_table[i] - 1)))
			*ret |= TOP_BIT >> i;
	}
	*ret >>= 32;
	*ret &= 0x00000000ffffffff;
	return 1;
}
