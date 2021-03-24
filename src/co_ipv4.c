/*
Copyright (c) 2018-2021 Alexander Pogudaev

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

    Alexander Pogudaev
    pogudaev@yandex.ru

*/

#include "co_ipv4.h"
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

CO_CREATE(co_ipv4)
{
	co_ipv4 *co_ipv4_obj = (co_ipv4 *) calloc(sizeof(co_ipv4), 1);
	return co_ipv4_obj;
}

CO_FREE(co_ipv4)
{
	if (co_ipv4_obj) {
		free(co_ipv4_obj);
	}
}

CO_CLONE(co_ipv4)
{
	if (!co_ipv4_src) {
		return NULL;
	}

	co_ipv4 *co_ipv4_obj = (co_ipv4 *) malloc(sizeof(co_ipv4));

	if (co_ipv4_obj) {
		co_ipv4_obj->addr = co_ipv4_src->addr;
	}

	return co_ipv4_obj;
}

CO_COPY(co_ipv4)
{
	if (!co_ipv4_dst || !co_ipv4_src) {
		return CO_BAD_ARG_ERR;
	}

	co_ipv4_dst->addr = co_ipv4_src->addr;
	return CO_OK;
}

CO_RESET(co_ipv4)
{
	if (!co_ipv4_obj) {
		return CO_BAD_ARG_ERR;
	}

	co_ipv4_obj->addr = 0;
	return CO_OK;
}

CO_COMPARE(co_ipv4)
{
	if (!co_ipv4_a || !co_ipv4_b) {
		return CO_CMP_ERR;
	}

	if (co_ipv4_a->addr == co_ipv4_b->addr) {
		return CO_CMP_EQ;
	} else {
		return CO_CMP_NE;
	}
}

co_ipv4 *co_ipv4_create_from_c_str(const char *str)
{
	if (!str) {
		return NULL;
	}

	co_ipv4 *co_ipv4_obj = NULL;
	uint32_t addr = 0;
	uint8_t *ptr = (uint8_t *)(&addr);
	char end;

	if (sscanf(str, "%d.%d.%d.%d%c", &ptr[0], &ptr[1], &ptr[2], &ptr[3], &end) == 4) {
		co_ipv4_obj = (co_ipv4 *) calloc(sizeof(co_ipv4), 1);

		if (co_ipv4_obj) {
			co_ipv4_obj->addr = addr;
		}
	}

	return co_ipv4_obj;
}

co_status co_ipv4_apply_mask(co_ipv4 *co_ipv4_obj, const co_ipv4 *co_ipv4_mask)
{
	if (!co_ipv4_obj || !co_ipv4_mask) {
		return CO_BAD_ARG_ERR;
	}

	co_ipv4_obj->addr = co_ipv4_obj->addr & co_ipv4_mask->addr;
	return CO_OK;
}

//TODO byte order
co_status co_ipv4_is_valid_mask(const co_ipv4 *co_ipv4_mask)
{
	if (!co_ipv4_mask) {
		return CO_BAD_ARG_ERR;
	}

	uint32_t slot = 0x80000000;
	uint32_t test = htonl(co_ipv4_mask->addr);

	while (test & slot) {
		test <<= 1;
	}

	if (test == 0) {
		return CO_OK;
	}

	return CO_CHECK_FAIL;
}
