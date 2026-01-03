#include "pthread_impl.h"
#include <wasi/api.h>

int pthread_cond_destroy(pthread_cond_t *c)
{
#ifdef __wasip3__
	filesystem_stream_u8_drop_readable(c->__u.__i[0]);
	filesystem_stream_u8_drop_writable(c->__u.__i[1]);
#endif
	return 0;
}
