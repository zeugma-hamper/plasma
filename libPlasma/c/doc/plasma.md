# libPlasma #         {#libplasma_doc}

# Oblong's C library for distributed computing

# Introduction and tutorial #     {#libplasma_intro}

Pools can be viewed as a transport and immutable storage mechanism for
proteins, linearly ordered by time deposited. Proteins are something like data
structures, but they are more flexible and divided into two parts. Slawx are
data units, with the ability to store multiple types of data.

# Slawx #     {#libplasma_slawx}

Slawx (plural for "slaw") are the lowest level of libPlasma and constitute the
core serialization API for the plasma system. The term slaw encapsulates the
idea of a self-describing, highly structured, strongly typed (but arbitrarily
composed) data unit.

A slaw can be an unsigned 64-bit integer, a complex number,
a vector, or a string. The data can be either atomic or composite, with both
homogeneous (array) and heterogeneous (lists, maps) containers. They're the
construct used to embed typed data inside proteins. A more complete list of
types implemented as slawx can be found in \ref ob-types.h and \ref
slaw-numeric-ilk-rumpus.h. Let's create a few of them.

```{cpp}
slaw s = slaw_int32 (235);
slaw r = slaw_unt64 ((unt64) (5 * pow (10,10)));
// unsigned and signed version available in 8-, 16-,32- and 64-bit variety
v4float64 v; v.x = 3.5; v.y = 6.4; v.z = 132.6; v.w = 10;
slaw t = slaw_v4float64(v);
slaw l = slaw_string ("My Life in the Bush of Ghosts"); //google it
slaw sub = slaw_string_from_substring ("oblong", 2); // "ob"
```

So no matter what we're living in a C world, and we have to clean up after
ourselves. Also, how the heck do we get our data out?

```{cpp}
// knowledge of the contents is helpful
const unt64 *ref = slaw_unt64_emit (r);
// or
unt64 dup = *(slaw_unt64_emit (r));
// or
unt64 dup2 = *(slaw_unt64_emit (r));

slaw_free (r);
// here dup and dup2 is still usable, but ref is not.
```

Arrays are of a single type and can be initialized as filled, empty, or with
a type array of the same size as the slaw array.

```{cpp}
int size = 64;
int value = 5;
// oh joy, an array of sixty-four fives.
slaw arr = slaw_int32_array_filled (size, value);
const int32 *elem = slaw_int32_array_emit_nth (4);
// clean up; same as simple slawx from earlier.
slaw_free (arr); // memory released

unt64 *sixtyfourbitgoodness = (unt64 *)malloc (64 * sizeof (unt64));
for (int i = 0; i < 64; i++)
  sixtyfourbitgoodness[i] = 5 * i;

// now the slaw array
arr = slaw_unt64_array (sixtyfourbitgoodness, 64);
free (sixtyfourbitgoodness);
slaw_free (arr);
```

Now we know how to make slaw and retrieve data and clean up our memory
allocations. Oh right, I almost forgot lists. Lists, unlike arrays, can hold
slawx of different types. They are built using SlawBundles.

```{cpp}
// ah! a fresh SlawBundle.
slabu *sb = slabu_new ();
slabu_list_add_f (sb, slaw_unt32(2356));
slaw line = slaw_string ("New Orleans Piano Professor"); // google
slabu_list_add_x (sb, line);

// at this point only a ref to line is held.
// if we free line before we make a slaw out of the bundle, we'll seg fault.
slabu_list_remove (sb, line);
// on the other hand, we can copy line right away.
slabu_list_add (sb, line);
slaw_free (line);
slabu_list_add_f (sb, slaw_int64 (-193247519234));
// now to get a usable slaw out of this experience
slaw list = slaw_list (sb);
// or two
slaw xerox = slaw_dup (list);

// cleanup using the same method
slaw_free (list);
slabu_free (sb);
```

And now for retrieval:

```{cpp}
int64 howmany = slaw_list_count (xerox);
bslaw which = slaw_list_emit_nth (xerox, howmany - 1);
if (which != NULL)
  // do the magic.
```

