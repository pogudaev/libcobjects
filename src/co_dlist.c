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

#include "co_dlist.h"
#include <stdlib.h>

#include "co_string.h"
#include <string.h>

static co_status _co_dlist_tail_clear(co_free_function free_function, co_dlist_iterator node)
{
	co_dlist_iterator iter = node;
    while(iter){
        free_function(iter->data);
		co_dlist_iterator old_iter = iter;
        iter = iter->next;
        free(old_iter);
    }
    return CO_OK;
}

static co_status _co_dlist_clear(co_dlist *co_dlist_obj)
{
	_co_dlist_tail_clear(co_dlist_obj->free_function, co_dlist_obj->head);
	co_dlist_obj->head = NULL;
	co_dlist_obj->tail = NULL;
    return CO_OK;
}

static co_dlist_node * _co_dlist_node_create_from_object(co_clone_function clone_function, const void *object)
{
	co_dlist_node *co_dlist_node_obj = (co_dlist_node *) malloc(sizeof(co_dlist_node));
	if (co_dlist_node_obj == NULL){
        return NULL;
    }
	co_dlist_node_obj->next = NULL;
	co_dlist_node_obj->prev = NULL;
	co_dlist_node_obj->data = clone_function(object);
	if (co_dlist_node_obj->data == NULL) {
		free(co_dlist_node_obj);
        return NULL;
    }
	return co_dlist_node_obj;
}

static co_status _co_dlist_push_back(co_dlist *co_dlist_obj, const void *object)
{
	co_dlist_node * new_node= _co_dlist_node_create_from_object(co_dlist_obj->clone_function, object);
	if (new_node == NULL){
		return CO_MEM_ALLOC_ERR;
	}
	co_dlist_iterator last_iter = co_dlist_obj->tail;
	if (last_iter) {
		last_iter->next = new_node;
		new_node->prev = last_iter;
	}
	else {
		co_dlist_obj->head = new_node;
	}
	co_dlist_obj->tail = new_node;

    return CO_OK;
}

static co_status _co_dlist_push_front(co_dlist *co_dlist_obj, const void *object)
{
	co_dlist_node *new_node = _co_dlist_node_create_from_object(co_dlist_obj->clone_function, object);
    if (new_node == NULL){
        return CO_MEM_ALLOC_ERR;
    }
	co_dlist_iterator first_iter = co_dlist_obj->head;
	if (first_iter){
		first_iter->prev = new_node;
		new_node->next = first_iter;
	} else {
		co_dlist_obj->tail = new_node;
	}
	co_dlist_obj->head = new_node;
    return CO_OK;
}

CO_CREATE(co_dlist)
{
	co_dlist *co_dlist_obj = (co_dlist *) malloc(sizeof(co_dlist));
	if (co_dlist_obj){
		co_dlist_obj->head = NULL;
		co_dlist_obj->tail = NULL;
		co_dlist_obj->free_function = NULL;
		co_dlist_obj->clone_function = NULL;
		co_dlist_obj->compare_function = NULL;
    }
	return co_dlist_obj;
}

CO_FREE(co_dlist)
{
	if (co_dlist_obj){
		_co_dlist_clear(co_dlist_obj);
		free(co_dlist_obj);
    }
}

CO_CLONE(co_dlist)
{
	if (co_dlist_src == NULL){
        return NULL;
    }
	co_dlist *co_dlist_obj = (co_dlist *) malloc(sizeof(co_dlist));
	if (co_dlist_obj){
		co_dlist_obj->head = NULL;
		co_dlist_obj->free_function = co_dlist_src->free_function;
		co_dlist_obj->clone_function = co_dlist_src->clone_function;
		co_dlist_obj->compare_function = co_dlist_src->compare_function;
		co_dlist_iterator iter_src = co_dlist_src->head;
		while(iter_src) {
			if (_co_dlist_push_back(co_dlist_obj, iter_src->data) != CO_OK){
				co_dlist_free(co_dlist_obj);
				return NULL;
			}
            iter_src = iter_src->next;
        }
    }
	return co_dlist_obj;
}

