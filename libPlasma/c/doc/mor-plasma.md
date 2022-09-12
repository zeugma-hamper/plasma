# More about c plasma #        {#libplasma_more}
# Description: Proteins, Pools, & Slaw

This text describes initial libPlasma components used for exchange of data
within and between computing processes.

A "process," in our usage, means a computer program executing for a period of
time. This execution may be interrupted by pauses or transfers between
processors or machines, so that a process is possibly suspended indefinitely
and "serialized" or "marshaled" to disk, across a memory bus, or over a
network.

The present work's context is a new programming environment that permits
(indeed: encourages) large scale multi-process interoperation. Existing
programming environments do not fully support multi-cpu and cross-network
execution, or flexible sharing of data between large numbers of computing
processes.

In pursuit of this goal we have designed and implemented several constructs
that together enable

- Efficient exchange of data between large numbers of processes
- Flexible data "typing" and structure, so that widely varying kinds
  and uses of data are supported
- Flexible mechanisms for data exchange (local memory, disk,
  network, etc.), all driven by substantially similar APIs.
- Data exchange between processes written in different programming
  languages
- Automatic maintenance of data caching and aggregate state

The principal constructs include slawx (plural of "slaw"), a mechanism for
efficient, platform-independent data representation and access; proteins, a
data encapsulation and transport scheme (whose payload is generally and
ideally slawx); and pools, which provide structured yet flexible aggregation,
ordering, filtering, and distribution of proteins -- within a process, among
local processes, across a network between remote or distributed processes, and
via 'longer term' (e.g. on-disk) storage.


# Proteins

The "protein" is a new mechanism for encapsulating data that needs to be
shared between processes (or moved across a bus or network).

A protein is a structured record format and an associated set of methods for
manipulating records (putting data in, taking data out, querying the format
and existence of data). Proteins are designed to be used via code written in a
variety of computer languages; to be the basic building block for "pools",
described below; and to be natively able to move between processors and across
networks while maintaining intact the data they contain.

One immediate -- and important -- use of proteins is as an improved mechanism
for transport and manipulation of user interface "events." Today's mainstream,
user-facing computing platforms (OS X, Microsoft Windows, X Windows) provide
facilities for transmitting user interface event data between processes. But
these existing mechanisms all suffer from several major design shortcomings
that make it difficult to build multi-process and multi-machine applications,
and that force users working in more than one programming language to jump
through frustrating hoops. Existing event frameworks are

- strongly typed, which makes them inflexible, privileges the
  assumptions of the systems vendor over the application programmer,
  and forms a mis-match with the facilities of increasingly popular
  dynamic languages (such as Ruby, Python and Perl)
- point-to-point, which makes coordinating the activity of more than
  a few distinct processes difficult or impossible
- strongly dependent on particular local, in-memory data structures,
  which renders them unsuited for on-disk storage or transmission
  across a network

In contrast, proteins are untyped (but provide a powerful and flexible
pattern-matching facility, on top of which "type-like" functionality is
implemented); are inherently multi-point (although point-to-point forms are
easily implemented as a subset of multi-point transmission); and define a
"universal" record format that does not differ (or differs only in the types
of optional optimizations that are performed) between in-memory, on-disk, and
on-the-wire (network) formats.

A protein is a linear sequence of bytes. Within these bytes are encapsulated a
"descrips" list -- an arbitrarily elaborate but efficiently filterable
per-protein description -- and a set of key-value pairs called "ingests" --
the actual "contents" of the protein.

A minimal (and read-only) protein implementation might define the following
behaviors in one or more programming languages:

- query the length in bytes of a protein
- query the number of descrips entries
- query the number of ingests
- retrieve a descrip entry by index number
- retrieve an ingest by index number

In addition, most implementations also define basic methods allowing proteins
to be constructed and filled with data, helper-methods that make common tasks
easier for programmers, and hooks for creating optimizations:

- create a new protein
- append a series of descrips entries
- append an ingest
- query the presence of a matching descrip
- query the presence of a matching ingest key
- retrieve an ingest value given a key
- pattern match across descrips
- delete one or more descrips
- delete one or more ingests
- embed non-structured metadata near the beginning of a protein

Proteins' concern with key-value pairs, as well as some core ideas about
network-friendly and multi-point data interchange, are shared with earlier
systems that privilege the concept of "tuples" (examples include Linda and
Jini). Proteins differ from tuple-oriented systems in several major ways,
including the use of the descrips list to provide a standard, optimizable
pattern matching substrate; and the rigorous specification of a record format
appropriate for a variety of storage and language constructs (along with
several particular implementations of "interfaces" to that record format).

