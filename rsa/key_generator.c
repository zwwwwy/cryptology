#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int is_prime(unsigned long n)
{
	if (n <= 1)
		return 0;
	for (int i = 2; i < sqrt(n) + 1; ++i)
		if (n % i == 0 && n != i)
			return 0;
	return 1;
}

unsigned long key_generate()
{
	unsigned long a, b, c, d;
	unsigned long p = 0, q = 0;
	while (~(is_prime(p) & is_prime(q)))
	{
		a = rand();
		b = rand();
		c = rand();
		d = rand();

		p = a << 32 | b;
		q = c << 32 | d;

		if (is_prime(p) & is_prime(q))
		{
			printf("p = 0x%lx\n", p);
			printf("q = 0x%lx\n", q);
		}
	}
	return 0;
}

int main()
{
	key_generate();
	return 0;
}
