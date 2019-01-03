#ifndef TEST_UNIT_H
#define TEST_UNIT_H

#include <stdio.h>
#include <stdlib.h>

#define FAIL	-1
#define OK		0

#define TEST(func) \
	{ \
		printf("\033[36m%s\033[0m:\t\t", #func); \
		if (func() == 0) printf("\033[32mOK\033[0m\n"); \
		else { printf("\033[31mFAIL\033[0m\n"); exit(-1);} \
	}

#define TEST_OK(object) \
        { printf("\033[32mAll test %s is OK\033[0m\n", object); }

#endif
