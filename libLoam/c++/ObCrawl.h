
/* (c)  oblong industries */

#ifndef OB_CRAWL_CRABBINESS
#define OB_CRAWL_CRABBINESS


#include <assert.h>

#include <libLoam/c++/patella-macros.h>
#include <libLoam/c++/ObRef.h>



namespace oblong {
namespace loam {


// forward declare, so that we can declare 'weaktrove()', etc.
template <typename ELEMType, template <typename DUMMY> class MEM_MGR_CLASS>
class ObTrove;

/**
 * An ObCrawl is analogous to the better-than-C++ iterator device -- called
 * 'ranges' -- developed by Andrei Alexandrescu for the language D...
 */
template <typename ELEMType>
class ObCrawl
{
 public:
/**
   * \cond INTERNAL
   */
#undef WITH_MEM_MANAGEMENT_SENSITIVITY
#include "OT_Helpy.h"

  template <typename>
  struct ObCrawl_RetroGuts;
  template <typename>
  struct ObCrawl_ChainGuts;

  //
  // most basic: alpha of OC_Guts
  //
  class OC_Guts
  {
    PATELLA_CLASS (OC_Guts);

   public:
    virtual ~OC_Guts () {}
    virtual bool IsEmpty () const { return true; }
    virtual CRAWLRetType PopFore ()
    {
      OB_FATAL_BUG_CODE (0x11030001, "PopFore not available\n");
    }
    virtual CRAWLRetType PopAft ()
    {
      OB_FATAL_BUG_CODE (0x11030004, "PopAft not available\n");
    }
    virtual CRAWLRetType Fore () const
    {
      OB_FATAL_BUG_CODE (0x11030005, "Fore not available\n");
    }
    virtual CRAWLRetType Aft () const
    {
      OB_FATAL_BUG_CODE (0x11030006, "Aft not available\n");
    }

    virtual void Reload () {}  // note that it's not illegal to reload an empty OC_Guts...
    virtual OC_Guts *Dup () const { return new OC_Guts (); }
    virtual OC_Guts *Retro () const { return new OC_RetroGuts (this); }
    OC_Guts *Chain (const OC_Guts *moreguts) const
    {
      return new OC_ChainGuts (this->Dup (), moreguts->Dup ());
    }

#define A1 OC_Guts *g1
#define A2 A1, OC_Guts *g2
#define A3 A2, OC_Guts *g3
#define A4 A3, OC_Guts *g4
#define A5 A4, OC_Guts *g5
#define A6 A5, OC_Guts *g6
#define A7 A6, OC_Guts *g7
#define A8 A7, OC_Guts *g8
#define A9 A8, OC_Guts *g9
#define A10 A9, OC_Guts *g10
#define A11 A10, OC_Guts *g11
#define A12 A11, OC_Guts *g12
#define A13 A12, OC_Guts *g13
#define A14 A13, OC_Guts *g14
#define A15 A14, OC_Guts *g15
#define A16 A15, OC_Guts *g16
#define A17 A16, OC_Guts *g17
#define A18 A17, OC_Guts *g18
#define A19 A18, OC_Guts *g19

#define L1 g1->Dup ()
#define L2 L1, g2->Dup ()
#define L3 L2, g3->Dup ()
#define L4 L3, g4->Dup ()
#define L5 L4, g5->Dup ()
#define L6 L5, g6->Dup ()
#define L7 L6, g7->Dup ()
#define L8 L7, g8->Dup ()
#define L9 L8, g9->Dup ()
#define L10 L9, g10->Dup ()
#define L11 L10, g11->Dup ()
#define L12 L11, g12->Dup ()
#define L13 L12, g13->Dup ()
#define L14 L13, g14->Dup ()
#define L15 L14, g15->Dup ()
#define L16 L15, g16->Dup ()
#define L17 L16, g17->Dup ()
#define L18 L17, g18->Dup ()
#define L19 L18, g19->Dup ()

