#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define LOG 0

// 打印2048位整数的有效位
int print_uint2048(const uint2048_t* num)
{
	if (num == NULL)
		return -1;

	int flag = 0;
	printf("0x");
	for (int i = 0; i < 64; ++i)
	{
		if (num->data[i])
			flag = 1;
		if (flag)
			printf("%.8x", num->data[i]);
	}

	return 0;
}

// 填充2048位整数的低1024位为随机数
int rand1024_in_2048(uint2048_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 32; i < 64; ++i)
		num->data[i] = rand() & 0xFFFFFFFF;

	return 0;
}
int rand512_in_2048(uint2048_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 48; i < 64; ++i)
		num->data[i] = rand() & 0xFFFFFFFF;

	return 0;
}

// 2048位整数置0
int set0_uint2048(uint2048_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 0; i < 64; ++i)
		num->data[i] = 0;

	return 0;
}

// 4090位整数置0
int set0_uint4096(uint4096_t* num)
{
	if (num == NULL)
		return -1;

	for (int i = 0; i < 128; ++i)
		num->data[i] = 0;

	return 0;
}

// 左移
int shl_uint2048(uint2048_t* num, unsigned shift)
{
	unsigned tmp  = 0;
	unsigned last = 0;

	int zero_blocks = shift / 32;
	shift %= 32;
	if (shift)
	{
		for (int i = 63; i >= zero_blocks; --i)
		{
			tmp = num->data[i];
			num->data[i] <<= shift;
			num->data[i] |= last;
			last = tmp >> (32 - shift);
		}
	}

	if (zero_blocks)
	{
		for (int i = 0; i < 64 - zero_blocks; ++i)
			num->data[i] = num->data[i + zero_blocks];
		for (int i = 0; i < zero_blocks; ++i)
			num->data[63 - i] = 0;
	}
	return 1;
}

// 右移
int shr_uint2048(uint2048_t* num, unsigned shift)
{
	unsigned tmp  = 0;
	unsigned last = 0;

	int zero_blocks = shift / 32;
	shift %= 32;
	if (shift)
	{
		for (int i = 0; i < 64 - zero_blocks; ++i)
		{
			tmp = num->data[i];
			num->data[i] >>= shift;
			num->data[i] |= last;
			last = tmp << (32 - shift);
		}
	}

	if (zero_blocks)
	{
		for (int i = 63; i >= zero_blocks; --i)
			num->data[i] = num->data[i - zero_blocks];
		for (int i = 0; i < zero_blocks; ++i)
			num->data[i] = 0;
	}
	return 1;
}

// 比大小
int isbig_uint2048(const uint2048_t* a, const uint2048_t* b)
{
	for (int i = 0; i < 64; ++i)
	{
		if (a->data[i] == b->data[i])
			continue;
		else if ((unsigned)a->data[i] > (unsigned)b->data[i])
			return 1;
		else
			return 0;
	}
	return 0;
}

// 是否相等
int isequal_uint2048(const uint2048_t* a, const uint2048_t* b)
{
	for (int i = 63; i >= 0; --i)
		if (a->data[i] != b->data[i])
			return 0;
	return 1;
}

// 判0
int iszero_uint2048(const uint2048_t* num)
{
	for (int i = 63; i >= 0; --i)
	{
		if (num->data[i])
			return 0;
	}
	return 1;
}

// 计算2048位整数的比特数
int countbit_uint2048(const uint2048_t* num)
{
	int zero_blocks;
	int zero_counts;
	for (zero_blocks = 0; zero_blocks < 64; ++zero_blocks)
		if (num->data[zero_blocks])
			break;
	for (zero_counts = 0; zero_counts < 32; ++zero_counts)
		if (num->data[zero_blocks] & (1 << (31 - zero_counts)))
			break;

	return 2048 - zero_blocks * 32 - zero_counts;
}

// 复制
int cpy_uint2048(uint2048_t* dest, const uint2048_t* src)
{
	for (int i = 0; i < 64; ++i)
		dest->data[i] = src->data[i];
	return 1;
}

