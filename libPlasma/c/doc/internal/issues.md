_The following list of issues and desires is dated May 4, 2012 and does
not represent current issues/desires but should also not be forgotten as
(as of February 2020) we have had little Plasma oversight and focus since
Patrick Pelletier's departure in 2012. If, in the future, any of the
following become priorities again, the corresponding bugs should be moved
to gitlab._

----

# Current issues:

- <s>encryption ([bug 3347](https://bugs.oblong.com/show_bug.cgi?id=3347))</s>
  and authentication ([bug 536](https://bugs.oblong.com/show_bug.cgi?id=536))
- asynchronous API ([bug 315](https://bugs.oblong.com/show_bug.cgi?id=315))
- copy monstrousness
- ichor refactor
- metadata ([bug 4457](https://bugs.oblong.com/show_bug.cgi?id=4457))
- pool mirroring ([bug 535](https://bugs.oblong.com/show_bug.cgi?id=535))
- UDP pool server ([bug 865](https://bugs.oblong.com/show_bug.cgi?id=865))
- IPv6 (<s>[bug 232](https://bugs.oblong.com/show_bug.cgi?id=232)</s>)

# Desired new language bindings:

- Python
- Lua
- Flash
- Javascript (V8)

----------------------------------------------------------------------

# Older items / nitpicks

low-level tools for synthetic creation of pools (setting timestamps,
etc), validity-checking a pool, perhaps doing this efficiently without
rebuilding an index, rebuilding an index. (bug 369, bug 1156)

- [bug 921](https://bugs.oblong.com/show_bug.cgi?id=921),
  [bug 1050](https://bugs.oblong.com/show_bug.cgi?id=1050) - Andy's pool
  discovery feature requests
- [bug 1092](https://bugs.oblong.com/show_bug.cgi?id=1092),
  [bug 1093](https://bugs.oblong.com/show_bug.cgi?id=) - mmap pool performance
- [bug 1290](https://bugs.oblong.com/show_bug.cgi?id=1290) - pool_nth_protein
  and negative indices
- [bug 1473](https://bugs.oblong.com/show_bug.cgi?id=1473) - should we
  eliminate hierarchical pools?

* Should coercion of 2-vectors to 3-vectors
  (<s>[bug 329](https://bugs.oblong.com/show_bug.cgi?id=329)</s> comment 4) be a
  special case, or should we more generally allow coercion of a
  smaller vector into a larger vector?

* Should we resurrect libPlasmaYaml, effectively undoing commit
  e7e6f3ec1af5ebb258f9d96648b2a3c231d723a3?

* Should Unicode characters, and/or "special" ASCII characters be
  allowed in pool names?
  (<s>[bug 1592](https://bugs.oblong.com/show_bug.cgi?id=1592)</s>)  Currently
  not supported since no one has said we need them.  But might need to revisit
  this later if internationalization is important.
