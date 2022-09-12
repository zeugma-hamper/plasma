
/* (c)  oblong industries */

#include <iostream>
#include "libLoam/c++/ArgParse.h"
#include "libPlasma/c++/Plasma.h"

using namespace oblong::plasma;

int main (int argc, char **argv)
{
  bool rewind_flag = false, exit_flag = false, spew_flag = false;
  ArgParse ap (argc, argv, "pool");

  ap.ArgFlag ("rewind", "\arewind input pool to oldest available protein",
              &rewind_flag);
  ap.ArgFlag ("exit", "\aexit when no protein available in input pool",
              &exit_flag);
  ap.ArgFlag ("spew", "\ause spew format instead of YAML format", &spew_flag);

  ap.Alias ("rewind", "r");
  ap.Alias ("exit", "x");
  ap.Alias ("spew", "S");

  // We require exactly one non-option argument: the mandatory pool name
  ap.EasyFinish (1);
  const Str pname = ap.Leftovers ().Nth (0);

  ObRetort tort;
  Hose *h = Pool::Participate (pname, &tort);
  if (tort.IsError ())
    OB_FATAL_ERROR ("Couldn't participate in '%s' because '%s'\n",
                    pname.utf8 (), tort.Description ().utf8 ());

  if (rewind_flag)
    OB_DIE_ON_ERROR (h->Rewind ().Code ());

  // either wait one second or forever, depending on -x option
  pool_timestamp timeout = (exit_flag ? 1.0 : Hose::WAIT);

  for (;;)
    {
      Protein p = h->Next (timeout);
      if (p.IsNull ())
        break;

      if (spew_flag)
        p.Spew (std::cout);
      else
        {
          Str s = p.ToSlaw ().ToString (&tort);
          OB_DIE_ON_ERROR (tort.Code ());
          std::cout << s.utf8 ();
        }
      std::cout.flush ();
    }

  OB_DIE_ON_ERROR (h->Withdraw ().Code ());
  h->Delete ();

  return EXIT_SUCCESS;
}
