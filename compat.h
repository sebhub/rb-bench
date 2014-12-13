/*
 * Copyright (c) 2014 Sebastian Huber <sebhub@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef COMPAT_H
#define COMPAT_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __rtems__

#include <rtems/counter.h>
#include <rtems/bspIo.h>

typedef rtems_counter_ticks ticks;

#define ticks_read() rtems_counter_read()

#define ticks_difference(t1, t0) rtems_counter_difference(t1, t0)

#define ticks_to_nanoseconds(t) rtems_counter_ticks_to_nanoseconds(t)

#else /* __rtems__ */

#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

#define printk printf

typedef uint64_t ticks;

static inline uintptr_t ticks_read(void)
{
	struct rusage r;

	getrusage(RUSAGE_SELF, &r);

	return r.ru_utime.tv_sec * 1000000000ULL + r.ru_utime.tv_usec * 1000ULL;
}

#define ticks_difference(t1, t0) ((t1) - (t0))

#define ticks_to_nanoseconds(t) (t)

#endif /* __rtems__ */

#ifndef __inline
#define __inline inline
#endif

#ifndef __unused
#define __unused __attribute__((__unused__))
#endif

#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif

#ifndef RTEMS_INLINE_ROUTINE
#define RTEMS_INLINE_ROUTINE static inline
#endif

#ifndef RTEMS_CONTAINER_OF
#define RTEMS_CONTAINER_OF( _m, _type, _member_name ) \
  ( (_type *) ( (uintptr_t) ( _m ) - offsetof( _type, _member_name ) ) )
#endif

#define EXPORT_SYMBOL(x)

#endif /* COMPAT_H */
