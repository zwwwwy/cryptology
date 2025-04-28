#include "des.h"
#include "odd_check.h"
#include <stdio.h>

int main()
{
	unsigned long key0 = 0x54685abd59c65486;
	unsigned long key  = 0;
	unsigned long key_lst[16];
	key_generate(key0, &key);
	key0 = 0;
	key_check(key, &key0);
	key_lst_generate(key, key_lst);
	printf("\n");

	for (int i = 0; i < 16; ++i)
		printf("key%d:\t0x%.16lx\n", i, key_lst[i]);
	printf("子钥生成完成\n\n");

	unsigned long cipher = des(0x6666667777666666, key_lst);
	printf("密文:0x%.16lx\n", cipher);

	unsigned long key_lst_2[16];
	for (int i = 0; i < 16; ++i)
	{
		key_lst_2[15 - i] = key_lst[i];
	}
	unsigned long plain = des(cipher, key_lst_2);
	printf("明文:0x%.16lx\n", plain);

	return 0;
}
