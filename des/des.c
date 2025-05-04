#include "odd_check.h"
#include "replacement.h"
#include <stdio.h>
extern const unsigned char ip_table[64];
extern const unsigned char ip_table_r[64];

#define SHL_1_CIR(x) (((x << 1) | (x >> 27)) & 0x0fffffff)
#define SHL_2_CIR(x) (((x << 2) | (x >> 26)) & 0x0fffffff)
int key_lst_generate(unsigned long key, unsigned long* key_lst)
{
	unsigned long CD = 0;
	unsigned int  C = 0, D = 0;
	PC1_replacement(key, &CD);
	D = CD & 0x0fffffff;
	C = CD >> 28;
	for (int i = 1; i <= 16; ++i)
	{
		if (i == 1 || i == 2 || i == 9 || i == 16)
		{
			C = SHL_1_CIR(C);
			D = SHL_1_CIR(D);
		}
		else
		{
			C = SHL_2_CIR(C);
			D = SHL_2_CIR(D);
		}
		key_lst[i - 1] = ((unsigned long)C << 28) + D;
	}
	return 1;
}

unsigned long des(unsigned long info, unsigned long* key_lst)
{
	unsigned long key_i;
	unsigned long IP = 0;
	IP_replacement(info, &IP, ip_table);
	unsigned long L = IP >> 32;
	unsigned long R = IP & 0xffffffff;
	unsigned long R_replaced;
	unsigned long R_init;
	unsigned long tmp;

	// printf("init L:0x%.16lx\n", L);
	// printf("init R:0x%.16lx\n", R);
	for (int i = 0; i < 16; ++i)
	{
		R_init = R;
		// printf("开始第%d轮DES\n", i + 1);
		key_i = 0;
		PC2_replacement(key_lst[i], &key_i);
		// printf("本轮的子密钥:0x%.16lx\n", key_i);

		R_replaced = 0;
		// printf("E表扩展前的R%d:0x%.16lx\n", i + 1, R);
		E_replacement(R, &R_replaced);
		// printf("E表扩展后的R%d:0x%.16lx\n", i + 1, R_replaced);

		R_replaced ^= key_i;
		R = 0;
		S_replacement(R_replaced, &R);
		// printf("S盒代换后的R%d:0x%.16lx\n", i + 1, R);

		R_replaced = 0;
		P_replacement(R, &R_replaced);
		// printf("P盒置换后的R%d:0x%.16lx\n", i + 1, R_replaced);

		R = R_replaced ^ L;
		L = R_init;
		// printf("L%d:0x%.16lx\n", i + 2, L);
		// printf("R%d:0x%.16lx\n", i + 2, R);
		// printf("\n");
	}

	tmp = R;
	R	= L;
	L	= tmp;
	tmp = (L << 32) + R;
	IP	= 0;
	IP_replacement(tmp, &IP, ip_table_r);

	return IP;
}

unsigned long des_n_rounds(unsigned long info, unsigned long* key_lst, int rounds)
{
	unsigned long key_i;
	unsigned long IP = 0;
	IP_replacement(info, &IP, ip_table);
	unsigned long L = IP >> 32;
	unsigned long R = IP & 0xffffffff;
	unsigned long R_replaced;
	unsigned long R_init;
	unsigned long tmp;

	// printf("init L:0x%.16lx\n", L);
	// printf("init R:0x%.16lx\n", R);
	for (int i = 0; i < rounds; ++i)
	{
		R_init = R;
		// printf("开始第%d轮DES\n", i + 1);
		key_i = 0;
		// PC2_replacement(key_lst[i], &key_i);
		key_i = key_lst[i];
		// printf("本轮的子密钥:0x%.16lx\n", key_i);

		R_replaced = 0;
		// printf("E表扩展前的R%d:0x%.16lx\n", i + 1, R);
		E_replacement(R, &R_replaced);
		// printf("E表扩展后的R%d:0x%.16lx\n", i + 1, R_replaced);

		R_replaced ^= key_i;
		R = 0;
		// printf("S盒代换前的R%d:0x%.16lx\n", i + 1, R_replaced);
		S_replacement(R_replaced, &R);
		// printf("S盒代换后的R%d:0x%.16lx\n", i + 1, R);

		// R_replaced = 0;
		// P_replacement(R, &R_replaced);
		R_replaced = R;
		// printf("P盒置换后的R%d:0x%.16lx\n", i + 1, R_replaced);

		R = R_replaced ^ L;
		L = R_init;
		// printf("L%d:0x%.16lx\n", i + 2, L);
		// printf("R%d:0x%.16lx\n", i + 2, R);
		// printf("\n");
	}

	tmp = R;
	R	= L;
	L	= tmp;
	tmp = (L << 32) + R;
	IP	= 0;
	IP_replacement(tmp, &IP, ip_table_r);

	return IP;
}
