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
	do
		rand1024_in_2048(p);
	while (!isprime_uint2048(p));
	printf("p="), print_uint2048(p), printf("\n");
	do
		rand1024_in_2048(q);
	while (!isprime_uint2048(q));
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

	uint2048_t c;
	set0_uint2048(&c);
	c.data[63] = 0x66666;
	printf("\nm="), print_uint2048(&c), printf("\n");
	mod_pow_uint2048(&c, &c, &e, n);
	printf("c="), print_uint2048(&c), printf("\n");
	printf("验证：\n");
	mod_pow_uint2048(&c, &c, d, n);
	printf("m="), print_uint2048(&c), printf("\n");

	free(p), free(q), free(t), free(n), free(d);
	return 1;
}

int main()
{
	key_generate();
	return 0;
}
