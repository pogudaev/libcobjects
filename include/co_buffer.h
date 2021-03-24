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

#ifndef CO_BUFFER_H
#define CO_BUFFER_H

#include <stddef.h> //for size_t
#include "co_status_code.h"
#include "co_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

CO_OBJECT_DEFINITION(co_buffer);

struct co_buffer{
    void *data;
    size_t length;
    size_t alloc_length;
};

co_buffer *co_buffer_create_and_alloc(size_t alloc_length);
co_status co_buffer_realloc(co_buffer *co_buffer_obj, size_t alloc_length);
co_status co_buffer_set_alloc_length(co_buffer *co_buffer_obj, size_t alloc_length);
co_status co_buffer_append(co_buffer *co_buffer_obj, const co_buffer *co_buffer_src);
co_status co_buffer_add(co_buffer *co_buffer_obj, const void *data, size_t length);
co_status co_buffer_set(co_buffer *co_buffer_obj, const void *data, size_t length);
co_status co_buffer_get(co_buffer *co_buffer_obj, void **data, size_t *length);

#ifdef __cplusplus
}
#endif

#endif // CO_BUFFER_H