The most efficient way to iterate over a slaw list is:

```{cpp}
bslaw cole;
for (cole = slaw_list_emit_first (xerox);
     cole != NULL;
     cole = slaw_list_emit_next (xerox, cole))
  {
    slaw_spew_overview_to_stderr (cole);
    fprintf (stderr, "\n-----\n");
  }
```

Also, there's cons, car, and cdr for lisp-esque lists, but they aren't the
same as lists created with slaw_list. More typically, cons is used as a
key/value pair in a map. Note: `slaw_cons_ff ()` frees its slaw arguments,
while `slaw_cons ()` allows them to live. That's about it for slawx.

See also \ref PlasmaSlawGroup.

# Proteins #      {#libplasma_proteins}

From slawx, we create proteins. I think of them like a data structure. They
have two parts: descrips and ingests. Descrips are supposed to be slaw
strings, and ingests are supposed to be key-value pairs, where the key is a
string. This facilitates access. Currently it is not forced on the user.

```{cpp}
protein p = protein_from_ff (slaw_list_inline_c ("dis pdu", NULL),
                             slaw_map_inline_cf ("id", slaw_unt8 (1234),
                                                 "radioid", slaw_unt16 (10),
                                                 NULL));
```

And for cleanup:
```{cpp}
protein_free (p);
```

We have a variety of functions to access the slawx in the protein. First the
necessary:

```{cpp}
bslaw d = protein_descrips (p);
bslaw i = protein_ingests (p);
```

# Pools #     {#libplasma_pools}

A "pool" is a repository for proteins, providing linear sequencing and state
caching; multi-process access; and a set of common, optimizable filtering and
pattern-matching behaviors.

A pool is a generalization of the end-to-end communications channel. A pool is
a multipoint of contact. Contacts can be ephemeral, so other processes can
join and leave a pool without anyone else caring or needing to notice. Pools
are conceptually infinite, but implemented as ring buffers. The result is a
networking formalism that is essentially rewindable. This idea flows into our
modern program construction from thinking about things biologically.

The idea is that we can de-couple processes and bring them back together, such
that only a few properties are important. First, monotonically increasing
order. With n objects depositing proteins within the pool, we may not know
where the proteins come from—the depositors may be spread around the network
or may be on different cores. We may not be able to predict which protein gets
into the pool first, but once a protein is there, it is always in a known,
permanent, and unalterable order with respect to other proteins. That means
that everyone reading proteins out of a pool is guaranteed to see the proteins
in the same order.

The rewindability is important as well. Instead of having to write a huge
amount of boilerplate code to allow for every possibly contingency, the idea
of pools is to have a simple check for the extraordinary case, and to be able
to back up and figure out how you got there. If one wants to know what was the
event context that ended in an extraordinary state, one should be able to load
dynamic libraries after rolling back to allow interpretation of events going
forwards. This articulates the need for a rewindable networking
component—which the pools are.

Pools look a lot like circular queues of proteins. A pool can be viewed as a
sliding window on a stream of proteins; the size of the window is (roughly)
the size of the pool, and the leading edge of the window keeps up with the
most recently deposited protein. Pools are persistent across system reboot and
exist independently of any particular process. Multiple processes can access a
pool simultaneously.

Pools begin life as an empty queue with pool_create() and end it with
pool_dispose(). Proteins can be added to the end of the queue via
pool_deposit(); they can be read via pool_nth() and a host of associated
functions. Pools are fixed size; once a pool is full new deposits will
overwrite the oldest deposits. When a protein is deposited, it gains an index
(starting at 0 and incrementing on each deposit) and a timestamp denoting the
time of deposit.

Pools are implemented such that deposits don't block on reads and reads are
best-effort. In particular, if a reader would like to read every single
protein deposited in the pool, and proteins are deposited faster than the
reader can read them, the reader will miss some proteins. This case can be
detected by looking for gaps in the protein indexes.

