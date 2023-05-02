#pragma once

#include "../mutex/mutex.h"

typedef struct green_cond_t {
	green_t *wait;
} green_cond_t;

void green_cond_init(green_cond_t *cond);
int green_cond_wait(green_cond_t *cond, green_mutex_t *mutex);
void green_cond_signal(green_cond_t *cond);
