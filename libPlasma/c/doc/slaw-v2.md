# Slaw Version 2 #      {#SlawV2}

Version 2 slawx are organized into 8-byte units called "octs".  (Not
to be confused with "octet", which is only one byte.)  Version 2
ensures that all numbers are naturally aligned, which was not
guaranteed by version 1.

The first oct of every slaw contains type information. This oct is a
64-bit number in the native endianness. The four most significant bits
contain the most coarse type information. The meanings of these bits
are listed below, as is the relationship between "octlen" and the
types that these bits indicate.

octlen encoded in header:

    0000 - backwards protein (i. e. a protein in the non-native endianness)
    0001 - forwards protein

octlen is always 1:

    0010 - boolean or nil
    0011 - wee string (six UTF-8 bytes or less, not counting terminating NUL)

octlen encoded in header:

    0100 - list
    0101 - map
    0110 - cons
    0111 - full string (seven UTF-8 bytes or more, not counting terminating NUL)

octlen implied by bsize:

    1000 - singleton signed integer
    1001 - singleton unsigned integer
    1010 - singleton float
    1011 - reserved

octlen implied by bsize and breadth:

    1100 - array signed integer
    1101 - array unsigned integer
    1110 - array float
    1111 - reserved

__________________________________________________

# f i r s t    o c t w o r d    o f    a n y    s l a w #   {#v2-first-octoword}

    76543210 76543210 76543210 76543210 76543210 76543210 76543210 76543210

protein:

    0001oooo oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo 0000oooo
    "o" is 56 bits of octlen (length of protein in octs, including this oct)

boolean:

    00100000 00000000 00000000 00000000 00000000 00000000 00000000 0000000b
    "b" is 0 for false or 1 for true

nil:

    00100000 00000000 00000000 00000000 00000000 00000000 00000000 00000010

wee string:

    00110bbb SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS
    "b" is number of bytes in string (1-7) including terminating NUL.
    "S" is "special bytes" (see below) containing the string itself.

list or map:

    010mnnnn oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo
    "m" is 0 for list or 1 for map
    "n" is number of elements in list (or pairs in map) if 0-14, or
           15 if 15 elements or greater (if 15, header oct is followed by
           a second oct which is the number of elements)
    "o" is 56 bits of octlen (length of slaw in octs, including this oct)

cons:

    01100010 oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo
    "o" is 56 bits of octlen (length of slaw in octs, including this oct)

full string:

    01110ppp oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo
    "p" is number of bytes of padding (additional 0 bytes after the
           terminating NUL and before the end of the slaw)
    "o" is 56 bits of octlen (length of slaw in octs, including this oct)

numeric singleton:

    10fusscv vvbbbbbb bb000000 00000000 SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS
    "f" is 0 for integer or 1 for floating point
    "u" is 0 for signed integer or floating point, or 1 for unsigned integer
    "ss" is 00/01/10/11 for 8/16/32/64-bit number
    "c" is 0 for real or 1 for complex
    "vvv" is:
          000 - scalar
          001 - 2-vector
          010 - 3-vector
          011 - 4-vector
          100 - 2-multivector
          101 - 3-multivector
          110 - 4-multivector
          111 - 5-multivector
    "b" is bsize - 1 (bsize is the size in bytes of the entire numeric type)
    "S" is "special bytes" (see below) containing the number if bsize is 4 or less.

numeric array:

    11fusscv vvbbbbbb bbBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB BBBBBBBB
    "f", "u", "s", "c", "v", and "b" are as above.  (bsize is the size in
    bytes of one element of the array)
    "B" is array breadth (number of elements)

___________________________________________________________________

# Layout of protein #           {#v2-protein-layout}

Proteins always have two header octs.  The first header oct is as
described above.  The second header oct is as follows:

seven bytes of rude data or less:

    ndif0rrr SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS SSSSSSSS

