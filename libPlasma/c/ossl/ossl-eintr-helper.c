
/* (c)  oblong industries */

/* Trivial EINTR helper for simple openssl apps
 *
 * Apps with proper event loops won't need this,
 * but apps that use blocking OpenSSL calls and
 * fail due to interrupted system calls may find it useful.
 *
 * Copyright 2016 Dan Kegel
 * Permission granted to redistribute subject to the terms of the
 * 2-clause BSD License, the GPL or LGPL versions 2 or 3,
 * or the OpenSSL License.
 *
 * Copyright (c) 2016 Dan Kegel. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ossl-eintr-helper.h"

#include <errno.h>

/*
 * Replacement for blocking SSL_connect() that handles EINTR better.
 * OpenSSL's SSL_connect simply doesn't do the job, even with
 * SSL_MODE_AUTO_RETRY set.
 * (That's probably because serious openssl apps all use
 * the nonblocking interface, which is better at retries.)
 *
 * Note that the underlying system call that's being retried is
 * probably read() or write(), not connect() (you need to handle
 * that before calling SSL_connect if you want proper EINTR
 * handling on Mac OS X, see EINTR_connect_harder() in eintr-helper.h).
 */

int EINTR_SSL_connect_harder (SSL *ssl)
{
#if defined(_WIN32)
  /* win32 doesn't have EINTR, not really */
  return SSL_connect (ssl);
#else
  int e;
  do
    e = SSL_connect (ssl);
  while (e < 0 && errno == EINTR);
  return e;
#endif
}
