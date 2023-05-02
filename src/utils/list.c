#include <stdlib.h>

#include "list.h"

void
add_last(green_t **list, green_t *thread)
{
	if (!*list) {
		*list = thread;
	} else {
		green_t *tmp = *list;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = thread;
	}
}

void
add_first(green_t **list, green_t *thread)
{
	if (!*list) {
		*list = thread;
		(*list)->next = NULL;
	} else {
		thread->next = *list;
		*list = thread;
	}
}

green_t *
remove_last(green_t **list)
{
	green_t *tmp = *list, *copy = NULL;
	if (!*list) {
		exit(1);
	} else if (!(*list)->next) {
		*list = NULL;
		return tmp;
	} else {
		while (tmp->next->next) {
			tmp = tmp->next;
		}
		copy = tmp->next;
		tmp->next = NULL;
		return copy;
	}
}

green_t *
remove_first(green_t **list)
{
	if (!*list) {
		exit(1);
	} else {
		green_t *cp = *list;
		*list = (*list)->next;
		cp->next = NULL;
		return cp;
	}
}
