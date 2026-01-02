#include "pthread_impl.h"

__attribute__((import_module("$root"), import_name("[thread-yield]")))
extern int thread_yield();

int pthread_cond_wait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m)
{
	__pthread_mutex_unlock(m);
	thread_yield();
	return 0;
}
