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

#include "co_list.h"
#include <stdlib.h>

#include "co_string.h"
#include <string.h>

static co_status _co_list_tail_clear(co_free_function free_function, co_list_iterator node)
{
    co_list_iterator iter = node;
    while(iter){
        free_function(iter->data);
        co_list_iterator old_iter = iter;
        iter = iter->next;
        free(old_iter);
    }
    return CO_OK;
}

static co_status _co_list_clear(co_list *co_list_obj)
{
    _co_list_tail_clear(co_list_obj->free_function, co_list_obj->head);
    co_list_obj->head = NULL;
    return CO_OK;
}

static co_list_node * _co_list_node_create_from_object(co_clone_function clone_function, const void *object)
{
    co_list_node *co_list_node_obj = (co_list_node *) malloc(sizeof(co_list_node));
    if (co_list_node_obj == NULL){
        return NULL;
    }
    co_list_node_obj->next = NULL;
    co_list_node_obj->data = clone_function(object);
    if (co_list_node_obj->data == NULL){
        free(co_list_node_obj);
        return NULL;
    }
    return co_list_node_obj;
}

static co_status _co_list_push_back(co_list *co_list_obj, const void *object)
{
    co_list_iterator *iter = &co_list_obj->head;
    while(*iter){
        iter = &((*iter)->next);
    }
    (*iter) = _co_list_node_create_from_object(co_list_obj->clone_function, object);
    if ((*iter) == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    return CO_OK;
}

static co_status _co_list_push_front(co_list *co_list_obj, const void *object)
{
    co_list_node *new_node = _co_list_node_create_from_object(co_list_obj->clone_function, object);
    if (new_node == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    new_node->next = co_list_obj->head;
    co_list_obj->head = new_node;
    return CO_OK;
}

CO_CREATE(co_list)
{
    co_list *co_list_obj = (co_list *) malloc(sizeof(co_list));
    if (co_list_obj){
        co_list_obj->head = NULL;
        co_list_obj->free_function = NULL;
        co_list_obj->clone_function = NULL;
    }
    return co_list_obj;
}

CO_FREE(co_list)
{
    if (co_list_obj){
        _co_list_clear(co_list_obj);
        free(co_list_obj);
    }
}

CO_CLONE(co_list)
{
    if (co_list_src == NULL){
        return NULL;
    }
    co_list *co_list_obj = (co_list *) malloc(sizeof(co_list));
    if (co_list_obj){
        co_list_obj->head = NULL;
        co_list_obj->free_function = co_list_src->free_function;
        co_list_obj->clone_function = co_list_src->clone_function;
        co_list_iterator iter_src = co_list_src->head;
        co_list_iterator *iter = &co_list_obj->head;
        co_list_iterator prev_iter = NULL; //предыдущий элемент списка
        while(iter_src){
            (*iter) = _co_list_node_create_from_object(co_list_obj->clone_function, iter_src->data);
            if (*iter == NULL){
                _co_list_clear(co_list_obj);
                free(co_list_obj);
                return NULL;
            }
            if (prev_iter) prev_iter->next = (*iter);
            prev_iter = (*iter);
            iter = &prev_iter->next;
            iter_src = iter_src->next;
        }
    }
    return co_list_obj;
}

CO_COPY(co_list)
{
    if (co_list_src == NULL || co_list_dst == NULL){
        return CO_BAD_ARG_ERR;
    }

    co_list_iterator new_head = NULL;
    co_list_iterator iter_src = co_list_src->head;
    co_list_iterator *iter = &new_head;
    co_list_iterator prev_iter = NULL; //предыдущий элемент списка
    while(iter_src){
        (*iter) = _co_list_node_create_from_object(co_list_src->clone_function, iter_src->data);
        if (*iter == NULL){
            _co_list_tail_clear(co_list_src->free_function, new_head);
            return CO_MEM_ALLOC_ERR;
        }
        if (prev_iter) prev_iter->next = (*iter);
        prev_iter = (*iter);
        iter = &prev_iter->next;
        iter_src = iter_src->next;
    }

    _co_list_clear(co_list_dst);
    co_list_dst->head = new_head;
    co_list_dst->free_function = co_list_src->free_function;
    co_list_dst->clone_function = co_list_src->clone_function;
    return CO_OK;
}

CO_RESET(co_list)
{
    if (co_list_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_status status = _co_list_clear(co_list_obj);
    co_list_obj->free_function = NULL;
    co_list_obj->clone_function = NULL;
    return status;
}

CO_COMPARE(co_list)
{
    if (co_list_a == NULL || co_list_b == NULL){
        return CO_CMP_ERR;
    }
    if (co_list_a->compare_function != co_list_b->compare_function){
        return CO_CMP_ERR;
    }
    co_list_iterator iter_a = co_list_a->head;
    co_list_iterator iter_b = co_list_b->head;
    while(iter_a != NULL && iter_b != NULL){
        int compare_result = co_list_a->compare_function(iter_a->data, iter_b->data);
        if (compare_result == CO_CMP_ERR){
            return CO_CMP_ERR;
        }
        else if (compare_result != CO_CMP_EQ){
            return CO_CMP_NE;
        }
        iter_a = iter_a->next;
        iter_b = iter_b->next;
    }
    if (iter_a != NULL || iter_b != NULL){
        return CO_CMP_NE;
    }
    return CO_CMP_EQ;
}

CO_CLEAR(co_list)
{
    if (co_list_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    return _co_list_clear(co_list_obj);
}

CO_PUSH_BACK(co_list)
{
    if (co_list_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
    return _co_list_push_back(co_list_obj, object);
}

CO_BACK(co_list)
{
    if (co_list_obj == NULL || co_list_obj->head == NULL){
        return NULL;
    }
    co_list_iterator iter = co_list_obj->head;
    while(iter->next){
        iter = iter->next;
    }
    return iter->data;
}

CO_PUSH_FRONT(co_list)
{
    if (co_list_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
    return _co_list_push_front(co_list_obj, object);
}

CO_FRONT(co_list)
{
    if (co_list_obj == NULL || co_list_obj->head == NULL){
        return NULL;
    }
    return co_list_obj->head->data;
}

CO_SIZE(co_list)
{
    co_list_iterator iter = co_list_obj->head;
    size_t counter = 0;
    while (iter) {
        counter++;
        iter = iter->next;
    }
    return counter;
}

CO_POP_BACK(co_list)
{
    if (co_list_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_list_iterator iter = co_list_obj->head;
    if (iter == NULL){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
    co_list_iterator prev = NULL;
    while(iter->next){
        prev = iter;
        iter = iter->next;
    }
    if (prev) prev->next = NULL;
    co_list_obj->free_function(iter->data);
    free(iter);
    return CO_OK;
}

CO_POP_FRONT(co_list)
{
    if (co_list_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_list_iterator iter = co_list_obj->head;
    if (iter == NULL){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
    co_list_iterator tail = co_list_obj->head->next;
    co_list_obj->free_function(co_list_obj->head->data);
    free(co_list_obj->head);
    co_list_obj->head = tail;
    return CO_OK;
}

CO_MOVE_BACK(co_list)
{
    if (co_list_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_list_iterator *iter = &co_list_obj->head;
    while(*iter){
        iter = &((*iter)->next);
    }
    (*iter) = (co_list_node *) malloc(sizeof(co_list_node));
    if ((*iter) == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    (*iter)->data = object;
    (*iter)->next = NULL;
    return CO_OK;
}

CO_MOVE_FRONT(co_list)
{
    if (co_list_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_list_node *new_node = (co_list_node *) malloc(sizeof(co_list_node));
    if (new_node == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    new_node->data = object;
    new_node->next = co_list_obj->head;
    co_list_obj->head = new_node;
    return CO_OK;
}

co_list *co_string_split_to_list(const co_string *co_string_obj, const char *divider)
{
    if (!divider || !*divider || !co_string_obj){
        return NULL;
    }
    char * new_str = (char *) malloc(co_string_obj->length + 1);
    if (new_str == NULL){
        return NULL;
    }
    co_list *co_list_obj = co_list_create();
    if (co_list_obj == NULL){
        return NULL;
    }
    co_list_set_type(co_list_obj, co_string);
    memcpy(new_str, co_string_obj->c_str, co_string_obj->length + 1);

    char *end_new_str = new_str + strlen(new_str);

    char * begin_ptr = new_str;
    char * end_ptr = new_str;

    while(end_ptr != end_new_str){
        end_ptr = strstr(begin_ptr, divider);
        if (end_ptr){
            *end_ptr = '\0';
        }
        else{
            end_ptr = begin_ptr;
            while(*end_ptr) end_ptr++;
        }

        co_string *co_string_elem = co_string_create_from_c_str(begin_ptr);
        if (co_string_elem == NULL){
            free(new_str);
            co_list_free(co_list_obj);
            return NULL;
        }
        co_list_move_back(co_list_obj, co_string_elem);
        begin_ptr = end_ptr + strlen(divider);
    }
    free(new_str);
    return co_list_obj;
}

co_status co_list_remove_by_value(co_list *co_list_obj, const void *object)
{
    return co_list_remove_by_cond(co_list_obj, object, co_list_obj->compare_function);
}

co_status co_list_remove_by_cond(co_list *co_list_obj, const void *data, co_compare_function compare_function)
{
    if (co_list_obj == NULL || data == NULL || compare_function == NULL){
        return CO_BAD_ARG_ERR;
    }
    co_list_iterator *iter = &(co_list_obj->head);
    while(*iter){
        if (compare_function((*iter)->data, data) == CO_CMP_EQ){
            co_list_iterator rem = *iter;
            *iter = (*iter)->next;
            co_list_obj->free_function(rem->data);
            free(rem);
            continue;
        }
        iter = &((*iter)->next);
    }
    return CO_OK;
}
