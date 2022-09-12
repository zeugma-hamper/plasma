
/* (c)  oblong industries */

/* This file is not for redistribution.  Even if you obtained a copy
 * of it by downloading the Greenhouse SDK, you may not redistribute
 * this or any part of the Greenhouse SDK without written permission
 * from oblong industries.
 */

#define FOR_ALL_NUMERIC_INTEGER_BASICS(M, PRE, POST, DUFFEL)                   \
  M (PRE##int8##POST, int8, DUFFEL);                                           \
  M (PRE##int16##POST, int16, DUFFEL);                                         \
  M (PRE##int32##POST, int32, DUFFEL);                                         \
  M (PRE##int64##POST, int64, DUFFEL);                                         \
  M (PRE##unt8##POST, unt8, DUFFEL);                                           \
  M (PRE##unt16##POST, unt16, DUFFEL);                                         \
  M (PRE##unt32##POST, unt32, DUFFEL);                                         \
  M (PRE##unt64##POST, unt64, DUFFEL)
#define FOR_ALL_NUMERIC_FLOAT_BASICS(M, PRE, POST, DUFFEL)                     \
  M (PRE##float32##POST, float32, DUFFEL);                                     \
  M (PRE##float64##POST, float64, DUFFEL)

#define FOR_ALL_NUMERIC_BASICS(M, PRE, POST, DUFFEL)                           \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, PRE, POST, DUFFEL);                       \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, PRE, POST, DUFFEL)

#define FOR_ALL_NUMERIC_INTEGER_VECTS(M, POST)                                 \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v2, POST, 2);                             \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v3, POST, 3);                             \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v4, POST, 4)

#define FOR_ALL_NUMERIC_FLOAT_VECTS(M, POST)                                   \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v2, POST, 2);                               \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v3, POST, 3);                               \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v4, POST, 4)

#define FOR_ALL_NUMERIC_2VECTS(M, DUFFEL)                                      \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v2, , DUFFEL);                            \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v2, , DUFFEL);

#define FOR_ALL_NUMERIC_3VECTS(M, DUFFEL)                                      \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v3, , DUFFEL);                            \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v3, , DUFFEL);

#define FOR_ALL_NUMERIC_4VECTS(M, DUFFEL)                                      \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v4, , DUFFEL);                            \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v4, , DUFFEL);

#define FOR_ALL_NUMERIC_234VECTS(M)                                            \
  FOR_ALL_NUMERIC_2VECTS (M, 2);                                               \
  FOR_ALL_NUMERIC_3VECTS (M, 3);                                               \
  FOR_ALL_NUMERIC_4VECTS (M, 4);

#define FOR_ALL_NUMERIC_V234FLOAT_ARRAYS(M)                                    \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v2, _array, 2);                             \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v3, _array, 3);                             \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v4, _array, 4)

#define FOR_ALL_NUMERIC_V234INTEGER_ARRAYS(M)                                  \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v2, _array, 2);                           \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v3, _array, 3);                           \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v4, _array, 4)

#define FOR_ALL_NUMERIC_V234_ARRAYS(M)                                         \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v2, _array, 2);                           \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v2, _array, 2);                             \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v3, _array, 3);                           \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v3, _array, 3);                             \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, v4, _array, 4);                           \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, v4, _array, 4);



#define FOR_ALL_NUMERIC_BASIC_ARRAYS(M, DUFFEL)                                \
  FOR_ALL_NUMERIC_INTEGER_BASICS (M, , _array, DUFFEL);                        \
  FOR_ALL_NUMERIC_FLOAT_BASICS (M, , _array, DUFFEL)

#define FOR_ALL_NUMERIC_TYPES(M)                                               \
  FOR_ALL_NUMERIC_BASICS (M, , , );                                            \
  FOR_ALL_NUMERIC_INTEGER_VECTS (M, );                                         \
  FOR_ALL_NUMERIC_FLOAT_VECTS (M, );                                           \
  FOR_ALL_NUMERIC_BASIC_ARRAYS (M, );                                          \
  FOR_ALL_NUMERIC_V234FLOAT_ARRAYS (M);                                        \
  FOR_ALL_NUMERIC_V234INTEGER_ARRAYS (M)


#define DECLAR_TYPE_ID(T, x, y) ID id_##T
#define INTERN_TYPE_ID(T, x, y) id_##T = rb_intern (#T)

#define CHECK_TYPE_AND_RETURN_SYM(T, x, y)                                     \
  else if (slaw_is_##T (s)) return ID2SYM (id_##T)

#define KERNEL_CONSTRUCTOR_CDEFINE_1(T, x, NUM_ARGS)                           \
  static VALUE rcSlaw_##T (VALUE self, VALUE arg1)                             \
  {                                                                            \
    VALUE args[2];                                                             \
    args[0] = arg1;                                                            \
    args[1] = ID2SYM (id_##T);                                                 \
    VALUE rslaw = rb_class_new_instance (2, args, rcSlaw);                     \
    return rslaw;                                                              \
  }

#define KERNEL_CONSTRUCTOR_CDEFINE_2(T, x, NUM_ARGS)                           \
  static VALUE rcSlaw_##T (VALUE self, VALUE arg1, VALUE arg2)                 \
  {                                                                            \
    VALUE args[2];                                                             \
    args[0] = rb_ary_new3 (NUM_ARGS, arg1, arg2);                              \
    args[1] = ID2SYM (id_##T);                                                 \
    VALUE rslaw = rb_class_new_instance (2, args, rcSlaw);                     \
    return rslaw;                                                              \
  }

#define KERNEL_CONSTRUCTOR_CDEFINE_3(T, x, NUM_ARGS)                           \
  static VALUE rcSlaw_##T (VALUE self, VALUE arg1, VALUE arg2, VALUE arg3)     \
  {                                                                            \
    VALUE args[2];                                                             \
    args[0] = rb_ary_new3 (NUM_ARGS, arg1, arg2, arg3);                        \
    args[1] = ID2SYM (id_##T);                                                 \
    VALUE rslaw = rb_class_new_instance (2, args, rcSlaw);                     \
    return rslaw;                                                              \
  }

#define KERNEL_CONSTRUCTOR_CDEFINE_4(T, x, NUM_ARGS)                           \
  static VALUE rcSlaw_##T (VALUE self, VALUE arg1, VALUE arg2, VALUE arg3,     \
                           VALUE arg4)                                         \
  {                                                                            \
    VALUE args[2];                                                             \
    args[0] = rb_ary_new3 (NUM_ARGS, arg1, arg2, arg3, arg4);                  \
    args[1] = ID2SYM (id_##T);                                                 \
    VALUE rslaw = rb_class_new_instance (2, args, rcSlaw);                     \
    return rslaw;                                                              \
  }

#define KERNEL_CONSTRUCTOR_RDEFINE(T, x, NUM_ARGS)                             \
  rb_define_method (rcKernelModule, #T, RUBY_METHOD_FUNC (rcSlaw_##T),         \
                    NUM_ARGS);

#define FREEZE_INTEGER_BASIC(T, x, y)                                          \
  else if (rtypID == id_##T) s = slaw_##T (NUM2LL (rval))
#define FREEZE_FLOAT_BASIC(T, x, y)                                            \
  else if (rtypID == id_##T) s = slaw_##T (NUM2DBL (rval))


#define FREEZE_FLOAT_VECT(VT, BT, DIMS)                                        \
  else if (rtypID == id_##VT) do                                               \
  {                                                                            \
    int q;                                                                     \
    union                                                                      \
    {                                                                          \
      VT val;                                                                  \
      BT accessor[DIMS];                                                       \
    } u;                                                                       \
    for (q = 0; q < DIMS; q++)                                                 \
      u.accessor[q] = NUM2DBL (rb_ary_entry (rnums, q));                       \
    s = slaw_##VT (u.val);                                                     \
  }                                                                            \
  while (0)

#define FREEZE_INTEGER_VECT(VT, BT, DIMS)                                      \
  else if (rtypID == id_##VT) do                                               \
  {                                                                            \
    int q;                                                                     \
    union                                                                      \
    {                                                                          \
      VT val;                                                                  \
      BT accessor[DIMS];                                                       \
    } u;                                                                       \
    for (q = 0; q < DIMS; q++)                                                 \
      u.accessor[q] = NUM2LL (rb_ary_entry (rnums, q));                        \
    s = slaw_##VT (u.val);                                                     \
  }                                                                            \
  while (0)


#define FREEZE_FLOAT_ARRAY(AT, BT, x)                                          \
  else if (rtypID == id_##AT) do                                               \
  {                                                                            \
    BT *p;                                                                     \
    s = slaw_##BT##_array_raw (len, &p);                                       \
    for (q = 0; q < len; q++)                                                  \
      *(p + q) = NUM2DBL (rb_ary_entry (rnums, q));                            \
  }                                                                            \
  while (0)

#define FREEZE_INTEGER_ARRAY(AT, BT, x)                                        \
  else if (rtypID == id_##AT) do                                               \
  {                                                                            \
    BT *p;                                                                     \
    s = slaw_##BT##_array_raw (len, &p);                                       \
    for (q = 0; q < len; q++)                                                  \
      *(p + q) = NUM2LL (rb_ary_entry (rnums, q));                             \
  }                                                                            \
  while (0)

#define FREEZE_V234FLOAT_ARRAY(AT, BT, DIMS)                                   \
  else if (rtypID == id_##AT) do                                               \
  {                                                                            \
    v##DIMS##BT *array_p;                                                      \
    s = slaw_##AT##_raw (len, &array_p);                                       \
    for (q = 0; q < len; q++)                                                  \
      {                                                                        \
        VALUE rentry = rb_ary_entry (rnums, q);                                \
        int qq;                                                                \
        BT *vect_p = (BT *) (array_p + q);                                     \
        for (qq = 0; qq < DIMS; qq++)                                          \
          vect_p[qq] = NUM2DBL (rb_ary_entry (rentry, qq));                    \
      }                                                                        \
  }                                                                            \
  while (0)

#define FREEZE_V234INTEGER_ARRAY(AT, BT, DIMS)                                 \
  else if (rtypID == id_##AT) do                                               \
  {                                                                            \
    v##DIMS##BT *array_p;                                                      \
    s = slaw_##AT##_raw (len, &array_p);                                       \
    for (q = 0; q < len; q++)                                                  \
      {                                                                        \
        VALUE rentry = rb_ary_entry (rnums, q);                                \
        int qq;                                                                \
        BT *vect_p = (BT *) (array_p + q);                                     \
        for (qq = 0; qq < DIMS; qq++)                                          \
          vect_p[qq] = NUM2LL (rb_ary_entry (rentry, qq));                     \
      }                                                                        \
  }                                                                            \
  while (0)


#define COERCE_TO_INT64(T, x, y)                                               \
  else if (slaw_is_##T (s)) do                                                 \
  {                                                                            \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##T));                            \
    int64 l;                                                                   \
    slaw_to_int64 (s, &l);                                                     \
    return LL2NUM (l);                                                         \
  }                                                                            \
  while (0)

// fix: is there a way to make the 32->64 conversion here
// not lose precision? 23.4, for example, isn't a good number
#define COERCE_TO_FLOAT(T, x, y)                                               \
  else if (slaw_is_##T (s)) do                                                 \
  {                                                                            \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##T));                            \
    const T *f;                                                                \
    f = slaw_##T##_emit (s);                                                   \
    return rb_float_new (*f);                                                  \
  }                                                                            \
  while (0)

#define COERCE_TO_FLOAT_VECT(VT, BT, x)                                        \
  else if (slaw_is_##VT (s)) do                                                \
  {                                                                            \
    int q;                                                                     \
    union                                                                      \
    {                                                                          \
      VT val;                                                                  \
      BT accessor[x];                                                          \
    } u;                                                                       \
    u.val = *slaw_##VT##_emit (s);                                             \
    VALUE rarray = rb_ary_new2 (x);                                            \
    for (q = 0; q < x; q++)                                                    \
      rb_ary_push (rarray, rb_float_new (u.accessor[q]));                      \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##VT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)

#define COERCE_TO_INTEGER_VECT(VT, BT, x)                                      \
  else if (slaw_is_##VT (s)) do                                                \
  {                                                                            \
    int q;                                                                     \
    union                                                                      \
    {                                                                          \
      VT val;                                                                  \
      BT accessor[x];                                                          \
    } u;                                                                       \
    u.val = *slaw_##VT##_emit (s);                                             \
    VALUE rarray = rb_ary_new2 (x);                                            \
    for (q = 0; q < x; q++)                                                    \
      rb_ary_push (rarray, LL2NUM (u.accessor[q]));                            \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##VT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)

#define COERCE_TO_FLOAT_VECT_ARRAY(AT, BT, N)                                  \
  else if (slaw_is_##AT (s)) do                                                \
  {                                                                            \
    int vq, vcnt = N;                                                          \
    int aq, acnt = slaw_numeric_array_count (s);                               \
    VALUE rarray = rb_ary_new2 (acnt);                                         \
    union                                                                      \
    {                                                                          \
      v##N##BT val;                                                            \
      BT accessor[N];                                                          \
    } u;                                                                       \
    for (aq = 0; aq < acnt; aq++)                                              \
      {                                                                        \
        u.val = *slaw_##AT##_emit_nth (s, aq);                                 \
        VALUE vrarray = rb_ary_new2 (vcnt);                                    \
        for (vq = 0; vq < vcnt; vq++)                                          \
          rb_ary_push (vrarray, rb_float_new (u.accessor[vq]));                \
        rb_ary_push (rarray, vrarray);                                         \
      }                                                                        \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##AT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)

#define COERCE_TO_INTEGER_VECT_ARRAY(AT, BT, N)                                \
  else if (slaw_is_##AT (s)) do                                                \
  {                                                                            \
    int vq, vcnt = N;                                                          \
    int aq, acnt = slaw_numeric_array_count (s);                               \
    VALUE rarray = rb_ary_new2 (acnt);                                         \
    union                                                                      \
    {                                                                          \
      v##N##BT val;                                                            \
      BT accessor[N];                                                          \
    } u;                                                                       \
    for (aq = 0; aq < acnt; aq++)                                              \
      {                                                                        \
        u.val = *slaw_##AT##_emit_nth (s, aq);                                 \
        VALUE vrarray = rb_ary_new2 (vcnt);                                    \
        for (vq = 0; vq < vcnt; vq++)                                          \
          rb_ary_push (vrarray, LL2NUM (u.accessor[vq]));                      \
        rb_ary_push (rarray, vrarray);                                         \
      }                                                                        \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##AT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)

#define COERCE_TO_INT64_ARRAY(AT, x, y)                                        \
  else if (slaw_is_##AT (s)) do                                                \
  {                                                                            \
    unt64 q, cnt = slaw_numeric_array_count (s);                               \
    VALUE rarray = rb_ary_new2 (cnt);                                          \
    for (q = 0; q < cnt; q++)                                                  \
      {                                                                        \
        int64 num = *slaw_##AT##_emit_nth (s, q);                              \
        rb_ary_push (rarray, LL2NUM (num));                                    \
      }                                                                        \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##AT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)

#define COERCE_TO_FLOAT64_ARRAY(AT, x, y)                                      \
  else if (slaw_is_##AT (s)) do                                                \
  {                                                                            \
    unt64 q, cnt = slaw_numeric_array_count (s);                               \
    VALUE rarray = rb_ary_new2 (cnt);                                          \
    for (q = 0; q < cnt; q++)                                                  \
      {                                                                        \
        float64 num = *slaw_##AT##_emit_nth (s, q);                            \
        rb_ary_push (rarray, rb_float_new (num));                              \
      }                                                                        \
    rb_iv_set (self, "@type_tag", ID2SYM (id_##AT));                           \
    return rarray;                                                             \
  }                                                                            \
  while (0)
