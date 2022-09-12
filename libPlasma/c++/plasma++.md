# libPlasma++ #         {#libplasmacc_doc}

# The Plasma++ library

One may think of @e Plasma++ as C++ bindings for plasma, our C library
for distributed computing. Its main goals are keeping you away from
explicit memory management and taking advantage of the inherent
immutability of plasma's slawx to provide a pleasant computing
experience based on value semantics. We also try to simplify pool
access and configuration. In the middle term, we aim at extending
plasma's metaphors and functionality by providing a pattern matching
layer on its top, together with what constitutes the embryo of a
mobile code engine.

Plasma++ is primarily composed of:
  - [Slaw and Protein types](#libplasmacc_types)
  - [Pools](#libplasmacc_pools)

# Namespaces

- oblong::plasma: Contains all the library's public interfaces
- oblong::plasma::detail: Implementation details

# Slaw and Protein types #        {#libplasmacc_types}

Higher-level wrappers for @e slawx and @e proteins.

A group of classes that conspire together to wrap C-world slawx in
a way that allows you to be oblivious of memory management and use
them, atomic or composite, with nice @e value semantics.

The center stage is occupied by Slaw, a class that wraps C-land
slawx and allows easy access to the slaw's value (the 'emit'
functions). Slaw instances are immutable and copying them is very
cheap (underlying C-slawx are not copied, but ref-counted, unless
really needed). Treating Slawx almost as built-in types is
liberating, and conductive to more robust and less buggy code.
Value semantics also plays nice with C++ standard containers.

Slaw's interface embraces and extends the dynamic nature of slawx.
The latter are already capable of representing a host of different
types, including composite ones: arrays, conses, lists, maps...
for each underlying type one has, in plasma, a set of applicable C
functions. In our C++ implementation, Slaw's interface offers
equivalent functionality, but goes one step beyond: @e all member
functions are applicable in any Slaw instance, interpreting the
inner slaw in a (hopefully) sensible manner as needed. For
example, if you have a Slaw initially constructed as a list and
call on it the Find member (key lookup in a map), the list is
viewed as a key/value succession and the lookup performed; or, if
you ask for the Nth() on an numeric array, it will be
reinterpreted in the natural way as a list and the corresponding
element returned. Note that reinterpretation does not imply
mutation (as mentioned above, Slaw is an immutable class).

Given the functionality already provided by Slaw, our wrapper for
proteins, the Protein class, is very lightweight: it needs just to
provide access to its two constitutive Slawx, descrips and
ingests, and we're in business.

## Classes ##

- oblong::plasma::Slaw
- oblong::plasma::Protein

# Low level slaw type handling #          {#libplasmacc_type_handling}

These functions use the @c slaw_traits template specializations to
provide conversions between slawx and C++ types. Most of the time,
you'll be using the higher level Slaw &co. types, which are
implemented in terms of this plumbing.
We also provide implementations of @c operator== and @c operator!=
for the @c v2int32 &co. type family.

## Classes ##

- slaw-traits.h

# Pool creation, configuration and handling #        {#libplasmacc_pools}

User friendly handling of pools and their configuration. We use
abstract interfaces to facilitate testing via dependency injection.

## Classes ##

- oblong::plasma::Hose
- oblong::plasma::HoseGang
- oblong::plasma::Pool
