
/* (c)  oblong industries */

#ifndef OBLONG_ZEROCONF_SERVICES_H
#define OBLONG_ZEROCONF_SERVICES_H

#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-log.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZEROCONF_SERVICE_TYPE "_pool-server._tcp"
#define ZEROCONF_DEF_SERVICE_NAME "TCP Pool Server"

#define ZEROCONF_MAX_SERVICE_LEN 63
#define ZEROCONF_MAX_SUBTYPE_LEN 14

#define ZEROCONF_LOG_CODE 0x2010e000

#define ZEROCONF_LOG(LOGGER, FMT, ...)                                         \
  LOGGER (ZEROCONF_LOG_CODE + __LINE__, "Zeroconf: " __FILE__ ":%d " FMT,      \
          __LINE__, __VA_ARGS__)

#define ZEROCONF_LOG_INFO(FMT, ...)                                            \
  ZEROCONF_LOG (OB_LOG_INFO_CODE, FMT, __VA_ARGS__)

#define ZEROCONF_LOG_WARNING(FMT, ...)                                         \
  ZEROCONF_LOG (OB_LOG_WARNING_CODE, FMT, __VA_ARGS__)

#define ZEROCONF_LOG_DEBUG(FMT, ...)                                           \
  ZEROCONF_LOG (OB_LOG_DEBUG_CODE, FMT, __VA_ARGS__)

#define ZEROCONF_LOG_ERROR(FMT, ...)                                           \
  ZEROCONF_LOG (OB_LOG_ERROR_CODE, FMT, __VA_ARGS__)

#define zeroconf_check_subtype(SUB)                                            \
  ((SUB) == NULL || strlen (SUB) <= ZEROCONF_MAX_SUBTYPE_LEN)


// Server side
typedef struct zc_server_data zc_server_data;
typedef void (*reg_callback) (void *);

OB_PLASMA_API zc_server_data *
zeroconf_server_announce (const char *name, const char *type,
                          const char *subtypes, int port, reg_callback on_reg,
                          void *reg_data);


OB_PLASMA_API void zeroconf_server_shutdown (zc_server_data *data);


// Client side
typedef void (*add_callback) (const char *name, const char *subtype,
                              const char *host, int port);

typedef void (*del_callback) (const char *name, const char *subtype);
typedef void (*flushed_callback) (void);

OB_PLASMA_API bool zeroconf_browser_start (add_callback on_add,
                                           del_callback on_del,
                                           flushed_callback on_end);

OB_PLASMA_API void zeroconf_browser_stop (void);


// Utilities
OB_PLASMA_API void zeroconf_enable_debug_messages (void);

#ifdef __cplusplus
}
#endif

#endif  // OBLONG_ZEROCONF_SERVICES_H