Following is a somewhat detailed account of proteins' present specification.
Note that the specification is still subject to occasional adjustments and
improvements; the account is presented here merely by way of illustrating the
system's character.

In the current implementation of proteins, a protein is a type of slaw. This
is advantageous because it is possible to insert a protein into a slaw list or
slaw cons without needing to encapsulate it. It also means that it is possible
to write function that can accept either a slaw or a protein.

A protein in the current implementation consists of three parts, all of which
are optional. These parts are the descrips, the ingests, and the rude data.

The descrips and ingests are each a single slaw. It is envisioned that the
descrips will be a slaw list, and the ingests will be a slaw map. (This is why
"descrips" and "ingests" are plural, even though they are each a single slaw.)
Although they are not required to be a list and a map, it is strongly
recommended so that the protein semantics described above can be implemented.

The rude data is simply zero or more bytes, with no further meaning attached
to them.  One possible use of rude data is for storing large binary
data such as video frames.


# Slawx

The construct used to embed typed data inside proteins, as described above, is
a tagged byte-sequence specification and abstraction called a "slaw." (Plural:
slawx.)

A slaw is a linear sequence of bytes representing a piece of (possibly
aggregate) typed data, and is associated with programming-language-specific
APIs that allow slawx to be created, modified and moved around between memory
spaces, storage media, and machines. The slaw type scheme is intended to be
extensible and as lightweight as possible, and to be a common substrate that
can be used from any programming language.

The desire to build an efficient, large-scale inter-process communication
mechanism is the driver of the slaw design. Modern programming languages
provide sophisticated data structures and type facilities that work
beautifully in process-specific memory layouts, but these data representations
invariably break down when data needs to be moved between processes or stored
on disk. The slaw architecture is, first, a substantially efficient,
multi-platform friendly, low-level data model for inter-process communication.

Slawx are assumed to be in native byte order (thus, they need to be
encapsulated in a protein to indicate the byte order when they are stored on
disk or transferred across a network). As a result, well-designed slaw types
are quite efficient even when used from within a single process. Any slaw
anywhere in a process's accessible memory can be addressed without a copy or
"deserialization" step.


# Slaw types

## Atomic numeric types

- int8
- int16
- int32
- int64
- unt8
- unt16
- unt32
- unt64
- float32
- float64

Each of these types is distinct; i. e. you can't use an unt32 when an int32 is
expected. (Although the functions in slaw-coerce.h can be used to convert from
an unknown type to a known type.)

## Other atomic types

- nil (contains no data)
- boolean (true or false)
- string (UTF-8 encoded)

## Generic containers

- cons - contains exactly two elements, of any types
- list - contains zero or more elements, of any types
- map - contains zero or more elements, of type "cons"
- protein - has two slots ("descrips" and "ingests") which may each contain
  zero or one element of any type, plus optionally an arbitrary sequence of
  bytes called "rude data"

Lists and maps are almost entirely interchangeable. Nearly any function that
can be used on one can be used on the other. The main way to tell the
difference is by calling slaw_is_list or slaw_is_map.

Generally, a protein's "descrips" should be a list, and a proteins "ingests"
should be a map, but this is not required.

## Numeric containers

The type of the container's elements are considered part of the container's
type; i. e. a complex containing float32 is a different type than a complex
containing float64.

- complex - contains exactly two elements.  Both elements must be of the same
  type, and must be an atomic numeric type.

- vector - contains two, three, or four elements.  All elements must be of the
  same type, and must either be an atomic numeric type, or complex.  Vectors
  with different numbers of elements count as different types, for the purposes
  of determining what can be in an array together.

- multivector - contains four, eight, sixteen, or thirty-two elements.
  (Dimensions 2 through 5 are supported, and an n-dimensional multivector
  contains 2**n elements.)  All elements must be of the same type, and must be
  an atomic numeric type. (Unlike vectors, multivectors cannot contain
  complexes.) Multivectors with different numbers of elements count as
  different types, for the purposes of determining what can be in an array together.

- array - contains zero or more elements. All elements must be of the same
  type, and must be either an atomic numeric type, complex, vector, or
  multivector.  (Note that a 1-element array of type x is a distinct type from
  type x itself.)


# Slaw encoding

There are multiple binary encodings for slawx. The "current version" is always
used as the in-memory representation for slawx, but functions in
slaw-interop.h can convert to and from older versions of slawx, allowing
compatibility with legacy binaries.

See \ref SlawV2 for full details on the encoding.

Because slawx are meant to be compared for equality in a bytewise fashion, any
particular binary encoding for slawx must be canonical; i. e. there must be
only one byte sequence for any given semantic value. Among other things, this
implies that padding bytes must always be a known value (usually 0).