eight bytes of rude data or more:

    ndif1rrr rrrrrrrr rrrrrrrr rrrrrrrr rrrrrrrr rrrrrrrr rrrrrrrr rrrrrrrr

    "n" is the nonstandard flag.  The implications of this flag being set
           to 1 are beyond the scope of this document.  So set it to 0,
           please!
    "d" is the descrips flag.  "1" indicates a descrips slaw is present.
    "i" is the ingests flag.   "1" indicates an ingests slaw is present.
    "f" is the future flag.  It is reserved for future use, and should be
           0 for now.  But unlike the "n" flag, it being 1 does not imply
           all bets are off.

If there are seven bytes of rude data or less, then "r" is a 3-bit
number indicating the number of bytes of rude data, 0-7.  The rude
data is stored in the "special bytes" in much the same fashion as a
wee string.

If there are eight bytes of rude data or more, then "r" is a 59-bit
number indicating the number of bytes of rude data.  The rude data
comes after the descrips and ingests, as described below.

After the two header octs, there is a "descrips" slaw if and only if
the "d" flag was set.  The descrips slaw is strongly recommended to be
a list, but this is not required; it may be any valid slaw.

After the "descrips" slaw (or immediately after the header octs if "d"
is 0), there is an "ingests" slaw if and only if the "i" flag was
set.  The ingests slaw is strongly recommended to be a map, but this
is not required; it may be any valid slaw.

If there are 8 or more bytes of rude data, the rude data is the last
thing in the protein, following the descrips and/or ingests if they
are present.  If the number of bytes of rude data is not a multiple of
8, there are padding bytes filled with "0" following the rude data, up
to the next oct boundary.

___________________________________________________________________

# Layout of list, map, and cons #       {#v2-layout-containers}

The header oct is as described above.  If a list or map has 15 or more
elements, the header oct is followed by another oct, which is simply a
64-bit unsigned integer indicating the number of elements.
Conceptually, conses follow the same pattern and lists or maps, but
since conses always have exactly two elements, they never need this
extra oct.

Following the initial oct (or two octs) are the elements of the
container, in order.

___________________________________________________________________

# Layout of string #            {#v2-layout-string}

Both wee and full strings are UTF-8 encoded, and always have a
terminating NUL character. Since the length is encoded unambiguously,
it is acceptable for strings to have embedded NULs, though not all
higher-level software may support this. If you want to get the length
of a slaw string in a way that is robust to embedded NULs, be sure to
call slaw_string_emit_length (s) instead of strlen (slaw_string_emit
(s)). (In the absence of embedded NULs, these two expressions should
be equivalent.)

Wee strings are a single oct, and contain the string data in the
"special bytes" (see section below) of the header oct.  This still
includes the terminating NUL, though.

Full strings have a header oct as described above which encodes the
octlen of the slaw, as well as the number of bytes of padding.  The
bytes of the string follow the header oct.  If the string (including
the terminating NUL character) is not a multiple of 8 bytes, there are
additional bytes of "0" to pad up to the next oct.  The padding number
from the header counts these padding bytes (but the terminating NUL is
not counted as a padding byte).  This makes it possible to distinguish
between the padding bytes, the terminating NUL, and any embedded NULs
which may immediately precede the terminating NUL.  (Thus the
terminating NUL is merely a convenience for C-language programs, and
is not necessary to determine the length.)

___________________________________________________________________

# Layout of numerics #      {#v2-layout-numerics}

Numeric singletons whose bsize is 4 or less consist of only a single
oct.  The numeric value is encoded in the "special bytes".

Numeric singletons whose bsize is 5 or more have a header oct followed
immediately by the numeric value.  If the bsize is not divisible by 8,
additional padding bytes filled with "0" are added to round up to the
next oct.

Numeric arrays consist of (bsize * breadth) bytes of numeric data.
This data always starts immediately following the header oct.
(Special bytes are never used, even if the total length is 4 or less.)
If (bsize * breadth) is not divisible by 8, additional padding bytes
of "0" round up to the next oct.

___________________________________________________________________

# "Special Bytes" #         {#v2-special-bytes}

To avoid wasting too much space, slawx with very small payloads may
encode the payload inside the header oct using the "special bytes"
mechanism.  Specifically, "special bytes" are used in the header oct
of wee strings, may be used in the header oct of numeric singletons,
and may be used in the second oct of proteins.  These specific use
cases are described above.

