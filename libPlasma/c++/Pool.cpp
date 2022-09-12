
/* (c)  oblong industries */

#include <libPlasma/c/pool_options.h>

#include "Pool.h"
#include "Hose.h"


namespace oblong {
namespace plasma {


/* this trove is used by Create (), Participate(),
 * and DisposeOfAutoDisposables () */
ObTrove<Str> auto_disposable_pools (2.0);

const PoolType Pool::MMAP ("mmap");

const unt64 Pool::SIZE_SMALL = POOL_SIZE_SMALL;
const unt64 Pool::SIZE_MEDIUM = POOL_SIZE_MEDIUM;
const unt64 Pool::SIZE_LARGE = POOL_SIZE_LARGE;
const unt64 Pool::SIZE_HUGE = POOL_SIZE_HUGE;

// these are historically the 4 predefined "configurations"
const Pool::Configuration Pool::MMAP_SMALL =
  Pool::Configuration::Historical (SIZE_SMALL);

const Pool::Configuration Pool::MMAP_MEDIUM =
  Pool::Configuration::Historical (SIZE_MEDIUM);

const Pool::Configuration Pool::MMAP_LARGE =
  Pool::Configuration::Historical (SIZE_LARGE);

const Pool::Configuration Pool::MMAP_HUGE =
  Pool::Configuration::Historical (SIZE_HUGE);


ObRetort Pool::Create (const char *name, PoolType type, bool auto_dispose,
                       Protein options)
{
  if (auto_dispose)
    auto_disposable_pools.Append (name);
  return pool_create (name, type, options.ProteinValue ());
}

ObRetort Pool::Create (const char *name, PoolType type, bool auto_dispose,
                       Slaw options)
{
  if (auto_dispose)
    auto_disposable_pools.Append (name);
  return pool_create (name, type, options.SlawValue ());
}

Slaw Pool::OptionsSlawFromConfiguration (Configuration conf)
{
  Protein p (toc_mmap_pool_options (conf.size, conf.toc_capacity));
  return p.Ingests ();
}


ObRetort Pool::Create (const char *name, Configuration conf, bool auto_dispose)
{
  return Create (name, MMAP, auto_dispose, OptionsSlawFromConfiguration (conf));
}


ObRetort Pool::Dispose (const char *name)
{
  return pool_dispose (name);
}


void Pool::DisposeOfAutoDisposables ()
{
  ObCrawl<Str> cr = auto_disposable_pools.Crawl ();
  while (!cr.isempty ())
    {
      Str pname = cr.popfore ();
      ObRetort tort = Dispose (pname);
      if (tort.IsError ())
        OB_LOG_INFO_CODE (0x21010000, "Couldn't dispose of '%s' because '%s'\n",
                          pname.utf8 (), tort.Description ().utf8 ());
    }
}


Hose *Pool::Participate (const char *pool_name, ObRetort *ret)
{
  Hose *h = new Hose (pool_name);
  if (ret)
    *ret = h->LastRetort ();

  if (!h->IsConfigured ())
    {
      h->Delete ();
      h = NULL;
    }
  return h;
}


Hose *Pool::Participate (const char *name, Configuration conf, ObRetort *ret,
                         bool old_crufty_auto_dispose)
{
  Slaw optionsSlaw = OptionsSlawFromConfiguration (conf);
  optionsSlaw = optionsSlaw.MapPut ("resizable", true);

  pool_hose hose;
  bool attemptCreation = (conf.cpolicy == Create_Auto_Disposable
                          || conf.cpolicy == Create_Persistent);
  ob_retort pret;
  if (attemptCreation)
    pret = pool_participate_creatingly (name, "mmap", &hose,
                                        optionsSlaw.SlawValue ());
  else
    pret = pool_participate (name, &hose, optionsSlaw.SlawValue ());

  if (ret != NULL)
    *ret = pret;

  // an assumption
  if (pret == OB_OK)
    OB_LOG_DEBUG_CODE (0x21010001, "Joining existing pool %s.\n", name);
  else if (attemptCreation && pret == POOL_CREATED)
    {
      if (conf.cpolicy == Create_Auto_Disposable)
        {
          // mark as "auto-release"
          Slaw releaseOption = Slaw::Map ("auto-dispose", true);
          pret = pool_change_options (hose, releaseOption.SlawValue ());
          // do not update ret here, because all-in-all, participation succeeded
          if (pret != OB_OK)
            {
              OB_LOG_WARNING_CODE (0x21010002,
                                   "Cannot set pool %s to auto-release (%s)."
                                   " Continuing.\n",
                                   name, ob_error_string (pret));
            }
        }
      if (old_crufty_auto_dispose)
        {
          auto_disposable_pools.Append (name);
          OB_LOG_DEPRECATION_CODE (0x21010006,
                                   "The old, crufty auto-dispose option is\n"
                                   "deprecated, and you should instead use\n"
                                   "the new, shiny auto-dispose option.  See\n"
                                   "bug 4020 and bug 2827 for more info.\n");
        }
      OB_LOG_DEBUG_CODE (0x21010003, "New pool %s created.\n", name);
    }
  else
    {
      OB_LOG_ERROR_CODE (0x21010004,
                         "Cannot join up to pool %s: %s. Giving up.\n", name,
                         ob_error_string (pret));
      return NULL;
    }

  return new Hose (hose);
}
}
}  // namespace oblong::plasma
