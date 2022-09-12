
/* (c)  oblong industries */

#include <string.h>


#ifndef PATELLA_MACROS_H
#define PATELLA_MACROS_H

#include "../c/ob-attrs.h"

/**
 * The patella macros should be used like this:
 *
 * \code
 * class C
 * {
 *   PATELLA_CLASS (C)
 *  public:
 *   ...
 * };
 * \endcode
 *
 * \code
 * class C : public B
 * {
 *   PATELLA_SUBCLASS (C, B)
 *  public:
 *   ...
 * };
 * \endcode
 */

#define PATELLA_CLASS(CLASSNAME)                                               \
  _PATELLA_CLASS_HELPER (CLASSNAME)                                            \
 public:                                                                       \
  static const char *StaticClassName () { return #CLASSNAME; }                 \
 OB_PRIVATE:                                                         \
 OB_PROTECTED:                                                         \
  bool ThoughtlessIsInclusiveDescendentOf (const char *cl) const               \
  {                                                                            \
    return (ThoughtlessIsOfClass (cl));                                        \
  }                                                                            \
  bool ThoughtlessIsDescendentOf (OB_UNUSED const char *cl) const              \
  {                                                                            \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
 public:                                                                       \
  virtual const char *ParentClassName () const { return ""; }                  \
  static bool ClassIsInclusiveDescendentOf (const char *cl)                    \
  {                                                                            \
    return (0 == strcmp (StaticClassName (), cl));                             \
  }                                                                            \
  static bool ClassIsDescendentOf (OB_UNUSED const char *cl) { return false; } \
 OB_PRIVATE:

#define PATELLA_CLASS_OVERRIDE(CLASSNAME)                                      \
  _PATELLA_CLASS_HELPER_OVERRIDE (CLASSNAME)                                   \
 public:                                                                       \
  static const char *StaticClassName () { return #CLASSNAME; }                 \
 OB_PRIVATE:                                                         \
 OB_PROTECTED:                                                         \
  bool ThoughtlessIsInclusiveDescendentOf (const char *cl) const               \
  {                                                                            \
    return (ThoughtlessIsOfClass (cl));                                        \
  }                                                                            \
  bool ThoughtlessIsDescendentOf (const char *cl) const { return 0; }          \
 public:                                                                       \
  const char *ParentClassName () const override { return ""; }                 \
  static bool ClassIsInclusiveDescendentOf (const char *cl)                    \
  {                                                                            \
    return (0 == strcmp (StaticClassName (), cl));                             \
  }                                                                            \
  static bool ClassIsDescendentOf (const char *cl) { return false; }           \
 OB_PRIVATE:


#define PATELLA_SUBCLASS(CLASSNAME, PARENTCLASSNAME)                           \
  _PATELLA_CLASS_HELPER_OVERRIDE (CLASSNAME)                                   \
 public:                                                                       \
  static const char *StaticClassName () { return #CLASSNAME; }                 \
 OB_PRIVATE:                                                           \
  _PATELLA_SUBCLASS_HELPER (CLASSNAME, PARENTCLASSNAME)

// this macro is hereby introduced in support of template classes
// in the main class hierarchy, which should pass ClassName<T> as
// CLASSNAME, and must implement StaticClassName to return the
// appropriate value based on the template parameter;
// see libBasement/EaseSoft.h for an example
#define PATELLA_TEMPLATE_SUBCLASS(CLASSNAME, PARENTCLASSNAME)                  \
  _PATELLA_CLASS_HELPER_OVERRIDE (CLASSNAME)                                   \
  _PATELLA_SUBCLASS_HELPER (CLASSNAME, PARENTCLASSNAME)


#define _PATELLA_SUBCLASS_HELPER(CLASSNAME, PARENTCLASSNAME)                   \
  typedef PARENTCLASSNAME super;                                               \
                                                                               \
 OB_PROTECTED:                                                         \
  bool ThoughtlessIsInclusiveDescendentOf (const char *cl) const               \
  {                                                                            \
    return (ThoughtlessIsOfClass (cl) || super::IsInclusiveDescendentOf (cl)); \
  }                                                                            \
  bool ThoughtlessIsDescendentOf (const char *cl) const                        \
  {                                                                            \
    return super::ThoughtlessIsInclusiveDescendentOf (cl);                     \
  }                                                                            \
                                                                               \
 public:                                                                       \
  const char *ParentClassName () const override                                \
  {                                                                            \
    return super::ClassName ();                                                \
  }                                                                            \
  static bool ClassIsInclusiveDescendentOf (const char *cl)                    \
  {                                                                            \
    return (0 == strcmp (StaticClassName (), cl))                              \
           || super::ClassIsInclusiveDescendentOf (cl);                        \
  }                                                                            \
  static bool ClassIsDescendentOf (OB_UNUSED const char *cl) { return false; } \
  PARENTCLASSNAME *__Compile__Time__Paternity__Test__ ()                       \
  {                                                                            \
    return (CLASSNAME *) NULL;                                                 \
  }                                                                            \
                                                                               \
 OB_PRIVATE:


#define _PATELLA_CLASS_HELPER(CLASSNAME)                                       \
 public:                                                                       \
  virtual bool IsOfClass (const char *cl) const                                \
  {                                                                            \
    return (0 == strcmp (ClassName (), cl));                                   \
  }                                                                            \
  virtual bool IsInclusiveDescendentOf (const char *cl) const                  \
  {                                                                            \
    return CLASSNAME::ThoughtlessIsInclusiveDescendentOf (cl);                 \
  }                                                                            \
  virtual bool IsDescendentOf (const char *cl) const                           \
  {                                                                            \
    return CLASSNAME::ThoughtlessIsDescendentOf (cl);                          \
  }                                                                            \
  virtual const char *ClassName () const                                       \
  {                                                                            \
    return CLASSNAME::StaticClassName ();                                      \
  }                                                                            \
  size_t ThoughtlessClassSize () const { return sizeof (CLASSNAME); }          \
  virtual size_t ClassSize () const { return sizeof (CLASSNAME); }             \
 OB_PROTECTED:                                                         \
  bool ThoughtlessIsOfClass (const char *cl) const                             \
  {                                                                            \
    return (0 == strcmp (ThoughtlessClassName (), cl));                        \
  }                                                                            \
  const char *ThoughtlessClassName () const                                    \
  {                                                                            \
    return CLASSNAME::StaticClassName ();                                      \
  }                                                                            \
  virtual void DeletePrimallyButWithCorrectThisPointer () { ::delete (this); } \
 public:

#define _PATELLA_CLASS_HELPER_OVERRIDE(CLASSNAME)                              \
 public:                                                                       \
  bool IsOfClass (const char *cl) const override                               \
  {                                                                            \
    return (0 == strcmp (ClassName (), cl));                                   \
  }                                                                            \
  bool IsInclusiveDescendentOf (const char *cl) const override                 \
  {                                                                            \
    return CLASSNAME::ThoughtlessIsInclusiveDescendentOf (cl);                 \
  }                                                                            \
  bool IsDescendentOf (const char *cl) const override                          \
  {                                                                            \
    return CLASSNAME::ThoughtlessIsDescendentOf (cl);                          \
  }                                                                            \
  const char *ClassName () const override                                      \
  {                                                                            \
    return CLASSNAME::StaticClassName ();                                      \
  }                                                                            \
  size_t ThoughtlessClassSize () const { return sizeof (CLASSNAME); }          \
  size_t ClassSize () const override { return sizeof (CLASSNAME); }            \
 OB_PROTECTED:                                                         \
  bool ThoughtlessIsOfClass (const char *cl) const                             \
  {                                                                            \
    return (0 == strcmp (ThoughtlessClassName (), cl));                        \
  }                                                                            \
  const char *ThoughtlessClassName () const                                    \
  {                                                                            \
    return CLASSNAME::StaticClassName ();                                      \
  }                                                                            \
  void DeletePrimallyButWithCorrectThisPointer () override                     \
  {                                                                            \
    ::delete (this);                                                           \
  }                                                                            \
                                                                               \
 public:


#endif
