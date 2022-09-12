#!/usr/bin/perl -w

# Try running "indent -nut" on the output

@Types = qw(
int32
unt32
int64
unt64
float32
float64
int8
unt8
int16
unt16
int32c
unt32c
int64c
unt64c
float32c
float64c
int8c
unt8c
int16c
unt16c
v2int32
v2unt32
v2int64
v2unt64
v2float32
v2float64
v2int8
v2unt8
v2int16
v2unt16
v3int32
v3unt32
v3int64
v3unt64
v3float32
v3float64
v3int8
v3unt8
v3int16
v3unt16
v4int32
v4unt32
v4int64
v4unt64
v4float32
v4float64
v4int8
v4unt8
v4int16
v4unt16
v2int32c
v2unt32c
v2int64c
v2unt64c
v2float32c
v2float64c
v2int8c
v2unt8c
v2int16c
v2unt16c
v3int32c
v3unt32c
v3int64c
v3unt64c
v3float32c
v3float64c
v3int8c
v3unt8c
v3int16c
v3unt16c
v4int32c
v4unt32c
v4int64c
v4unt64c
v4float32c
v4float64c
v4int8c
v4unt8c
v4int16c
v4unt16c
m2int32
m2unt32
m2int64
m2unt64
m2float32
m2float64
m2int8
m2unt8
m2int16
m2unt16
m3int32
m3unt32
m3int64
m3unt64
m3float32
m3float64
m3int8
m3unt8
m3int16
m3unt16
m4int32
m4unt32
m4int64
m4unt64
m4float32
m4float64
m4int8
m4unt8
m4int16
m4unt16
m5int32
m5unt32
m5int64
m5unt64
m5float32
m5float64
m5int8
m5unt8
m5int16
m5unt16
);

$Counter = 1;

