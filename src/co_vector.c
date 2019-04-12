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

#include "co_vector.h"
#include <stdlib.h>
#include <string.h>


#define CO_VECTOR_PRE_ALLOC 5
#define CO_VECTOR_POST_ALLOC 5

CO_CREATE(co_vector)
{
    co_vector *co_vector_obj = (co_vector *) malloc(sizeof(co_vector));
    if (co_vector_obj){
        co_vector_obj->raw_data = NULL;
        co_vector_obj->raw_data_size = 0;
        co_vector_obj->begin_item_shift = 0;
        co_vector_obj->item_size = 0;
        co_vector_obj->length = 0;
        co_vector_obj->free_function = NULL;
        co_vector_obj->clone_function = NULL;
        co_vector_obj->compare_function = NULL;
    }
    return co_vector_obj;
}

CO_FREE(co_vector)
{
    if (co_vector_obj){
        if (co_vector_obj->raw_data) free(co_vector_obj->raw_data);
        free(co_vector_obj);
    }
}

CO_CLONE(co_vector)
{
    if (co_vector_src == NULL){
        return NULL;
    }
    co_vector *co_vector_obj = (co_vector *) malloc(sizeof(co_vector));
    if (co_vector_obj){
        if (co_vector_src->raw_data_size != 0){
            co_vector_obj->raw_data = malloc(co_vector_src->raw_data_size);
            if (co_vector_obj->raw_data == NULL){
                free(co_vector_obj);
                return NULL;
            }
            memcpy(co_vector_obj->raw_data, co_vector_src->raw_data, co_vector_src->raw_data_size);
            co_vector_obj->raw_data_size = co_vector_src->raw_data_size;
        }
        else {
            co_vector_obj->raw_data = 0;
            co_vector_obj->raw_data_size = 0;
        }
        co_vector_obj->begin_item_shift = co_vector_src->begin_item_shift;
        co_vector_obj->item_size = co_vector_src->item_size;
        co_vector_obj->length = co_vector_src->length;
        co_vector_obj->free_function = co_vector_src->free_function;
        co_vector_obj->clone_function = co_vector_src->clone_function;
        co_vector_obj->compare_function = co_vector_src->compare_function;
    }
    return co_vector_obj;
}

CO_COPY(co_vector)
{
    if (co_vector_src == NULL || co_vector_dst == NULL){
        return CO_BAD_ARG_ERR;
    }
    void *new_raw_data = NULL;
    if (co_vector_src->raw_data_size != 0){
        new_raw_data = malloc(co_vector_src->raw_data_size);
        if (new_raw_data == NULL){
            return CO_MEM_ALLOC_ERR;
        }
        memcpy(new_raw_data, co_vector_src->raw_data, co_vector_src->raw_data_size);
    }
    if (co_vector_dst->raw_data) free(co_vector_dst->raw_data);
    co_vector_dst->raw_data = new_raw_data;
    co_vector_dst->raw_data_size = co_vector_src->raw_data_size;
    co_vector_dst->begin_item_shift = co_vector_src->begin_item_shift;
    co_vector_dst->item_size = co_vector_src->item_size;
    co_vector_dst->length = co_vector_src->length;
    co_vector_dst->free_function = co_vector_src->free_function;
    co_vector_dst->clone_function = co_vector_src->clone_function;
    co_vector_dst->compare_function = co_vector_src->compare_function;
    return CO_OK;
}