    OC_Guts *Zip (A1) const { return new OC_ZipGuts (this->Dup (), L1); }
    OC_Guts *Zip (A2) const { return new OC_ZipGuts (this->Dup (), L2); }
    OC_Guts *Zip (A3) const { return new OC_ZipGuts (this->Dup (), L3); }
    OC_Guts *Zip (A4) const { return new OC_ZipGuts (this->Dup (), L4); }
    OC_Guts *Zip (A5) const { return new OC_ZipGuts (this->Dup (), L5); }
    OC_Guts *Zip (A6) const { return new OC_ZipGuts (this->Dup (), L6); }
    OC_Guts *Zip (A7) const { return new OC_ZipGuts (this->Dup (), L7); }
    OC_Guts *Zip (A8) const { return new OC_ZipGuts (this->Dup (), L8); }
    OC_Guts *Zip (A9) const { return new OC_ZipGuts (this->Dup (), L9); }
    OC_Guts *Zip (A10) const { return new OC_ZipGuts (this->Dup (), L10); }
    OC_Guts *Zip (A11) const { return new OC_ZipGuts (this->Dup (), L11); }
    OC_Guts *Zip (A12) const { return new OC_ZipGuts (this->Dup (), L12); }
    OC_Guts *Zip (A13) const { return new OC_ZipGuts (this->Dup (), L13); }
    OC_Guts *Zip (A14) const { return new OC_ZipGuts (this->Dup (), L14); }
    OC_Guts *Zip (A15) const { return new OC_ZipGuts (this->Dup (), L15); }
    OC_Guts *Zip (A16) const { return new OC_ZipGuts (this->Dup (), L16); }
    OC_Guts *Zip (A17) const { return new OC_ZipGuts (this->Dup (), L17); }
    OC_Guts *Zip (A18) const { return new OC_ZipGuts (this->Dup (), L18); }
    OC_Guts *Zip (A19) const { return new OC_ZipGuts (this->Dup (), L19); }
  };

#include "OC_Undeffy.h"
  //
  // omega for basic ol' OC_Guts
  //


  //
  // alpha of OC_RetroGuts
  //
  class OC_RetroGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_RetroGuts, OC_Guts);

   OB_PRIVATE:
    OC_Guts *fwd_guts;

   private:
    OC_RetroGuts () {}
   public:
    OC_RetroGuts (const OC_Guts *fg) : fwd_guts (fg->Dup ()) {}
    ~OC_RetroGuts () override { delete fwd_guts; }
    bool IsEmpty () const override { return fwd_guts->IsEmpty (); }
    CRAWLRetType PopFore () override { return fwd_guts->PopAft (); }
    CRAWLRetType PopAft () override { return fwd_guts->PopFore (); }
    CRAWLRetType Fore () const override { return fwd_guts->Aft (); }
    CRAWLRetType Aft () const override { return fwd_guts->Fore (); }
    void Reload () override { fwd_guts->Reload (); }
    OC_Guts *Dup () const override { return new OC_RetroGuts (fwd_guts); }
    OC_Guts *Retro () const override { return fwd_guts->Dup (); }
  };
  //
  // omega for OC_RetroGuts
  //


