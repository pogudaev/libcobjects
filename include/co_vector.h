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

#ifndef CO_VECTOR_H
#define CO_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

#include "co_macros.h"
#include "co_status_code.h"
#include "co_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

CO_OBJECT_DEFINITION(co_vector);
CO_CONTAINER_DEFINITION(co_vector);
CO_LINEAR_CONTAINER_DEFINITION(co_vector);

struct co_vector {
	void *raw_data;
	size_t raw_data_size;
	size_t begin_item_shift;
	size_t item_size;
	size_t length;
	co_free_function free_function; //Функция для удаления элемента
	co_clone_function clone_function; //Функция для клонирования элемента
	co_compare_function compare_function; //Функция для сравнения элементов
};

#define co_vector_set_type(vector, object) \
    vector->free_function = (co_free_function) object##_free; \
    vector->clone_function = (co_clone_function) object##_clone; \
    vector->compare_function = (co_compare_function) object##_compare; \
    vector->item_size = sizeof(object);


#ifdef __cplusplus
}
#endif


#endif // CO_VECTOR_H
