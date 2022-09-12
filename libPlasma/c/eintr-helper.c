
/* (c)  oblong industries */

/* Trivial EINTR helper for simple socket apps
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

#include "eintr-helper.h"

#ifndef _WIN32
#include <errno.h>
#include <poll.h>
#include <sys/socket.h>
#endif

/*
 * Replacement for blocking connect() that handles EINTR better.
 * On error: returns errno
 * On success: returns zero
 * See http://www.madore.org/~david/computers/connect-intr.html
 */
int EINTR_connect_harder (int fd, const struct sockaddr *address,
                          socklen_t address_len)
{
#ifdef _WIN32
  /* Windows doesn't benignly interrupt system calls, no need to retry */
  if (connect (fd, address, address_len))
    return WSAGetLastError ();
  return 0;
#endif
#ifdef __linux__
  /* Linux is gracious, and lets you simply retry the system call,
   * even though it's finishing the connect behind the scenes
   */
  while (connect (fd, address, address_len))
    {
      if (errno == EINTR)
        continue;
      return errno;
    }
  return 0;
#endif
#ifdef __APPLE__
  /* Mac OS X gets upset if you retry an interrupted connect;
   * it wants you to wait for the result using poll().
   */
  if (connect (fd, address, address_len))
    {
      struct pollfd waiter;
      int socket_error;
      socklen_t sizeof_socket_error;

      if (errno != EINTR)
        {
          /* Early failure, e.g. remote refused connection, uninterrupted. */
          return errno;
        }

      /* Our view of the connect was interrupted by a signal.
       * Using the Linux approach above results in error EISCONN.
       * We assume that the O/S is continuing the connection behind the scenes.
       * Wait for it to finish.  This should really be in the app's
       * event loop, but g-speak's I/O doesn't have a nonblocking
       * interface, so we have to block here.
       */
      waiter.fd = fd;
      waiter.events = POLLOUT;
      while (poll (&waiter, 1, -1) == -1)
        {
          if (errno != EINTR)
            return errno; /* can't poll? */
        }
      /* Connect done, let's get result */
      sizeof_socket_error = sizeof (socket_error);
      if (getsockopt (fd, SOL_SOCKET, SO_ERROR, &socket_error,
                      &sizeof_socket_error)
          == -1)
        return errno; /* can't get result? */
      if (socket_error != 0)
        {
          /* Real network error, e.g. remote refused connection */
          return socket_error;
        }
      /* Late success! Fall through to early success case. */
    }
  return 0;
#endif
}
