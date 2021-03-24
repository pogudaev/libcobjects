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

#ifndef CO_STRING_H
#define CO_STRING_H

#include <stddef.h>
#include "co_macros.h"
#include "co_status_code.h"

#ifdef __cplusplus
extern "C" {
#endif

CO_OBJECT_DEFINITION(co_string);

struct co_string{
    char *c_str;
    size_t length;
    size_t alloc_length;
};

co_string *co_string_create_from_c_str(const char *str);
co_string *co_string_create_with_allocate(size_t max_str_length);
co_status co_string_set(co_string *co_string_obj, const char *str);
co_status co_string_add(co_string *co_string_obj, const char *str);
co_status co_string_add_substr(co_string *co_string_obj, const char *begin, size_t length);
co_status co_string_append(co_string *co_string_obj, const co_string *co_string_src);
co_status co_string_crop(co_string *co_string_obj, size_t begin, size_t end);

__attribute__((__format__ (__printf__, 2, 0)))
co_status co_string_sprintf(co_string *co_string_obj, const char *__restrict format, ...);



#ifdef __cplusplus
}
#endif

#endif // CO_STRING_H
