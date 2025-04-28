#include "odd_check.h"
#include <stdio.h>
int main()
{
	unsigned long key;
	unsigned long key2;
	key_generate(9128739872198346, &key);
	key_check(key, &key2);
	printf("\n");
	key_generate(9128739872198346, &key);
	key |= 1;
	key_check(key, &key2);
	return 0;
}
