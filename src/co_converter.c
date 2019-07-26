#include "co_converter.h"

co_status co_ipv4_copy_to_co_string(co_string *co_string_dst, const co_ipv4 *co_ipv4_src)
{
    if (!co_string_dst || !co_ipv4_src){
        return CO_BAD_ARG_ERR;
    }
    uint8_t *ptr = (uint8_t *)(&co_ipv4_src->addr);
    return co_string_sprintf(co_string_dst, "%d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);
}
