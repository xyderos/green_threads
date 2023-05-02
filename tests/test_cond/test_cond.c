#include "test_cond.h"

Suite *
test_cond_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_cond");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, TEST_COND_WITH_MUTEX);
	tcase_add_test(tc_core, TEST_COND_WITH_NO_MUTEX);

	suite_add_tcase(s, tc_core);

	return s;
}
