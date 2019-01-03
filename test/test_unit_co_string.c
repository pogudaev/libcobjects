#include <stdio.h>
#include "co_string.h"
#include "test_unit.h"
#include "string.h"


static int co_string_is_valid(const co_string *string)
{
    if (string == NULL) return FAIL;
    if (string->c_str == NULL) return FAIL;
    if (string->length + 1 > string->alloc_length) return FAIL;
    if (string->alloc_length == 0) return FAIL;
    if (string->c_str[string->length] != '\0') return FAIL;
    if (strlen(string->c_str) != string->length) return FAIL;
    return OK;
}

static int co_string_is_default(const co_string *string)
{
    if (co_string_is_valid(string) != OK) return FAIL;
    if (string->length != 0) return FAIL;
    if (string->alloc_length != 1) return FAIL;
    return OK;
}

static int co_string_is_eq_value(const co_string *string, const char *cstring)
{
    if (co_string_is_valid(string) != OK) return FAIL;
    if (strcmp(string->c_str, cstring) != 0) return FAIL;
    if (string->c_str == cstring) return FAIL;
    return OK;
}

//---------------------------------------------------------------------------
static int test_co_string_free_0()
{
    co_string_free(NULL);
    return OK;
}

static int test_co_string_create_0()
{
    co_string *string = co_string_create();
    if (co_string_is_default(string) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_create_from_c_str_0()
{
    const char *test_value = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value);
    if (co_string_is_eq_value(string, test_value) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_create_from_c_str_1()
{
    const char *test_value = "";
    co_string *string = co_string_create_from_c_str(test_value);
    if (co_string_is_eq_value(string, test_value) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_create_from_c_str_2()
{
    co_string *string = co_string_create_from_c_str(NULL);
    if (string != NULL) return FAIL;
    return OK;
}

static int test_co_string_clone_0()
{
    const char *test_value = "qwertyuiop";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_clone(string1);
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_clone_1()
{
    const char *test_value = "";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_clone(string1);
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_clone_2()
{
    co_string *string = co_string_clone(NULL);
    if (string != NULL) return FAIL;
    return OK;
}


static int test_co_string_copy_0()
{
    const char *test_value = "qwertyuiop";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_create();
    if (co_string_copy(string2, string1) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_copy_1()
{
    const char *test_value = "qwertyuiop";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_create_from_c_str("test");
    if (co_string_copy(string2, string1) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_copy_2()
{
    const char *test_value = "qwertyuiop";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_create_from_c_str("testesttesttesttest");
    if (co_string_copy(string2, string1) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_copy_3()
{
    const char *test_value = "";
    co_string *string1 = co_string_create_from_c_str(test_value);
    co_string *string2 = co_string_create_from_c_str("testesttesttesttest");
    if (co_string_copy(string2, string1) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string1, test_value) != OK) return FAIL;
    if (co_string_is_eq_value(string2, test_value) != OK) return FAIL;
    if (string1 == string2) return FAIL;
    if (string1->c_str == string2->c_str) return FAIL;
    co_string_free(string1);
    co_string_free(string2);
    return OK;
}

static int test_co_string_copy_4()
{
    const char *test_value = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value);
    if (co_string_copy(string, NULL) == CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_copy_5()
{
    const char *test_value = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value);
    if (co_string_copy(NULL, string) == CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_reset_0()
{
    const char *test_value = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value);
    if (co_string_reset(string) != CO_OK) return FAIL;
    if (co_string_is_default(string) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_reset_1()
{
    if (co_string_reset(NULL) == CO_OK) return FAIL;
    return OK;
}

static int test_co_string_set_0()
{
    const char *test_value1 = "qwertyuiop";
    const char *test_value2 = "asdfghjkl";
    co_string *string = co_string_create_from_c_str(test_value1);
    if (co_string_set(string, test_value2) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value2) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_set_1()
{
    const char *test_value2 = "asdfghjkl";
    co_string *string = co_string_create();
    if (co_string_set(string, test_value2) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value2) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_set_2()
{
    const char *test_value1 = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value1);
    if (co_string_set(string, NULL) == CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value1) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_set_3()
{
    const char *test_value1 = "qwertyuiop";
    if (co_string_set(NULL, test_value1) == CO_OK) return FAIL;
    return OK;
}

static int test_co_string_add_0()
{
    const char *test_value1 = "qwertyuiop";
    const char *test_value2 = "asdfghjkl";
    const char *test_value3 = "qwertyuiopasdfghjkl";
    co_string *string = co_string_create_from_c_str(test_value1);
    if (co_string_add(string, test_value2) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value3) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_add_1()
{
    const char *test_value1 = "qwertyuiop";
    const char *test_value2 = "";
    co_string *string = co_string_create_from_c_str(test_value1);
    if (co_string_add(string, test_value2) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value1) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_add_2()
{
    const char *test_value2 = "qwertyuiop";
    co_string *string = co_string_create();
    if (co_string_add(string, test_value2) != CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value2) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_add_3()
{
    const char *test_value1 = "qwertyuiop";
    co_string *string = co_string_create_from_c_str(test_value1);
    if (co_string_add(string, NULL) == CO_OK) return FAIL;
    if (co_string_is_eq_value(string, test_value1) != OK) return FAIL;
    co_string_free(string);
    return OK;
}

static int test_co_string_add_4()
{
    const char *test_value1 = "qwertyuiop";
    if (co_string_add(NULL, test_value1) == CO_OK) return FAIL;
    return OK;
}

int main(void)
{
    TEST(test_co_string_free_0);
    TEST(test_co_string_create_0);
    TEST(test_co_string_create_from_c_str_0);
    TEST(test_co_string_create_from_c_str_1);
    TEST(test_co_string_create_from_c_str_2);
    TEST(test_co_string_clone_0);
    TEST(test_co_string_clone_1);
    TEST(test_co_string_clone_2);
    TEST(test_co_string_copy_0);
    TEST(test_co_string_copy_1);
    TEST(test_co_string_copy_2);
    TEST(test_co_string_copy_3);
    TEST(test_co_string_copy_4);
    TEST(test_co_string_copy_5);
    TEST(test_co_string_reset_0);
    TEST(test_co_string_reset_1);
    TEST(test_co_string_set_0);
    TEST(test_co_string_set_1);
    TEST(test_co_string_set_2);
    TEST(test_co_string_set_3);
    TEST(test_co_string_add_0);
    TEST(test_co_string_add_1);
    TEST(test_co_string_add_2);
    TEST(test_co_string_add_3);
    TEST(test_co_string_add_4);

    //TODO test append
    //TODO test crop


    TEST_OK("co_string");
    return 0;
}



