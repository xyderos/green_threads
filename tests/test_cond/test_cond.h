#include <check.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/cond/cond.h"

Suite *test_cond_suite(void);

static int flag = 0;
static green_cond_t cond;
static green_mutex_t mutex;

static void *
test_cond_with_mutex(void *arg)
{
	int id = *(int *)arg;
	int loop = 4;
	while (loop > 0) {
		green_mutex_lock(&mutex);
		while (flag != id) {
			green_cond_wait(&cond, &mutex);
		}
		flag = (id + 1) % 2;
		green_cond_signal(&cond);
		green_mutex_unlock(&mutex);
		loop--;
	}
	return EXIT_SUCCESS;
}

static void *
test_cond_with_no_mutex(void *arg)
{
	int id = *(int *)arg;
	int loop = 4;
	while (loop > 0) {
		if (flag == id) {
			loop--;
			flag = (id + 1) % 2;
			green_cond_signal(&cond);
		} else {
			green_cond_wait(&cond, NULL);
		}
	}
	return EXIT_SUCCESS;
}

START_TEST(TEST_COND_WITH_MUTEX)
{
	green_t g0, g1;
	int a0 = 0;
	int a1 = 1;
	green_cond_init(&cond);
	green_mutex_init(&mutex);
	green_create(&g0, test_cond_with_mutex, &a0);
	green_create(&g1, test_cond_with_mutex, &a1);

	green_join(&g0, NULL);
	green_join(&g1, NULL);

	ck_assert_int_eq(flag, 0);
}
END_TEST

START_TEST(TEST_COND_WITH_NO_MUTEX)
{
	green_t g0, g1;
	int a0 = 0;
	int a1 = 1;

	flag = 0;
	green_cond_init(&cond);
	green_create(&g0, test_cond_with_no_mutex, &a0);
	green_create(&g1, test_cond_with_no_mutex, &a1);

	green_join(&g0, NULL);
	green_join(&g1, NULL);

	ck_assert_int_eq(flag, 0);
}
END_TEST
