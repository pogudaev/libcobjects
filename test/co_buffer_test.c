#include <assert.h>
#include <co_buffer.h>
#include <string.h>
#include <stdlib.h>

#define DO_TEST(x) if (!strcmp(argv[1], #x)) { x(); return 0; }


static void co_buffer_create_free_test();
static void co_buffer_set_test();
static void co_buffer_add_test();
static void co_buffer_get_test();

int main(const int argc, const char *argv[])
{
	DO_TEST(co_buffer_create_free_test);
	DO_TEST(co_buffer_set_test);
	DO_TEST(co_buffer_add_test);
	DO_TEST(co_buffer_get_test);
	assert(0); //Имя теста не найдено
	return 0;
}

static void co_buffer_create_free_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(co_buffer_obj->length == 0);
	assert(co_buffer_obj->alloc_length == 0);
	co_buffer_free(co_buffer_obj);
}

static void co_buffer_set_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data13 = "QSD\0FGH\0C\nIKI";
	const char *data5 = "FGDST";

	co_buffer_set(co_buffer_obj, data13, 6);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data13, 6) == 0);
	assert(co_buffer_obj->length == 6);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data13);

	co_buffer_set(co_buffer_obj, data5, 5);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data5, 5) == 0);
	assert(co_buffer_obj->length == 5);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data5);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_add_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data13 = "QSD\0FGH\0C\nIKI";
	const char *data5 = "FGDST";
	const char *data_res1 = "QSD\0FGFGDST";
	const char *data8 = "sxdcfvbb";
	const char *data_res2 = "QSD\0FGFGDSTsxdc";

	co_buffer_add(co_buffer_obj, data13, 6);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data13, 6) == 0);
	assert(co_buffer_obj->length == 6);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data13);

	co_buffer_add(co_buffer_obj, data5, 5);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data_res1, 11) == 0);
	assert(co_buffer_obj->length == 11);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);

	co_buffer_add(co_buffer_obj, data8, 4);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data_res2, 15) == 0);
	assert(co_buffer_obj->length == 15);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_get_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data12 = "Qsdskie\0sfas";

	void *ret_data = NULL;
	size_t ret_len = 0;

	co_buffer_get(co_buffer_obj, &ret_data, &ret_len);
	assert(ret_data);
	assert(ret_len == 0);
	free(ret_data);

	co_buffer_set(co_buffer_obj, data12, 12);
	co_buffer_get(co_buffer_obj, &ret_data, &ret_len);
	assert(ret_data);
	assert(memcmp(ret_data, data12, 12) == 0);
	assert(ret_len == 12);
	free(ret_data);

	co_buffer_free(co_buffer_obj);
}


