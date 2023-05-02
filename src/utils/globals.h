#pragma once

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "../green/green.h"

extern ucontext_t main_cntx;
extern green_t main_green;
extern green_t *running;
extern green_t *ready;
extern sigset_t block;
