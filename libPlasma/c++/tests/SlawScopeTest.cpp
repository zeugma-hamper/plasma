
/* (c)  oblong industries */

// Unit tests for Slaw checking scope issues in presence of dynamic allocation

#include <Slaw.h>
#include <Protein.h>

#include <libPlasma/c/protein.h>
#include "libLoam/c++/LoamStreams.h"
#include "libPlasma/c++/PlasmaStreams.h"
#include "libPlasma/c++/SlawIterator.h"
#include <gtest/gtest.h>

using namespace oblong::plasma;

namespace {

const int SIZE (5);

slaw make_list ()
{
  slabu *sb (slabu_new ());
  for (int i = 0; i < SIZE; ++i)
    slabu_list_add_f (sb, slaw_int32 (i));
  return slaw_list_f (sb);
}

slaw make_map (int offset = 0)
{
  slabu *sb2 (slabu_new ());
  for (int i = 0; i < SIZE; ++i)
    slabu_map_put_ff (sb2, slaw_int32 (i + offset), slaw_string ("value"));
  return slaw_map_f (sb2);
}

Slaw *make_cons_slawx ()
{
  Slaw cons (slaw_cons_ff (slaw_int32 (0), slaw_int32 (1)));
  Slaw *result = new Slaw[2];
  result[0] = cons.Car ();
  result[1] = cons.Cdr ();
  return result;
}

Slaw *make_slawx_list ()
{
  Slaw *result = new Slaw[SIZE];
  Slaw list (make_list ());
  EXPECT_TRUE (list.IsList ()) << list;
  EXPECT_EQ (SIZE, list.Count ()) << list;
  for (int i = 0; i < SIZE; ++i)
    result[i] = list.Nth (i);
  return result;
}


Slaw *make_map_values ()
{
  Slaw *result = new Slaw[SIZE];
  Slaw m (make_map ());
  EXPECT_TRUE (m.IsMap ()) << m;
  EXPECT_EQ (SIZE, m.Count ()) << m;
  for (int32 i = 0; i < SIZE; ++i)
    {
      result[i] = m.Find (i);
      EXPECT_FALSE (result[i].IsNull ()) << i << "th is " << m.Nth (i);
    }
  return result;
}

Slaw *make_map_kvs ()
{
  Slaw *result = new Slaw[SIZE];
  Slaw m (make_map ());
  EXPECT_TRUE (m.IsMap ()) << m;
  EXPECT_EQ (SIZE, m.Count ()) << m;
  Slaw keys (m.MapKeys ());
  Slaw vals (m.MapValues ());
  for (int32 i = 0; i < SIZE; ++i)
    result[i] = Slaw::Cons (keys[i], vals[i]);
  return result;
}

Slaw *make_merged_map_values ()
{
  Slaw *result = new Slaw[SIZE + 1];
  Slaw m0 (make_map ());
  Slaw m1 (make_map (1));
  Slaw m01 = m0.MapMerge (m1);
  EXPECT_EQ (SIZE + 1, m01.Count ()) << m01;
  for (int32 i = 0; i < SIZE + 1; ++i)
    result[i] = m01[i];
  return result;
}

Slaw *make_descrips ()
{
  Slaw *result = new Slaw[SIZE];
  Protein p (protein_from_ff (make_list (), make_map ()));
  Slaw descrips (p.Descrips ());
  EXPECT_TRUE (descrips.IsList ()) << descrips;
  EXPECT_EQ (SIZE, descrips.Count ());
  for (int i = 0; i < SIZE; ++i)
    result[i] = descrips.Nth (i);
  return result;
}

Slaw *make_ingests ()
{
  Slaw *result = new Slaw[SIZE];
  Protein p (protein_from_ff (slaw_nil (), make_map ()));
  Slaw ingests (p.Ingests ());
  for (int i = 0; i < SIZE; ++i)
    result[i] = ingests.Nth (i).Car ();
  return result;
}

void test_dynamic_list (Slaw *s)
{
  for (int i = 0; i < SIZE; ++i)
    {
      EXPECT_FALSE (s[i].IsNull ()) << s[i];
      EXPECT_EQ (i, s[i].Emit<int32> ()) << s[i];
    }
  delete[] s;
}

TEST (SlawScopeTest, Cons)
{
  Slaw *s (make_cons_slawx ());
  EXPECT_EQ (Slaw (int32 (0)), s[0]);
  EXPECT_EQ (Slaw (int32 (1)), s[1]);
  delete[] s;
}

TEST (SlawScopeTest, List)
{
  test_dynamic_list (make_slawx_list ());
}

TEST (SlawScopeTest, Map)
{
  Slaw *values (make_map_values ());
  Slaw v ("value");
  for (int i = 0; i < SIZE; ++i)
    EXPECT_EQ (v, values[i]) << values[i];
  delete[] values;
}

TEST (SlawScopeTest, MapKeyValues)
{
  Slaw *kvs (make_map_kvs ());
  Slaw v ("value");
  for (int i = 0; i < SIZE; ++i)
    EXPECT_EQ (v, kvs[i].Cdr ()) << kvs[i];
  delete[] kvs;
}

TEST (SlawScopeTest, MergedMap)
{
  Slaw *values (make_merged_map_values ());
  const Slaw v ("value");
  for (int32 i = 0; i < SIZE + 1; ++i)
    EXPECT_EQ (v, values[i].Cdr ()) << values[i];
  delete[] values;
}

TEST (SlawScopeTest, ProteinDescrips)
{
  test_dynamic_list (make_descrips ());
}

TEST (SlawScopeTest, ProteinIngests)
{
  test_dynamic_list (make_ingests ());
}

}  // namespace
