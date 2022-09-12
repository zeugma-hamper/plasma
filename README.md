# Plasma

Plasma is an open-source system for platform- and language-independent data
encapsulation and transmission.

Arbitrarily complex -- in the senses of aggregation and nesting -- data
structures are represented by objects called 'Slawx' (the plural of 'Slaw')
and, when intended for cross-process relay, by elaborated Slawx called
'Proteins'. Slawx and Proteins are schemaless but self-describing, meaning
that a recipient can interrogate any of these objects to discover its
structure, the component data types at each sublocation, and of course the
data itself.

A process deposits Proteins into, and retrieves Proteins from, network-soluble
ring buffers called 'Pools'. Multiple processes can be simultaneously
connected (via 'Hoses') to a single pool, with each both depositing and
retrieving Proteins. The ordering of Proteins stored in a Pool is monotonic
and immutable, such that all retrieving processes observe the same
sequence. Processes most typically read from Pools in something like real
time, with Proteins being retrieved immediately after being deposited; but
Pools are also 'rewindable' so that, for example, a new process joining a
distributed system might attach to a Pool already in use and begin reading
Proteins from a time far enough in the past to be able to reconstruct system
context.

The Plasma framework embodies a philosophy of system design that appeals to an
endocrinology (rather than telegraphy) metaphor. The name 'Plasma' accordingly
refers not to the superheated, ionized stellar substance but rather to the
liquid medium by which biological organisms' messaging molecules are
transported and diffused.

# building the thing

Building Plasma requires:

- ninja
- cmake
- libyaml
- boost
- icu4c
- openssl

Use your package manager (brew, apt, yum, zypper, etc) to install them.

To build on linux/intel mac, assuming you're in the same directory as this README:

- `mkdir build`
- `cd build`
- `cmake -GNinja ..`
- `ninja`

Building on arm macs is a bit more complicated:

- `brew install ninja cmake libyaml boost icu4c openssl`
- `export CXXFLAGS="-I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/icu4c/include"`
- `export CFLAGS="-I/opt/homebrew/opt/openssl@3/include -I/opt/homebrew/opt/icu4c/include"`
- `export LDFLAGS="-L/opt/homebrew/opt/openssl@3/lib -L/opt/homebrew/opt/icu4c/lib -L/opt/homebrew/lib"`
- `mkdir build`
- `cd build`
- `cmake -GNinja ..`
- `ninja`

