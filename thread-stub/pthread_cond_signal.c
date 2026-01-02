#include "pthread_impl.h"

__attribute__((import_module("$root"), import_name("[thread-yield]")))
extern int thread_yield();

int pthread_cond_signal(pthread_cond_t *c)
{
	thread_yield();
	return 0;
}