  //
  // alpha of OC_ChainGuts
  //
  class OC_ChainGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_ChainGuts, OC_Guts);

   OB_PRIVATE:
    mutable OC_Guts *ell_guts, *arr_guts;
    OC_Guts *ell_orig, *arr_orig;

   private:
    OC_ChainGuts () {}
   public:
    OC_ChainGuts (OC_Guts *lg, OC_Guts *rg)
        : ell_guts (lg), arr_guts (rg), ell_orig (lg), arr_orig (rg)
    {
      assert (!(lg == NULL && rg == NULL));
    }
    OC_ChainGuts (OC_Guts *lg, OC_Guts *lo, OC_Guts *rg, OC_Guts *ro)
        : ell_guts (lg), arr_guts (rg), ell_orig (lo), arr_orig (ro)
    {
      assert (!(lo == NULL && ro == NULL));
    }

    ~OC_ChainGuts () override
    {
      if (ell_orig != arr_orig)
        delete ell_orig;
      delete arr_orig;
    }

    bool IsEmpty () const override
    {
      return ((ell_guts == NULL || ell_guts->IsEmpty ())
              && ((arr_guts == NULL) || arr_guts->IsEmpty ()));
    }
    CRAWLRetType PopFore () override
    {
      if (ell_guts)
        {
          if (ell_guts->IsEmpty ())
            ell_guts = NULL;
          else
            {
              CRAWLRetType crt = ell_guts->PopFore ();
              if (ell_guts->IsEmpty ())
                ell_guts = NULL;
              return crt;
            }
        }
      assert (arr_guts != NULL);
      CRAWLRetType crt = arr_guts->PopFore ();
      if (arr_guts->IsEmpty ())
        arr_guts = NULL;
      return crt;
    }
    CRAWLRetType PopAft () override
    {
      if (arr_guts)
        {
          if (arr_guts->IsEmpty ())
            arr_guts = NULL;
          else
            {
              CRAWLRetType crt = arr_guts->PopAft ();
              if (arr_guts->IsEmpty ())
                arr_guts = NULL;
              return crt;
            }
        }
      assert (ell_guts != NULL);
      CRAWLRetType crt = ell_guts->PopAft ();
      if (ell_guts->IsEmpty ())
        ell_guts = NULL;
      return crt;
    }
    CRAWLRetType Fore () const override
    {
      if (ell_guts)
        {
          if (ell_guts->IsEmpty ())
            ell_guts = NULL;
          else
            return ell_guts->Fore ();
        }
      assert (arr_guts != NULL);
      return arr_guts->Fore ();
    }
    CRAWLRetType Aft () const override
    {
      if (arr_guts)
        {
          if (arr_guts->IsEmpty ())
            arr_guts = NULL;
          else
            return arr_guts->Aft ();
        }
      assert (ell_guts != NULL);
      return ell_guts->Aft ();
    }
    void Reload () override
    {
      if (ell_orig)
        (ell_guts = ell_orig)->Reload ();
      if (arr_orig)
        (arr_guts = arr_orig)->Reload ();
    }
    OC_Guts *Dup () const override
    {
      OC_Guts *dopp_ell = ell_orig->Dup ();
      OC_Guts *dopp_arr = arr_orig->Dup ();
      return new OC_ChainGuts (ell_guts ? dopp_ell : NULL, dopp_ell,
                               arr_guts ? dopp_arr : NULL, dopp_arr);
    }
  };
  //
  // omega for OC_ChainGuts
  //


  //
  // alpha of OC_ZipGuts
  //
  class OC_ZipGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_ZipGuts, OC_Guts);

   OB_PRIVATE:
    OC_Guts *g[20];
    int16 num_g;
    int16 cur_g;

   private:
    OC_ZipGuts () {}
   public:
    typedef OC_Guts OCG;
    OC_ZipGuts (int16 num, OCG *const *gees) : num_g (num), cur_g (0)
    {
      OCG **fill_g = g + num_g;
      OCG *const *from_g = gees + num_g;
      while (fill_g > g)
        if ((*--fill_g = (*--from_g)->Dup ())->IsEmpty ())
          cur_g = -1;
    }
    OC_ZipGuts (int16 num, OB_UNUSED int16 cur, OCG *const *gees)
        : num_g (num), cur_g (0)
    {
      OCG **fill_g = g + num_g;
      OCG *const *from_g = gees + num_g;
      while (fill_g > g)
        if ((*--fill_g = (*--from_g)->Dup ())->IsEmpty ())
          cur_g = -1;
    }
#define A0 OCG *g0, OCG *g1
#define A1 A0, OCG *g2
#define A2 A1, OCG *g3
#define A3 A2, OCG *g4
#define A4 A3, OCG *g5
#define A5 A4, OCG *g6
#define A6 A5, OCG *g7
#define A7 A6, OCG *g8
#define A8 A7, OCG *g9
#define A9 A8, OCG *g10
#define A10 A9, OCG *g11
#define A11 A10, OCG *g12
#define A12 A11, OCG *g13
#define A13 A12, OCG *g14
#define A14 A13, OCG *g15
#define A15 A14, OCG *g16
#define A16 A15, OCG *g17
#define A17 A16, OCG *g18
#define A18 A17, OCG *g19

#define L0                                                                     \
  g[0] = g0;                                                                   \
  g[1] = g1
#define L1                                                                     \
  L0;                                                                          \
  g[2] = g2
#define L2                                                                     \
  L1;                                                                          \
  g[3] = g3
#define L3                                                                     \
  L2;                                                                          \
  g[4] = g4
#define L4                                                                     \
  L3;                                                                          \
  g[5] = g5
#define L5                                                                     \
  L4;                                                                          \
  g[6] = g6
#define L6                                                                     \
  L5;                                                                          \
  g[7] = g7
#define L7                                                                     \
  L6;                                                                          \
  g[8] = g8
