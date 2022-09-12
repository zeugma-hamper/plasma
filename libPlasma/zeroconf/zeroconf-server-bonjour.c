
/* (c)  oblong industries */

#include "zeroconf-services.h"
#include "libLoam/c/ob-pthread.h"
#include "libLoam/c/ob-sys.h"

#include <dns_sd.h>

#include <stdio.h>
#include <string.h>

struct zc_server_data
{
  DNSServiceRef service_ref;
  pthread_t thread;
  reg_callback on_reg;
  void *reg_data;
};

static void *handle_events (void *r)
{
  DNSServiceRef ref = (DNSServiceRef) r;
  while (DNSServiceProcessResult (ref) == kDNSServiceErr_NoError)
    continue;
  return NULL;
}

static void WINAPI register_callback (DNSServiceRef service,
                                      DNSServiceFlags flags,
                                      DNSServiceErrorType err, const char *name,
                                      const char *type, const char *domain,
                                      void *data)
{
  if (err != kDNSServiceErr_NoError)
    ZEROCONF_LOG_ERROR ("register_callback returned %d", err);
  else
    {
      ZEROCONF_LOG_INFO ("%-5s %s.%s%s", "REGISTER", name, type, domain);
      zc_server_data *sd = (zc_server_data *) data;
      if (sd->on_reg)
        {
          sd->on_reg (sd->reg_data);
          sd->on_reg = NULL;
        }
    }
}

// this horrible function replace a string for another one
static char *replace_str (const char *inputstr, const char *oldstr,
                          const char *newstr)
{
  char *outputstr;
  int i = 0;
  int count = 0;
  int newlen;
  newlen = strlen (newstr);
  int oldlen;
  oldlen = strlen (oldstr);
  for (i = 0; inputstr[i] != '\0'; i++)
    {
      if (strstr (&inputstr[i], oldstr) == &inputstr[i])
        {
          count++;
          i += oldlen - 1;
        }
    }
  outputstr = (char *) malloc (i + count * (newlen - oldlen));
  i = 0;
  while (*inputstr)
    {
      if (strstr (inputstr, oldstr) == inputstr)
        {
          strcpy (&outputstr[i], newstr);
          i += newlen;
          inputstr += oldlen;
        }
      else
        outputstr[i++] = *inputstr++;
    }
  outputstr[i] = '\0';
  return outputstr;
}

static char *format_type (const char *type, const char *subtypes)
{
  if (!subtypes || strlen (subtypes) == 0)
    return strdup (type);
  char *input = (char *) subtypes;
  // 'jump' the first underscore of the first subtype
  if (subtypes[0] == '_')
    input++;
  // remove first occurrence of underscores for each subtype after the first
  char *no_und = replace_str (input, ",_", ",");
  // add underscores after each comma
  char *und_subtypes = replace_str (no_und, ",", ",_");
  int len = strlen (type) + strlen (und_subtypes) + 3;
  char *ftype = (char *) malloc (len);
  // here the underscore is prepended to the first subtype
  snprintf (ftype, len, "%s,_%s", type, und_subtypes);
  return ftype;
}

static TXTRecordRef parse_subtypes (const char *subtypes)
{
  TXTRecordRef result;
  TXTRecordCreate (&result, 0, NULL);
  if (subtypes && strlen (subtypes) > 0)
    {
      char *str = strdup (subtypes);
      while (true)
        {
          char *sub = strtok (str, ", ");
          if (!sub)
            break;
          TXTRecordSetValue (&result, sub, 0, NULL);
          str = NULL;
        }
      free (str);
    }
  return result;
}

static zc_server_data *make_server_data (reg_callback rc, void *rd)
{
  zc_server_data *sd = (zc_server_data *) malloc (sizeof (zc_server_data));
  sd->on_reg = rc;
  sd->reg_data = rd;
  return sd;
}

void zeroconf_server_shutdown (zc_server_data *data)
{
  if (data)
    {
      DNSServiceRefDeallocate (data->service_ref);
      pthread_join (data->thread, NULL);
      free (data);
    }
}

///////////////// TESTS //////////////////////
void DO_TEST (const char *subtypes)
{
  ZEROCONF_LOG_INFO ("TEST:  'type' + '%s' becomes '%s'", subtypes,
                     format_type ("type", subtypes));
}
//////////////// END OF TESTS ////////////////

zc_server_data *zeroconf_server_announce (const char *name, const char *type,
                                          const char *subtypes, int port,
                                          reg_callback on_reg, void *reg_data)
{
  if (!type)
    {
      ZEROCONF_LOG_ERROR ("Null service type for '%s'", name);
      return NULL;
    }

  zc_server_data *data = make_server_data (on_reg, reg_data);

  DO_TEST ("");
  DO_TEST ("remote");
  DO_TEST ("_remote");
  DO_TEST ("__remote");
  DO_TEST ("remote,mz");
  DO_TEST ("_remote,mz");
  DO_TEST ("remote,_mz");
  DO_TEST ("_remote,_mz");
  DO_TEST ("one,two,three,four");
  DO_TEST ("one,_two,three,_four");
  DO_TEST ("_one,two,three,_four");

  char *ftype = format_type (type, subtypes);
  ZEROCONF_LOG_INFO ("Announcing '%s' with type %s", name, ftype);

  TXTRecordRef txt = parse_subtypes (subtypes);
  DNSServiceErrorType error =
    DNSServiceRegister (&data->service_ref,
                        0,  // no flags
                        0,  // all network interfaces
                        name, ftype,
                        "",    // register in default domain(s)
                        NULL,  // use default host name
                        htons (port), TXTRecordGetLength (&txt),
                        TXTRecordGetBytesPtr (&txt), register_callback, data);
  TXTRecordDeallocate (&txt);
  if (ftype)
    free (ftype);

  if (error != kDNSServiceErr_NoError)
    {
      ZEROCONF_LOG_ERROR ("DNSServiceRegister returned %d", error);
      free (data);
      return NULL;
    }

  if (pthread_create (&data->thread, NULL, handle_events, data->service_ref))
    {
      ZEROCONF_LOG_ERROR ("Couldn't start thread %s", "");
      free (data);
      return NULL;
    }

  return data;
}
