#include <check.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test_cond/test_cond.h"
#include "test_green/test_green.h"
#include "test_mutex/test_mutex.h"

int
main(void)
{
	int no_failed = 0;
	Suite *s = NULL;
	SRunner *runner = NULL;

	s = test_mutex_suite();
	runner = srunner_create(s);
	srunner_add_suite(runner, test_cond_suite());
	srunner_add_suite(runner, test_green_suite());

	srunner_run_all(runner, CK_NORMAL);
	no_failed = srunner_ntests_failed(runner);
	srunner_free(runner);

	return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
