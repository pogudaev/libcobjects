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

#include "co_buffer.h"
#include <stdlib.h>
#include <string.h>

static co_status _co_buffer_realloc(co_buffer *co_buffer, size_t alloc_length)
{
	void *new_data = realloc(co_buffer->data, alloc_length);

	if (new_data == NULL) {
		return CO_MEM_ALLOC_ERR;
	}

	co_buffer->data = new_data;
	co_buffer->alloc_length = alloc_length;
	return CO_OK;
}

static co_status _co_buffer_set_alloc_length(co_buffer *co_buffer, size_t alloc_length)
{
	void *new_data = malloc(alloc_length);

	if (new_data == NULL) {
		return CO_MEM_ALLOC_ERR;
	}

	free(co_buffer->data);
	co_buffer->data = new_data;
	co_buffer->alloc_length = alloc_length;
	return CO_OK;
}

static co_status _co_buffer_add(co_buffer *co_buffer_obj, const void *data, size_t length)
{
	if (length > co_buffer_obj->alloc_length - co_buffer_obj->length) {
		co_status status = _co_buffer_realloc(co_buffer_obj, co_buffer_obj->length + length);

		if (status < 0) {
			return status;
		}
	}

	memcpy((char *) co_buffer_obj->data + co_buffer_obj->length, data, length);
	co_buffer_obj->length += length;
	return CO_OK;
}

CO_CREATE(co_buffer)
{
	return co_buffer_create_and_alloc(0);
}

CO_FREE(co_buffer)
{
	if (co_buffer_obj) {
		if (co_buffer_obj->data) {
			free(co_buffer_obj->data);
		}

		free(co_buffer_obj);
	}
}

CO_CLONE(co_buffer)
{
	if (!co_buffer_src) {
		return NULL;
	}

	co_buffer *co_buffer_obj = (co_buffer *) malloc(sizeof(co_buffer));

	if (co_buffer_obj) {
		co_buffer_obj->data = malloc(co_buffer_src->length);

		if (co_buffer_obj->data == NULL) {
			free(co_buffer_obj);
			return NULL;
		}

		co_buffer_obj->length = co_buffer_src->length;
		co_buffer_obj->alloc_length = co_buffer_src->length;
		memcpy(co_buffer_obj->data, co_buffer_src->data, co_buffer_src->length);
	}

	return co_buffer_obj;
}

CO_COPY(co_buffer)
{
	if (!co_buffer_src || !co_buffer_dst) {
		return CO_BAD_ARG_ERR;
	}

	void *new_data = malloc(co_buffer_src->length);  //обеспечиваем копирование только значимой части

	if (new_data == NULL) {
		return CO_MEM_ALLOC_ERR;
	}

	free(co_buffer_dst->data);
	co_buffer_dst->data = new_data;
	co_buffer_dst->length = co_buffer_src->length;
	co_buffer_dst->alloc_length = co_buffer_src->length;
	memcpy(co_buffer_dst->data, co_buffer_src->data, co_buffer_dst->alloc_length);
	return CO_OK;
}

CO_RESET(co_buffer)
{
	if (co_buffer_obj == NULL) {
		return CO_BAD_ARG_ERR;
	}

	void *new_data = malloc(0);

	if (new_data == NULL) {
		return CO_MEM_ALLOC_ERR;
	}

	free(co_buffer_obj->data);
	co_buffer_obj->data = new_data;
	co_buffer_obj->length = 0;
	co_buffer_obj->alloc_length = 0;
	return CO_OK;
}

CO_COMPARE(co_buffer)
{
	if (co_buffer_a == NULL || co_buffer_b == NULL) {
		return CO_CMP_ERR;
	}

	size_t compare_length = (co_buffer_a->length < co_buffer_b->length) ? co_buffer_a->length : co_buffer_b->length;
	int compare_result = memcmp(co_buffer_a->data, co_buffer_b->data, compare_length);

	if (compare_result == CO_CMP_EQ) {
		if (co_buffer_a->length < co_buffer_b->length) {
			compare_result = CO_CMP_LT;
		} else if (co_buffer_a->length > co_buffer_b->length) {
			compare_result = CO_CMP_GT;
		}
	}

	return compare_result;
}

co_status co_buffer_realloc(co_buffer *co_buffer_obj, size_t alloc_length)
{
	if (co_buffer_obj == NULL) {
		return CO_BAD_ARG_ERR;
	}

	return _co_buffer_realloc(co_buffer_obj, alloc_length);
}

co_status co_buffer_set_alloc_length(co_buffer *co_buffer_obj, size_t alloc_length)
{
	if (co_buffer_obj == NULL) {
		return CO_BAD_ARG_ERR;
	}

	return _co_buffer_set_alloc_length(co_buffer_obj, alloc_length);
}

co_status co_buffer_set(co_buffer *co_buffer_obj, const void *data, size_t length)
{
	if (co_buffer_obj == NULL || data == NULL) {
		return CO_BAD_ARG_ERR;
	}

	if (length > co_buffer_obj->alloc_length) {
		co_status status = _co_buffer_set_alloc_length(co_buffer_obj, length);

		if (status < 0) {
			return status;
		}
	}

	memcpy(co_buffer_obj->data, data, length);
	co_buffer_obj->length = length;
	return CO_OK;
}

co_status co_buffer_add(co_buffer *co_buffer_obj, const void *data, size_t length)
{
	if (co_buffer_obj == NULL || data == NULL) {
		return CO_BAD_ARG_ERR;
	}

	return _co_buffer_add(co_buffer_obj, data, length);
}

co_status co_buffer_get(co_buffer *co_buffer_obj, void **data, size_t *length)
{
	if (co_buffer_obj == NULL || data == NULL || length == NULL) {
		return CO_BAD_ARG_ERR;
	}

	*data = malloc(co_buffer_obj->length);

	if (*data == NULL) {
		*length = 0;
		return CO_MEM_ALLOC_ERR;
	}

	memcpy(*data, co_buffer_obj->data, co_buffer_obj->length);
	*length = co_buffer_obj->length;
	return CO_OK;
}

co_buffer *co_buffer_create_and_alloc(size_t alloc_length)
{
	co_buffer *co_buffer_obj = (co_buffer *) malloc(sizeof(co_buffer));

	if (co_buffer_obj) {
		co_buffer_obj->data = malloc(alloc_length);

		if (co_buffer_obj->data == NULL) {
			free(co_buffer_obj);
			return NULL;
		}

		co_buffer_obj->length = 0;
		co_buffer_obj->alloc_length = alloc_length;
	}

	return co_buffer_obj;
}

co_status co_buffer_append(co_buffer *co_buffer_obj, const co_buffer *co_buffer_src)
{
	if (co_buffer_obj == NULL || co_buffer_src == NULL) {
		return CO_BAD_ARG_ERR;
	}

	return _co_buffer_add(co_buffer_obj, co_buffer_src->data, co_buffer_src->length);
}
