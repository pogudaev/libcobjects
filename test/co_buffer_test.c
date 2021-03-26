#include <assert.h>
#include <co_buffer.h>
#include <string.h>
#include <stdlib.h>

#define DO_TEST(x) if (!strcmp(argv[1], #x)) { x(); return 0; }

static void co_buffer_create_test();
static void co_buffer_free_test();
static void co_buffer_set_test();
static void co_buffer_add_test();
static void co_buffer_get_test();
static void co_buffer_clone_test();
static void co_buffer_copy_test();
static void co_buffer_reset_test();
static void co_buffer_compare_test();
static void co_buffer_realloc_test();
static void co_buffer_set_alloc_length_test();
static void co_buffer_create_and_alloc_test();
static void co_buffer_append_test();

int main(const int argc, const char *argv[])
{
	(void) argc;
	DO_TEST(co_buffer_create_test);
	DO_TEST(co_buffer_free_test);
	DO_TEST(co_buffer_set_test);
	DO_TEST(co_buffer_add_test);
	DO_TEST(co_buffer_get_test);
	DO_TEST(co_buffer_clone_test);
	DO_TEST(co_buffer_copy_test);
	DO_TEST(co_buffer_reset_test);
	DO_TEST(co_buffer_compare_test);
	DO_TEST(co_buffer_realloc_test);
	DO_TEST(co_buffer_set_alloc_length_test);
	DO_TEST(co_buffer_create_and_alloc_test);
	DO_TEST(co_buffer_append_test);
	assert(0); //Имя теста не найдено
	return 0;
}

static void co_buffer_create_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(co_buffer_obj->length == 0);
	assert(co_buffer_obj->alloc_length == 0);
	free(co_buffer_obj->data);
	free(co_buffer_obj);
}

