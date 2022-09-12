
/* (c)  oblong industries */

#include <libLoam/c++/AnkleObject.h>


using namespace oblong::loam;


void AnkleObject::Delete ()
{
  // we want to "delete" if we aren't under memory management
  if (!grip_mnd)
    {
      CleanUp ();
      return;
    }
  // or if our memory manager is just hanging out, holding down the
  // fort for a motley passel of weak references
  if (grip_mnd->QueryIsWeakRefShill ())
    {
      delete grip_mnd;
      CleanUp ();
      return;
    }
  // otherwise, this external call to Delete() is a noop
  // ... which is because, if we've gotten this far, we
  // have a wrap-mgr and we're so far from being a wekref
  // that it's only funny to the lithuanians.
}


// The definition following now calls the 'indirection enabling'
// method 'DeletePrimallyButWithCorrectThisPointer()', newly
// defined afresh and virtually for every AnkleObject derivative
// through the damp ministrations of patella-macros.h, in order
// to make sure that proper pointer adjustments are made before
// memory deallocation takes place. The necessity and the solution
// are documented in Klee Dienes's bug 4969; fundamentally,
// multiple inheritance and virtual inheritance can result in
// a situation in which ::delete() -- which is what AO:CleanUp()
// below used to call directly -- could be applied to the 'offset'
// pointer-to-base with a predictably double plus ungood outcome.
void AnkleObject::CleanUp ()
{
  DeletePrimallyButWithCorrectThisPointer ();
}


void AnkleObject::operator delete (void *goner)
{
  OB_LOG_BUG_CODE (
    0x11020000,
    "\n\n\nno. No. No! NO! You shall not. Shall not call "
    "operator-delete on any object derived from AnkleObject. "
    "That means, chum, that you dare not call operator-delete "
    "on pretty much any of g-speak's / yovo's objects (Vect "
    "is one exception; those other layabouts in Loam -- Quat, "
    "Matrix44, and Str -- are the only others as of 21 October "
    "2009). Why now this proscription, and why this textful "
    "jeremiad? The thing is. This is the thing. What's going "
    "on here is that we don't want anyone to call delete directly "
    "on an AnkleObject-derived anything -- there's a more "
    "sophisticated mechanism in place that acknowledges the "
    "possibility that cleanup activity may have to be deferrred (the "
    "reclamation of GL resources is a prime example -- that "
    "mayn't happen at any arbitrary time, which is exactly what "
    "the moment of the call to delete in general is: arbitrary). "
    "Sooooo... what we'd like is to do one o' them fangled tricks "
    "where you make either the destructor or operator-delete private, "
    "but in fact those no longer really work: if you try that, "
    "the compiler will disallow you from either making the "
    "object on the stack or making it on the heap. We want to "
    "allow both.\n\n"

    "(The operator-delete one is a recenter turd misbegotten "
    "in the late 1990s, when it was decided that if an exception "
    "was thrown during execution of a 'new expression' in which "
    "operator new succeeded but the constructor failed (i.e. the "
    "constructor threw the exception) then it was the responsibility "
    "of the language -- that is, of the compiler -- to call delete "
    "on the mangled semiformed pile. So far so good. A nice gesture, "
    "actually. Problem is, the compiler doofuses decided that for "
    "the 'language' to be able to call operator-delete, it (o.d.) "
    "must be public. Here, now, we arrive squarely in the middle "
    "of the turdliness. For if execution was able to make it into "
    "the new expression and at least partway into the constructor, "
    "then the instantaneous state of the execution is, in every "
    "sense that matters, well inside the "
    "scope of the class in question; by which it should absolutely "
    "be allowed to call the custom operator-delete irrespective of "
    "its permissiveness or lack thereof. But, alas, turditude "
    "prevailed in certain minds, which incorrectly asserted that "
    "that overridden operator-delete must be accessible from the "
    "most general possible scope, thus robbing us of a hugely "
    "useful mechanism: the private operator-delete that would prevent "
    "'outsiders' ever from calling delete on our objects. Instead, "
    "the best we can do is to discourage use of lowercase-d delete "
    "through the present logorrhea, and the promise that the "
    "memory is not being freed. For: that thing we do have in our "
    "control. We say: YOU ARE NOW LEAKING PRECIOUS MEMORY, MUAD'DIB) "
    "End of parenthetical foregoing. To hear someone else "
    "confirming the facts if not the vitriol, seek out sprightly "
    "young Scott Meyers (before the regrettable episode of "
    "sudden-onset gingivitis) over at\n\n"

    "http://www.ddj.com/cpp/184403484?pgno=1\n\n"

    "Anyway, you should never do\n\n"

    "  delete whizzo;\n\n"

    "if 'whizzo' is an AnkleObject / KneeObject derivative; instead, "
    "to be beloved of the universe, you must do\n\n"

    "  whizzo->Delete ();\n\n"

    "see?\n\n"

    "Incidentally, you were calling delete on <%p>; we'd love "
    "to tell you what the class is, but by now the vtable's been "
    "ever so thoughtfully stripped. Nice.\n\n\n",
    goner);

  ::delete ((AnkleObject *) goner);
}

#ifdef __clang__
// Detect whether the compiler has this bug:
// http://llvm.org/bugs/show_bug.cgi?id=10341

namespace {

class Detect10341
{
 public:
  Detect10341 () {}
  virtual ~Detect10341 () {}
  void operator delete (void *);
};

void Detect10341::operator delete (void *)
{
  OB_FATAL_BUG_CODE (0x11020001,
                     "Compiled with a version of Clang which has this bug:\n"
                     "http://llvm.org/bugs/show_bug.cgi?id=10341\n"
                     "which will make AnkleObject go into an infinite loop!\n");
}

const int dummy_10341 = (::delete new Detect10341, 0);
}
#endif
