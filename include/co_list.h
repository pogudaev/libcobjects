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

#ifndef CO_LIST_H
#define CO_LIST_H

#include <stddef.h>
#include <stdbool.h>

#include "co_macros.h"
#include "co_status_code.h"
#include "co_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

struct co_list_node;
typedef struct co_list_node co_list_node; //элемент связного списка
typedef co_list_node * co_list_iterator; //указатель на элемент связного списка
struct co_list_node{
    void *data; //указатель на данные
    co_list_iterator next; //следущий элемент списка
};

CO_OBJECT_DEFINITION(co_list);
CO_CONTAINER_DEFINITION(co_list);
CO_LINEAR_CONTAINER_DEFINITION(co_list);

struct co_list{
    co_list_iterator head; //начало списка
    co_free_function free_function; //Функция для удаления элемента
    co_clone_function clone_function; //Функция для клонирования элемента
    co_compare_function compare_function; //Функция для сравнения элементов
};

#define co_list_set_type(list, object) \
    list->free_function = (co_free_function) object##_free; \
    list->clone_function = (co_clone_function) object##_clone; \
    list->compare_function = (co_compare_function) object##_compare


#define co_list_foreach(list, iterator) \
    for (co_list_iterator iterator = list->head; iterator; iterator = iterator->next)

co_status co_list_concat_move_back(co_list *co_list_dst, co_list *co_list_src);
co_status co_list_concat_move_front(co_list *co_list_dst, co_list *co_list_src);
bool co_list_check_by_value(const co_list *co_list_obj, const void *object);
bool co_list_check_by_cond(const co_list *co_list_obj, const void *data, co_compare_function compare_function);
co_status co_list_remove_by_value(co_list *co_list_obj, const void *object);
co_status co_list_remove_by_cond(co_list *co_list_obj, const void *data, co_compare_function compare_function);

typedef struct co_string co_string; //чтобы не создать циклическую зависимость #include
co_list *co_string_split_to_list(const co_string *co_string_obj, const char *divider);

#ifdef __cplusplus
}
#endif

#endif // CO_LIST_H
