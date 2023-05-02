#include <signal.h>
#include <stdlib.h>

#include "../utils/globals.h"
#include "../utils/list.h"
#include "mutex.h"

int
green_mutex_init(green_mutex_t *mutex)
{
	mutex->taken = 0;
	mutex->susp_l = NULL;

	return 0;
}

int
green_mutex_lock(green_mutex_t *mutex)
{
	green_t *susp = running, *next = NULL;

	sigprocmask(SIG_BLOCK, &block, NULL);

	if (mutex->taken) {

		susp->next = NULL;

		add_last(&mutex->susp_l, susp);

		next = remove_first(&ready);
		running = next;

		swapcontext(susp->context, next->context);
	} else {
		mutex->taken = 1;
	}

	sigprocmask(SIG_UNBLOCK, &block, NULL);

	return 0;
}

int
green_mutex_unlock(green_mutex_t *mutex)
{
	green_t *resumed = NULL;

	sigprocmask(SIG_BLOCK, &block, NULL);

	if (mutex->susp_l) {

		resumed = remove_first(&mutex->susp_l);
		resumed->next = NULL;

		add_last(&ready, resumed);
	} else {
		mutex->taken = 0;
	}

	sigprocmask(SIG_UNBLOCK, &block, NULL);

	return 0;
}