#define L8                                                                     \
  L7;                                                                          \
  g[9] = g9
#define L9                                                                     \
  L8;                                                                          \
  g[10] = g10
#define L10                                                                    \
  L9;                                                                          \
  g[11] = g11
#define L11                                                                    \
  L10;                                                                         \
  g[12] = g12
#define L12                                                                    \
  L11;                                                                         \
  g[13] = g13
#define L13                                                                    \
  L12;                                                                         \
  g[14] = g14
#define L14                                                                    \
  L13;                                                                         \
  g[15] = g15
#define L15                                                                    \
  L14;                                                                         \
  g[16] = g16
#define L16                                                                    \
  L15;                                                                         \
  g[17] = g17
#define L17                                                                    \
  L16;                                                                         \
  g[18] = g18
#define L18                                                                    \
  L17;                                                                         \
  g[19] = g19

    OC_ZipGuts (A0) : num_g (2), cur_g (0) { L0; }
    OC_ZipGuts (A1) : num_g (3), cur_g (0) { L1; }
    OC_ZipGuts (A2) : num_g (4), cur_g (0) { L2; }
    OC_ZipGuts (A3) : num_g (5), cur_g (0) { L3; }
    OC_ZipGuts (A4) : num_g (6), cur_g (0) { L4; }
    OC_ZipGuts (A5) : num_g (7), cur_g (0) { L5; }
    OC_ZipGuts (A6) : num_g (8), cur_g (0) { L6; }
    OC_ZipGuts (A7) : num_g (9), cur_g (0) { L7; }
    OC_ZipGuts (A8) : num_g (10), cur_g (0) { L8; }
    OC_ZipGuts (A9) : num_g (11), cur_g (0) { L9; }
    OC_ZipGuts (A10) : num_g (12), cur_g (0) { L10; }
    OC_ZipGuts (A11) : num_g (13), cur_g (0) { L11; }
    OC_ZipGuts (A12) : num_g (14), cur_g (0) { L12; }
    OC_ZipGuts (A13) : num_g (15), cur_g (0) { L13; }
    OC_ZipGuts (A14) : num_g (16), cur_g (0) { L14; }
    OC_ZipGuts (A15) : num_g (17), cur_g (0) { L15; }
    OC_ZipGuts (A16) : num_g (18), cur_g (0) { L16; }
    OC_ZipGuts (A17) : num_g (19), cur_g (0) { L17; }
    OC_ZipGuts (A18) : num_g (20), cur_g (0) { L18; }

