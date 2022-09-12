# Slaw Version 1

_This document outlines the original encoding of Slaw and is maintained
for historical reasons. V1 Slaw has not been in use since g-speak 1.0
(around 2010). The only potential place I can think of where these pools
may still be in use is the original optical/dot tracking pipeline._

-----

Version 1 slawx are organized into 4-byte units called "quads".  In
version 1, 8-byte numbers may not be naturally aligned, which can
cause problems on non-x86 processors, as well as in vectorized code on
x86 processors.

The first quad of every slaw contains type information:

```
                   f i r s t   q u a d w o r d   o f   a n y   s l a w

                         76543210  76543210  76543210  76543210

length-follows:          1xxxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx
eight-byte length:       11xxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx

nil:                     00000001  00000001  00000001  00000001

wee cons:                01xxxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx
wee cons quadlen:        rrqqqqqq  qqqqqqqq  qqqqqqqq  qqqqqqqq

wee string:              001xxxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx
wee string quadlen:      rrrqqqqq  qqqqqqqq  qqqqqqqq  qqqqqqqq

wee list/map:            0001mxxx  xxxxxxxx  xxxxxxxx  xxxxxxxx
wee list/map elem count: rrrrrccc  cccccccc  cccccccc  cccccccc

(where "m" is 0 for list and 1 for map)

numeric:                 00001xxx  xxxxxxxx  xxxxxxxx  xxxxxxxx

full string:             1*100000  00000000  00000000  00000001
full cons:               1*100000  00000000  00000000  00000010
full list/map:           1*100000  00000000  00000000  0000010m

(the penulti-MSB above is zero or one as the length is contained in the
next one or two quadwords, i.e. if it's a four or eight byte length,
per the 'eight-byte length' bit description second from top)

boolean:                 00000000  00000000  00000000  0000001b

(where "b" is 0 for false and 1 for true)

protein:                 1*0wndir  1qqqqqqq  0ppppppp  100fffff

w = "wee"; means that the quadlen is the seven-bit number "q", and
there is not a separate quad following the header quad to contain the
quadlen.  (and if "w" is 1, "*" must be 0, since "*" indicates two
quads of quadlen, which is contradictory to zero quads of quadlen.)

n = "nonstandard"; this means all bets are off about the content, and
in particular the d, i, and r flags no longer have their usual
meaning, and could be used for other purposes.  The flags for
determining endianness and quadlen still work in the usual way,
though.

d = "descrips present"; if this is 0, then getting the descrips
returns NULL.

i = "ingests present"; if this is 0, then getting the ingests returns
NULL.

r = "rude data present"; this is really a redundant convenience bit to
indicate that the length of rude data is 0.  There is *not* any
distinction between "no rude data" and "zero-length rude data".

q = "quadlen"; only meaningful if "w" is 1.

p = "padding"; indicates number of bytes of padding at the end of the
protein; this is necessary to compute the exact length of the rude
data.

f = "flags"; these are reserved for future flags, and should currently
be 0.

numeric:                 00001xxx  xxxxxxxx  xxxxxxxx  xxxxxxxx

numeric float:           000011xx  xxxxxxxx  xxxxxxxx  xxxxxxxx
numeric complex:         00001x1x  xxxxxxxx  xxxxxxxx  xxxxxxxx
numeric unsigned:        00001xx1  xxxxxxxx  xxxxxxxx  xxxxxxxx
numeric wide:            00001xxx  1xxxxxxx  xxxxxxxx  xxxxxxxx
numeric stumpy:          00001xxx  x1xxxxxx  xxxxxxxx  xxxxxxxx

(wide and stumpy conspire to express whether the number in question is 8,
16, 32, or 64 bits long; neither-wide-nor-stumpy, i.e. both zero, is sort
of canonical and thus means 32 bits; stumpy alone is 8; stumpy and wide
is 16; and just wide is 64)

numeric scalar:          00001xxx  xx000xxx  xxxxxxxx  xxxxxxxx
numeric 2-vector:        00001xxx  xx001xxx  xxxxxxxx  xxxxxxxx
numeric 3-vector:        00001xxx  xx010xxx  xxxxxxxx  xxxxxxxx
numeric 4-vector:        00001xxx  xx011xxx  xxxxxxxx  xxxxxxxx
numeric 2-multivector:   00001xxx  xx100xxx  xxxxxxxx  xxxxxxxx
numeric 3-multivector:   00001xxx  xx101xxx  xxxxxxxx  xxxxxxxx
numeric 4-multivector:   00001xxx  xx110xxx  xxxxxxxx  xxxxxxxx
numeric 5-multivector:   00001xxx  xx111xxx  xxxxxxxx  xxxxxxxx

(Scalars and vectors can be complex, but multivectors cannot be
complex.  Also, note that an n-dimensional vector has n elements, but
an n-dimensional multivector has 2**n elements.)

for any numeric entity, array or not, a size-in-bytes-minus-one is stored
in the last eight bits -- if a singleton, this describes the size of the
data part; if an array, it's the size of a single element -- so:

num'c unit bsize mask:   00001xxx  xxxxxxxx  xxxxxxxx  mmmmmmmm

and then for arrays, there're also these:

num'c breadth follows:   00001xxx  xxxxx1xx  xxxxxxxx  xxxxxxxx
num'c 8-byte breadth:    00001xxx  xxxxx11x  xxxxxxxx  xxxxxxxx
num'c wee breadth mask:  00001xxx  xxxxx0mm  mmmmmmmm  xxxxxxxx
```

