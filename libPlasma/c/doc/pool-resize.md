# Resizable mmap pools #      {#ResizableMMAPPools}

This article documents the design of resizable mmap pools, since it is
tricky and non-trivial.

# Background

Mmap pools have a "backing file" which is mapped into the process's
address space.  At the beginning of the backing file is a header, and
all of the rest of the file after the header is used for the storage
of proteins, in a ring-buffer fashion.

The header contains, among other things, a pointer (in the more
general sense of the word, not the C sense) to the oldest and newest
proteins in the ring buffer.  These two pointers are each stored as a
64-bit integer offset from the beginning of the backing file.
However, this offset (also called an "entry") is interpreted modulo
the size of the mmap file.  In other words, imagine a hypothetical
virtual address space where the backing file repeats over and over
again, indefintely.  The offsets are stored this way so that they can
always increase, and never decrease.  (This assumes that we will never
use a pool to the point where a 64-bit integer overflows.  Although
this is slightly disconcerting, and it's certainly worth heeding the
lessons of history with regard to "640K is enough for anyone", in
practice it should take hundreds of years of continuous pool use
before this number overflows, so we should be safe.)  So, even though
the physical storage of proteins periodically wraps around and
overwrites itself in ring-buffer fashion, the offsets are always
increasing.  That means that (except for the special conditions
documented below), the "newest" offset is always greater than the
"oldest" offset.

(This doesn't mean we can ignore the wrapping, though.  We still have
to handle things specially when we wrap, in order to skip over the
header, since the header occurs repeatedly in this infinite address
space, too.)

There are a couple of special cases.  When the pool is empty (before
any proteins have been deposited), the "newest" offset is 0, and the
"oldest" offset points to the space right after the header.  The other
special case is when the pool is "temporarily" empty.  This happens
when the protein being deposited is so big that it is going to
overwrite all the existing proteins in the pool.  (And in, fact the
old protein disappear shortly before the new protein appears, leaving
a brief period when the pool is "temporarily" empty, but we know it
will become non-empty again in a bounded amount of time.)  In the
"temporarily" empty case, the newest offset is less than the oldest
offset (although it can't be 0, since that would be the "normal" empty
case).

When attempting to read from a pool which is temporarily empty, the
pool code simply keeps trying until the pool is no longer temporarily
empty.

Depositing to the pool is protected by a lock, so when you are
depositing to the pool, there is no need to worry about anyone else
depositing to it.  However, it is still possible for there to be one
or more readers, even while a deposit is going on.  The read path is
allegedly "lockless", and indeed it does not use any OS locks.
However, the "temporarily empty" case could be considered a sort of
homemade spinlock.  But that is an unusual case, and normally the read
path does indeed proceed without having to wait on any locks.  There
is a mechanism (known affectionately as "stompled") to determine
whether data has become invalid while being read.

# Resize design

Resizing a pool while it is being read is a little tricky, especially
since the read path is lockless.  However, the solution comes from the
fact that the read path is not entirely lockless-- the "temporarily
empty" case is essentially a spinlock, as we have just observed.
Therefore, we can mark the pool temporarily empty, and then feel free
to move proteins around while we resize the pool, and all the other
threads will spin while we do this, and then we can mark the pool as
no longer empty when we're done.

We want to maintain the invariant that the oldest and newest offset
are always increasing.  This way, if a reader starts reading a protein
before the pool is resized (and doesn't encounter the "temporarily
empty" case-- there's no guarantee that it will, since the thread
might not be sheduled during that time) but the resize happens before
it is finished reading, we want the pre-resize data to appear
"stompled".  This is fairly straightforward, although requires a
little bit of math, since the new offsets will be interpreted
according to a new modulus, because the pool size has changed.  So, we
need to calculate the new values of the newest and oldest offsets such
that:

* they are equal to the new physical locations of the proteins in the
  pool, mod the new size
* they are greater than the previous values

When converting an offset (or entry) to an actual address we can
dereference, we must check whether the pool has changed since.  If the
size has changed (espeically if it has gotten larger), we much re-mmap
the backing file, so that we once again have the whole pool in our
address space.

Making a pool smaller presents a slightly different problem.  On UNIX,
if you decrease the size of a file while it is mmapped, addresses
beyond the end of the file will cause SIGBUS if they are
dereferenced.  Although our "stompled" mechanism means we don't have
to worry about reading bad data, it does make the assumption that we
can at least read the bad data, and decide at a later point in time
that it was bad.  If we get a SIGBUS, we are in trouble.

On Windows, the situation is slightly different.  Windows simply
doesn't allow you do reduce the size of a file (i. e. you will get an
error, and the file will not be resized) if anyone has it mapped.  So,
to accommodate these issues on both UNIX and Windows, we only resize
the backing file right away if the pool is getting larger.  If the
pool is getting smaller, we make the "effective" size of the pool
smaller (i. e. we are using a smaller part of the file), but we don't
resize the backing file right away.  When closing the last hose, we
check if the pool has shrunk, and if so we resize the backing file at
that time.

Another case we have to deal with is what happens if the pool becomes
too small to contain the most recent protein.  If that happens, the
pool becomes empty.  We represent this the same way as the "initially
empty" case, by having "newest" be 0.  One tricky thing here is that
when depositing a protein, we normally give it an index which is one
greater than the previous protein.  But if the pool is empty, there is
no previous protein, so the first protein to be deposited gets the
index 0.  But if the pool has been made empty by resizing, we don't
want the index to reset to 0.  So, we hack around this by having a
special field in the header to keep track of what the next index
should be.  We only need to use this value if the pool is empty;
otherwise we keep doing it the way it was always done.

Speaking of "index" (although a different sense of the word; yay for
ambiguous terminology), if the pool is indexed, we need to update the
index, too, since all the offsets have changed.  Although there are
more subtle and probably faster ways to do this, by adjusting the
offsets in the existing index, we instead choose a much simpler
brute-force technique where we erase the index and rebuild it from
scratch, as we walk though all the proteins in the pool.  (The
rationale for keeping it simple, even at the expense of performance,
is that more subtle techniques create opportunities for more subtle
bugs.)