#include "OC_Undeffy.h"

    ~OC_ZipGuts () override
    {
      OCG **kill_g = g + num_g;
      while (--kill_g >= g)
        delete *kill_g;
    }
    bool IsEmpty () const override
    {
      if (cur_g < 0 || num_g < 1)
        return true;
      if (cur_g < num_g)
        return false;
      return true;
    }
    CRAWLRetType PopFore () override
    {
      assert (!IsEmpty ());
      CRAWLRetType crt = g[cur_g]->PopFore ();
      if (++cur_g >= num_g)
        {
          // idea here is that we'll check the next 'slice' of the array
          // of crawls; if any is empty, we're done.
          OCG *const *test_g = g + num_g;
          while (--test_g >= g)
            if ((*test_g)->IsEmpty ())
              {
                cur_g = -1;
                return crt;
              }
          cur_g = 0;
        }
      return crt;
    }
    CRAWLRetType PopAft () override
    {
      OB_FATAL_BUG_CODE (0x11030000, "PopAft() says no! Zipped crawls only go "
                                     "forward!\n");
    }
    CRAWLRetType Fore () const override
    {
      assert (!IsEmpty ());
      return g[cur_g]->Fore ();
    }
    CRAWLRetType Aft () const override
    {
      OB_FATAL_BUG_CODE (0x11030002,
                         "Aft() says no! Zipped crawls only go forward!\n");
    }
    void Reload () override
    {
      OCG **rel_g = g + num_g;
      while (--rel_g >= g)
        (*rel_g)->Reload ();
      cur_g = 0;
    }
    OC_Guts *Retro () const override
    {
      OB_FATAL_BUG_CODE (0x11030003,
                         "Retro() says no! Zipped crawls only go forward!\n");
    }
    OC_Guts *Dup () const override { return new OC_ZipGuts (num_g, cur_g, g); }
  };
  //
  // omega for OC_ZipGuts
  //

  /** \endcond */


  //
  // here's the start of ObCrawl proper, all preparatories discharged.
  //

 OB_PRIVATE:
  OC_Guts *guts;

 public:
  ObCrawl () : guts (new OC_Guts) {}

  /**
   * \cond INTERNAL
   */
  ObCrawl (OC_Guts *og) : guts (og) {}
  /** \endcond */

  ObCrawl (const ObCrawl<ELEMType> &other_crawl)
      : guts (other_crawl.guts->Dup ())
  {
  }

  ObCrawl (ObCrawl &&other_crawl) : guts (new OC_Guts)
  {
    std::swap (guts, other_crawl.guts);
  }

  ~ObCrawl () { delete guts; }
  void Delete () { delete this; }

  ObCrawl<ELEMType> &operator= (const ObCrawl<ELEMType> &oc)
  {
    if (this == &oc)
      return *this;

    if (guts)
      delete guts;
    guts = oc.guts->Dup ();
    return *this;
  }

  ObCrawl &operator= (ObCrawl &&oc) noexcept
  {
    if (this == &oc)
      return *this;

    delete guts;
    guts = oc.guts;
    oc.guts = nullptr;

    return *this;
  }

  /**
   * Clones this crawl and returns it by value.
   */
  ObCrawl<ELEMType> dup () const { return ObCrawl<ELEMType> (guts->Dup ()); }

  /**
   * Clones this crawl and returns it by pointer.
   */
  ObCrawl<ELEMType> *heapydup () const
  {
    return new ObCrawl<ELEMType> (guts->Dup ());
  }

  /**
   * Does this crawl contain any elements?
   */
  bool isempty () const { return guts->IsEmpty (); }
  /**
   * Returns the first element in the crawl and removes it.
   */
  CRAWLRetType popfore () { return guts->PopFore (); }
  /**
   * Returns the last element in the crawl and removes it.
   */
  CRAWLRetType popaft () { return guts->PopAft (); }
  /**
   * Returns the first element in the crawl without removing it.
   */
  CRAWLRetType fore () const { return guts->Fore (); }
  /**
   * Returns the last element in the crawl without removing it.
   */
  CRAWLRetType aft () const { return guts->Aft (); }
  /**
   * Sounds like it might restore the crawl to its original, pristine
   * state, but it would be good to get jh to verify this.
   */
  void reload () { guts->Reload (); }

  /**
   * Returns, by value, a new crawl which is identical to this one,
   * but in the opposite order.
   */
  ObCrawl<ELEMType> retro () const
  {
    return ObCrawl<ELEMType> (guts->Retro ());
  }
  /**
   * Returns, by pointer, a new crawl which is identical to this one,
   * but in the opposite order.
   */
  ObCrawl<ELEMType> *heapyretro () const
  {
    return new ObCrawl<ELEMType> (guts->Retro ());
  }
  /**
   * Combines the current crawl with \a more, and returns the combined
   * crawl by value.
   */
  ObCrawl<ELEMType> chain (const ObCrawl &more) const
  {
    return ObCrawl<ELEMType> (guts->Chain (more.guts));
  }
  /**
   * Combines the current crawl with \a more, and returns the combined
   * crawl by pointer.
   */
  ObCrawl<ELEMType> *heapychain (const ObCrawl &more) const
  {
    return new ObCrawl<ELEMType> (guts->Chain (more.guts));
  }


  ObTrove<ELEMType, UnspecifiedMemMgmt> trove ();
  ObTrove<ELEMType, UnspecifiedMemMgmt> *heapytrove ();

  ObTrove<ELEMType, WeakRef> weaktrove ();
  ObTrove<ELEMType, WeakRef> *heapyweaktrove ();

/**
   * \cond INTERNAL
   */
#define A1 const ObCrawl &oc1
#define A2 A1, const ObCrawl &oc2
#define A3 A2, const ObCrawl &oc3
#define A4 A3, const ObCrawl &oc4
#define A5 A4, const ObCrawl &oc5
#define A6 A5, const ObCrawl &oc6
#define A7 A6, const ObCrawl &oc7
#define A8 A7, const ObCrawl &oc8
#define A9 A8, const ObCrawl &oc9
#define A10 A9, const ObCrawl &oc10
#define A11 A10, const ObCrawl &oc11
#define A12 A11, const ObCrawl &oc12
#define A13 A12, const ObCrawl &oc13
#define A14 A13, const ObCrawl &oc14
#define A15 A14, const ObCrawl &oc15
#define A16 A15, const ObCrawl &oc16
#define A17 A16, const ObCrawl &oc17
#define A18 A17, const ObCrawl &oc18
#define A19 A18, const ObCrawl &oc19

