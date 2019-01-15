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

#ifndef CO_MACROS_H
#define CO_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#define CO_CMP_LT -1 //меньше
#define CO_CMP_GT 1 //больше
#define CO_CMP_EQ 0 //равно
#define CO_CMP_NE 2 //не равно
#define CO_CMP_ERR 3 //ошибка

//для примитивных типов

#define CO_PRIMITIVE_CLONE_DEF(Primitive) \
    Primitive *Primitive##_clone(const Primitive *value) \
    { \
        if (value == NULL) return NULL; \
        Primitive *ret = (Primitive *) malloc(sizeof(Primitive)); \
        if (ret == NULL) return NULL; \
        memcpy(ret, value, sizeof(Primitive)); \
        return ret; \
    }

#define CO_PRIMITIVE_FREE_DEF(Primitive) \
    void Primitive##_free(Primitive *value){ \
        if (value){ \
            free(value); \
        } \
    }

#define CO_PRIMITIVE_COMPARE_DEF(Primitive) \
    int Primitive##_compare(const Primitive *value_a, const Primitive *value_b){ \
        if (value_a == NULL || value_b == NULL){ \
            return CO_CMP_ERR; \
        } \
        if (*value_a < *value_b) return CO_CMP_LT; \
        else if (*value_b > *value_a) return CO_CMP_GT; \
        return CO_CMP_EQ; \
    }

#define CO_PRIMITIVE_CLONE_DECL(Primitive) Primitive *Primitive##_clone(const Primitive *value)

#define CO_PRIMITIVE_FREE_DECL(Primitive) void Primitive##_free(Primitive *value)

#define CO_PRIMITIVE_COMPARE_DECL(Primitive) int Primitive##_compare(const Primitive *value_a, const Primitive *value_b)

#define CO_PRIMITIVE_DEFINITION(Primitive) \
    CO_PRIMITIVE_CLONE_DEF(Primitive) \
    CO_PRIMITIVE_FREE_DEF(Primitive) \
    CO_PRIMITIVE_COMPARE_DEF(Primitive)

#define CO_PRIMITIVE_DECLARATION(Primitive) \
    CO_PRIMITIVE_CLONE_DECL(Primitive); \
    CO_PRIMITIVE_FREE_DECL(Primitive); \
    CO_PRIMITIVE_COMPARE_DECL(Primitive);

//Для всех объектов

/**
 * @brief Object *Object_create(void) выделяет память в куче и создает в ней инициализированный объект
 * @return возвращает указатель на созданный объект или NULL в случае неудачи
 */
#define CO_CREATE(Object) Object * Object##_create(void)

/**
 * @brief void Object_free(Object *Object_obj) освобождает память занятую объектом
 * @param Object_obj указатель на объект
 */
#define CO_FREE(Object) void Object##_free(Object * Object##_obj)

/**
 * @brief Object *Object_clone(const Object *Object_src) создание копии указанного объекта, аналог конструктора копирования.
 * @param Object_src указатель на объект - источник
 * @return возвращает указатель на созданный объект или NULL в случае неудачи
 */
#define CO_CLONE(Object) Object * Object##_clone(const Object * Object##_src)

/**
 * @brief co_status Object_copy(Object *Object_dst, const Object *Object_src) копирует содержимое объекта Object_src в Object_dst
 * @param Object_dst указатель на объект - приёмник
 * @param Object_src указатель на объект - источник
 * @return CO_OK в случае успеха, или код ошибки
 */
#define CO_COPY(Object) co_status Object##_copy(Object * Object##_dst, const Object * Object##_src)

/**
 * @brief co_status Object_reset(Object *Object_obj) сбрасывает все характеристики объекта к состоянию на момент создания функцией Object_create
 * @param Object_obj указатель на объект
 * @return CO_OK в случае успеха, или код ошибки
 */
#define CO_RESET(Object) co_status Object##_reset(Object * Object##_obj)

/**
 * @brief int Object_compare(const Object * Object_a, const Object * Object_b)
 * @param Object_a указатель на первый сравниваемый объект
 * @param Object_b указатель на второй сравниваемый объект
 * @return если Object_a < Object_b, то CO_CMP_LT.
 *         если Object_a > Object_b, то CO_CMP_GT.
 *         если Object_a == Object_b, то CO_CMP_EQ.
 *         если Object_a != Object_b, но нельзя определить какой больше то CO_CMP_NE.
 *         если при сравнении произошла ошибка или сравнить невозможно то CO_CMP_ERR.
 */
#define CO_COMPARE(Object) int Object##_compare(const Object * Object##_a, const Object * Object##_b)

/**
 * @brief CO_OBJECT_DEFINITION Объявление методов необходимых для любого объекта библиотеки
 */

#define CO_OBJECT_DEFINITION(Object) \
    typedef struct Object Object; \
    CO_FREE(Object); \
    CO_CREATE(Object); \
    CO_CLONE(Object); \
    CO_COPY(Object); \
    CO_RESET(Object); \
    CO_COMPARE(Object);


//========================== Для контейнеров ==============================

/**
 * @brief co_status Object_clear(Object *Object_obj) удаляет все элементы из контейнера
 * @param Object_obj указатель на контейнер
 * @return CO_OK в случае успеха, или код ошибки
 */
#define CO_CLEAR(Object) co_status Object##_clear(Object * Object##_obj)

/**
 * @brief ssize_t Object_size(const Object *Object_obj) находит количество элементов в контейнере
 * @param Object_obj указатель на контейнер
 * @return количество элементов
 */
#define CO_SIZE(Object) size_t Object##_size(const Object *Object##_obj)

#define CO_CONTAINER_DEFINITION(Object) \
    typedef void (*co_free_function)(void *); \
    typedef void * (*co_clone_function)(const void *); \
    CO_CLEAR(Object); \
    CO_SIZE(Object)

//=========================Для линейных контейнеров=========================

#define CO_PUSH_BACK(Object) co_status Object##_push_back(Object * Object##_obj, const void *object)
#define CO_MOVE_BACK(Object) co_status Object##_move_back(Object * Object##_obj, void *object)
#define CO_PUSH_FRONT(Object) co_status Object##_push_front(Object * Object##_obj, const void *object)
#define CO_MOVE_FRONT(Object) co_status Object##_move_front(Object * Object##_obj, void *object)
#define CO_POP_BACK(Object) co_status Object##_pop_back(Object * Object##_obj)
#define CO_POP_FRONT(Object) co_status Object##_pop_front(Object * Object##_obj)
#define CO_BACK(Object) const void * Object##_back(const Object * Object##_obj)
#define CO_FRONT(Object) const void * Object##_front(const Object * Object##_obj)

#define CO_LINEAR_CONTAINER_DEFINITION(Object) \
    CO_PUSH_BACK(Object); \
    CO_MOVE_BACK(Object); \
    CO_PUSH_FRONT(Object); \
    CO_MOVE_FRONT(Object); \
    CO_POP_BACK(Object); \
    CO_POP_FRONT(Object); \
    CO_BACK(Object); \
    CO_FRONT(Object)

#ifdef __cplusplus
}
#endif

#endif // CO_MACROS_H
