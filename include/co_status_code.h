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

#ifndef CO_STATUS_CODE_H
#define CO_STATUS_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    CO_OK = 0,
    CO_CHECK_FAIL = -1,
    CO_MEM_ALLOC_ERR = -2,
    CO_BAD_ARG_ERR = -3,
    CO_IMPOSIBLE_OPERATION_ERR = -4,
    //...
    CO_UNKNOWN_ERR = -255
} co_status;


#ifdef __cplusplus
}
#endif

#endif // CO_STATUS_CODE_H