// 2048位整数加法
int add_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);
	unsigned long carry = 0;
	for (int i = 63; i >= 0; --i)
	{
		unsigned long sum = (unsigned long)a->data[i] + (unsigned long)b->data[i] + carry;
		tmp->data[i]	  = (unsigned)(sum & 0xFFFFFFFF);
		carry			  = sum >> 32;
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 2048位整数减法
int sub_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	// if (isbig_uint2048(b, a))
	// {
	//     printf("减法溢出\n");
	//     return 0;
	// }

	unsigned long borrow = 0;
	for (int i = 63; i >= 0; --i)
	{
		unsigned long aval = (unsigned long)a->data[i];
		unsigned long bval = (unsigned long)b->data[i] + borrow;

		if (aval >= bval)
		{
			tmp->data[i] = (unsigned)(aval - bval);
			borrow		 = 0;
		}
		else
		{
			tmp->data[i] = (unsigned)(aval + 0x100000000UL - bval); // 加上 2^32 进行借位
			borrow		 = 1;
		}
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 1024位整数乘法
int mul_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);
	for (int i = 63; i >= 32; --i)
	{
		unsigned long carry = 0;
		for (int j = 63; j >= 32; --j)
		{
			int k = i + j - 63;
			if (k < 0 || k > 63)
				continue;
			unsigned long mul = (unsigned long)a->data[i] * b->data[j];
			unsigned long sum = (unsigned long)tmp->data[k] + mul + carry;
			tmp->data[k]	  = (unsigned)(sum & 0xFFFFFFFF);
			carry			  = sum >> 32;
		}
		int k = i + 32 - 63 - 1;
		if (k >= 0 && k <= 63)
			tmp->data[k] += (unsigned)carry;
	}

	cpy_uint2048(result, tmp);
	free(tmp);
	return 1;
}

// 二分法整数除法，要求b小于1024位
int div_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t remainder, divisor, quotient, one;
	set0_uint2048(&remainder);
	set0_uint2048(&divisor);
	set0_uint2048(&quotient);
	set0_uint2048(&one);
	one.data[63] = 1;

	cpy_uint2048(&remainder, a);

	int shift = countbit_uint2048(a) - countbit_uint2048(b);
	if (shift < 0)
	{
		set0_uint2048(result);
		return 1;
	}
	cpy_uint2048(&divisor, b);
	shl_uint2048(&divisor, shift);

	for (int i = shift; i >= 0; --i)
	{
		if (isbig_uint2048(&remainder, &divisor) || isequal_uint2048(&remainder, &divisor))
		{
			sub_uint2048(&remainder, &remainder, &divisor);
			quotient.data[63 - i / 32] |= (1U << (i % 32));
		}
		shr_uint2048(&divisor, 1);
	}
	cpy_uint2048(result, &quotient);
	return 1;
}

// 简化的高效取模函数（避免Montgomery的复杂性）
int mod_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	uint2048_t remainder, divisor;
	set0_uint2048(&remainder);
	set0_uint2048(&divisor);

	cpy_uint2048(&remainder, a);

	// 如果 a < b，直接返回 a
	if (!isbig_uint2048(a, b))
	{
		cpy_uint2048(result, a);
		return 1;
	}

	int shift = countbit_uint2048(a) - countbit_uint2048(b);
	if (shift < 0)
	{
		cpy_uint2048(result, a);
		return 1;
	}

	cpy_uint2048(&divisor, b);
	shl_uint2048(&divisor, shift);

	// 长除法的余数计算部分
	for (int i = shift; i >= 0; --i)
	{
		if (isbig_uint2048(&remainder, &divisor) || isequal_uint2048(&remainder, &divisor))
		{
			sub_uint2048(&remainder, &remainder, &divisor);
		}
		shr_uint2048(&divisor, 1);
	}

	cpy_uint2048(result, &remainder);
	return 1;
}

// 快速指数
int pow_uint2048(uint2048_t* result, const uint2048_t* base, const unsigned exp)
{
	uint2048_t* tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);

	tmp->data[63] = 1;
	for (int j = 31; j >= 0; --j)
	{
		mul_uint2048(tmp, tmp, tmp);
		if (exp & (1 << j))
			mul_uint2048(tmp, tmp, base);
	}
	cpy_uint2048(result, tmp);
	return 1;
}

// 快速幂指数求模
int mod_pow_uint2048(uint2048_t* result, const uint2048_t* base, const uint2048_t* exp, const uint2048_t* mod)
{
	uint2048_t* tmp		 = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* tmp_base = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp), set0_uint2048(tmp_base);
	cpy_uint2048(tmp_base, base);

	mod_uint2048(tmp_base, tmp_base, mod);
	tmp->data[63] = 1;

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 31; j >= 0; --j)
		{
			mul_uint2048(tmp, tmp, tmp);
			mod_uint2048(tmp, tmp, mod);
			if (exp->data[i] & (1 << j))
			{
				mul_uint2048(tmp, tmp, tmp_base);
				mod_uint2048(tmp, tmp, mod);
			}
		}
	}

	cpy_uint2048(result, tmp);
	free(tmp), free(tmp_base);
	return 1;
}

// 欧几里得算法求公约数，要求a>b
int gcd_uint2048(uint2048_t* result, const uint2048_t* a, const uint2048_t* b)
{
	if (isbig_uint2048(b, a))
	{
		printf("公约数b大于a\n");
		return 0;
	}
	uint2048_t* tmp	  = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* a_tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	uint2048_t* b_tmp = (uint2048_t*)malloc(sizeof(uint2048_t));
	set0_uint2048(tmp);
	cpy_uint2048(a_tmp, a), cpy_uint2048(b_tmp, b);

	while (!iszero_uint2048(b_tmp))
	{
		cpy_uint2048(tmp, a_tmp);
		cpy_uint2048(a_tmp, b_tmp);
		mod_uint2048(b_tmp, tmp, b_tmp);
	}
	cpy_uint2048(result, a_tmp);

	free(tmp), free(a_tmp), free(b_tmp);
	return 1;
}