# Pools

A "pool" is a repository for proteins, providing linear sequencing and state
caching; multi-process access; and a set of common, optimizable filtering and
pattern-matching behaviors.

Slawx provide the lowest-level of data definition for inter-process exchange;
proteins provide mid-level structure and hooks for querying and filtering; and
pools provide for high-level organization and access semantics.

The pools API is designed to allow pools to be implemented in a variety of
ways, in order to account both for system-specific goals and for the available
capabilities of given hardware and network architectures. The two fundamental
system provisions upon which pools depend are a storage facility and a means
of inter-process communication. In our extant systems we use a flexible
combination of shared memory, virtual memory, and disk for the former, and IPC
queues and TCP/IP sockets for the latter.

The most basic pool functionality is as follows:

- "participate in" a pool
- put a protein in a pool
- retrieve the next unseen protein from a pool
- "rewind" or "fast-forward" within a pool

A very simple pool implementation might provide only those four methods.
However, most real-world pools would also support (and encourage):

- setting up a "streaming" pool call-back for a process
- selectively retrieving proteins that match particular patterns of
  descrips or ingests keys
- scanning backward and forwards for proteins that match particular
  patterns of descrips or ingests keys

Critical to the design of the pools architecture is the conception of pools as
maintaining state, so that individual processes can offload much of the
tedious bookkeeping common to multi-process program code. A pool attempts to
keep a large buffer of past proteins available -- the Platonic pool is
explicitly infinite -- so that participating processes can scan both backwards
and forwards in a pool at will. The size of the buffer is implementation
dependent, of course, but in common usage it is often possible to keep
proteins in a pool for hours or days.

With so much data available to potentially legion pool participants, pool
behavior optimization becomes important. We have implemented a number of
optimized caching and storage schemes that make it possible to accommodate
tens of thousands of proteins in a pool, and we use careful connection pooling
on the unix operating system to handle hundreds of concurrent connections with
relatively low overhead. As with slawx, we expect eventually to implement many
of these algorithms in hardware, leading to further efficiency and
scalability.

The most common "style" of pool usage in our programs hews to a biological
metaphor, in contrast to the mechanistic, point-to-point approach taken by
existing inter-process communication frameworks. The name "protein" alludes to
biological inspiration: data proteins in pools are available for flexible
querying and pattern matching by a large number of computational processes, as
chemical proteins in a living organism are available for pattern matching and
filtering by large numbers of cellular agents.


# Slawx represented as YAML

Although pools are very useful for communicating proteins from one process to
another, and can also be used for storage of proteins, it's also sometimes
useful to store proteins (or other slawx) in a traditional file.

The functions in slaw-io.h support reading and writing slawx to and from
files. Two formats are supported: a binary format and a text format.

The binary format, apart from an 8-byte header to identify the file, is simply
the slawx themselves, since slawx are designed to be a binary serialization
format.

The text format is based on YAML. (See www.yaml.org.) YAML tags are used to
indicate the slaw types, to make sure that the slawx can be reproduced with
complete fidelity. Some of the tags are in the standard namespace
"tag:yaml.org,2002:" (indicated by "!!") while other tags are in Oblong's own
namespace "tag:oblong.com,2009:slaw/" (indicated by "!"). Here are how the
slaw types are represented with YAML tags:

## Atomic numeric types

- int8    - !i8
- int16   - !i16
- int32   - !i32
- int64   - !i64
- unt8    - !u8
- unt16   - !u16
- unt32   - !u32
- unt64   - !u64
- float32 - !f32
- float64 - !f64

## Other atomic types

- nil     - !!null
- boolean - !!bool
- string  - !!str
- protein - !protein for standard proteins, or !nonstd for nonstandard proteins

  !protein is a map containing the keys "ingests" and "descrips" (both of which
  are lists) and optionally the key "rude_data" (which has tag `!!binary` and is
  the base64-encoded rude data)

  !nonstd is the binary data of the nonstandard protein encoded as base64

## Generic containers

- cons - !cons (encoded as a map with one entry - car is key and cdr is value)
- map  - !!omap (can use either !!map or !!omap on input, but !!omap is always
  used on output, because slaw maps are ordered)
- list - !!seq

## Numeric containers

- complex     - !complex
- vector      - !vector
- multivector - !multivector
- array       - !array

  (Except for empty arrays, since you can't tell the type of the elements when
  there are no elements. So these are represented like `!empty/u64` or
  `!empty/vector/4/complex/i16` or `!empty/multivector/5/f32`)
