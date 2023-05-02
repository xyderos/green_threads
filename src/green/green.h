#pragma once

#include <ucontext.h>

typedef struct green_t {
	ucontext_t *context;
	void *(*fun)(void *);
	void *arg;
	struct green_t *next;
	struct green_t *join;
	void *retval;
	int zombie;
} green_t;

int green_create(green_t *, void *(*)(void *), void *);
int green_yield(void);
int green_join(green_t *, void **);