// Miller–Rabin素性检验
int isprime_uint2048(const uint2048_t* num)
{
	// 小于2的素性检验
	uint2048_t two, one, n_minus1, d, a, x;
	set0_uint2048(&two);
	set0_uint2048(&one);
	two.data[63] = 2;
	one.data[63] = 1;

	if (!isbig_uint2048(num, &two))
	{
		if (isequal_uint2048(num, &two))
			return 1;
		else
			return 0;
	}

	if ((num->data[63] & 1) == 0) // 判偶
		return 0;

	// 选用确定性底数集合
	unsigned A[]   = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	int		 a_len = sizeof(A) / sizeof(A[0]);

	// n-1 = d * 2^r
	set0_uint2048(&n_minus1);
	sub_uint2048(&n_minus1, num, &one);

	set0_uint2048(&d);
	cpy_uint2048(&d, &n_minus1);

	unsigned r = 0;
	while ((d.data[63] & 1) == 0)
	{
		shr_uint2048(&d, 1);
		r++;
	}

	for (int i = 0; i < a_len; ++i)
	{
		set0_uint2048(&a);
		a.data[63] = A[i];
		// 若底数大于等于num，跳过
		if (!isbig_uint2048(num, &a))
			continue;

		set0_uint2048(&x);
		mod_pow_uint2048(&x, &a, &d, num); // x = a^d mod num

		if (isequal_uint2048(&x, &one) || isequal_uint2048(&x, &n_minus1))
			continue;

		int maybe_prime = 0;
		for (unsigned j = 1; j < r; ++j)
		{
			mul_uint2048(&x, &x, &x);
			mod_uint2048(&x, &x, num);
			if (isequal_uint2048(&x, &n_minus1))
			{
				maybe_prime = 1;
				break;
			}
		}
		if (!maybe_prime)
			return 0;
	}
	return 1;
}

// 扩展欧几里得算法求乘法逆元
uint2048_t mod_inverse_uint2048(uint2048_t a, uint2048_t b, uint2048_t* x, uint2048_t* y)
{
	if (iszero_uint2048(&b))
	{
		set0_uint2048(x), set0_uint2048(y);
		x->data[63] = 1;
		y->data[63] = 0;
		return a;
	}
	uint2048_t tmp;
	mod_uint2048(&tmp, &a, &b);
	uint2048_t d = mod_inverse_uint2048(b, tmp, x, y);
	uint2048_t t;
	cpy_uint2048(&t, x);
	cpy_uint2048(x, y);
	div_uint2048(&tmp, &a, &b);
	mul_uint2048(&tmp, &tmp, x);
	sub_uint2048(y, &t, &tmp);

	return d;
}

// 从十六进制字符串读取大数（只处理十六进制）
int str_to_uint2048(uint2048_t* result, const char* str)
{
	if (result == NULL || str == NULL)
	{
		return -1;
	}

	set0_uint2048(result);

	// 跳过前导空格
	while (*str == ' ' || *str == '\t')
	{
		str++;
	}

	// 检查是否为空字符串
	if (*str == '\0')
	{
		return -1;
	}

	// 处理"0x"或"0X"前缀
	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2;
	}

	// 检查是否为空字符串（去掉前缀后）
	if (*str == '\0')
	{
		return -1;
	}

	// 计算有效字符长度
	const char* p	= str;
	size_t		len = 0;
	while (*p)
	{
		if ((*p >= '0' && *p <= '9') || (*p >= 'A' && *p <= 'F') || (*p >= 'a' && *p <= 'f'))
		{
			len++;
		}
		else
		{
			return -1; // 无效字符
		}
		p++;
	}

	// 每8个字符可以填充一个32位块
	// 最多可以填充64个块（2048位）
	if (len > 512) // 512个十六进制字符 = 2048位
	{
		return -1; // 超出范围
	}

	// 从字符串末尾开始处理，每8个字符一组
	int block_index = 63;			 // 从最低位开始填充
	p				= str + len - 1; // 指向最后一个字符

	// 处理完整的8个字符块
	while (p >= str)
	{
		unsigned int value = 0;
		for (int i = 0; i < 8 && p >= str; i++, p--)
		{
			int digit;
			if (*p >= '0' && *p <= '9')
				digit = *p - '0';
			else if (*p >= 'A' && *p <= 'F')
				digit = *p - 'A' + 10;
			else // (*p >= 'a' && *p <= 'f')
				digit = *p - 'a' + 10;

			value |= (digit << (i * 4));
		}

		result->data[block_index--] = value;
		if (block_index < 0) // 防止超出范围
			break;
	}

	return 0;
}
