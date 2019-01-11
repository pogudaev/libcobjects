#include <stdio.h>
#include "co_string.h"
#include "co_list.h"
#include "co_map.h"

int main(void)
{
	printf("Example libcobjects\n");

    co_string *str = co_string_create();

    co_string_set(str, "qwer; wsdf; rtyg; sads; rtrg; sdfa");

    printf("%s\n", str->c_str);

    co_list *list = co_string_split_to_list(str, "; ");

    co_list_foreach(list, iter) {
        co_string * elem = (co_string *) iter->data;
        printf("%s\n", elem->c_str);
    }

    co_list_free(list);

    co_string_free(str);

    str = co_string_create();
    co_string_sprintf(str, "hello %s; %d; %% %c %s", "world", 42, 'Z', "end");

    co_string_crop(str, 1, 2);

    printf("%s\n", str->c_str);

    co_string_free(str);


    //---
    co_map *map = co_map_create();
    co_map_set_type(map, char);

    char q = '1';
    co_map_set(map, "bbb", &q);
    q = '2';
    co_map_set(map, "aaaa", &q);
    q = '3';
    co_map_set(map, "ddd", &q);
    q = '4';
    co_map_set(map, "ccc", &q);

    const char *a = (const char *) co_map_get(map, "aaaa"); //'2'

    co_map_free(map);


	return 0;
}
