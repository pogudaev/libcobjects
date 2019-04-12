#include <stdio.h>
#include "co_string.h"
#include "co_list.h"
#include "co_map.h"
#include "co_vector.h"

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

    char w = '2';
    co_map_remove_by_value(map, &w);

    q = '4';
    co_map_set(map, "ccc", &q);

    //const char *a = (const char *) co_map_get(map, "aaaa"); //'2'

    co_map_free(map);

    //---

    co_string *str2 = co_string_create_with_allocate(100);

    co_string_set(str2, "234234234234234234234234234");

    co_string_set(str2, "test");

    const char *qwwww = "qwertyuiopasdfghjkl";

    co_string_add_substr(str2, qwwww + 4, 3);

    printf("%s\n", str2->c_str);

    printf("----------------------------\n");

    co_vector *vector = co_vector_create();
    co_vector_set_type(vector, co_string);

    printf("size = %lu\n", co_vector_size(vector));


    for (int i = 0; i < 100; i++){
        char buff[30];
        sprintf(buff, "test_%d", i);
        co_string *str = co_string_create_from_c_str(buff);
        if (i%2 == 0){
            co_vector_move_front(vector, str);
        }
        else{
            co_vector_move_back(vector, str);
        }
    }

    co_vector *vec = co_vector_clone(vector);

    co_vector_move_front(vec, co_string_create_from_c_str("*"));

    printf("CMP: %d\n", co_vector_compare(vec, vector));


    do{
        const co_string *co_str = (const co_string *) co_vector_back(vec);
        if (co_str)
            printf("%s\n", co_str->c_str);
    }
    while(co_vector_pop_back(vec) != CO_IMPOSIBLE_OPERATION_ERR);

	return 0;
}
