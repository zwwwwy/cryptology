#include "utils.h"
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
	// b->data[63] = 17;
	// b->data[62] = 1231;
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

	str_to_uint2048(
		&t, "0x03cb0de6ba600a52196f72325e609533314a9b2427575199242deec54f1df92db37235ccdcd737abc4190699c725f024d4e98599"
			"a3a08aaa0fd0f24b548e7424c5f122d495218d9c056fca55c11865e514ca6a52cea1584e3b1b63f8af35513da035535cd7a04b9117"
			"2a35ff62b7d6a6bb5156da18c740f2be0ba4219c07082b9f056c8aaf6a7808c750506a78f2826f83b5cdc1bba49118fda61789569c"
			"388bc2295baa018bb4d7bc5fc128e0e4bc5b68e85bbc8a7a4d268c1bd74d1bbf2067046c89894624d960fe9bfaed763f807838af99"
			"6b5270472ab49ee152bfe706cd9876189edf54d8254ee268ce3d25e12532a5cf3f3498fd679986d87ad5c56b9c");
	e.data[63] = 65537;
	mod_inverse_uint2048_fast(x, &e, &t);
	// sub_uint2048(x, x, &t);
	print_uint2048(x);

	free(x), free(y);
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
	return 0;
}
