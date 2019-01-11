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

#include "co_map.h"
#include <stdlib.h>
#include <string.h>

static co_status _co_map_tail_clear(co_free_function free_function, co_map_iterator node)
{
    co_map_iterator iter = node;
    while(iter){
        free_function(iter->data);
        free(iter->key);
        co_map_iterator old_iter = iter;
        iter = iter->next;
        free(old_iter);
    }
    return CO_OK;
}

static co_status _co_map_clear(co_map *co_map_obj)
{
    _co_map_tail_clear(co_map_obj->free_function, co_map_obj->head);
    co_map_obj->head = NULL;
    return CO_OK;
}

static co_map_node * _co_map_node_create_from_key_and_object(co_clone_function clone_function, const char *key, const void *object)
{
    co_map_node *co_map_node_obj = (co_map_node *) malloc(sizeof(co_map_node));
    if (co_map_node_obj == NULL){
        return NULL;
    }
    co_map_node_obj->next = NULL;

    co_map_node_obj->key = strdup(key);
    if (co_map_node_obj->key == NULL){
        free(co_map_node_obj);
        return NULL;
    }
    co_map_node_obj->data = clone_function(object);
    if (co_map_node_obj->data == NULL){
        free(co_map_node_obj->key);
        free(co_map_node_obj);
        return NULL;
    }
    return co_map_node_obj;
}

CO_CREATE(co_map)
{
    co_map *co_map_obj = (co_map *) malloc(sizeof (co_map));
    if (co_map_obj){
        co_map_obj->head = NULL;
        co_map_obj->free_function = NULL;
        co_map_obj->clone_function = NULL;
    }
    return co_map_obj;
}

CO_FREE(co_map)
{
    if (co_map_obj){
        _co_map_clear(co_map_obj);
        free(co_map_obj);
    }
}


CO_CLONE(co_map)
{
    if (co_map_src == NULL){
        return NULL;
    }
    co_map *co_map_obj = (co_map *) malloc(sizeof(co_map));
    if (co_map_obj){
        co_map_obj->head = NULL;
        co_map_obj->free_function = co_map_src->free_function;
        co_map_obj->clone_function = co_map_src->clone_function;
        co_map_iterator iter_src = co_map_src->head;
        co_map_iterator *iter = &co_map_obj->head;
        co_map_iterator prev_iter = NULL; //предыдущий элемент списка
        while(iter_src){
            (*iter) = _co_map_node_create_from_key_and_object(co_map_obj->clone_function, iter_src->key, iter_src->data);
            if (*iter == NULL){
                _co_map_clear(co_map_obj);
                free(co_map_obj);
                return NULL;
            }
            if (prev_iter) prev_iter->next = (*iter);
            prev_iter = (*iter);
            iter_src = iter_src->next;
        }
    }
    return co_map_obj;
}

CO_COPY(co_map)
{
    if (co_map_src == NULL || co_map_dst == NULL){
        return CO_BAD_ARG_ERR;
    }

    co_map_iterator new_head = NULL;
    co_map_iterator iter_src = co_map_src->head;
    co_map_iterator *iter = &new_head;
    co_map_iterator prev_iter = NULL; //предыдущий элемент списка
    while(iter_src){
        (*iter) = _co_map_node_create_from_key_and_object(co_map_src->clone_function, iter_src->key, iter_src->data);
        if (*iter == NULL){
            _co_map_tail_clear(co_map_src->free_function, new_head);
            return CO_MEM_ALLOC_ERR;
        }
        if (prev_iter) prev_iter->next = (*iter);
        prev_iter = (*iter);
        iter_src = iter_src->next;
    }

    _co_map_clear(co_map_dst);
    co_map_dst->head = new_head;
    co_map_dst->free_function = co_map_src->free_function;
    co_map_dst->clone_function = co_map_src->clone_function;
    return CO_OK;
}

CO_RESET(co_map)
{
    if (co_map_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_status status = _co_map_clear(co_map_obj);
    co_map_obj->free_function = NULL;
    co_map_obj->clone_function = NULL;
    return status;
}

CO_CLEAR(co_map)
{
    if (co_map_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    return _co_map_clear(co_map_obj);
}

CO_SIZE(co_map)
{
    co_map_iterator iter = co_map_obj->head;
    size_t counter = 0;
    while (iter) {
        counter++;
        iter = iter->next;
    }
    return counter;
}

co_status co_map_set(co_map *co_map_obj, const char *key, const void *object)
{
    if (co_map_obj == NULL || key == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }

    co_map_iterator *iter = &co_map_obj->head;

    while(*iter){
        int cmp = strcmp((*iter)->key, key);
        if (cmp < 0){ //переходим к следующей ноде
            iter = &(*iter)->next;
        } else if (cmp == 0){ //такой ключ уже есть, заменяем данные
            void *new_data = co_map_obj->clone_function(object);
            if (new_data == NULL){
                return CO_MEM_ALLOC_ERR;
            }
            co_map_obj->free_function((*iter)->data);
            (*iter)->data = new_data;
            return CO_OK;
        } else { //if cmp > 0
            co_map_node *node =_co_map_node_create_from_key_and_object(co_map_obj->clone_function, key, object);
            if (node == NULL){
                return CO_MEM_ALLOC_ERR;
            }
            node->next = *iter;
            *iter = node;
            return CO_OK;
        }
    }

    *iter = _co_map_node_create_from_key_and_object(co_map_obj->clone_function, key, object);
    if (*iter == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    return CO_OK;
}

const void *co_map_get(co_map *co_map_obj, const char *key)
{
    if (co_map_obj == NULL || key == NULL){
        return NULL;
    }

    co_map_iterator iter = co_map_obj->head;
    while(iter){
        int cmp = strcmp(iter->key, key);
        if (cmp < 0){
            iter = iter->next;
            continue;
        }
        else if (cmp == 0){
            return iter->data;
        }
        else { //cmp > 0
            return NULL;
        }
    }
    return NULL;
}