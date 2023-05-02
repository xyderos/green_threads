#include "test_mutex.h"

Suite *
test_mutex_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_mutex");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, TEST_MUTEX);

	suite_add_tcase(s, tc_core);

	return s;
}