For "wee" types, the quadlen (length of the slaw, in 32-bit units) is
contained in the first quad of the slaw.  If the length-follows bit is
set, the next one or two quads (depending on the "eight-byte length"
bit) contain the quadlen.

Lists, however, are an exception to this rule.  Lists separately store
both a quadlen and an element count.  Which one comes first depends on
whether the list is "wee" or not.  There are three possibilities:

- For wee lists, the element count is embedded in the first quad
  along with the type bits, and the quadlen is in its own quad after
  that.  The rationale for ordering these "backwards" (compared to
  the other two cases) is that since every slaw is at least one quad
  long, the quadlen will always be greater than or equal to the
  element count.  Therefore, it makes sense to store the quadlen in
  the field that has more bits available (32) and the element count
  in the field with fewer bits (27).
- For lists with the length-follows bit set, but not the eight-byte
  length bit set, the quadlen is in the second quad, and the element
  count is in the third quad.
- For lists with both the length-follows and eight-byte length bits
  set, the quadlen is in the second and third quads, and the element
  count is in the fourth and fifth quads.

For numeric types, the quadlen is not specified directly.  For numeric
singletons (non-arrays), the bsize is the number of bytes in the
payload, so just round up to an integral number of quads, and add in
the length of the header (which for numeric singletons is one quad) to
get the total quadlen.  The bsize is technically redundant, since it
is a known value for any particular numeric type, but it is convenient
to have.

All of the numeric types have corresponding structs, defined in
\ref libLoam/c/ob-types.h.

For arrays, the "breadth" is the number of elements in the array.
(And each element is bsize bytes.)  So one can compute the length by
multiplying breadth by bsize, and rounding up.  Note that for "wee
breadth mask", the 10-bit number "mm mmmmmmmm" is the breadth plus
one.  So a zero-length array has a 1 in the wee breadth mask, and a
1-length array has a 2.  Zero in the wee breadth mask indicates a
numeric singleton.  (That's why there isn't actually a specific bit to
indicate whether a numeric type is an array or a singleton.)

Slaw strings are encoded in UTF-8 and must include a terminating NUL
byte.  (Thus, slaw strings can encode any Unicode character except
NUL.)  After the required NUL, there may be additional bytes of
padding to get to the next quad boundary.  The padding bytes are
required to be 0.
