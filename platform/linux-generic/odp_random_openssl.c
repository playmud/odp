/* Copyright (c) 2014-2018, Linaro Limited
 * Copyright (c) 2020, Nokia
 * All rights reserved.
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#include <odp_posix_extensions.h>
#include <stdint.h>
#include <odp/api/random.h>
#include <odp/autoheader_internal.h>
#include <odp_init_internal.h>
#include <odp_random_openssl_internal.h>

#if _ODP_OPENSSL
#include <openssl/rand.h>

odp_random_kind_t _odp_random_openssl_max_kind(void)
{
	return ODP_RANDOM_CRYPTO;
}

int32_t _odp_random_openssl_data(uint8_t *buf, uint32_t len,
				 odp_random_kind_t kind)
{
	int rc;

	switch (kind) {
	case ODP_RANDOM_BASIC:
	case ODP_RANDOM_CRYPTO:
		rc = RAND_bytes(buf, len);
		return (1 == rc) ? (int)len /*success*/: -1 /*failure*/;

	case ODP_RANDOM_TRUE:
	default:
		return -1;
	}
}

int32_t _odp_random_openssl_test_data(uint8_t *buf, uint32_t len,
				      uint64_t *seed)
{
	union {
		uint32_t rand_word;
		uint8_t rand_byte[4];
	} u;
	uint32_t i = 0, j;
	uint32_t seed32 = (*seed) & 0xffffffff;

	while (i < len) {
		u.rand_word = rand_r(&seed32);

		for (j = 0; j < 4 && i < len; j++, i++)
			*buf++ = u.rand_byte[j];
	}

	*seed = seed32;
	return len;
}
#else
/* Dummy functions for building without OpenSSL support */
odp_random_kind_t _odp_random_openssl_max_kind(void)
{
	return ODP_RANDOM_BASIC;
}

int32_t _odp_random_openssl_data(uint8_t *buf ODP_UNUSED,
				 uint32_t len ODP_UNUSED,
				 odp_random_kind_t kind ODP_UNUSED)
{
	return -1;
}

int32_t _odp_random_openssl_test_data(uint8_t *buf ODP_UNUSED,
				      uint32_t len ODP_UNUSED,
				      uint64_t *seed ODP_UNUSED)
{
	return -1;
}
#endif /* _ODP_OPENSSL */

int _odp_random_openssl_init_local(void)
{
	return 0;
}

int _odp_random_openssl_term_local(void)
{
	return 0;
}
