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

#ifndef CO_IPV4_H
#define CO_IPV4_H

#include "co_status_code.h"
#include "co_macros.h"
#include <stdint.h>


CO_OBJECT_DEFINITION(co_ipv4);

struct co_ipv4{
    uint32_t addr;
};

co_ipv4 *co_ipv4_create_from_c_str(const char *str);
co_status co_ipv4_apply_mask(co_ipv4 *co_ipv4_obj, const co_ipv4 *co_ipv4_mask);
co_status co_ipv4_is_valid_mask(const co_ipv4 *co_ipv4_mask);

#endif // CO_IPV4_H
