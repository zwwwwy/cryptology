#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int key_generate()
{
	uint2048_t* p = malloc(sizeof(uint2048_t));
	uint2048_t* q = malloc(sizeof(uint2048_t));
	uint2048_t* t = malloc(sizeof(uint2048_t));
	uint2048_t* n = malloc(sizeof(uint2048_t));
	uint2048_t* d = malloc(sizeof(uint2048_t));
	uint2048_t	one, e;
	set0_uint2048(&one), set0_uint2048(&e);
	one.data[63] = 1;
	set0_uint2048(p), set0_uint2048(q), set0_uint2048(t), set0_uint2048(d);

	// 素数生成
	// generate_prime512(p);
	// generate_prime512(q);
	do
		rand512_in_2048(p);
	while (!isprime_uint2048(p));
	do
		rand512_in_2048(q);
	while (!isprime_uint2048(q));

	printf("p="), print_uint2048(p), printf("\n");
	printf("q="), print_uint2048(q), printf("\n");

	mul_uint2048(n, p, q);
	sub_uint2048(p, p, &one);
	sub_uint2048(q, q, &one);
	mul_uint2048(t, p, q);

	int E[] = {65537, 17, 3};
	int idx = 0;
	do
	{
		if (idx >= sizeof(E) / sizeof(E[0]))
		{
			printf("未找到合适的e.\n");
			break;
		}
		e.data[63] = E[idx++];
		gcd_uint2048(q, t, &e);
	} while (!q);

	set0_uint2048(d), set0_uint2048(q);
	mod_inverse_uint2048(e, *t, d, q);
	add_uint2048(d, d, t);
	printf("t = "), print_uint2048(t), printf("\n");
	printf("n = "), print_uint2048(n), printf("\n");
	printf("e = "), print_uint2048(&e), printf("\n");
	printf("d = "), print_uint2048(d), printf("\n");

	char* str	  = "Hello, world! Encrypted by RSA.";
	int	  str_len = 0;
	for (char* ptr = str; *ptr; ++ptr)
		++str_len;
	// for (int i = 0; i < str_len; i += 4)
	//     printf("%.8x ", *((unsigned*)str + i));

	for (int i = 0; i < str_len; i += 64)
	{
		uint2048_t* c = (uint2048_t*)malloc(sizeof(uint2048_t));
		set0_uint2048(c);
		for (int i = 48; i < 64; ++i)
		{
			c->data[i] |= (unsigned)*(str) << 24;
			c->data[i] |= (unsigned)*(str + 1) << 16;
			c->data[i] |= (unsigned)*(str + 2) << 8;
			c->data[i] |= (unsigned)*(str + 3);
			str += 4;
		}

		set0_uint2048(c);
		c->data[63] = 0x66666;

		printf("m0="), print_uint2048(c), printf("\n");
		mod_pow_uint2048(c, c, &e, n);
		printf("\n加密：\n%s", (char*)c);
		printf("c="), print_uint2048(c), printf("\n");
		mod_pow_uint2048(c, c, d, n);
		printf("解密：%s", (char*)c);
		printf("m="), print_uint2048(c), printf("\n");
	}

	// FILE* fp = fopen("rsa.pub", "w");
	// fprintf(fp, "This is testing for fprintf...\n");
	// fputs("This is testing for fputs...\n", fp);
	// fclose(fp);
	free(p), free(q), free(t), free(n), free(d);
	return 1;
}

int main()
{
	key_generate();
	return 0;
}
