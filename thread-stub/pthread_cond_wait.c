#include "pthread_impl.h"
#include <wasi/api.h>

#ifdef __wasip3__
#include <assert.h>

__attribute__((import_module("$root"), import_name("[thread-yield]")))
extern int thread_yield();
#endif

int pthread_cond_wait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m)
{
	__pthread_mutex_unlock(m);
#ifdef __wasip3__
	if (c->__u.__i[0]==0) {
		pthread_cond_init(c, 0);
	}
	uint8_t buf[1];
	wasip3_waitable_status_t status = filesystem_stream_u8_read(c->__u.__i[0], buf, 1);
	if (status == WASIP3_WAITABLE_STATUS_BLOCKED) {
    wasip3_waitable_set_t set = wasip3_waitable_set_new();
    wasip3_waitable_join(c->__u.__i[0], set);
    wasip3_event_t event;
    wasip3_waitable_set_wait(set, &event);
    assert(event.event == WASIP3_EVENT_STREAM_READ);
    assert(event.waitable == c->__u.__i[0]);
    wasip3_waitable_set_drop(set);
  } else if (WASIP3_WAITABLE_STATE(status) == WASIP3_WAITABLE_COMPLETED) {
  } else {
    abort();
  }
#endif
	return 0;
}
