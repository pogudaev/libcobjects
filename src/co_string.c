/*
Copyright (c) 2018-2019 Alexander Pogudaev

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

#include "co_string.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static co_status _co_string_realloc(co_string *co_string_obj, size_t alloc_length)
{
    char * new_str = (char *) realloc(co_string_obj->c_str, alloc_length);
    if (new_str == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    co_string_obj->c_str = new_str;
    co_string_obj->c_str[alloc_length - 1] = '\0';
    co_string_obj->alloc_length = alloc_length;
    return CO_OK;
}

CO_CREATE(co_string)
{
    co_string * co_string_obj = (co_string *) malloc(sizeof(co_string));
    if (co_string_obj){
        co_string_obj->c_str = (char *) malloc(1);
        if (co_string_obj->c_str == NULL){
            free(co_string_obj);
            return NULL;
        }
        co_string_obj->length = 0;
        co_string_obj->alloc_length = 1;
        co_string_obj->c_str[0] = '\0';
    }
    return co_string_obj;
}

CO_FREE(co_string)
{
    if (co_string_obj){
        if (co_string_obj->c_str){
            free(co_string_obj->c_str);
        }
        free(co_string_obj);
    }
}

CO_CLONE(co_string)
{
    if (!co_string_src){
        return NULL;
    }
    co_string * co_string_obj = (co_string *) malloc(sizeof(co_string));
    if (co_string_obj){
        co_string_obj->c_str = (char *) malloc(co_string_src->length + 1);
        if (co_string_obj->c_str == NULL){
            free(co_string_obj);
            return NULL;
        }
        co_string_obj->length = co_string_src->length;
        co_string_obj->alloc_length = co_string_src->length + 1;
        memcpy(co_string_obj->c_str, co_string_src->c_str, co_string_obj->alloc_length);
    }
    return co_string_obj;
}

CO_COPY(co_string)
{
    if (!co_string_src || !co_string_dst){
        return CO_BAD_ARG_ERR;
    }
    if (co_string_src->length + 1 > co_string_dst->alloc_length){
        char * new_str = (char *) malloc(co_string_src->length + 1);
        if (new_str == NULL){
            return CO_MEM_ALLOC_ERR;
        }
        free(co_string_dst->c_str);
        co_string_dst->c_str = new_str;
        co_string_dst->alloc_length = co_string_src->length + 1;
    }
    memcpy(co_string_dst->c_str, co_string_src->c_str, co_string_src->length + 1);
    co_string_dst->length = co_string_src->length;
    return CO_OK;
}

CO_RESET(co_string)
{
    if (co_string_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    char * new_str = (char *) malloc(1);
    if (new_str == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    free(co_string_obj->c_str);
    co_string_obj->c_str = new_str;
    co_string_obj->length = 0;
    co_string_obj->alloc_length = 1;
    co_string_obj->c_str[0] = '\0';
    return CO_OK;
}

CO_COMPARE(co_string)
{
    if (co_string_a == NULL || co_string_b == NULL){
        return CO_CMP_ERR;
    }
    return strcmp(co_string_a->c_str, co_string_b->c_str);
}

co_status co_string_set(co_string *co_string_obj, const char *str)
{
    if (co_string_obj == NULL || str == NULL){
        return CO_BAD_ARG_ERR;
    }
    size_t length = strlen(str);
    if (co_string_obj->alloc_length < length + 1){
        char * new_str = (char *) malloc(length + 1);
        if (new_str == NULL){
            return CO_MEM_ALLOC_ERR;
        }
        free(co_string_obj->c_str);
        co_string_obj->c_str = new_str;
        co_string_obj->alloc_length = length + 1;
    }
    co_string_obj->length = length;
    memcpy(co_string_obj->c_str, str, length + 1);
    return CO_OK;
}

co_string *co_string_create_from_c_str(const char *str)
{
    if (str == NULL){
        return NULL;
    }
    co_string * co_string_obj = (co_string *) malloc(sizeof(co_string));
    if (co_string_obj){
        size_t length = strlen(str);
        co_string_obj->c_str = (char *) malloc(length + 1);
        if (co_string_obj->c_str == NULL){
            free(co_string_obj);
            return NULL;
        }
        co_string_obj->length = length;
        co_string_obj->alloc_length = length + 1;
        memcpy(co_string_obj->c_str, str, co_string_obj->alloc_length);
    }
    return co_string_obj;
}

co_status co_string_add(co_string *co_string_obj, const char *str)
{
    if (co_string_obj == NULL || str == NULL){
        return CO_BAD_ARG_ERR;
    }
    size_t str_length = strlen(str);
    size_t new_length = co_string_obj->length + str_length;
    if (new_length + 1 > co_string_obj->alloc_length){
        co_status status = _co_string_realloc(co_string_obj, new_length + 1);
        if (status != CO_OK){
            return status;
        }
    }
    memcpy(co_string_obj->c_str + co_string_obj->length, str, str_length + 1);
    co_string_obj->length = new_length;
    return CO_OK;
}

co_status co_string_append(co_string *co_string_obj, const co_string *co_string_src)
{
    if (co_string_obj == NULL || co_string_src == NULL){
        return CO_BAD_ARG_ERR;
    }
    size_t new_length = co_string_obj->length + co_string_src->length;
    if (new_length + 1 > co_string_obj->alloc_length){
        co_status status = _co_string_realloc(co_string_obj, new_length + 1);
        if (status != CO_OK){
            return status;
        }
    }
    memcpy(co_string_obj->c_str + co_string_obj->length, co_string_src->c_str, co_string_src->length + 1);
    co_string_obj->length = new_length;
    return CO_OK;
}

co_status co_string_crop(co_string *co_string_obj, size_t begin, size_t end)
{
    if (co_string_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    if (begin + end > co_string_obj->length){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
    size_t old_length = co_string_obj->length; //для отката в случае ошибки
    co_string_obj->length -= end;
    co_string_obj->c_str[co_string_obj->length] = '\0';
    if (begin > 0){
        size_t new_length = co_string_obj->length - begin; //end - уже обрезан
        char * new_str = (char *) malloc(new_length + 1);
        if (new_str == NULL){
            co_string_obj->length = old_length;
            return CO_MEM_ALLOC_ERR;
        }
        memcpy(new_str, co_string_obj->c_str + begin, new_length + 1);
        free(co_string_obj->c_str);
        co_string_obj->c_str = new_str;
        co_string_obj->alloc_length = new_length + 1;
        co_string_obj->length = new_length;
    }
    return CO_OK;
}

co_status co_string_sprintf(co_string *co_string_obj, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int length = vsnprintf(NULL, 0, format, args);
    if (length < 0){
        va_end(args);
        return CO_BAD_ARG_ERR;
    }
    va_end(args);
    char * new_str = (char *) malloc((size_t)(length + 1));
    if (new_str == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    va_start(args, format);
    vsnprintf(new_str, (size_t)(length + 1), format, args);
    va_end(args);
    free(co_string_obj->c_str);
    co_string_obj->c_str = new_str;
    co_string_obj->length = (size_t) length;
    co_string_obj->alloc_length = (size_t) length + 1;
    return CO_OK;
}

