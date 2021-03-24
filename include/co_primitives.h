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

#ifndef CO_PRIMITIVES_H
#define CO_PRIMITIVES_H

#include <stddef.h>
#include "co_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

CO_PRIMITIVE_DECLARATION(int);
CO_PRIMITIVE_DECLARATION(short);
CO_PRIMITIVE_DECLARATION(long);
CO_PRIMITIVE_DECLARATION(float);
CO_PRIMITIVE_DECLARATION(double);
CO_PRIMITIVE_DECLARATION(char);
CO_PRIMITIVE_DECLARATION(size_t);

#ifdef __cplusplus
}
#endif

#endif // CO_PRIMITIVES_H
