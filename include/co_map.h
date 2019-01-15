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

#ifndef CO_MAP_H
#define CO_MAP_H

#include "co_macros.h"
#include "co_status_code.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

CO_OBJECT_DEFINITION(co_map);
CO_CONTAINER_DEFINITION(co_map);

struct co_map_node;
typedef struct co_map_node co_map_node;
typedef co_map_node * co_map_iterator;

struct co_map_node{
    char *key;
    void *data;
    co_map_iterator next;
};

struct co_map {
    co_map_iterator head;
    co_free_function free_function; //Функция для удаления элемента
    co_clone_function clone_function; //Функция для клонирования элемента
    co_compare_function compare_function; //Функция для сравнения элементов
};

#define co_map_set_type(map, object) \
    map->free_function = (co_free_function) object##_free; \
    map->clone_function = (co_clone_function) object##_clone; \
    map->compare_function = (co_compare_function) object##_compare

#define co_map_foreach(map, iterator) \
    for (co_map_iterator iterator = map->head; iterator; iterator = iterator->next)

co_status co_map_set(co_map *co_map_obj, const char *key, const void *object);
const void *co_map_get(co_map *co_map_obj, const char *key);
co_status co_map_remove_by_value(co_map *co_map_obj, const void *object);
co_status co_map_remove_by_key(co_map *co_map_obj, const char *key);
co_status co_map_remove_by_cond(co_map *co_map_obj, const void *data, co_compare_function compare_function);

#ifdef __cplusplus
}
#endif

#endif // CO_MAP_H
