#include <sys/time.h>

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "../utils/globals.h"
#include "../utils/list.h"
#include "green.h"

#define STACK_SIZE 4096
#define PERIOD 100

ucontext_t main_cntx = { 0 };
green_t main_green = { &main_cntx, NULL, NULL, NULL, NULL, NULL, 0 };
green_t *running = &main_green;
green_t *ready = NULL;

sigset_t block;

void timer_handler(int);

static void init(void) __attribute__((constructor));

static void
init(void)
{
	struct sigaction act = { 0 };
	struct timeval interval;
	struct itimerval period;

	getcontext(&main_cntx);
	sigemptyset(&block);
	sigaddset(&block, SIGVTALRM);

	act.sa_handler = timer_handler;

	assert(sigaction(SIGVTALRM, &act, NULL) == 0);

	interval.tv_sec = 0;
	interval.tv_usec = PERIOD;
	period.it_interval = interval;
	period.it_value = interval;

	assert(setitimer(ITIMER_VIRTUAL, &period, NULL) == 0);
}

void
timer_handler(int sig)
{
	green_t *susp = NULL, *next = NULL;

	(void)sig;

	sigprocmask(SIG_BLOCK, &block, NULL);

	susp = running;
	susp->next = NULL;

	add_last(&ready, susp);

	next = remove_first(&ready);
	running = next;

	swapcontext(susp->context, next->context);

	sigprocmask(SIG_UNBLOCK, &block, NULL);
}

static void
green_thread(void)
{
	green_t *this = running, *next = NULL;
	void *result = (*this->fun)(this->arg);

	if (this->join) {

		this->join->next = NULL;

		add_last(&ready, this->join);
	}

	this->retval = result;
	this->zombie = 1;
	next = remove_first(&ready);
	running = next;

	setcontext(next->context);
}

int
green_create(green_t *new, void *(*fun)(void *), void *arg)
{
	ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
	void *stack = NULL;

	getcontext(cntx);

	stack = malloc(STACK_SIZE);
	cntx->uc_stack.ss_sp = stack;
	cntx->uc_stack.ss_size = STACK_SIZE;

	makecontext(cntx, green_thread, 0);

	new->context = cntx;
	new->fun = fun;
	new->arg = arg;
	new->next = NULL;
	new->join = NULL;
	new->retval = NULL;
	new->zombie = 0;

	add_last(&ready, new);

	return 0;
}

int
green_yield(void)
{
	green_t *susp = running, *next = NULL;

	susp->next = NULL;

	add_last(&ready, susp);

	next = remove_first(&ready);
	running = next;

	swapcontext(susp->context, next->context);

	return 0;
}

int
green_join(green_t *thread, void **res)
{
	green_t *susp = NULL, *next = NULL;

	if (!thread->zombie) {

		susp = running;
		next = remove_first(&ready);
		thread->join = susp;
		running = next;

		swapcontext(susp->context, next->context);
	}

	if (res) {
		*res = thread->retval;
	}

	free(thread->context);
	return 0;
}
