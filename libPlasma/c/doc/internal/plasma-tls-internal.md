\internal

This file contains random implementation notes about Plasma's support
for TLS.  For the end-user documentation, see \ref PlasmaTLS (plasma-tls.md)

# FASCINATING AND EXCITING URLS

- https://en.wikipedia.org/wiki/Transport_Layer_Security
- https://en.wikipedia.org/wiki/Comparison_of_TLS_Implementations
- https://openssl.org/
- http://wiki.openssl.org/

# BUILDING

To build yovo without TLS support, specify --without-openssl to
configure.  Otherwise, yovo will be built with TLS support on Linux
and Mac OS X, if the presence of OpenSSL headers and libraries is
detected.  OpenSSL versions 0.9.7 and up are supported, although for
security reasons, using the most recent version is strongly
recommended.

When OpenSSL is being used, the files in libPlasma/c/ossl are linked
into libPlasma.  When OpenSSL is not being used, the single file
libPlasma/c/nossl.c is used in place of those files.  Both of these
alternatives expose the same API to the rest of libPlasma.  This
(internal) API is defined in \ref libPlasma/c/private/pool-tls.h.

# IMPLEMENTATION NOTES

[Bug 3347](https://bugs.oblong.com/show_bug.cgi?id=3347) lists the original
requirements that Kwin gave to me when I implemented TLS for Plasma.

TLS (formerly known as SSL) is a complex and tricky enough protocol
that we want to use a library to implement it.  In the case of
libPlasma/c, we settled on OpenSSL.  It has terrible documentation,
but is the most ubiquitous library, and has favorable licensing.
(Versus GnuTLS, which has wonderful documentation, but we had concerns
that the LGPL might be incompatible with the iPhone App Store.)
OpenSSL also has integration with libevent, which was another deciding
factor.

Although it is quite old, the O'Reilly book "Network Security with
OpenSSL" is helpful in overcoming OpenSSL's poor documentation:

http://shop.oreilly.com/product/9780596002701.do

(Some of the files in libPlasma/c/ossl are derived from the sample
code from the O'Reilly book.  Such files are noted with an appropriate
copyright notice.  IANAL, but my understanding is that O'Reilly's
license allows us to use their code in this way.)

Using TLS with the pool TCP protocol is done using a STARTTLS style of
interaction:

https://en.wikipedia.org/wiki/STARTTLS

where we first open an unencrypted connection to the pool server,
connect as normally, and then issue a "STARTTLS" command, after which
we hand over the TCP socket to the TLS library (which is OpenSSL in
the case of libPlasma/c), which then does its TLS handshake thing, and
then once it has established a secure tunnel for us, we initiate the
pool protocol all over again, this time on top of the TLS connection.

As the Wikipedia article mentions, this is done so that we can have a
single server port which accepts both encrypted and unencrypted
connections, rather than having two different ports, as HTTP/HTTPS
does.

Although using a single port is what Kwin specified in his requirements
document, there has recently been some debate over the use of STARTTLS
in [bug 10034](https://bugs.oblong.com/show_bug.cgi?id=10034) and
[bug 10829](https://bugs.oblong.com/show_bug.cgi?id=10829).

# SESSION RESUMPTION

We currently don't support session resumption.  It would be a little
tricky, since we use a forking server, so the server instances would
have to share a database of some sort, rather than keeping the session
information in memory.  Perhaps we could use SQLite for this purpose.
Or, we might be able to use a pool to keep the session information,
although pools make a poor database, since search is linear, and there
is no deletion other than through wrapping.

Session tickets, which are supported by recent OpenSSL versions, would
avoid this problem, since they keep all the session information on the
client side, rather than the server.  We would still need to add
client support, though.  And I don't think any of the Java TLS
libraries support session tickets, so if we want to support session
resumption with the Jelly client, we'd still have to do it the
old-fashioned way, on the server.

# CURRENT APPROACH

In an effort which is hackish but minimally invasive to existing code,
the current approach for using OpenSSL is to create a new thread for
each connection, which communicates using TLS on the network socket,
and then communicates unencrypted on a pipe.  This pipe is then used
by the existing pool TCP code in place of the original network socket.

# FUTURE DIRECTIONS

Eventually, we would like to use libevent to do networking operations
in libPlasma.  Besides a number of other advantages, it would allow us
to use libevent's integration with OpenSSL:

http://www.wangafu.net/~nickm/libevent-book/Ref6a_advanced_bufferevents.html#_bufferevents_and_ssl

OTHER RESOURCES

A nice paper about how to do certificate validation properly:

https://github.com/iSECPartners/ssl-conservatory/blob/master/openssl/everything-you-wanted-to-know-about-openssl.pdf?raw=true

A local copy of that paper:

http://obdumper/development/papers/everything-you-wanted-to-know-about-openssl.pdf

A survey of other TLS libraries we could use in place of OpenSSL:

https://gitlab.oblong.com/platform/docs/-/wikis/Alternatives-to-OpenSSL

\endinternal
