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

#include "co_ipv4.h"
#include <stdlib.h>

CO_CREATE(co_ipv4)
{
    co_ipv4 *co_ipv4_obj = (co_ipv4 *) calloc(sizeof(co_ipv4), 1);
    return co_ipv4_obj;
}

CO_FREE(co_ipv4)
{
    if (co_ipv4_obj){
        free(co_ipv4_obj);
    }
}

CO_CLONE(co_ipv4)
{
    if (!co_ipv4_src){
        return NULL;
    }
    co_ipv4 *co_ipv4_obj = (co_ipv4 *) malloc(sizeof(co_ipv4));
    if (co_ipv4_obj){
        co_ipv4_obj->addr = co_ipv4_src->addr;
    }
    return co_ipv4_obj;
}

CO_COPY(co_ipv4)
{
    if (!co_ipv4_dst || !co_ipv4_src){
        return CO_BAD_ARG_ERR;
    }
    co_ipv4_dst->addr = co_ipv4_src->addr;
    return CO_OK;
}

CO_RESET(co_ipv4)
{
    if (!co_ipv4_obj){
        return CO_BAD_ARG_ERR;
    }
    co_ipv4_obj->addr = 0;
    return CO_OK;
}

CO_COMPARE(co_ipv4)
{
    if (!co_ipv4_a || !co_ipv4_b){
        return CO_CMP_ERR;
    }
    if (co_ipv4_a->addr == co_ipv4_b->addr) return CO_CMP_EQ;
    else return CO_CMP_NE;
}