static void co_buffer_free_test()
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
	assert(co_buffer_set(NULL, NULL, 0) == CO_BAD_ARG_ERR);
	assert(co_buffer_set(NULL, NULL, 10) == CO_BAD_ARG_ERR);
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data13 = "QSD\0FGH\0C\nIKI";
	const char *data5 = "FGDST";

	assert(co_buffer_set(co_buffer_obj, data13, 6) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data13, 6) == 0);
	assert(co_buffer_obj->length == 6);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data13);

	assert(co_buffer_set(co_buffer_obj, data5, 5) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data5, 5) == 0);
	assert(co_buffer_obj->length == 5);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data5);

	assert(co_buffer_set(co_buffer_obj, NULL, 1) == CO_BAD_ARG_ERR);
	assert(co_buffer_set(NULL, data5, 1) == CO_BAD_ARG_ERR);

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

	assert(co_buffer_add(co_buffer_obj, data13, 6) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data13, 6) == 0);
	assert(co_buffer_obj->length == 6);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);
	assert(co_buffer_obj->data != data13);

	assert(co_buffer_add(co_buffer_obj, data5, 5) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data_res1, 11) == 0);
	assert(co_buffer_obj->length == 11);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);

	assert(co_buffer_add(co_buffer_obj, data8, 4) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, data_res2, 15) == 0);
	assert(co_buffer_obj->length == 15);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);

	assert(co_buffer_add(NULL, data8, 4) == CO_BAD_ARG_ERR);
	assert(co_buffer_add(co_buffer_obj, NULL, 4) == CO_BAD_ARG_ERR);
	assert(co_buffer_add(NULL, NULL, 4) == CO_BAD_ARG_ERR);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_get_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data12 = "Qsdskie\0sfas";

	void *ret_data = NULL;
	size_t ret_len = 0;

	assert(co_buffer_get(co_buffer_obj, &ret_data, &ret_len) == CO_OK);
	assert(ret_data);
	assert(ret_len == 0);
	free(ret_data);

	assert(co_buffer_set(co_buffer_obj, data12, 12) == CO_OK);
	assert(co_buffer_get(co_buffer_obj, &ret_data, &ret_len) == CO_OK);
	assert(ret_data);
	assert(memcmp(ret_data, data12, 12) == 0);
	assert(ret_len == 12);
	free(ret_data);

	ret_data = NULL;
	ret_len = 0;
	assert(co_buffer_get(co_buffer_obj, &ret_data, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(co_buffer_obj, NULL, &ret_len) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(NULL, &ret_data, &ret_len) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(NULL, NULL, &ret_len) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(NULL, &ret_data, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(co_buffer_obj, NULL, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_get(NULL, NULL, NULL) == CO_BAD_ARG_ERR);
	assert(ret_data == NULL);
	assert(ret_len == 0);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_clone_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "QSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	co_buffer *co_buffer_cloned = co_buffer_clone(co_buffer_obj);
	assert(co_buffer_cloned);
	assert(co_buffer_cloned->data);
	assert(memcmp(co_buffer_cloned->data, data8, 8) == 0);
	assert(co_buffer_cloned->length == 8);
	assert(co_buffer_cloned->alloc_length >= co_buffer_cloned->length);
	assert(co_buffer_cloned->data != co_buffer_obj->data);
	co_buffer_free(co_buffer_obj);
	co_buffer_free(co_buffer_cloned);
}

static void co_buffer_copy_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "QSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	assert(co_buffer_copy(NULL, co_buffer_obj) == CO_BAD_ARG_ERR);
	assert(co_buffer_copy(co_buffer_obj, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_copy(NULL, NULL) == CO_BAD_ARG_ERR);

	co_buffer *co_buffer_obj2 = co_buffer_create();
	co_buffer *co_buffer_obj3 = co_buffer_create();
	co_buffer_set(co_buffer_obj3, "QSCWWWWWWW", 10);

	assert(co_buffer_copy(co_buffer_obj2, co_buffer_obj) == CO_OK);
	assert(co_buffer_copy(co_buffer_obj3, co_buffer_obj) == CO_OK);

	assert(co_buffer_obj2);
	assert(co_buffer_obj2->data);
	assert(memcmp(co_buffer_obj2->data, data8, 8) == 0);
	assert(co_buffer_obj2->length == 8);
	assert(co_buffer_obj2->alloc_length >= co_buffer_obj2->length);
	assert(co_buffer_obj2->data != co_buffer_obj->data);

	assert(co_buffer_obj3);
	assert(co_buffer_obj3->data);
	assert(memcmp(co_buffer_obj3->data, data8, 8) == 0);
	assert(co_buffer_obj3->length == 8);
	assert(co_buffer_obj3->alloc_length >= co_buffer_obj3->length);
	assert(co_buffer_obj3->data != co_buffer_obj->data);

	co_buffer_free(co_buffer_obj);
	co_buffer_free(co_buffer_obj2);
	co_buffer_free(co_buffer_obj3);
}

static void co_buffer_reset_test()
{
	assert(co_buffer_reset(NULL) == CO_BAD_ARG_ERR);
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "QSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	assert(co_buffer_reset(co_buffer_obj) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(co_buffer_obj->length == 0);
	assert(co_buffer_obj->alloc_length == 0);
	co_buffer_free(co_buffer_obj);
}

static void co_buffer_compare_test()
{
	co_buffer *co_buffer_obj_a = co_buffer_create();
	const char *data8 = "FSEFTsdf";
	co_buffer_set(co_buffer_obj_a, data8, 8);

	assert(co_buffer_compare(co_buffer_obj_a, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_compare(NULL, co_buffer_obj_a) == CO_BAD_ARG_ERR);
	assert(co_buffer_compare(NULL, NULL) == CO_BAD_ARG_ERR);

	co_buffer *co_buffer_obj_b = co_buffer_create();
	co_buffer_set(co_buffer_obj_b, data8, 8);

	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_EQ);

	co_buffer_set(co_buffer_obj_b, "ZZZ", 3);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_LT);

	co_buffer_set(co_buffer_obj_b, "ZZZZZZZZZ", 9);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_LT);

	co_buffer_set(co_buffer_obj_b, "AAA", 3);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_GT);

	co_buffer_set(co_buffer_obj_b, "AAAAAAAA", 8);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_GT);

	co_buffer_set(co_buffer_obj_b, "AAAAAAAAA", 9);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_GT);

	co_buffer_set(co_buffer_obj_b, "FSEFTsdz", 8);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_LT);

	co_buffer_set(co_buffer_obj_b, "FSEFTsda", 8);
	assert(co_buffer_compare(co_buffer_obj_a, co_buffer_obj_b) == CO_CMP_GT);

	co_buffer_free(co_buffer_obj_a);
	co_buffer_free(co_buffer_obj_b);
}

