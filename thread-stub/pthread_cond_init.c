#include "pthread_impl.h"
#include <wasi/api.h>

int pthread_cond_init(pthread_cond_t *restrict c, const pthread_condattr_t *restrict a)
{
#ifdef __wasip3__
	filesystem_stream_u8_writer_t writer;
	c->__u.__i[0] = filesystem_stream_u8_new(&writer);
	c->__u.__i[1] = writer;
#endif
	return 0;
}
