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
	mod_inverse_uint2048(e, t, x, y);
	add_uint2048(x, x, &t);
	print_uint2048(x);

	free(x), free(y);
	return 1;
}

int test_file()
{
	const char* n_hex = "057195a6b9e2334c2b3a704decab07055589bfa263e3955ac047d0d5ea1e6b7e4f57d538d1e70b641205c7b652d2"
						"8a87099f601544b02304d41ebd07b4fa619be27cc5057e52568c0d9242931670e35bce16144719ea596ce861d7330e"
						"928b9406a0f6294f4651187c11267f64d6040a5656cfc6184799781a330e671ac3ab2d";
	const char* e_hex = "010001";
	const char* d_hex = "040065c0845db9d5a0e5394e9ecd6dc0c70b15fa5a311e45f29817f4573ffb46832e8e87510ce4bbafdfddd9b17220"
						"f078eb55e6ec72099bf00d9468c8e950bcd735c0d21b32012a5779822b68623b6c43f8adf73fc4b81b87c765405d8d"
						"94fd3f42c8fb7fdcfd10268342111ccb340980a4bb7f3ab1d8e4dcbb27bd013069a5";
	const char* p_hex =
		"168e121f1eba5d23661e3f1e5dc79ea8540a471c7bd3ee7b51d9c564613efdc50bf72b1411447b7342963e5a0a0382c5"
		"08f2b15e1a32234b3b0fd37968eb2f63";
	const char* q_hex = "e52bcc649469fa82f659db7c753d95435e226f3535dfa75208f2b15e1a32234b3b0fd37968eb2f635a3a654498c587"
						"34eff7d4422f69a8e3241eb79444e8fae102b2e7d5d60d11064b";

	BIGNUM *  n_bn = NULL, *e_bn = NULL, *d_bn = NULL, *p_bn = NULL, *q_bn = NULL;
	BIGNUM *  dmp1 = NULL, *dmq1 = NULL, *iqmp = NULL;
	BIGNUM *  p_minus_1 = NULL, *q_minus_1 = NULL;
	BN_CTX*	  ctx	  = NULL;
	RSA*	  rsa	  = NULL;
	EVP_PKEY* pkey	  = NULL;
	FILE*	  priv_fp = NULL;
	FILE*	  pub_fp  = NULL;
	int		  ret	  = 1; // 假设失败

	// 0. 创建 BN_CTX
	ctx = BN_CTX_new();
	if (!ctx)
	{
		fprintf(stderr, "BN_CTX_new failed.\n");
		goto cleanup;
	}

	// 1. 转换为 BIGNUM
	if (!BN_hex2bn(&n_bn, n_hex) || !BN_hex2bn(&e_bn, e_hex) || !BN_hex2bn(&d_bn, d_hex) || !BN_hex2bn(&p_bn, p_hex) ||
		!BN_hex2bn(&q_bn, q_hex))
	{
		fprintf(stderr, "BN_hex2bn failed for one or more components.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// 1.5 计算 CRT 参数
	p_minus_1 = BN_new();
	q_minus_1 = BN_new();
	dmp1	  = BN_new();
	dmq1	  = BN_new();
	iqmp	  = BN_new();

	if (!p_minus_1 || !q_minus_1 || !dmp1 || !dmq1 || !iqmp)
	{
		fprintf(stderr, "Failed to allocate BIGNUMs for CRT params.\n");
		goto cleanup;
	}

	// p-1 and q-1
	if (!BN_sub(p_minus_1, p_bn, BN_value_one()) || !BN_sub(q_minus_1, q_bn, BN_value_one()))
	{
		fprintf(stderr, "BN_sub for p-1 or q-1 failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// dmp1 = d mod (p-1)
	if (!BN_mod(dmp1, d_bn, p_minus_1, ctx))
	{
		fprintf(stderr, "BN_mod for dmp1 failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// dmq1 = d mod (q-1)
	if (!BN_mod(dmq1, d_bn, q_minus_1, ctx))
	{
		fprintf(stderr, "BN_mod for dmq1 failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// iqmp = q^-1 mod p
	if (!BN_mod_inverse(iqmp, q_bn, p_bn, ctx))
	{
		fprintf(stderr, "BN_mod_inverse for iqmp failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// 2. 创建 RSA 对象
	rsa = RSA_new();
	if (!rsa)
	{
		fprintf(stderr, "RSA_new failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup;
	}

	// 3. 设置 n, e, d
	// RSA_set0_key 会接管 n_bn, e_bn, d_bn 的所有权
	if (!RSA_set0_key(rsa, n_bn, e_bn, d_bn))
	{
		fprintf(stderr, "RSA_set0_key failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_rsa;
	}
	n_bn = NULL;
	e_bn = NULL;
	d_bn = NULL; // 所有权已转移

	// 4. 设置 p, q
	// RSA_set0_factors 会接管 p_bn, q_bn 的所有权
	if (!RSA_set0_factors(rsa, p_bn, q_bn))
	{
		fprintf(stderr, "RSA_set0_factors failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_rsa;
	}
	p_bn = NULL;
	q_bn = NULL; // 所有权已转移

	// 5. 设置 CRT 参数
	// RSA_set0_crt_params 会接管 dmp1, dmq1, iqmp 的所有权
	if (!RSA_set0_crt_params(rsa, dmp1, dmq1, iqmp))
	{
		fprintf(stderr, "RSA_set0_crt_params failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_rsa;
	}
	dmp1 = NULL;
	dmq1 = NULL;
	iqmp = NULL; // 所有权已转移

	// 6. 创建 EVP_PKEY 并分配 RSA
	pkey = EVP_PKEY_new();
	if (!pkey)
	{
		fprintf(stderr, "EVP_PKEY_new failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_rsa;
	}

	// EVP_PKEY_assign_RSA 会接管 rsa 的所有权
	if (!EVP_PKEY_assign_RSA(pkey, rsa))
	{
		fprintf(stderr, "EVP_PKEY_assign_RSA failed.\n");
		ERR_print_errors_fp(stderr);
		RSA_free(rsa);
		rsa = NULL;
		goto cleanup_pkey;
	}
	rsa = NULL; // 所有权已转移

	// 7. 写入私钥
	priv_fp = fopen("key/rsa.key", "w");
	if (!priv_fp)
	{
		perror("Failed to open private.pem for writing");
		goto cleanup_pkey;
	}
	if (!PEM_write_PrivateKey(priv_fp, pkey, NULL, NULL, 0, NULL, NULL))
	{
		fprintf(stderr, "PEM_write_PrivateKey failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_pkey;
	}
	printf("Private key written to private.pem\n");

	// 8. 写入公钥
	pub_fp = fopen("key/pub.key", "w");
	if (!pub_fp)
	{
		perror("Failed to open public.pem for writing");
		goto cleanup_pkey;
	}
	if (!PEM_write_PUBKEY(pub_fp, pkey))
	{
		fprintf(stderr, "PEM_write_PUBKEY failed.\n");
		ERR_print_errors_fp(stderr);
		goto cleanup_pkey;
	}
	printf("Public key written to public.pem\n");

	ret = 0; // 成功

cleanup_pkey:
	if (pkey)
		EVP_PKEY_free(pkey);
	goto final_cleanup_label;

cleanup_rsa:
	if (rsa)
		RSA_free(rsa);

cleanup:
	// 释放任何未被 RSA/EVP_PKEY 对象接管的 BIGNUM
	if (n_bn)
		BN_free(n_bn);
	if (e_bn)
		BN_free(e_bn);
	if (d_bn)
		BN_free(d_bn);
	if (p_bn)
		BN_free(p_bn);
	if (q_bn)
		BN_free(q_bn);
	if (dmp1)
		BN_free(dmp1);
	if (dmq1)
		BN_free(dmq1);
	if (iqmp)
		BN_free(iqmp);

final_cleanup_label: // 避免与 cleanup 标签混淆
	if (p_minus_1)
		BN_free(p_minus_1);
	if (q_minus_1)
		BN_free(q_minus_1);
	if (ctx)
		BN_CTX_free(ctx);
	if (priv_fp)
		fclose(priv_fp);
	if (pub_fp)
		fclose(pub_fp);

	return ret;
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
	// printf("乘法逆元测试\n");
	// test_inv();
	printf("公钥写入\n");
	test_file();
	return 0;
}
