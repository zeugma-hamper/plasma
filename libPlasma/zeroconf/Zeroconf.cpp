
/* (c)  oblong industries */

#include "Zeroconf.h"
#include "zeroconf-services.h"

#include <libLoam/c++/Mutex.h>

#include <vector>
#include <algorithm>
#include <iostream>

namespace oblong {
namespace plasma {

typedef ::std::vector<ZeroconfHandler *> Handlers;
typedef ::std::vector<PoolServer> Servers;

typedef Servers::iterator SIt;

namespace {

Servers servers_;
Handlers handlers_;
loam::Mutex mutex_ (false);
bool running_ (false);

#define LOCK loam::MutexLock lock_ (mutex_)

void del_hdlr (ZeroconfHandler *h)
{
  delete h;
}

void clear_handlers (Handlers &hs)
{
  ::std::for_each (hs.begin (), hs.end (), del_hdlr);
  hs.clear ();
}

ZeroconfHandler *remove_handler (Handlers &hs, ZeroconfHandler *h)
{
  Handlers::iterator pos (::std::find (hs.begin (), hs.end (), h));
  if (pos != hs.end ())
    {
      ZeroconfHandler *ret = *pos;
      hs.erase (pos);
      return ret;
    }
  return NULL;
}

void apply_add_handlers (const Handlers &hs, const PoolServer &s)
{
  for (unt64 i = 0; i < hs.size (); ++i)
    (*hs[i]).Add (s);
}

void apply_remove_handlers (const Handlers &hs, const PoolServer &s)
{
  for (unt64 i = 0; i < hs.size (); ++i)
    (*hs[i]).Remove (s);
}

SIt find_server (const char *name, const char *type)
{
  for (SIt i (servers_.begin ()), e (servers_.end ()); i != e; ++i)
    if (!strcmp (i->Name (), name) && (!type || !strcmp (i->Type (), type)))
      return i;
  return servers_.end ();
}

bool know_about (const char *name, const char *type)
{
  return find_server (name, type) != servers_.end ();
}

void on_add (const char *name, const char *subtype, const char *host, int port)
{
  LOCK;
  if (!running_)
    return;
  if (subtype && strlen (subtype) == 0)
    subtype = NULL;
  if (!know_about (name, subtype))
    {
      PoolServer s (host, port, name, subtype);
      servers_.push_back (s);
      apply_add_handlers (handlers_, s);
    }
}

void on_remove (const char *name, const char *subtype)
{
  LOCK;
  if (!running_)
    return;
  SIt i;
  while ((i = find_server (name, subtype)) != servers_.end ())
    {
      const PoolServer s (*i);
      servers_.erase (i);
      apply_remove_handlers (handlers_, s);
    }
}

}  // internal namespace

bool ZeroconfStart ()
{
  running_ = zeroconf_browser_start (on_add, on_remove, NULL);
  return running_;
}

void ZeroconfStop ()
{
  {
    LOCK;
    clear_handlers (handlers_);
    servers_.clear ();
    running_ = false;
  }
  zeroconf_browser_stop ();
}

PoolServers ZeroconfServers (const char *type)
{
  LOCK;
  PoolServers result;
  for (SIt i (servers_.begin ()), e (servers_.end ()); i != e; ++i)
    {
      if (!type || (i->Type () && !strcmp (i->Type (), type)))
        result.Append (*i);
    }
  return result;
}

void RegisterServerHandler (ZeroconfHandler *h)
{
  LOCK;
  if (h)
    handlers_.push_back (h);
}

ZeroconfHandler *UnregisterServerHandler (ZeroconfHandler *h)
{
  LOCK;
  return remove_handler (handlers_, h);
}
}
}  // namespace oblong::plasma