static void co_buffer_realloc_test()
{
	assert(co_buffer_realloc(NULL, 1) == CO_BAD_ARG_ERR);

	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "FSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	assert(co_buffer_realloc(co_buffer_obj, 4) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, "FSEF", 4) == 0);
	assert(co_buffer_obj->length == 4);
	assert(co_buffer_obj->alloc_length == 4);

	assert(co_buffer_realloc(co_buffer_obj, 20) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, "FSEF", 4) == 0);
	assert(co_buffer_obj->length == 4);
	assert(co_buffer_obj->alloc_length == 20);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_set_alloc_length_test()
{
	assert(co_buffer_set_alloc_length(NULL, 1) == CO_BAD_ARG_ERR);

	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "FSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	assert(co_buffer_set_alloc_length(co_buffer_obj, 4) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(co_buffer_obj->length == 0);
	assert(co_buffer_obj->alloc_length == 4);

	co_buffer_free(co_buffer_obj);
}

static void co_buffer_create_and_alloc_test()
{
	co_buffer *co_buffer_obj = co_buffer_create_and_alloc(10);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(co_buffer_obj->length == 0);
	assert(co_buffer_obj->alloc_length == 10);
	free(co_buffer_obj->data);
	free(co_buffer_obj);

	co_buffer *co_buffer_obj2 = co_buffer_create_and_alloc(0);
	assert(co_buffer_obj2);
	assert(co_buffer_obj2->data);
	assert(co_buffer_obj2->length == 0);
	assert(co_buffer_obj2->alloc_length == 0);
	free(co_buffer_obj2->data);
	free(co_buffer_obj2);

	co_buffer *co_buffer_obj3 = co_buffer_create_and_alloc(230);
	assert(co_buffer_obj3);
	assert(co_buffer_obj3->data);
	assert(co_buffer_obj3->length == 0);
	assert(co_buffer_obj3->alloc_length == 230);
	free(co_buffer_obj3->data);
	free(co_buffer_obj3);
}

static void co_buffer_append_test()
{
	co_buffer *co_buffer_obj = co_buffer_create();
	const char *data8 = "FSEFTsdf";
	co_buffer_set(co_buffer_obj, data8, 8);

	co_buffer *co_buffer_obj2 = co_buffer_create();
	const char *data10 = "QsXCDDD458";
	co_buffer_set(co_buffer_obj2, data10, 10);

	assert(co_buffer_append(NULL, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_append(co_buffer_obj, NULL) == CO_BAD_ARG_ERR);
	assert(co_buffer_append(NULL, co_buffer_obj) == CO_BAD_ARG_ERR);

	assert(co_buffer_append(co_buffer_obj, co_buffer_obj2) == CO_OK);
	assert(co_buffer_obj);
	assert(co_buffer_obj->data);
	assert(memcmp(co_buffer_obj->data, "FSEFTsdfQsXCDDD458", 18) == 0);
	assert(co_buffer_obj->length == 18);
	assert(co_buffer_obj->alloc_length >= co_buffer_obj->length);

	free(co_buffer_obj2);
	free(co_buffer_obj);
}
