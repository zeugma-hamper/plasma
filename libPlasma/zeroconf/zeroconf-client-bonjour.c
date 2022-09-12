
/* (c)  oblong industries */

#include "zeroconf-services.h"

#include "libLoam/c/ob-pthread.h"
#include "libLoam/c/ob-sys.h"

#include <dns_sd.h>

#include <string.h>
#include <stdlib.h>

typedef struct browser_data
{
  DNSServiceRef service;
  add_callback on_add;
  del_callback on_del;
  flushed_callback on_flush;
  volatile bool stopped;
  volatile int32 unresolved;
  bool flushed;
} browser_data;

typedef struct resolver_data
{
  DNSServiceRef service;
  char *name;
  browser_data *browser;
} resolver_data;

static browser_data *new_browser_data (add_callback a, del_callback d,
                                       flushed_callback f)
{
  browser_data *bd = (browser_data *) malloc (sizeof (browser_data));
  bd->on_add = a;
  bd->on_del = d;
  bd->on_flush = f;
  bd->stopped = false;
  bd->flushed = false;
  bd->unresolved = 0;
  return bd;
}

static void free_browser_data (browser_data *bd)
{
  DNSServiceRefDeallocate (bd->service);
  free (bd);
}

static resolver_data *new_resolver_data (DNSServiceRef s, const char *name,
                                         browser_data *bd)
{
  resolver_data *rd = (resolver_data *) malloc (sizeof (resolver_data));
  rd->service = s;
  rd->browser = bd;
  rd->name = strdup (name ? name : "");
  return rd;
}

static void free_resolver_data (resolver_data *rd)
{
  DNSServiceRefDeallocate (rd->service);
  if (rd->name)
    free (rd->name);
  free (rd);
}

static void *browser_main (void *data)
{
  browser_data *bd = (browser_data *) data;
  while (!bd->stopped)
    {
      DNSServiceErrorType e = DNSServiceProcessResult (bd->service);
      if (e)
        {
          ZEROCONF_LOG_ERROR ("Browser: DNSServiceProcessResult: %d", e);
          break;
        }
    }
  if (bd->on_flush)
    bd->on_flush ();
  free_browser_data (bd);
  return NULL;
}

static void *resolver_main (void *data)
{
  resolver_data *rd = (resolver_data *) data;
  if (!rd->browser->stopped)
    {
      DNSServiceErrorType e = DNSServiceProcessResult (rd->service);
      if (e)
        ZEROCONF_LOG_ERROR ("Resolver: DNSServiceProcessResult: %d", e);
    }
  free_resolver_data (rd);
  return NULL;
}

static bool launch_server_thread (void *(*fun) (void *), void *data)
{
  pthread_t thread;
  return pthread_create (&thread, NULL, fun, data) == 0
         && pthread_detach (thread) == 0;
}

static void WINAPI resolve_callback (DNSServiceRef serviceRef,
                                     DNSServiceFlags flags, uint32_t iface,
                                     DNSServiceErrorType errorCode,
                                     const char *fullname, const char *host,
                                     uint16_t port, uint16_t txtLen,
                                     const unsigned char *txt, void *context)
{
  resolver_data *rd = (resolver_data *) context;
  browser_data *bd = rd->browser;
  if (!bd || !bd->on_add)
    {
      ZEROCONF_LOG_ERROR ("Null service data for %s", fullname);
      return;
    }
  if (bd->stopped)
    return;

  if (errorCode != kDNSServiceErr_NoError)
    ZEROCONF_LOG_DEBUG ("resolve_callback returned %d\n", errorCode);
  else
    {
      ZEROCONF_LOG_DEBUG ("RESOLVE: %s is at %s:%d\n", rd->name, host,
                          ntohs (port));
      if (bd->on_add)
        {
          uint16_t c = TXTRecordGetCount (txtLen, txt);
          uint16_t p = ntohs (port);
          char subtype[256];
          uint8_t len;
          const void *val;
          uint16_t i;
          for (i = 0; i < c; i++)
            {
              if (kDNSServiceErr_NoError
                  == TXTRecordGetItemAtIndex (txtLen, txt, i, 256, subtype,
                                              &len, &val))
                {
                  if (0 == len)
                    bd->on_add (rd->name, subtype, host, p);
                }
            }
        }
    }

  if (bd->unresolved-- < 2 && bd->flushed && bd->on_flush)
    {
      bd->on_flush ();
      bd->on_flush = NULL;
    }
}

static void WINAPI browse_callback (DNSServiceRef service,
                                    DNSServiceFlags flags,
                                    uint32_t interfaceIndex,
                                    DNSServiceErrorType errorCode,
                                    const char *name, const char *type,
                                    const char *domain, void *context)
{
  if (errorCode != kDNSServiceErr_NoError)
    {
      ZEROCONF_LOG_DEBUG ("browse_callback received %d\n", errorCode);
      return;
    }
  browser_data *bd = (browser_data *) context;
  if (!(flags & kDNSServiceFlagsAdd))
    {
      ZEROCONF_LOG_DEBUG ("REMOVE: %s.%s%s", name, type, domain);
      if (bd->on_del)
        bd->on_del (name, NULL);
    }
  else
    {
      ZEROCONF_LOG_DEBUG ("ADD: %s.%s%s", name, type, domain);
      resolver_data *rd = new_resolver_data (service, name, bd);
      DNSServiceErrorType e =
        DNSServiceResolve (&rd->service, 0, kDNSServiceInterfaceIndexAny, name,
                           type, domain, resolve_callback, rd);
      if (e == kDNSServiceErr_NoError)
        {
          bd->unresolved++;
          launch_server_thread (resolver_main, rd);
        }
      bd->flushed = !(flags & kDNSServiceFlagsMoreComing);
    }
}

static browser_data *active_data_ = NULL;

bool zeroconf_browser_start (add_callback a, del_callback d, flushed_callback f)
{
  zeroconf_browser_stop ();
  browser_data *data = new_browser_data (a, d, f);
  DNSServiceErrorType error =
    DNSServiceBrowse (&data->service,
                      0,  // no flags
                      kDNSServiceInterfaceIndexAny, ZEROCONF_SERVICE_TYPE, "",
                      browse_callback, data);

  if (error == kDNSServiceErr_NoError)
    {
      active_data_ = data;
      return launch_server_thread (browser_main, data);
    }
  else
    {
      ZEROCONF_LOG_ERROR ("DNSServiceBrowse error: %d", error);
      free (data);
    }

  return (error == kDNSServiceErr_NoError);
}

void zeroconf_browser_stop (void)
{
  if (active_data_)
    {
      active_data_->stopped = true;
      active_data_ = NULL;
    }
}
