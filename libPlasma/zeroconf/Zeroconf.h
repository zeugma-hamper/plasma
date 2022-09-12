
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_ZEROCONF_H
#define OBLONG_PLASMA_ZEROCONF_H

#include "PoolServer.h"

#include <libLoam/c++/ObUniqueTrove.h>

namespace oblong {
namespace plasma {

/**
 * @defgroup PlasmaxxPoolServerBrowser Zeroconf server browsing
 * Functions to browse zeroconf-announced servers and to react to new
 * announcements. To start the zeroconf browsing funcionality, you must
 * invoke ZeroconfStart(). If it returns \c true, all is well and good,
 * your program will be listening to zeroconf events, and the rest of
 * the API should work as publicised.
 *
 * Once the zeroconf service is initialised you can either obtain a
 * list of known pool servers (using ZeroconfServers()) or, more
 * idiomatically, register callabacks to be notified when a server
 * enters or leaves the network, using RegisterSeverHandler().
 *
 * If you're done with the zeroconf service but your program should
 * keep running, it's a good idea to call ZeroconfStop().
 *
 * @ingroup PlasmaZeroconf
 */

/**
 * Start zeroconf services. This function must be called before any
 * other zeroconf browsing service. It returns a success indicator.
 * Reasons for unsuccessful initialisation include non-availability of
 * zeroconf systems services in the node where your program is running
 * (e.g., no avahi or bonjour deamon running), or the fact that your
 * program was compiled with a version of plasma not supporting
 * zeroconf. It is safe to call this function repeatedly, even without
 * intervening calls to ZeroconfStop().
 *
 * @ingroup PlasmaxxPoolServerBrowser.
 */
OB_PLASMAXX_API bool ZeroconfStart ();

/**
 * Stops the zeroconf browsing services. After invoking this function,
 * no more zeroconf events will be received; i.e., your callbacks will
 * not only not be called, but actually forgotten. A single call to
 * this function suffices to shutdown the service, no matter how many
 * times you called ZeroconfStart() before.
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
OB_PLASMAXX_API void ZeroconfStop ();

/**
 * A type representing a list of servers.
 *
 * @see ZeroconfServers
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
typedef ObUniqueTrove<PoolServer> PoolServers;

/**
 * Returns a list of all known servers. The servers will be filtered
 * according to @a type, with @c NULL meaning "any type".
 *
 * If you call this function immediately after ZeroconfStart(), the
 * returned list will most probably be empty, because the zeroconf
 * client must wait until the information cached by the system daemon
 * is retrieved.
 *
 * The idiomatic way to use the zeroconf services is @e not to use this
 * function, but regiter ZeroconfHandler callbacks to react to zerconf
 * events.
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
OB_PLASMAXX_API PoolServers ZeroconfServers (const char *type = NULL);

/**
 * Interface for all the zeroconf event handlers.
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
class OB_PLASMAXX_API ZeroconfHandler
{
 public:
  virtual void Add (OB_UNUSED const PoolServer &server) {}
  virtual void Remove (OB_UNUSED const PoolServer &server) {}
  virtual ~ZeroconfHandler () {}
};

/**
 * Convenience implementation of ZeroconfHandler encapsulating any
 * callable object. This is particularly useful for free functions, in
 * which @a F has the type of a function taking a const PoolServer by
 * value or reference. Typically, you will use the convenience function
 * MakeZeroconfHandler to let the compiler deduce the type F for you.
 *
 * @sa MakeZeroconfHandler
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
template <typename F>
class ZeroconfHandlerAdapter : public ZeroconfHandler
{
 public:
  ZeroconfHandlerAdapter (F f) : f_ (f) {}
  void operator() (const PoolServer &server) { f_ (server); }
 private:
  F f_;
};

/**
 * Convenience factory method for ZeroconfHandlerAdapter, to avoid
 * having to specify the template type. Examples:
 *
 * @code
 *
 *   void on_remove (const PoolServer &s);
 *   RegisterServerHandler (MakeZeroconfHandler (on_remove));
 *
 *   int on_add (PoolServer s);
 *   RegisterServerHandler (MakeZeroconfHandler (on_add));
 *
 * @endcode
 *
 * @sa ZeroconfHandlerAdapter
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
template <typename F>
ZeroconfHandlerAdapter<F> *MakeZeroconfHandler (F f)
{
  return new ZeroconfHandlerAdapter<F> (f);
}

/**
 * Adds a new handler for "zeroconf events. Whenever a new
 * pool server announces itself or announces its presence or disappearance,
 * @a hdl's Add or Remove will be called with two
 * arguments, namely, a pool_server instance and the @a arg value used
 * to register this callback. Note that there will be a @a hdl
 * invokation for each subtype announced by a given pool server, as
 * well as a "generic" one with no type only in case the server has
 * none.
 *
 * It is safe to call this function before ZeroconfStart(): all
 * registered callbacks will be remembered unless you call
 * ZeroconfStop().
 *
 * By calling this function, you lose ownership of @a hdl, whose memory
 * will be managed by the library.
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
OB_PLASMAXX_API void RegisterServerHandler (ZeroconfHandler *hdl);


/**
 * Unregisters callbacks registered via
 * RegisterServerHandler(). Pointer equality is used to locate the
 * first appearance of @a hdl in the list of registered callbacks for
 * the "new server" event. The extra closure argument used during
 * registration is not considered, and handlers are removed in FIFO
 * fashion, one at a time.
 *
 * Returns @a hdl if it was found (and removed), @c NULL otherwise.
 *
 * By calling this function, you recover ownership of @a hdl, whose
 * memory should be managed by you from now on.
 *
 * @ingroup PlasmaxxPoolServerBrowser
 */
OB_PLASMAXX_API ZeroconfHandler *UnregisterServerHandler (ZeroconfHandler *hdl);
}
}  // namespace oblong::plasma


#endif  // OBLONG_PLASMA_ZEROCONF_H
