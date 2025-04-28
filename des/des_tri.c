#include "des.h"
#include "odd_check.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	unsigned long  key0[3] = {0x123456789abcdef, 0x987654321abcdef, 0xfedcba159876543};
	unsigned long  key	   = 0;
	unsigned long* key_lst[3];
	unsigned long* key_lst_2[3];
	for (int i = 0; i < 3; ++i)
	{
		key_lst[i]	 = (unsigned long*)malloc(sizeof(long) * 16);
		key_lst_2[i] = (unsigned long*)malloc(sizeof(long) * 16);
	}

	for (int i = 0; i < 3; ++i)
	{
		key_generate(key0[i], &key);
		key0[i] = 0;
		key_check(key, &key0[i]);
		key_lst_generate(key, key_lst[i]);
		printf("\n");
	}

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 16; ++j)
			key_lst_2[2 - i][15 - j] = key_lst[i][j];
	unsigned long* tmp = key_lst_2[1];

	// 兼容普通DES
	key_lst_2[1] = key_lst[1];
	key_lst[1]	 = tmp;

	char str[]	= "Hello, world! Encrypted by triple DES.";
	int	 length = 0;
	for (char* p = str; *p; ++p)
		++length;
	unsigned long ciphers[(length + 7) / 8];
	for (int i = 0; i < (length + 7) / 8; ++i)
		ciphers[i] = *((unsigned long*)str + i);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < (length + 7) / 8; ++j)
			ciphers[j] = des(ciphers[j], key_lst[0]);

	printf("密文:");
	for (int i = 0; i < length; ++i)
		printf("%lx", *((long*)ciphers + i));
	printf("\n");
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < (length + 7) / 8; ++j)
			ciphers[j] = des(ciphers[j], key_lst_2[2]);

	printf("明文:");
	for (int i = 0; i < length; ++i)
		printf("%c", *((char*)ciphers + i));
	printf("\n");
	return 0;
}
