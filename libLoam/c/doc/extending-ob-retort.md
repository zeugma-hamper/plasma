# Extending ob_retort #     {#ExtendingObRetort}

ob_retorts are extensible. Although a few very common ob_retorts are defined
in \ref libLoam/c/ob-retorts.h, most libraries define their own retorts.

First, a range of ob_retorts needs to be allocated for the library. This is
done in \ref libLoam/c/ob-retorts.h. Look for the comment "add additional
ranges for other libraries here". We're currently allocating 100000
ob_retorts to each library, which should be plenty.

Once a range has been allocated in \ref libLoam/c/ob-retorts.h, the library
should have its own header file to define its retorts. This is typically of
the form `libFoo/FooRetorts.h`. In this file, you should `#define` your custom
retorts in terms of `OB_RETORTS_FOO_FIRST`, keeping in mind that you only have
100000&dagger; retorts, so don't add a number larger than 99999.

A library with custom retorts needs to register its retorts so they can be
converted to strings. This is traditionally done in a file
`libFoo/FooRetorts.cpp`. This file needs to define a function which, if given
a retort that the library recognizes, returns a string literal for that
retort, and returns NULL otherwise. This can be done any way that makes you
happy, but traditionally it is done with a switch statement and a little
preprocessor stringification magic:

```
static const char *foo_error_string (ob_retort err)
{
#define E(x) case x: return #x
  switch (err)
    {
      E (MY_CUSTOM_RETORT);
      ...
    default: return NULL;
    }
#undef E
}
```

Now you need to register this function, so libLoam will know to call it. This
is done by calling the function \ref ob_add_error_names() at global
constructor time, like this:

```
static ob_retort dummy_foo = ob_add_error_names (foo_error_string);
```

For shared libraries, this is all that is necessary. But for static libraries,
the object file containing this global constructor won't be pulled in, because
nothing depends on it. In order to work with static libraries, it's necessary
to create an artificial dependency to pull in the object file. To do this,
declare the symbol `ob_private_hack_pull_in_foo_retorts` in `FooRetorts.h`,
define it in `FooRetorts.cpp`, and reference it from some other file in the
library that you already know is getting linked in.

\internal
Unfortunately, a small amount of manual effort is necessary to keep the
`FooRetorts.h` and `FooRetorts.cpp` files in sync. However, it's possible to
automatically check that they are in sync. For existing `*Retorts.[Ch]` files,
this is done by running `libLoam/c/check-retorts.rb`. If you add custom
retorts to your library, and follow the conventions (such as using the "E"
macro) described above, then you can add your library to `check-retorts.rb`,
so that it is checked, too.
\endinternal

&dagger; Observant readers will note that each library actually gets 200000
retorts: 100000 success codes, and 100000 failure codes, because if you are
allocated the following space:

```
#define OB_RETORTS_FOO_FIRST  900000
#define OB_RETORTS_FOO_LAST   999999
```

You actually get 900000 to 999999 for success codes, and -900000 to -999999
for failure codes. There is no relationship implied between a success code and
a failure code with the same absolute value.

If there is no single file that always gets linked in, you may need to
reference the symbol from several files. The most general approach would be to
reference the symbol from every file in the library, but usually that's
overkill.