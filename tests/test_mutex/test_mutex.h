#include <check.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/mutex/mutex.h"

Suite *test_mutex_suite(void);

static int global_mutex = 0;

static green_mutex_t mutex;

static void *
test_mutex(void *arg)
{
	(void)arg;
	int loop = 10;

	while (loop > 0) {
		green_mutex_lock(&mutex);
		global_mutex++;
		green_mutex_unlock(&mutex);
		loop--;
	}
	return EXIT_SUCCESS;
}

START_TEST(TEST_MUTEX)
{
	green_t g0, g1, g2;
	green_mutex_init(&mutex);
	green_create(&g0, test_mutex, NULL);
	green_create(&g1, test_mutex, NULL);
	green_create(&g2, test_mutex, NULL);

	green_join(&g0, NULL);
	green_join(&g1, NULL);
	green_join(&g2, NULL);

	ck_assert_int_eq(global_mutex, 30);
}
END_TEST