CO_RESET(co_vector)
{
    if (co_vector_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    if (co_vector_obj->raw_data) free(co_vector_obj->raw_data);
    co_vector_obj->raw_data = NULL;
    co_vector_obj->raw_data_size = 0;
    co_vector_obj->begin_item_shift = 0;
    co_vector_obj->item_size = 0;
    co_vector_obj->length = 0;
    co_vector_obj->free_function = NULL;
    co_vector_obj->clone_function = NULL;
    co_vector_obj->compare_function = NULL;
    return CO_OK;
}

CO_CLEAR(co_vector)
{
    if (co_vector_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    if (co_vector_obj->raw_data) free(co_vector_obj->raw_data);
    co_vector_obj->raw_data = NULL;
    co_vector_obj->raw_data_size = 0;
    co_vector_obj->begin_item_shift = 0;
    co_vector_obj->length = 0;
    return CO_OK;
}

CO_SIZE(co_vector)
{
    if (co_vector_obj == NULL){
        return 0;
    }
    return co_vector_obj->length;
}


static co_status _co_vector_add_first(co_vector *co_vector_obj, void *data) //data удаляется в этой функции
{
    size_t raw_data_size = co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC + CO_VECTOR_POST_ALLOC + 1);
    co_vector_obj->raw_data = malloc(raw_data_size);
    if (co_vector_obj->raw_data == NULL){
        co_vector_obj->free_function(data);
        return CO_MEM_ALLOC_ERR;
    }
    co_vector_obj->length = 1;
    co_vector_obj->raw_data_size = raw_data_size;
    co_vector_obj->begin_item_shift = CO_VECTOR_PRE_ALLOC;
    memcpy((char *) co_vector_obj->raw_data + (co_vector_obj->item_size * CO_VECTOR_PRE_ALLOC), data, co_vector_obj->item_size);
    free(data);
    return CO_OK;
}

CO_PUSH_BACK(co_vector)
{
    if (co_vector_obj == NULL || object == NULL || co_vector_obj->item_size == 0){
        return CO_BAD_ARG_ERR;
    }
    void *data = co_vector_obj->clone_function(object); //Оптимизировать!!!
    if (data == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    if (co_vector_obj->raw_data == NULL){
        return _co_vector_add_first(co_vector_obj, data);
    }
    else {
        size_t byte_shift = (co_vector_obj->begin_item_shift + co_vector_obj->length) * co_vector_obj->item_size;
        if (byte_shift + co_vector_obj->item_size > co_vector_obj->raw_data_size){
            //необходима реаллокация
            size_t raw_data_size = co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC + CO_VECTOR_POST_ALLOC + co_vector_obj->length + 1);
            void *new_raw_data = malloc(raw_data_size);
            if (new_raw_data == NULL){
                co_vector_obj->free_function(data);
                return CO_MEM_ALLOC_ERR;
            }
            memcpy((char *) new_raw_data + (co_vector_obj->item_size * CO_VECTOR_PRE_ALLOC),
                   (char *) co_vector_obj->raw_data + (co_vector_obj->item_size * co_vector_obj->begin_item_shift),
                   co_vector_obj->item_size * co_vector_obj->length);
            memcpy((char *) new_raw_data + (co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC + co_vector_obj->length)), data, co_vector_obj->item_size);
            free(data);
            free(co_vector_obj->raw_data);
            co_vector_obj->raw_data = new_raw_data;
            co_vector_obj->raw_data_size = raw_data_size;
            co_vector_obj->length++;
            co_vector_obj->begin_item_shift = CO_VECTOR_PRE_ALLOC;
            return CO_OK;
        }
        else{
            //нет нужды в реаллокации
            co_vector_obj->length++;
            memcpy((char *) co_vector_obj->raw_data + byte_shift, data, co_vector_obj->item_size);
            free(data);
            return CO_OK;
        }
    }
}

CO_POP_BACK(co_vector)
{
    if (co_vector_obj == NULL || co_vector_obj->item_size == 0){
        return CO_BAD_ARG_ERR;
    }
    if (co_vector_obj->length == 0){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
    co_vector_obj->length--;
    //Занятую ранее память не очищаем, она еще может пригодиться.
    return CO_OK;
}

CO_BACK(co_vector)
{
    if (co_vector_obj == NULL){
        return NULL;
    }
    if (co_vector_obj->length == 0){
        return NULL;
    }
    return (const char *) co_vector_obj->raw_data + (co_vector_obj->item_size * (co_vector_obj->begin_item_shift + co_vector_obj->length - 1));
}

CO_PUSH_FRONT(co_vector)
{
    if (co_vector_obj == NULL || object == NULL || co_vector_obj->item_size == 0){
        return CO_BAD_ARG_ERR;
    }
    void *data = co_vector_obj->clone_function(object);
    if (data == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    if (co_vector_obj->raw_data == NULL){
        return _co_vector_add_first(co_vector_obj, data);
    }
    else {
        if (co_vector_obj->begin_item_shift == 0){
            //необходима реаллокация
            size_t raw_data_size = co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC + CO_VECTOR_POST_ALLOC + co_vector_obj->length + 1);
            void *new_raw_data = malloc(raw_data_size);
            if (new_raw_data == NULL){
                co_vector_obj->free_function(data);
                return CO_MEM_ALLOC_ERR;
            }
            memcpy((char *) new_raw_data + (co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC + 1)),
                   (char *) co_vector_obj->raw_data + (co_vector_obj->item_size * co_vector_obj->begin_item_shift),
                   co_vector_obj->item_size * co_vector_obj->length);
            memcpy((char *) new_raw_data + (co_vector_obj->item_size * (CO_VECTOR_PRE_ALLOC)), data, co_vector_obj->item_size);
            free(data);
            free(co_vector_obj->raw_data);
            co_vector_obj->raw_data = new_raw_data;
            co_vector_obj->raw_data_size = raw_data_size;
            co_vector_obj->length++;
            co_vector_obj->begin_item_shift = CO_VECTOR_PRE_ALLOC;
            return CO_OK;
        }
        else{
            //нет нужды в реаллокации
            co_vector_obj->length++;
            co_vector_obj->begin_item_shift--;
            memcpy((char *) co_vector_obj->raw_data + ((co_vector_obj->begin_item_shift) * co_vector_obj->item_size), data, co_vector_obj->item_size);
            free(data);
            return CO_OK;
        }
    }
}

CO_POP_FRONT(co_vector)
{
    if (co_vector_obj == NULL || co_vector_obj->item_size == 0){
        return CO_BAD_ARG_ERR;
    }
    if (co_vector_obj->length == 0){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
    co_vector_obj->begin_item_shift++;
    co_vector_obj->length--;
    //Занятую ранее память не очищаем, она еще может пригодиться.
    return CO_OK;
}

CO_FRONT(co_vector)
{
    if (co_vector_obj == NULL){
        return NULL;
    }
    if (co_vector_obj->length == 0){
        return NULL;
    }
    return (const char *) co_vector_obj->raw_data + (co_vector_obj->item_size * (co_vector_obj->begin_item_shift));
}