sub fill_randomly {
    my ($type, $var, $state) = @_;

    my @components = ("");
    @components = qw(.x .y) if ($type =~ s/^v2//);
    @components = qw(.x .y .z) if ($type =~ s/^v3//);
    @components = qw(.x .y .z .w) if ($type =~ s/^v4//);

    if ($type =~ s/c$//) {
        @components = map {($_ . ".re", $_ . ".im")} @components;
    }

    if ($type =~ s/^m([2-5])//) {
        my $dimension = $1;
        my $length = (1 << $dimension);
        @components = ();
        for (my $i = 0 ; $i < $length ; $i++) {
            push @components, ".coef[$i]";
        }
    }

    foreach my $component (@components) {
        print "$var$component = random_$type ($state);\n";
    }
}

sub unique_var {
    return sprintf("p%03d", $Counter++);
}

sub declare {
    my ($type, $var, $dim) = @_;
    if (defined $dim) {
        print "$type $var", "[$dim];\n";
    } else {
        print "$type $var;\n";
    }
}

sub random_length {
    # some less than and greater than 1024, to test wee and non-wee breadth
    return 1 + int(rand(2000));
}

sub expand_types {
    my @types = map {($_, $_ . "_array")} @_;
    push @types, "nil";
    push @types, "string";
    push @types, "cons";
    push @types, "list";
    push @types, "map";
    push @types, "list_or_map";
    push @types, "protein";
    push @types, "swapped_protein";
    push @types, "boolean";
    return @types;
}

# Test each type in a separate function, because when I put all
# the tests directly in main() and compile with -O3, the compilation
# took 20 minutes on a Mac, and 2 minutes on Linux (but took up
# 6.5 gigs of RAM on Linux during that 2 minutes).  Hopefully
# smaller functions will tax the optimizer less.

sub exercise_type {
    my $type = $_[0];

    print "void exercise_$type (rand_state q1, rand_state q2, rand_state q3)\n";
    print "{ slaw s, s1, s2, s3, s4, s5, s6, s7, s8;\n";
    print "  int i, n;\n";

    my $equal = "slawx_equal";
    my $arrayLen = random_length();

    my $singletonVar = unique_var();
    my $singletonCopy = unique_var();
    my $arrayVar = unique_var();
    my $ptrVar = unique_var();

    declare ($type, $singletonVar);
    declare ($type, $singletonCopy);
    declare ($type, $arrayVar, $arrayLen);
    declare ($type, "*$ptrVar");

    fill_randomly ($type, $singletonVar, "q1");
    fill_randomly ($type, $singletonCopy, "q2");
    print "s = slaw_$type ($singletonVar);\n";

    print "TestAgainstPredicates (eSlawType_$type, s);\n";

    print "if (memcmp (& $singletonCopy, slaw_$type";
    print "_emit (s), sizeof ($type)) != 0)\n";
    print 'error_exit ("compare failed for ', $type, '\n");', "\n";
    print "if (memcmp (& $singletonCopy, slaw_$type";
    print "_emit_nocheck (s), sizeof ($type)) != 0)\n";
    print 'error_exit ("compare failed for ', $type, '\n");', "\n";
    print "slaw_free (s);\n";
    print "for (i = 0 ; i < $arrayLen ; i++) {\n";
    fill_randomly ($type, $arrayVar . "[i]", "q3");
    print "}\n";
    print "s1 = slaw_$type", "_array_raw ($arrayLen, &$ptrVar);\n";
    print "s2 = slaw_$type", "_array_empty (random_unt8 (q1));\n";
    print "s3 = slaw_$type";
    print "_array_filled (random_unt8 (q1), $singletonVar);\n";
    print "s4 = slaw_$type";
    print "_array ($arrayVar, $arrayLen);\n";

    my $arrayType = $type . "_array";

    print "TestAgainstPredicates (eSlawType_${type}_array, s1);\n";
    print "TestAgainstPredicates (eSlawType_${type}_array, s2);\n";
    print "TestAgainstPredicates (eSlawType_${type}_array, s3);\n";
    print "TestAgainstPredicates (eSlawType_${type}_array, s4);\n";

    print "n = sizeof ($type) * random_unt8 (q2);\n";
    print "for (i = 0 ; i < n ; i++) {\n";
    print "if (0 != ((byte*)slaw_$type", "_array_emit (s2))[i])\n";
    print 'error_exit ("nonzero at byte %d\n", i);', "\n";
    print "}\n";
    print "n = random_unt8(q2);\n";
    print "for (i = 0 ; i < n ; i++) {\n";
    print "if (memcmp (& $singletonCopy, i + slaw_$type";
    print "_array_emit (s3), sizeof ($type)) != 0)\n";
    print 'error_exit ("filled not equal at %d\n", i);', "\n";
    print "}\n";
    print "memcpy ($ptrVar, $arrayVar, sizeof($arrayVar));\n";
    print "if (! $equal (s1, s4))\n";
    print 'error_exit ("slawx not equal\n");', "\n";
    print "for (i = 0 ; i < n ; i++) {\n";
    print "if (slaw_$type", "_array_emit_nth (s3, i) != i + slaw_$type";
    print "_array_emit (s3))\n";
    print 'error_exit ("addresses not equal at %d\n", i);', "\n";
    print "}\n";
    print "s5 = slaw_$type";
    print "_arrays_concat (s3, s4, NULL);";
    print "s = slaw_$type", "_array_empty (0);\n";
    print "s6 = slaw_$type";
    print "_arrays_concat_f (s, slaw_dup (s3), s1, NULL);\n";
    print "s7 = slaw_$type";
    print "_array_concat_carray (s3, $arrayVar, $arrayLen);\n";
    print "s8 = slaw_$type";
    print "_array_concat_carray_f (s3, $arrayVar, $arrayLen);\n";
    print "if (! $equal (s5, s6))\n";
    print 'error_exit ("s5 != s6\n");', "\n";
    print "if (! $equal (s6, s7))\n";
    print 'error_exit ("s6 != s7\n");', "\n";
    print "if (! $equal (s7, s8))\n";
    print 'error_exit ("s7 != s8\n");', "\n";
    print "slaw_free (s2);\n";
    print "slaw_free (s4);\n";
    print "slaw_free (s5);\n";
    print "slaw_free (s6);\n";
    print "slaw_free (s7);\n";
    print "slaw_free (s8);\n";
    print "}\n\n";
}

srand(123);

print <<'EOF';

/* (c) 2009 Oblong Industries */

// this file is generated by libPlasma/c/t/gentest.pl

#include "libPlasma/c/pool.h"
#include "ilk-begotten-helper.h"
#include <stdlib.h>
#include <string.h>

typedef unsigned short rand_state[3];

#ifdef _MSC_VER

long jrand48(unsigned short xsubi[3])
{
  unt64 x;

  x =  (unt64)(unt16)xsubi[0] +
      ((unt64)(unt16)xsubi[1] << 16) +
      ((unt64)(unt16)xsubi[2] << 32);

  x = (0x5deece66dULL * x) + 0xb;

  xsubi[0] = (unsigned short)(unt16)x;
  xsubi[1] = (unsigned short)(unt16)(x >> 16);
  xsubi[2] = (unsigned short)(unt16)(x >> 32);

  return (long)(int32)(x >> 16);
}
#endif

static unt8 random_unt8 (rand_state s)
{ return (unt8) jrand48(s);
}

static int8 random_int8 (rand_state s)
{ return (int8) jrand48(s);
}

static unt16 random_unt16 (rand_state s)
{ return (unt16) jrand48(s);
}

static int16 random_int16 (rand_state s)
{ return (int16) jrand48(s);
}

static unt32 random_unt32 (rand_state s)
{ return (unt32) jrand48(s);
}

static int32 random_int32 (rand_state s)
{ return (int32) jrand48(s);
}

static unt64 random_unt64 (rand_state s)
{ unt64 hi = random_unt32(s);
  unt64 lo = random_unt32(s);
  hi <<= 32;
  return (hi ^ lo);
}

static int64 random_int64 (rand_state s)
{ return (int64) random_unt64(s);
}

static float32 random_float32 (rand_state s)
{ return (float32) random_int32(s);
}

static float64 random_float64 (rand_state s)
{ return (float64) random_int64(s);
}

EOF

foreach my $type (@Types) {
    exercise_type ($type);
}

print <<'EOF';
int main (int argc, char **argv)
{ int j;
  rand_state q1, q2, q3;

  memset (q1, 0x0b, sizeof(q1));
  memset (q2, 0x0b, sizeof(q2));
  memset (q3, 0xfc, sizeof(q3));

  for (j = 0 ; j < 10 ; j++)
    {
EOF

foreach my $type (@Types) {
    print "exercise_$type (q1, q2, q3);\n";
}

print "}\n\nreturn EXIT_SUCCESS;\n}\n";
