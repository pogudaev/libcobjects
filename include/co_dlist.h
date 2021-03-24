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

#ifndef CO_DLIST_H
#define CO_DLIST_H

#include <stddef.h>
#include <stdbool.h>

#include "co_macros.h"
#include "co_status_code.h"
#include "co_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

struct co_dlist_node;
typedef struct co_dlist_node co_dlist_node; //элемент связного списка
typedef co_dlist_node * co_dlist_iterator; //указатель на элемент связного списка
struct co_dlist_node{
    void *data; //указатель на данные
    co_dlist_iterator next; //следущий элемент списка
    co_dlist_iterator prev; //предыдущий элемент списка
};

CO_OBJECT_DEFINITION(co_dlist);
CO_CONTAINER_DEFINITION(co_dlist);
CO_LINEAR_CONTAINER_DEFINITION(co_dlist);

struct co_dlist {
    co_dlist_iterator head; //начало списка
    co_dlist_iterator tail; //конец списка
    co_free_function free_function; //Функция для удаления элемента
    co_clone_function clone_function; //Функция для клонирования элемента
    co_compare_function compare_function; //Функция для сравнения элементов
};

#define co_dlist_set_type(dlist, object) \
    dlist->free_function = (co_free_function) object##_free; \
    dlist->clone_function = (co_clone_function) object##_clone; \
    dlist->compare_function = (co_compare_function) object##_compare

#define co_dlist_foreach(dlist, iterator) \
    for (co_dlist_iterator iterator = dlist->head; iterator; iterator = iterator->next)

co_status co_dlist_concat_move_back(co_dlist *co_dlist_dst, co_dlist *co_dlist_src);
co_status co_dlist_concat_move_front(co_dlist *co_dlist_dst, co_dlist *co_dlist_src);
bool co_dlist_check_by_value(const co_dlist *co_dlist_obj, const void *object);
bool co_dlist_check_by_cond(const co_dlist *co_dlist_obj, const void *data, co_compare_function compare_function);
co_status co_dlist_remove_by_value(co_dlist *co_dlist_obj, const void *object);
co_status co_dlist_remove_by_cond(co_dlist *co_dlist_obj, const void *data, co_compare_function compare_function);

typedef struct co_string co_string; //чтобы не создать циклическую зависимость #include
co_dlist *co_string_split_to_dlist(const co_string *co_string_obj, const char *divider);

#ifdef __cplusplus
}
#endif

#endif // CO_DLIST_H
