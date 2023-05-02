#include "test_green.h"

Suite *
test_green_suite(void)
{
	Suite *s = NULL;
	TCase *tc_core = NULL;

	s = suite_create("test_green");
	tc_core = tcase_create("core");

	tcase_add_test(tc_core, TEST_GREEN_NO_LOCK);
	tcase_add_test(tc_core, TEST_GREEN_YIELD);

	suite_add_tcase(s, tc_core);

	return s;
}
