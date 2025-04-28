#include <stdio.h>
#include <stdlib.h>

typedef struct matrix
{
	int** data;
	int	  row;
	int	  col;
} matrix_t;

int matrix_init(matrix_t* matrix, int row, int col)
{
	matrix->data = (int**)malloc(sizeof(int*) * row);
	for (int i = 0; i < row; i++)
	{
		matrix->data[i] = (int*)malloc(sizeof(int) * col);
	}
	matrix->row = row;
	matrix->col = col;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			matrix->data[i][j] = 0;
		}
	}
	return 1;
}

int matrix_print(matrix_t matrix)
{
	for (int i = 0; i < matrix.row; ++i)
	{
		for (int j = 0; j < matrix.col; ++j)
			printf("%d\t", matrix.data[i][j]);
		printf("\n");
	}
	return 1;
}

int matrix_cpy(matrix_t* matrix, int* data)
{
	for (int i = 0; i < matrix->row; ++i)
	{
		for (int j = 0; j < matrix->col; ++j)
			matrix->data[i][j] = data[matrix->col * i + j];
	}
	return 1;
}

matrix_t* matrix_dot(matrix_t left, matrix_t right)
{
	if (left.col != right.row)
	{
		printf("维度错误\n");
		return NULL;
	}
	matrix_t* ans = (matrix_t*)malloc(sizeof(matrix_t));
	matrix_init(ans, left.row, right.col);

	for (int i = 0; i < left.row; ++i)
	{
		for (int j = 0; j < right.col; ++j)
		{
			for (int idx = 0; idx < left.col; ++idx)
				ans->data[i][j] += left.data[i][idx] * right.data[idx][j];
		}
	}
	return ans;
}

matrix_t* matrix_add(matrix_t left, matrix_t right)
{
	if (left.row != right.row || left.col != right.col)
	{
		printf("维度错误\n");
		return NULL;
	}

	matrix_t* ans = (matrix_t*)malloc(sizeof(matrix_t));
	matrix_init(ans, left.row, left.col);
	for (int i = 0; i < left.row; ++i)
		for (int j = 0; j < left.col; ++j)
			ans->data[i][j] = left.data[i][j] + right.data[i][j];

	return ans;
}

matrix_t* matrix_mul_k(matrix_t left, int k)
{
	matrix_t* ans = (matrix_t*)malloc(sizeof(matrix_t));
	matrix_init(ans, left.row, left.col);
	for (int i = 0; i < left.row; ++i)
		for (int j = 0; j < left.col; ++j)
			ans->data[i][j] = left.data[i][j] * k;

	return ans;
}

int main()
{
	matrix_t mat_A;
	matrix_t mat_B;
	matrix_t mat_A_r;
	matrix_init(&mat_A, 4, 4);
	matrix_init(&mat_B, 4, 1);
	matrix_init(&mat_A_r, 4, 4);
	int data_A[]   = {8, 6, 9, 5, 6, 9, 5, 10, 5, 8, 4, 9, 10, 6, 11, 4};
	int data_B[]   = {1, 21, 8, 17};
	int data_A_r[] = {23, 20, 5, 1, 2, 11, 18, 1, 2, 20, 6, 25, 25, 2, 22, 25};
	matrix_cpy(&mat_A, data_A);
	matrix_cpy(&mat_B, data_B);
	matrix_cpy(&mat_A_r, data_A_r);

	char m[]	= "hillcipheraa";
	int	 length = 0;
	for (char* p = m; *p; ++p)
		length++;

	// 加密
	char*	 c = (char*)malloc(sizeof(char) * length);
	matrix_t tmp;
	for (int i = 0; i < (length + 3) / 4; ++i)
	{
		matrix_init(&tmp, 4, 1);
		for (int j = 0; j < 4 && 4 * i + j < length; ++j)
		{
			tmp.data[j][0] = m[4 * i + j] - 97;
		}

		matrix_t* ans_dot = matrix_dot(mat_A, tmp);
		matrix_t* ans_add = matrix_add(*ans_dot, mat_B);

		for (int j = 0; j < 4 && 4 * i + j < length; ++j)
		{
			c[4 * i + j] = (ans_add->data[j][0] % 26) + 65;
		}
		free(ans_dot->data);
		free(ans_add->data);
		free(ans_dot);
		free(ans_add);
		free(tmp.data);
	}

	printf("密文：");
	for (char* p = c; *p; ++p)
		printf("%c", *p);
	printf("\n");

	// 解密
	char* m2 = (char*)malloc(sizeof(char) * length);
	for (int i = 0; i < (length + 3) / 4; ++i)
	{
		matrix_init(&tmp, 4, 1);
		for (int j = 0; j < 4 && 4 * i + j < length; ++j)
		{
			tmp.data[j][0] = c[4 * i + j] - 65;
		}
		matrix_t* ans_add = matrix_add(tmp, *matrix_mul_k(mat_B, -1));
		matrix_t* ans_dot = matrix_dot(mat_A_r, *ans_add);

		for (int j = 0; j < 4 && 4 * i + j < length; ++j)
		{
			while (ans_dot->data[j][0] < 0)
			{
				ans_dot->data[j][0] += 26;
			}
			m2[4 * i + j] = (ans_dot->data[j][0] % 26) + 97;
		}

		free(ans_dot->data);
		free(ans_add->data);
		free(ans_dot);
		free(ans_add);
		free(tmp.data);
	}

	printf("明文：");
	for (char* p = m2; *p; ++p)
		printf("%c", *p);
	printf("\n");
}
