
/* (c)  oblong industries */

#include <libLoam/c++/LoamxxRetorts.h>
#include <libLoam/c/ob-attrs.h>

#include <stddef.h>


// You can run the script libLoam/c/check-retorts.rb to make
// sure these match the retorts defined in LoamxxRetorts.h

static const char *loamxx_error_string (ob_retort err)
{
#define E(x)                                                                   \
  case x:                                                                      \
    return #x
  switch (err)
    {
      E (OB_ABSTRACT_OK);
      E (OB_ARGUMENT_PARSING_FAILED);
      default:
        return NULL;
    }
#undef E
}

static ob_retort dummy_loamxx OB_UNUSED =
  ob_add_error_names (loamxx_error_string);

int ob_private_hack_pull_in_loamxx_retorts;
