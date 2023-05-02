#include <signal.h>
#include <stdlib.h>

#include "../utils/globals.h"
#include "../utils/list.h"
#include "cond.h"

void
green_cond_init(green_cond_t *cond)
{
	cond->wait = NULL;
}

int
green_cond_wait(green_cond_t *cond, green_mutex_t *mutex)
{
	green_t *susp = NULL, *next = NULL;

	sigprocmask(SIG_BLOCK, &block, NULL);

	susp = running;
	susp->next = NULL;

	add_last(&cond->wait, susp);

	if (mutex) {
		mutex->taken = 0;

		add_last(&ready, mutex->susp_l);

		mutex->susp_l = NULL;
	}

	next = remove_first(&ready);
	running = next;

	swapcontext(susp->context, next->context);

	if (mutex) {

		if (mutex->taken) {

			susp = running;
			susp->next = NULL;

			add_last(&mutex->susp_l, susp);

			next = remove_first(&ready);
			running = next;

			swapcontext(susp->context, next->context);
		} else {
			mutex->taken = 1;
		}
	}

	sigprocmask(SIG_UNBLOCK, &block, NULL);

	return 0;
}

void
green_cond_signal(green_cond_t *cond)
{
	green_t *signaled = NULL;

	if (!cond || !cond->wait) {
		return;
	}

	signaled = remove_first(&cond->wait);
	signaled->next = NULL;

	add_last(&ready, signaled);
}
