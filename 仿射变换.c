#include <stdio.h>

int main()
{
	int key;
	for (int i = 1; i <= 26; ++i)
	{
		if ((7 * i) % 26 == 1)
		{
			printf("key=%d\n", i);
			key = i;
		}
	}

	char str[] = "china";
	printf("密文为：");
	for (char* p = str; *p; ++p)
	{
		*p = (((*p - 97) * 7 + 3) % 26) + 65;
		printf("%c", *p);
	}
	printf("\n");

	printf("明文为：");
	for (char* p = str; *p; ++p)
	{
		*p = (((*p - 65) * key + 7) % 26) + 97;
		printf("%c", *p);
	}
	printf("\n");
}
