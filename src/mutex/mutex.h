#pragma once

#include "../green/green.h"

typedef struct green_mutex_t {
	volatile int taken;
	green_t *susp_l;
} green_mutex_t;

int green_mutex_init(green_mutex_t *);
int green_mutex_lock(green_mutex_t *);
int green_mutex_unlock(green_mutex_t *);
