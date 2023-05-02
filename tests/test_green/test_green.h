#include <check.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/mutex/mutex.h"

Suite *test_green_suite(void);

static int global_green = 0;

static void *
test_green_no_mutex(void *arg)
{
	(void)arg;
	(void)arg;

	int loop = 4;
	while (loop > 0) {
		global_green++;
		loop--;
	}
	return EXIT_SUCCESS;
}

static void *
test_green_yield(void *arg)
{
	(void)arg;

	int loop = 4;
	while (loop > 0) {
		loop--;
		global_green++;
		green_yield();
	}
	return EXIT_SUCCESS;
}

START_TEST(TEST_GREEN_NO_LOCK)
{
	green_t g0, g1, g2;
	int a0 = 0;
	int a1 = 1;
	int a2 = 2;
	green_create(&g0, test_green_no_mutex, &a0);
	green_create(&g1, test_green_no_mutex, &a1);
	green_create(&g2, test_green_no_mutex, &a2);

	green_join(&g0, NULL);
	green_join(&g1, NULL);
	green_join(&g2, NULL);

	ck_assert_int_ne(global_green, 0);
}
END_TEST

START_TEST(TEST_GREEN_YIELD)
{
	global_green = 0;
	green_t g0, g1;
	int a0 = 0;
	int a1 = 1;
	green_create(&g0, test_green_yield, &a0);
	green_create(&g1, test_green_yield, &a1);

	green_join(&g0, NULL);
	green_join(&g1, NULL);

	ck_assert_int_eq(global_green, 8);
}
END_TEST