CO_COPY(co_dlist)
{
	if (co_dlist_src == NULL || co_dlist_dst == NULL){
        return CO_BAD_ARG_ERR;
    }

	co_dlist_iterator new_head = NULL;
	co_dlist_iterator iter_src = co_dlist_src->head;
	co_dlist_iterator *iter = &new_head;
	co_dlist_iterator prev_iter = NULL; //предыдущий элемент списка
    while(iter_src){
		(*iter) = _co_dlist_node_create_from_object(co_dlist_src->clone_function, iter_src->data);
        if (*iter == NULL){
			_co_dlist_tail_clear(co_dlist_src->free_function, new_head);
            return CO_MEM_ALLOC_ERR;
        }
		if (prev_iter) {
			prev_iter->next = (*iter);
		}
        prev_iter = (*iter);
        iter = &prev_iter->next;
        iter_src = iter_src->next;
    }

	_co_dlist_clear(co_dlist_dst);
	co_dlist_dst->head = new_head;
	co_dlist_dst->free_function = co_dlist_src->free_function;
	co_dlist_dst->clone_function = co_dlist_src->clone_function;
	co_dlist_dst->compare_function = co_dlist_src->compare_function;
    return CO_OK;
}

CO_RESET(co_dlist)
{
	if (co_dlist_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_status status = _co_dlist_clear(co_dlist_obj);
	co_dlist_obj->free_function = NULL;
	co_dlist_obj->clone_function = NULL;
	co_dlist_obj->compare_function = NULL;
    return status;
}

CO_COMPARE(co_dlist)
{
	if (co_dlist_a == NULL || co_dlist_b == NULL){
        return CO_CMP_ERR;
    }
	if (co_dlist_a->compare_function != co_dlist_b->compare_function){
        return CO_CMP_ERR;
    }
	co_dlist_iterator iter_a = co_dlist_a->head;
	co_dlist_iterator iter_b = co_dlist_b->head;
    while(iter_a != NULL && iter_b != NULL){
		int compare_result = co_dlist_a->compare_function(iter_a->data, iter_b->data);
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

CO_CLEAR(co_dlist)
{
	if (co_dlist_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
	return _co_dlist_clear(co_dlist_obj);
}

CO_PUSH_BACK(co_dlist)
{
	if (co_dlist_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
	return _co_dlist_push_back(co_dlist_obj, object);
}

CO_BACK(co_dlist)
{
	if (co_dlist_obj == NULL || co_dlist_obj->head == NULL){
		return NULL;
	}
	return co_dlist_obj->tail->data;
}

CO_PUSH_FRONT(co_dlist)
{
	if (co_dlist_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
	return _co_dlist_push_front(co_dlist_obj, object);
}

CO_FRONT(co_dlist)
{
	if (co_dlist_obj == NULL || co_dlist_obj->head == NULL){
        return NULL;
    }
	return co_dlist_obj->head->data;
}

CO_SIZE(co_dlist)
{
	if (co_dlist_obj == NULL){
        return 0;
    }
	co_dlist_iterator iter = co_dlist_obj->head;
    size_t counter = 0;
    while (iter) {
        counter++;
        iter = iter->next;
    }
    return counter;
}

CO_POP_BACK(co_dlist)
{
	if (co_dlist_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_iterator del_iter = co_dlist_obj->tail;
	if (del_iter == NULL){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
	co_dlist_iterator prev_iter = del_iter->prev;

	co_dlist_obj->tail = NULL;
	if (prev_iter) {
		prev_iter->next = NULL;
		co_dlist_obj->tail = prev_iter;
	}
	co_dlist_obj->free_function(del_iter->data);
	free(del_iter);

    return CO_OK;
}

CO_POP_FRONT(co_dlist)
{
	if (co_dlist_obj == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_iterator iter = co_dlist_obj->head;
    if (iter == NULL){
        return CO_IMPOSIBLE_OPERATION_ERR;
    }
	co_dlist_iterator tail = co_dlist_obj->head->next;
	co_dlist_obj->free_function(co_dlist_obj->head->data);
	free(co_dlist_obj->head);
	co_dlist_obj->head = tail;
	tail->prev = NULL;
    return CO_OK;
}

CO_MOVE_BACK(co_dlist)
{
	if (co_dlist_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_node *new_node = (co_dlist_node *) malloc(sizeof(co_dlist_node));
	if (new_node == NULL){
        return CO_MEM_ALLOC_ERR;
    }
	new_node->data = object;
	co_dlist_iterator last_iter = co_dlist_obj->tail;
	if (last_iter) {
		last_iter->next = new_node;
		new_node->prev = last_iter;
	} else {
		co_dlist_obj->head = new_node;
	}
	co_dlist_obj->tail = new_node;

    return CO_OK;
}

CO_MOVE_FRONT(co_dlist)
{
	if (co_dlist_obj == NULL || object == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_node *new_node = (co_dlist_node *) malloc(sizeof(co_dlist_node));
    if (new_node == NULL){
        return CO_MEM_ALLOC_ERR;
    }
    new_node->data = object;
	co_dlist_iterator first_iter = co_dlist_obj->head;
	if (first_iter) {
		new_node->next = first_iter;
		first_iter->prev = new_node;
	} else {
		co_dlist_obj->tail = new_node;
	}
	co_dlist_obj->head = new_node;
    return CO_OK;
}

co_dlist *co_string_split_to_dlist(const co_string *co_string_obj, const char *divider)
{
    if (!divider || !*divider || !co_string_obj){
        return NULL;
    }
    char * new_str = (char *) malloc(co_string_obj->length + 1);
    if (new_str == NULL){
        return NULL;
    }
	co_dlist *co_dlist_obj = co_dlist_create();
	if (co_dlist_obj == NULL){
        return NULL;
    }
	co_dlist_set_type(co_dlist_obj, co_string);
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
			co_dlist_free(co_dlist_obj);
            return NULL;
        }
		co_dlist_move_back(co_dlist_obj, co_string_elem);
        begin_ptr = end_ptr + strlen(divider);
    }
    free(new_str);
	return co_dlist_obj;
}

co_status co_dlist_remove_by_value(co_dlist *co_dlist_obj, const void *object)
{
	return co_dlist_remove_by_cond(co_dlist_obj, object, co_dlist_obj->compare_function);
}

co_status co_dlist_remove_by_cond(co_dlist *co_dlist_obj, const void *data, co_compare_function compare_function)
{
	if (co_dlist_obj == NULL || data == NULL || compare_function == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_iterator *iter = &(co_dlist_obj->head);
    while(*iter){
        if (compare_function((*iter)->data, data) == CO_CMP_EQ){
			co_dlist_iterator rem = *iter;
            *iter = (*iter)->next;
			(*iter)->prev = rem->prev;
			co_dlist_obj->free_function(rem->data);
            free(rem);
            continue;
        }
        iter = &((*iter)->next);
    }
    return CO_OK;
}

//TODO fix
co_status co_dlist_concat_move_back(co_dlist *co_dlist_dst, co_dlist *co_dlist_src)
{
	if (co_dlist_dst == NULL || co_dlist_src == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_iterator *iter = &(co_dlist_dst->head);
    while(*iter) iter = &((*iter)->next);
	*iter = co_dlist_src->head;
	co_dlist_src->head = NULL;
	co_dlist_free(co_dlist_src);
    return CO_OK;
}

//TODO fix
co_status co_dlist_concat_move_front(co_dlist *co_dlist_dst, co_dlist *co_dlist_src)
{
	if (co_dlist_dst == NULL || co_dlist_src == NULL){
        return CO_BAD_ARG_ERR;
    }
	co_dlist_iterator *iter = &(co_dlist_src->head);
    while(*iter) iter = &((*iter)->next);
	*iter = co_dlist_dst->head;
	co_dlist_dst->head = co_dlist_src->head;
	co_dlist_src->head = NULL;
	co_dlist_free(co_dlist_src);
    return CO_OK;
}

//TODO fix
bool co_dlist_check_by_cond(const co_dlist *co_dlist_obj, const void *data, co_compare_function compare_function)
{
	if (co_dlist_obj == NULL || data == NULL || compare_function == NULL){
        return false;
    }
	co_dlist_iterator iter = co_dlist_obj->head;
    while(iter){
        if (compare_function(iter->data, data) == CO_CMP_EQ){
            return true;
        }
        iter = iter->next;
    }
    return false;
}

//TODO fix
bool co_dlist_check_by_value(const co_dlist *co_dlist_obj, const void *object)
{
	return co_dlist_check_by_cond(co_dlist_obj, object, co_dlist_obj->compare_function);
}

