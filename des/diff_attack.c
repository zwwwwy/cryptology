#include "des.h"
#include "list.h"
#include "odd_check.h"
#include "replacement.h"
#include <stdio.h>
#include <stdlib.h>

int distribute(unsigned long in, int idx, single_in_out_s* l)
{
	unsigned long s1;
	unsigned long s2;
	unsigned long b1;
	unsigned long b2;
	unsigned char distribute_table[16];

	for (int i = 0; i < 16; ++i)
	{
		distribute_table[i] = 0;
	}

	for (b1 = 0; b1 < 0b100000; ++b1)
	{
		b2 = b1 ^ in;
		S_replacement_single(b1, &s1, idx);
		S_replacement_single(b2, &s2, idx);
		distribute_table[s1 ^ s2] += 2;
		list_append(l->in_xor[s1 ^ s2], b1);
		list_append(l->in_xor[s1 ^ s2], b2);
	}

	return 1;
}

unsigned long encrypt(unsigned long key, unsigned long plain, unsigned long rounds)
{
	unsigned long key0	  = key;
	unsigned long key_tmp = 0;
	unsigned long key_lst[16];
	key_generate(key0, &key_tmp);
	key0 = 0;
	key_check(key_tmp, &key0);
	key_lst_generate(key_tmp, key_lst);
	printf("\n");

	for (int i = 0; i < rounds; ++i)
		printf("key%d:\t0x%.16lx\n", i, key_lst[i]);
	printf("子钥生成完成\n\n");

	unsigned long cipher = des_n_rounds(plain, key_lst, rounds);
	printf("密文:0x%.16lx\n", cipher);

	unsigned long key_lst_2[16];
	for (int i = 0; i < rounds; ++i)
	{
		key_lst_2[rounds - 1 - i] = key_lst[i];
	}
	unsigned long plain_text = des_n_rounds(cipher, key_lst_2, rounds);
	printf("明文:0x%.16lx\n", plain_text);

	return cipher;
}

int main()
{
	single_in_out_s l;
	single_in_out_init(&l);
	// distribute(0b110100, 0, &l);
	// list_print(l.in_xor[0b0001]);

	unsigned long key	 = 0x123456789abcdef0;
	unsigned long plain1 = 0x213f231412345678;
	unsigned long plain2 = 0x2312314912345678;

	unsigned long c1 = encrypt(key, plain1, 3);
	unsigned long c2 = encrypt(key, plain2, 3);

	unsigned long L01 = plain1 >> 32;
	unsigned long L02 = plain2 >> 32;

	unsigned long L31 = c1 & 0xffffffff;
	unsigned long R31 = c1 >> 32;
	unsigned long L32 = c2 & 0xffffffff;
	unsigned long R32 = c2 >> 32;

	unsigned long R3_xor = R31 ^ R32;
	unsigned long L0_xor = L01 ^ L02;
	unsigned long C_xor	 = L0_xor ^ R3_xor;

	unsigned long EL31, EL32;
	E_replacement(L31, &EL31);
	E_replacement(L32, &EL32);

	distribute((EL31 ^ EL32) & 0x3f, 7, &l);
	list_print(l.in_xor[C_xor & 0xf]);

	int count[64] = {0};
	for (int i = 0; i < 0b1000000; ++i)
	{
		node_t* head = l.in_xor[C_xor & 0xf]->head;
		for (int j = 0; j < l.in_xor[C_xor & 0xf]->size; ++j)
		{
			++count[i ^ (head->data)];
			head = head->next;
		}
	}
	for (int i = 0; i < 64; ++i)
	{
		printf("count[%d]=%d\n", i, count[i]);
	}
	// printf("C_xor=%lx\n", C_xor);
	// printf("EL31=%lx\n", EL31);
	// printf("EL32=%lx\n", EL32);
	// printf("EL31_xor=%lx\n", EL31 ^ EL32);
	return 0;
}
