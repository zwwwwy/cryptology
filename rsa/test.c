#include "utils.h"
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int print_form(const uint2048_t* a, const uint2048_t* b, const uint2048_t* c, char* str)
{
	print_uint2048(a);
	printf(" %s ", str);
	print_uint2048(b);
	printf(" = ");
	print_uint2048(c);
	printf("\n");
	return 1;
}

// 除法测试
int test_div()
{
	uint2048_t* a = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* b = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* c = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(a), set0_uint2048(b), set0_uint2048(c);
	a->data[63] = 0xFFFFFFFF;
	a->data[62] = 0xFFFFFFFF;
	a->data[0]	= 0xFFFa6FFF;
	// b->data[63] = 17;
	// b->data[62] = 1231;
	b->data[12] = 0x123ab23f;
	b->data[20] = 0x123;
	b->data[63] = 0xFFFFFFFF;
	b->data[62] = 0xFFFFFFFF;
	div_uint2048(c, a, b);
	// print_uint2048(a), printf(" / "), print_uint2048(b), printf(" = "), print_uint2048(c);
	print_form(a, b, c, "/");
	free(a), free(b), free(c);
	return 1;
}

// 快速幂指数求模测试
int test_mod_div()
{
	uint2048_t* a = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* b = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* c = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(a), set0_uint2048(b), set0_uint2048(c);

	a->data[63] = 1612050119;
	b->data[63] = 1757316971;
	c->data[63] = 0x48C15219;
	c->data[62] = 1;
	mod_pow_uint2048(c, a, b, c);
	print_uint2048(c);
	printf("\n");

	free(a), free(b), free(c);
	return 1;
}

int test_isprime()
{
	clock_t start, end;
	clock_t start_1, end_1;

	uint2048_t* a = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(a);

	int try = 0;
	start	= clock();
	while (1)
	{
		++try;
		rand1024_in_2048(a);
		printf("尝试次数: %d\n", try);
		start_1	 = clock();
		int flag = isprime_uint2048(a);
		end_1	 = clock();
		printf("单次素性检验耗时=%fs\n", (double)(end_1 - start_1) / CLOCKS_PER_SEC);
		if (flag)
		{
			print_uint2048(a);
			printf("\n");
			break;
		}
		print_uint2048(a), printf("\n");
	}
	end = clock();
	printf("time=%fs\n", (double)(end - start) / CLOCKS_PER_SEC);

	free(a);
	return 1;
}

int test_gcd()
{
	uint2048_t* a = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* b = (uint2048_t*)malloc(sizeof(uint2048_t));
	a->data[63]	  = 11;
	b->data[63]	  = 10;
	gcd_uint2048(a, a, b);
	print_uint2048(a);

	free(a), free(b);
	return 1;
}

int test_inv()
{
	uint2048_t* x = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* y = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(x), set0_uint2048(y);

	uint2048_t e, t;
	set0_uint2048(&e), set0_uint2048(&t);

	str_to_uint2048(&t, "0xc0a41cca6d721f95a5ae2642fe8983a65f7ffbbc8f678c544e0135a2c79291d326e29aff359e70d50e979179a579"
						"109cbb3e3dd155ab80bfb50c9b247e3cac5fa34c8b3956d96ddcf65e1c1ddd16d22458db6836d577584bed37a70521"
						"8f86f911effab96f84abfcb6172d7bacf5f82a39a388d06f083229ed772ac20d066cc0");
	e.data[63] = 65537;
	// mod_inverse_uint2048(e, t, x, y);
	// add_uint2048(x, x, &t);

	mod_inverse_uint2048_new(x, &e, &t);
	print_uint2048(x);

	free(x), free(y);
	return 1;
}

int test_prime_generate()
{
	uint2048_t* a = (uint2048_t*)malloc(sizeof(uint2048_t));
	time_t		start, end;

	start = clock();
	generate_prime512(a);
	end = clock();
	print_uint2048(a), printf("\n");
	printf("素数生成算法耗时=%fs\n", (double)(end - start) / CLOCKS_PER_SEC);

	start = clock();
	do
		rand512_in_2048(a);
	while (!isprime_uint2048(a));
	end = clock();
	print_uint2048(a), printf("\n");
	printf("随机素数生成耗时=%fs\n", (double)(end - start) / CLOCKS_PER_SEC);

	return 1;
}

int main()
{
	// printf("快速除法测试\n");
	// test_div();
	// printf("\n快速幂指数求模测试\n");
	// test_mod_div();
	// printf("\n素性测试\n");
	// test_isprime();
	// printf("\n最大公约数测试\n");
	// test_gcd();
	printf("乘法逆元测试\n");
	test_inv();
	// printf("公钥写入\n");
	// test_file();
	// printf("素数生成测试\n");
	// test_prime_generate();
	return 0;
}
