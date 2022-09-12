# libLoam #         {#libloam_doc}

Oblong's c and c++ libraries containing basic type definitions (defined in the
c portion of the library) and higher-level object definitions (defined in the
c++ portion).

# Introduction and Tutorial #   {#loam_intro}

# Basic Types #   {#loam_types}

Code is more durable and long-lasting if one is specific about the size of
one's data. With today's machines, there's enough memory to use a larger type
if one is not sure about size. The plasma and slaw system allows one to be
fully specific about the size and nature of data types. There is always a
complex sort available.

int/unt {8, 16, 32, 64}. float {32, 64}. {2, 3, 4}-component c-vects {i, u,
f}. {2, 3, 4, 5}-dimensional multivectors. Multivectors are a version of
Grassmann (or exterior) algebra used to describe geometry vector space.
Multivectors are powerful when dealing with geometry as well as geometric
manipulations and constructs. Multivector representations are very compact,
leading to shorter code, and more compactly-expressed ideas—the next step
beyond vector-based manipulation.

# Logging #       {#loam_logging}

This is the low-level facility for logging. C and c++ levels of logging are
available.

# ObRef #         {#loam_obref}

This is our memory-managed reference type. This construct shares concepts with
smart pointers. \ref oblong::loam::ObRef "ObRef" needs to interact with a
number of subsystems in the platform.

ObRefs are not often used
directly, but they are the building blocks upon which lists and maps are
built.

The following code makes a new raw pointer and then wraps it in an \ref
oblong::loam::ObRef "ObRef":

    ExampleType *tq = new ExampleType;
    ObRef<ExampleType *> tq_r (tq);

This leaves open the kind of memory management that is going on. There are
managed and unmanaged ObRefs and there are weak and regular references that
use reference counting. Eventually garbage collection is to be folded into
\ref oblong::loam::ObRef "ObRef". Once the \ref oblong::loam::ObRef "ObRef" is
made from a raw pointer, a second is made from the first:

    ObRef<ExampleType *> tq_r2 = tq_r;

Then, a weak reference is made from the first (a passive view into an object
that can determine when the object might be deleted):

    ObWeakRef<ExampleType *> tq_wr (tq_r);

Now, if the reference goes out of scope it suddenly points to null. This is
the only other place in the platform where operator overloading is used. The
"~" asks for the pointer back:

    (~tq)->SetBackingColor (lurid_red);

Assign a new ExampleType to the original reference:

    tq_r = new ExampleType;

If it's being reference counted, the count is reduced to from two to one. And
once the second reference is set to null, the reference count is reduced to
zero:

    tq_r2 = NULL;

    if (! ~tq_wr)
      fprintf (stderr, “tq has snuffed it\n”);

When the reference count is reduced to zero, the original
ExampleType is deleted, but may still use the delayed
deletion and resource reclamation built into the platform.

# ObTrove #     {#loam_obtrove}

[ObTrove](\ref oblong::loam::ObTrove) is our list type. In typical fashion,
[ObTrove](\ref oblong::loam::ObTrove) can be appended to, prepended to, iterate
over itself, insert elements, swap elements, and so forth.

[ObTrove](\ref oblong::loam::ObTrove) takes the particular kind of memory
management one wants to invoke. The default invocation is reference counting.
Other examples are weak references, and no references at all.

    ObTrove <ExampleType *> tq_lst;
    tq_list . Append (new ExampleType);
    ...
    ObTrove <ExampleType *, RefCountWrap>
      tq_ref_cnt_lst;

    ObTrove <ExampleType *, WeakRef> tq_weak_lst;

    ObTrove <ExampleType *, NoMemMgmt> tq_raw_lst;

# ObCons and ObMap #        {#loam_cons_map}

Our cons types are key value pairs assembled into maps. ObMaps can be
conditioned to have one of three behavior types: they can guarantee unique
keys privileging the first key, they can guarantee keys privileging the most
recent key, or the map is used as an ordered list of key value pairs that
allow for key duplication. The most common case is the default (unique, most
recent).

- [ObMap](\ref oblong::loam::ObMap)
- [ObCons](\ref oblong::loam::ObCons)

# ObCrawl #         {#loam_obcrawl}

[ObCrawl](\ref oblong::loam::ObCrawl) is our take on iterators. The
[ObCrawl](\ref oblong::loam::ObCrawl) is a single iteration object that includes
a begin and end fencepost as an implementation, but also allows composition that
iterators with a specified begin and end cannot normally allow. The objects
appear are monolithic, so they can be chained together with a few basic
commands: chain snaps two together; retro turns an
[ObCrawl](\ref oblong::loam::ObCrawl) around back to front—what was in the fore
is now in the aft; and zip which takes between 2 and 21 ObCrawls and meshes
them together.

Any trove can be issued the crawl method, which returns an
[ObCrawl](\ref oblong::loam::ObCrawl). A few operations can be performed on the
[ObCrawl](\ref oblong::loam::ObCrawl): one can ask if it is empty, and as long as
it's not empty one can pop either from the front or the back. One can also do
it without popping if one wants to repeatedly access it, and one can also
reload it:

    ObCrawl <ExampleType *> crl = tq_lst.Crawl ();

    while (! crl.isempty ())
      if (ExampleType *tq = crl.popfore ())
        tq -> SetTranslation (...);
    crl.reload ();

The code below creates a synthetic crawl from two crawls (a and b) by chaining
a and b. The function runs through a then runs through b:

    ObCrawl <ExampleType *> ca, cb;
    ...
    ObCrawl <ExampleType *> synth_crl
      = ca.chain (cb);

One could also run through from b to a, or interleave fore and aft calls to
nibble in from either side.

Take a and zip it with b:

    synth_crl = ca.zip (cb);

Take a and zip it with itself reversed, then concatenate it onto b:

    synth_crl = cb.chain
      (ca.zip (ca.retro ()));

# ObRetort #        {#loam_obretort}

[ObRetort](\ref oblong::loam::ObRetort) is our error signaling mechanism. It
has c and c++ manifestations. In c, it is an int64. We have put together a
signaling system in which we think not just errors should be reported, but
also information-rich success values. Hence: negative numbers are error values
and positive numbers are success values. For example, reaching the end of a
list would generate a specific success value. It is not an error to reach the
end of the list, but a null value at the end of a list should be reported as
such. Most things in the platform that don't need to return something else
return an ObRetort. A value of zero is "OB OK."

See @subpage ExtendingObRetort "here" for how to extend ob_retort for your application or library.

# Strings #     {#loam_str}

[Str](\ref oblong::loam::Str) is our string class. Our string class is
completely unicode compliant. The platform automatically handles up and down
conversion from c strings, and has built-in regular expression searching.

# ObColor #     {#loam_obcolor}

[ObColor](\ref oblong::loam::ObColor) allows us to talk about color at the loam
level, even though we aren't drawing (or painting) anything yet.

# Geometry #        {#loam_geometry}

Geometry is built at this level. [Vect](\ref oblong::loam::Vect) is a three
element vector. The quaternian ([Quat](\ref oblong::loam::Quat)) is used for
rotation. [Vect4](\ref oblong::loam::Vect4) is a four element vector.
Four-by-four matrices are critical for doing general 3-space affine and
non-affine transformations ([Matrix44](\ref oblong::loam::Matrix44)).

- oblong::loam::Matrix44
- oblong::loam::Quat
- oblong::loam::Vect
- oblong::loam::Vect4