#define L1 oc1.guts
#define L2 L1, oc2.guts
#define L3 L2, oc3.guts
#define L4 L3, oc4.guts
#define L5 L4, oc5.guts
#define L6 L5, oc6.guts
#define L7 L6, oc7.guts
#define L8 L7, oc8.guts
#define L9 L8, oc9.guts
#define L10 L9, oc10.guts
#define L11 L10, oc11.guts
#define L12 L11, oc12.guts
#define L13 L12, oc13.guts
#define L14 L13, oc14.guts
#define L15 L14, oc15.guts
#define L16 L15, oc16.guts
#define L17 L16, oc17.guts
#define L18 L17, oc18.guts
#define L19 L18, oc19.guts

  ObCrawl<ELEMType> zip (A1) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L1));
  }
  ObCrawl<ELEMType> zip (A2) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L2));
  }
  ObCrawl<ELEMType> zip (A3) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L3));
  }
  ObCrawl<ELEMType> zip (A4) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L4));
  }
  ObCrawl<ELEMType> zip (A5) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L5));
  }
  ObCrawl<ELEMType> zip (A6) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L6));
  }
  ObCrawl<ELEMType> zip (A7) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L7));
  }
  ObCrawl<ELEMType> zip (A8) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L8));
  }
  ObCrawl<ELEMType> zip (A9) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L9));
  }
  ObCrawl<ELEMType> zip (A10) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L10));
  }
  ObCrawl<ELEMType> zip (A11) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L11));
  }
  ObCrawl<ELEMType> zip (A12) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L12));
  }
  ObCrawl<ELEMType> zip (A13) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L13));
  }
  ObCrawl<ELEMType> zip (A14) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L14));
  }
  ObCrawl<ELEMType> zip (A15) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L15));
  }
  ObCrawl<ELEMType> zip (A16) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L16));
  }
  ObCrawl<ELEMType> zip (A17) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L17));
  }
  ObCrawl<ELEMType> zip (A18) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L18));
  }
  ObCrawl<ELEMType> zip (A19) const
  {
    return ObCrawl<ELEMType> (guts->Zip (L19));
  }

  ObCrawl<ELEMType> *heapyzip (A1) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L1));
  }
  ObCrawl<ELEMType> *heapyzip (A2) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L2));
  }
  ObCrawl<ELEMType> *heapyzip (A3) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L3));
  }
  ObCrawl<ELEMType> *heapyzip (A4) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L4));
  }
  ObCrawl<ELEMType> *heapyzip (A5) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L5));
  }
  ObCrawl<ELEMType> *heapyzip (A6) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L6));
  }
  ObCrawl<ELEMType> *heapyzip (A7) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L7));
  }
  ObCrawl<ELEMType> *heapyzip (A8) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L8));
  }
  ObCrawl<ELEMType> *heapyzip (A9) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L9));
  }
  ObCrawl<ELEMType> *heapyzip (A10) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L10));
  }
  ObCrawl<ELEMType> *heapyzip (A11) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L11));
  }
  ObCrawl<ELEMType> *heapyzip (A12) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L12));
  }
  ObCrawl<ELEMType> *heapyzip (A13) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L13));
  }
  ObCrawl<ELEMType> *heapyzip (A14) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L14));
  }
  ObCrawl<ELEMType> *heapyzip (A15) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L15));
  }
  ObCrawl<ELEMType> *heapyzip (A16) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L16));
  }
  ObCrawl<ELEMType> *heapyzip (A17) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L17));
  }
  ObCrawl<ELEMType> *heapyzip (A18) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L18));
  }
  ObCrawl<ELEMType> *heapyzip (A19) const
  {
    return new ObCrawl<ELEMType> (guts->Zip (L19));
  }
/** \endcond */

#include "OC_Undeffy.h"
  //
  // and, at long last, the end of ObCrawl itself
  //
};
}
}  // final resting place of namespaces loam and oblong...



#endif
