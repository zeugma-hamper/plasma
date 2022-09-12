
/* (c)  oblong industries */

#ifndef OBLONG_POOLSERVER_H
#define OBLONG_POOLSERVER_H

#include <libLoam/c++/Str.h>
// #include <libLoam/c++/ObUniqueTrove.h>

#include <ostream>

namespace oblong {
namespace plasma {

using namespace loam;

/**
 * Thin abstraction over pool servers.  This class provides an
 * structured alternative to strings for referring to remote pool
 * servers.  A remote server has an address composed of a protocol
 * (which currently is always TCP), a hostname and a port.  Servers
 * also have a name and an (optional) type.
 *
 * This class is especially useful in conjunction with the @ref
 * ZeroconfDiscoveryAPI.  For servers identified via that protocol, the
 * name and type fields correspond to the Zeroconf service name and
 * service @e subtype.
 *
 * In addition, we provide some utlity methods to construct server
 * addresses and fully qualified pool names.
 *
 * PoolServer instances have value semantics and are immutable.
 *
 * @ingroup PlasmaZeroconf
 */
class OB_PLASMAXX_API PoolServer
{
 public:
  // PoolServer (const PoolServer &other);
  bool operator== (const PoolServer &other) const;
  bool operator!= (const PoolServer &other) const;

  /**
   * @name Constructors
   */
  //@{
  /**
   * A default server, without name or type, listening at localhost
   * and pool_tcp_server's default port (65455).
   */
  PoolServer ();
  /**
   * Explicit constructor from the server fields. A server cannot have
   * a NULL host: if @a host is NULL or an empty, this object's Host()
   * will be "localhost".  NULL @a name or @a type are translated to
   * the empty string. No attempt is made to check the given host and
   * port for availability. Since currently only TCP is accepted as a
   * protocol, this constructor has no parameter specifying it.
   */
  PoolServer (const char *host, unt16 port, const char *name, const char *type);
  //@}

  /**
   * @name Accessors
   * Only accessors to the data fields are provided, since this
   * class' instances are immutable.
   */
  //@{
  /**
   * This is the transport protocol used to communicate with this
   * server. Currently, this function always returns "tcp".
   */
  const Str &Protocol () const;
  /**
   * The host the server lives in.
   */
  const Str &Host () const;
  /**
   * The port the server is listening to.
   */
  unt16 Port () const;

  /**
   * When servers are registered as zeroconf services, they'll have a
   * name. This name can be only empty if you created this server
   * instance yourself: zeroconf service names are unique in the
   * network.
   */
  const Str &Name () const;
  /**
   * When servers are registered as zeroconf services, they can have
   * also a zeroconf subtype.
   */
  const Str &Type () const;
  //@}

  /**
   * Address used in Pool::Pools(const Str &server_address) and
   * composed by combining Host() and Port().
   */
  const Str &Address () const;

  /**
   * Utility function, to make easy using Pool::Participate() &co.
   */
  Str MakePoolUri (const Str &pool_name) const;

  // Connects to the server and retrieves a list of existing pools.
  // Every call to this function implies a connection to the server;
  // hence, the result is always up-to-date.
  // ObUniqueTrove<Str> Pools () const;

 private:
  Str host_;
  int port_;
  Str name_;
  Str type_;
  Str address_;
};

/**
 * Ostreaming for debugging purposes. Please, do not rely on this
 * string representation format.
 */
OB_PLASMAXX_API::std::ostream &operator<< (::std::ostream &os,
                                           const PoolServer &s);
}
}  // namespace oblong::plasma


#endif  // OBLONG_POOLSERVER_H
