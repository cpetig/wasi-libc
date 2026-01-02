#include "pthread_impl.h"
#include <wasi/api.h>
#ifdef __wasip3__
#include <assert.h>
#include "wasi/wasip3.h"
#endif

static void dummy_0()
{
}
weak_alias(dummy_0, __acquire_ptc);
weak_alias(dummy_0, __release_ptc);

#ifdef __wasip3__
typedef void (*thread_fun)(void*arg);
typedef void* (*pthread_fun)(void*arg);

__attribute__((import_module("$root"), import_name("[thread-new-indirect-v0]")))
extern int32_t thread_new_indirect(thread_fun a,void* b);

__attribute__((import_module("$root"), import_name("[thread-resume-later]")))
extern void thread_resume_later(int32_t tid);

void thread_wrapper(void *arg) {
	pthread_fun func = ((pthread_fun*)arg)[0];
	void *real_arg = ((void**)arg)[1];
	free(arg);
	void* result;

	result = func(real_arg);
	// TODO: How to handle the result?
}

int __pthread_create(pthread_t *restrict res, const pthread_attr_t *restrict attrp, void *(*entry)(void *), void *restrict arg)
{
	void** arg2 = (void**)malloc(2*sizeof(void*));
	arg2[0] = (void*)entry;
	arg2[1] = (void*)arg;

	int32_t ret = thread_new_indirect(thread_wrapper, arg2);
	assert(ret != 0);
	if (ret < 0) {
		return EAGAIN;
	} else {
		// TODO: TLS via context.set 
		thread_resume_later(ret);
		*res = (pthread_t)ret;
		return 0;
	}
}
#else
int __pthread_create(pthread_t *restrict res, const pthread_attr_t *restrict attrp, void *(*entry)(void *), void *restrict arg)
{
  /*
	"The system lacked the necessary resources to create another thread,
	or the system-imposed limit on the total number of threads in a process
	{PTHREAD_THREADS_MAX} would be exceeded."
  */
  return EAGAIN;
}
#endif

weak_alias(__pthread_create, pthread_create);