To talk to a pool, you'll need to connect a pool hose to it via
pool_participate() (or pool_participate_creatingly()). A pool hose keeps track
of its position in the pool (in terms of protein indexes), much in the same
way as a file descriptor keeps track of its position in the file (in terms of
bytes). Usually this pool index is the index of the protein following the
protein just read, but it can be set directly or indirectly otherwise by other
other pool functions (such as pool_set_index(), pool_prev(), etc.). Pool hoses
can only be used by one thread at a time, much like file descriptors.

# Plasma Utilities #    {#libplasma_utilities}

The libPlasma directory contains a selection of command line utilities for
working with pools (p-create, p-deposit, etc.), each of which is itself a good
example of how to use pools. First, a command-line example:

```{bash}
# Create a pool named "test"
$ ./p-create test

# Verify there are no proteins in it
$ ./p-newest-idx test
pool is empty

# Deposit a protein
$ ./p-deposit -d my_packet -i my_key:my_value test

# Check the index of the newest protein
$ ./p-newest-idx test
0

# Print out the first protein in the pool
$ ./p-nth test 0
time 1256603013.587510, size 96 - this protein:
slaw[12o.0x602150]: PROT: ((
descrips:
slaw[4o.0x602160]: LIST (1 elems): {
 1: slaw[3o.0x602168]: STR(9): "my_packet"
 }
ingests:
slaw[6o.0x602180]: MAP (1 elems): {
 1: slaw[5o.0x602188]: CONS:
 1:  L: slaw[1o.0x602190]: STR(6): "my_key"
 1:  R: slaw[3o.0x602198]: STR(8): "my_value"
 }
 ))

# We're done with the pool, destroy it
$ ./p-stop test
```

A common pool usage pattern is that of a long-running process that reads and
processes every protein deposited to the pool. The p-await command line
utility simply prints every protein deposited to a pool and looks more-or-less
like so:

```{cpp}
while (1)
  {
    ob_retort pret = pool_await_next (cmd.ph, 0, &p, &ts, NULL);
    if (pret < OB_OK)
      { pool_withdraw (cmd.ph);
        fprintf (stderr, "problem with pool_await_next(): %s\n",
                 ob_error_string (pret));
        return EXIT_FAILURE;
      }
    slaw_spew_overview (p, stdout, NULL);
    protein_free (p);
  }
```

\internal

For an example of advanced usage of pool read and navigation functions, see
seek_test.c.

\endinternal

See also \ref PlasmaPoolsGroup.

# MemoryManagement #      {#libplasma_memmgmt}

The slaw API contains multiple versions of each function, which make
it easy to free data you don't need anymore.

The basic idea of slaw memory management is that there is a
multicharacter suffix on the function name, where each character
corresponds to an argument.  "l" means "leave alone" (don't free) and
"f" means free.  If there is no suffix, then it means all "l".  You
can also tell this from the type signature, too, since "bslaw" is a
borrowed slaw (which won't be freed), while "slaw" means ownership is
being passed (so it can be freed).

There is also the "c" suffix, which means the argument is a string (a
`const char*`), and a string slaw will be automatically created and
freed behind the scenes.

For example, here are all the variants of cons:

```{cpp}
slaw slaw_cons (bslaw car, bslaw cdr);
slaw slaw_cons_ff (slaw car, slaw cdr);
slaw slaw_cons_lf (bslaw car, slaw cdr);
slaw slaw_cons_fl (slaw car, bslaw cdr);
slaw slaw_cons_cl (const char *car, bslaw cdr);
slaw slaw_cons_cf (const char *car, slaw cdr);
slaw slaw_cons_lc (bslaw car, const char *cdr);
slaw slaw_cons_fc (slaw car, const char *cdr);
slaw slaw_cons_cc (const char *car, const char *cdr);
slaw slaw_cons_ca (const char *car, const unt8 *cdr, int64 N);
```

# Going beyond

For a more academic overview of c plasma, check out \subpage libplasma_more

For an overview of the Slaw API, check out \ref PlasmaSlawGroup

For an overview of the Pool API, check out \ref PlasmaPoolsGroup

And for plasma helper functions, check out \ref SlawHelpers

\defgroup PlasmaSlawGroup Slaw API
\defgroup PlasmaPoolsGroup Pool API
