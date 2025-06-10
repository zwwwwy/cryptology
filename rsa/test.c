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
		printf("尝试次数: %d\n", try);
		rand1024_in_2048(a);
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

	return 1;
}

int main()
{
	// printf("快速除法测试\n");
	// test_div();
	// printf("\n快速幂指数求模测试\n");
	// test_mod_div();
	printf("\n素性测试\n");
	test_isprime();
	return 0;
}
