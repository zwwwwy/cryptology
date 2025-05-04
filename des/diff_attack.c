#include "des.h"
#include "list.h"
#include "odd_check.h"
#include "replacement.h"
#include <stdio.h>
#include <stdlib.h>

extern const unsigned char ip_table[64];
extern const unsigned char ip_table_r[64];

// idx为s盒的下标,l存放单个s盒的结果
int distribute(unsigned long in, int idx, single_in_out_s* l)
{
	unsigned long s1, s2, b1, b2;

	for (b1 = 0; b1 < 0b100000; ++b1)
	{
		b2 = b1 ^ in;
		S_replacement_single(b1, &s1, idx);
		S_replacement_single(b2, &s2, idx);
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
	// printf("\n");

	for (int i = 0; i < rounds; ++i)
		printf("key%d:\t0x%.16lx\n", i, key_lst[i]);
	printf("子钥生成完成\n\n");

	unsigned long cipher = des_n_rounds(plain, key_lst, rounds);
	// printf("密文:0x%.16lx\n", cipher);

	unsigned long key_lst_2[16];
	for (int i = 0; i < rounds; ++i)
	{
		key_lst_2[rounds - 1 - i] = key_lst[i];
	}
	unsigned long plain_text = des_n_rounds(cipher, key_lst_2, rounds);
	// printf("明文:0x%.16lx\n", plain_text);

	return cipher;
}

void single_diff_attack(unsigned long key, unsigned long plain1_ip, unsigned long plain2_ip, int* count, int idx)
{
	single_in_out_s l;
	single_in_out_init(&l);
	// 通过IP逆置换获得对应明文，用于下面加密
	unsigned long plain1 = 0, plain2 = 0;
	IP_replacement(plain1_ip, &plain1, ip_table_r);
	IP_replacement(plain2_ip, &plain2, ip_table_r);

	// 加密，DES在加密结束后进行了IP逆置换，对结果应用IP置换来恢复
	unsigned long c1 = 0, c2 = 0;
	IP_replacement(encrypt(key, plain1, 3), &c1, ip_table);
	IP_replacement(encrypt(key, plain2, 3), &c2, ip_table);

	// 公式不考虑IP置换，因此下面应当是IP置换后的结果
	unsigned long L01 = plain1_ip >> 32;
	unsigned long L02 = plain2_ip >> 32;

	unsigned long L31 = c1 & 0xffffffff;
	unsigned long R31 = c1 >> 32;
	unsigned long L32 = c2 & 0xffffffff;
	unsigned long R32 = c2 >> 32;

	unsigned long R3_xor = R31 ^ R32;
	unsigned long L0_xor = L01 ^ L02;
	unsigned long C_xor	 = L0_xor ^ R3_xor;

	unsigned long EL31 = 0, EL32 = 0;
	E_replacement(L31, &EL31);
	E_replacement(L32, &EL32);

	int move4 = 4 * (7 - idx), move6 = 6 * (7 - idx);

	distribute(((EL31 ^ EL32) & (0x3ful << move6)) >> move6, idx, &l);

	unsigned long current_X_xor = (C_xor & (0xful << move4)) >> move4;

	node_t* head = l.in_xor[current_X_xor]->head;
	for (int j = 0; j < l.in_xor[current_X_xor]->size; ++j)
	{
		++count[((EL31 & (0x3ful << move6)) >> move6) ^ (head->data)];
		head = head->next;
	}
}

int main()
{

	unsigned long key = 0x123456789abcdef0;
	srand(41);
	unsigned long k3 = 0;
	for (int idx = 0; idx < 8; ++idx)
	{
		int count[64] = {0};
		while (1)
		{
			unsigned int a = rand();
			unsigned int b = rand();
			unsigned int c = rand();

			unsigned long plain1_ip = ((unsigned long)a << 32) | 0xffffffff;
			unsigned long plain2_ip = ((unsigned long)b << 32) | 0xffffffff;

			single_diff_attack(key, plain1_ip, plain2_ip, count, idx);

			int max = 0, max_count = 0, max_i = 0;
			for (int i = 0; i < 64; ++i)
			{
				if (count[i])
				{
					if (count[i] > max)
					{
						max		  = count[i];
						max_i	  = i;
						max_count = 1;
					}
					else if (count[i] == max)
						++max_count;
				}
			}
			if (max_count == 1)
			{
				k3 |= ((unsigned long)max_i << (6 * (7 - idx)));
				break;
			}
		}
	}

	printf("k3=%.12lx\n", k3);
	return 0;
}