Special Bytes are a bit tricky, because the header oct is encoded as a
64-bit integer in the native endianness.  However, the Special Bytes
are used to encode data of a different size (bytes for wee strings or
protein rude data, and 1-, 2-, or 4-byte numbers for numeric
singletons.)

The rule to remember is that each of the slawx that can use special
bytes specify the exact number of special bytes they contain (for
example, "bbb" in a wee string), and the special bytes (however many
are specified) always reside in the least significant portion of the
oct when it is viewed as a 64-bit integer.  So, if there are n special
bytes, the special bytes will be the first n bytes of the oct on
little-endian machines, and the last n bytes of the oct on big-endian
machines.

For example, here is the wee string "Hello" as a big-endian slaw:

    36 00 48 65 6c 6c 6f 00

and here is the same slaw in little-endian format:

    48 65 6c 6c 6f 00 00 36

In both cases, the "3" indicates a wee string, and the "6" is the
length of the string (when counting the terminating NUL).  This "36"
is in the most significant byte, which is the first byte for big
endian, and the last byte for little endian.

There are six bytes of "special bytes"-- the five letter of "Hello"
followed by the terminating NUL.  These six bytes are "left justified"
for big endian, and "right justified" for little endian.  In both
cases, the six special bytes occupy bits 0-47 of the header oct when
viewed as an intger.  But, the two header octs do not actually encode
the same integer, since the special bytes always appear in the same
order, and merely "slide" when the header oct is swapped, but the
special bytes themsevles are not actually swapped.

Since there are only six special bytes in this example (although wee
strings can accomodate up to seven), there is one byte of padding.  It
is initialized to 0, and occurs in bits 48-55 of the integer.  That
means the padding byte comes before the string in big-endian format,
and after the string in little-endian format.

Here's a more detailed example of how to byte-swap a slaw that
contains special bytes...

Imagine you have a slaw constructed like this:

    int16c q = {0x1234, 0x5678};
    slaw qq = slaw_int16c (q);

This slaw fits in a single oct, and in little endian, it is
represented like this:

    34 12 78 56 00 c0 00 86

It's easiest to think about the header oct and the special bytes
separately, even though they co-exist in the same oct.  So the header
oct is:

    ss ss ss ss 00 c0 00 86

Where "ss" represents the space occupied by the special bytes, which
we will treat opaquely for now.  Then we just byte-swap the header as
a 64-bit integer, so in big endian it becomes:

    86 00 c0 00 ss ss ss ss

Now let's separately consider the special bytes.  In little-endian,
the special bytes were:

    34 12 78 56

Since these bytes represent two 16-bit integers, we need to byte-swap
each 16-bit integer individually.  So in big-endian, this becomes:

    12 34 56 78

And now we have the big-endian header:

    86 00 c0 00 ss ss ss ss

and the big-endian special bytes:

    12 34 56 78

and all we have to do is put the special bytes back where they belong,
in the space denoted by "ss":

    86 00 c0 00 12 34 56 78

___________________________________________________________________

# Protein endianness #     {#v2-protein-endianness}

Proteins self-describe their endianness, so if you have a sequence of
bytes that you know are a protein, you can determine the endianness
without any out-of-band information.

The first oct of a protein is:

    0001oooo oooooooo oooooooo oooooooo oooooooo oooooooo oooooooo 0000oooo
    "o" is 56 bits of octlen (length of protein in octs, including this oct)

and the interpretation of the most significant four bits is:

    0000 - backwards protein (i. e. a protein in the non-native endianness)
    0001 - forwards protein

Because of the strategic way the four 0000 bits are positioned in the
upper nibble of the least-significant byte, if you byte-swap a
protein, its most significant four bits will contain "0000",
indicating it is a byte-swapped protein (opposite of whatever your
native endianness is).  So that means you have to interpret the
protein with the opposite endianness.  This is the reason for the
otherwise annoying way that the bits of the octlen are not contiguous.

However, since this property requires an extra four bits in an
inconvenient location, proteins are the only slaw which self-describe
their endianness.  If you have a stream of bytes which represent an
arbitrary slaw, not necessarily a protein, then you'll need
out-of-band information to communicate the endianness.
