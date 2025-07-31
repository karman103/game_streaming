
 /**************************************************************************
 *                       THIS FILE WAS GENERATED                           *
 * Script: utility/generate_specenum.py                                    *
 *                       DO NOT CHANGE THIS FILE                           *
 **************************************************************************/

/***********************************************************************
 Freeciv - Copyright (C) 2009
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
***********************************************************************/

/*
 * Include this file to define tools to manage enumerators. First of all,
 * before including this file, you *MUST* define the following macros:
 * - SPECENUM_NAME: is the name of the enumeration (e.g. 'foo' for defining
 * 'enum foo').
 * - SPECENUM_VALUE%d: define like this all values of your enumeration type
 * (e.g. '#define SPECENUM_VALUE0 FOO_FIRST').
 *
 * The following macros *CAN* be defined:
 * - SPECENUM_INVALID: specifies a value that your 'foo_invalid()' function
 * will return. Note it cannot be a declared value with SPECENUM_VALUE%d.
 * - SPECENUM_BITWISE: defines if the enumeration should be like
 * [1, 2, 4, 8, etc...] instead of the default of [0, 1, 2, 3, etc...].
 * - SPECENUM_ZERO: can be defined only if SPECENUM_BITWISE was also defined.
 * It defines a 0 value. Note that if you don't declare this value, 0 passed
 * to the 'foo_is_valid()' function will return FALSE.
 * - SPECENUM_COUNT: a name for the maximum enumeration number plus 1. For
 * enums where every element from 0 to the maximum is defined, this is the
 * number of elements in the enum. This value is suitable to size an array
 * indexed by the enum. It can not be used in combination with
 * SPECENUM_BITWISE. SPECENUM_is_valid() will return the invalid element
 * for it.
 *
 * SPECENUM_VALUE%dNAME, SPECENUM_ZERONAME, SPECENUM_COUNTNAME: Can be used
 * to bind a string to the particular enumerator to be returned by
 * SPECENUM_name(), etc. If not defined, the default name for 'FOO_FIRST'
 * is '"FOO_FIRST"'. A name can be qualified. The qualification will only
 * be used for its translation. The returned name will be unqualified. To
 * mark a name as translatable use N_().
 *
 * SPECENUM_NAMEOVERRIDE: call callback function foo_name_cb(enum foo),
 * defined by specenum user, to get name of the enum value. If the function
 * returns NULL, compiled in names are used.
 *
 * SPECENUM_NAME_UPDATER: call callback function foo_name_update_cb(old_name),
 * defined by specenum user, to get current name to search enum value with.
 *
 * SPECENUM_BITVECTOR: specifies the name of a bit vector for the enum
 * values. It can not be used in combination with SPECENUM_BITWISE.
 *
 * Assuming SPECENUM_NAME were 'foo', including this file would provide
 * the definition for the enumeration type 'enum foo', and prototypes for
 * the following functions:
 *   bool foo_is_bitwise(void);
 *   enum foo foo_min(void);
 *   enum foo foo_max(void);
 *   enum foo foo_invalid(void);
 *   bool foo_is_valid(enum foo);
 *
 *   enum foo foo_begin(void);
 *   enum foo foo_end(void);
 *   enum foo foo_next(enum foo);
 *
 *   const char *foo_name(enum foo);
 *   const char *foo_translated_name(enum foo);
 *   enum foo foo_by_name(const char *name,
 *                        int (*strcmp_func)(const char *, const char *));
 *
 * Example:
 *   #define SPECENUM_NAME test
 *   #define SPECENUM_BITWISE
 *   #define SPECENUM_VALUE0 TEST0
 *   #define SPECENUM_VALUE1 TEST1
 *   #define SPECENUM_VALUE3 TEST3
 *   #include "specenum_gen.h"
 *
 *  {
 *    static const char *strings[] = {
 *      "TEST1", "test3", "fghdf", NULL
 *    };
 *    enum test e;
 *    int i;
 *
 *    log_verbose("enum test [%d; %d]%s",
 *                test_min(), test_max(), test_bitwise ? " bitwise" : "");
 *
 *    for (e = test_begin(); e != test_end(); e = test_next(e)) {
 *      log_verbose("Value %d is %s", e, test_name(e));
 *    }
 *
 *    for (i = 0; strings[i]; i++) {
 *      e = test_by_name(strings[i], fc_strcasecmp);
 *      if (test_is_valid(e)) {
 *        log_verbose("Value is %d for %s", e, strings[i]);
 *      } else {
 *        log_verbose("%s is not a valid name", strings[i]);
 *      }
 *    }
 *  }
 *
 * Will output:
 *   enum test [1, 8] bitwise
 *   Value 1 is TEST0
 *   Value 2 is TEST1
 *   Value 8 is TEST3
 *   Value is 2 for TEST1
 *   Value is 8 for test3
 *   fghdf is not a valid name
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* utility */
#include "fcintl.h"     /* translation */
#include "log.h"        /* fc_assert. */
#include "support.h"    /* bool type. */

#ifndef SPECENUM_NAME
#error Must define a SPECENUM_NAME to use this header
#endif

#define SPECENUM_PASTE_(x, y) x ## y
#define SPECENUM_PASTE(x, y) SPECENUM_PASTE_(x, y)

#define SPECENUM_STRING_(x) #x
#define SPECENUM_STRING(x) SPECENUM_STRING_(x)

#define SPECENUM_FOO(suffix) SPECENUM_PASTE(SPECENUM_NAME, suffix)

#ifndef SPECENUM_INVALID
#define SPECENUM_INVALID ((enum SPECENUM_NAME) -1)
#endif

#ifdef SPECENUM_BITWISE
#ifdef SPECENUM_COUNT
#error Cannot define SPECENUM_COUNT when SPECENUM_BITWISE is defined.
#endif
#define SPECENUM_VALUE(value) (1 << value)
#else /* SPECENUM_BITWISE */
#ifdef SPECENUM_ZERO
#error Cannot define SPECENUM_ZERO when SPECENUM_BITWISE is not defined.
#endif
#define SPECENUM_VALUE(value) (value)
#endif /* SPECENUM_BITWISE */

#ifdef SPECENUM_BITVECTOR
#include "bitvector.h"
#ifdef SPECENUM_BITWISE
#error SPECENUM_BITWISE and SPECENUM_BITVECTOR cannot both be defined.
#endif /* SPECENUM_BITWISE */
#endif /* SPECENUM_BITVECTOR */

#undef SPECENUM_MIN_VALUE
#undef SPECENUM_MAX_VALUE

/* Enumeration definition. */
enum SPECENUM_NAME {
#ifdef SPECENUM_ZERO
  SPECENUM_ZERO = 0,
#endif

#ifdef SPECENUM_VALUE0
  SPECENUM_VALUE0 = SPECENUM_VALUE(0),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE0
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE0
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (0 + 1)
#endif /* SPECENUM_VALUE0 */

#ifdef SPECENUM_VALUE1
  SPECENUM_VALUE1 = SPECENUM_VALUE(1),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE1
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE1
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (1 + 1)
#endif /* SPECENUM_VALUE1 */

#ifdef SPECENUM_VALUE2
  SPECENUM_VALUE2 = SPECENUM_VALUE(2),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE2
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE2
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (2 + 1)
#endif /* SPECENUM_VALUE2 */

#ifdef SPECENUM_VALUE3
  SPECENUM_VALUE3 = SPECENUM_VALUE(3),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE3
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE3
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (3 + 1)
#endif /* SPECENUM_VALUE3 */

#ifdef SPECENUM_VALUE4
  SPECENUM_VALUE4 = SPECENUM_VALUE(4),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE4
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE4
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (4 + 1)
#endif /* SPECENUM_VALUE4 */

#ifdef SPECENUM_VALUE5
  SPECENUM_VALUE5 = SPECENUM_VALUE(5),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE5
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE5
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (5 + 1)
#endif /* SPECENUM_VALUE5 */

#ifdef SPECENUM_VALUE6
  SPECENUM_VALUE6 = SPECENUM_VALUE(6),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE6
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE6
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (6 + 1)
#endif /* SPECENUM_VALUE6 */

#ifdef SPECENUM_VALUE7
  SPECENUM_VALUE7 = SPECENUM_VALUE(7),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE7
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE7
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (7 + 1)
#endif /* SPECENUM_VALUE7 */

#ifdef SPECENUM_VALUE8
  SPECENUM_VALUE8 = SPECENUM_VALUE(8),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE8
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE8
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (8 + 1)
#endif /* SPECENUM_VALUE8 */

#ifdef SPECENUM_VALUE9
  SPECENUM_VALUE9 = SPECENUM_VALUE(9),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE9
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE9
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (9 + 1)
#endif /* SPECENUM_VALUE9 */

#ifdef SPECENUM_VALUE10
  SPECENUM_VALUE10 = SPECENUM_VALUE(10),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE10
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE10
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (10 + 1)
#endif /* SPECENUM_VALUE10 */

#ifdef SPECENUM_VALUE11
  SPECENUM_VALUE11 = SPECENUM_VALUE(11),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE11
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE11
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (11 + 1)
#endif /* SPECENUM_VALUE11 */

#ifdef SPECENUM_VALUE12
  SPECENUM_VALUE12 = SPECENUM_VALUE(12),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE12
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE12
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (12 + 1)
#endif /* SPECENUM_VALUE12 */

#ifdef SPECENUM_VALUE13
  SPECENUM_VALUE13 = SPECENUM_VALUE(13),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE13
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE13
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (13 + 1)
#endif /* SPECENUM_VALUE13 */

#ifdef SPECENUM_VALUE14
  SPECENUM_VALUE14 = SPECENUM_VALUE(14),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE14
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE14
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (14 + 1)
#endif /* SPECENUM_VALUE14 */

#ifdef SPECENUM_VALUE15
  SPECENUM_VALUE15 = SPECENUM_VALUE(15),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE15
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE15
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (15 + 1)
#endif /* SPECENUM_VALUE15 */

#ifdef SPECENUM_VALUE16
  SPECENUM_VALUE16 = SPECENUM_VALUE(16),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE16
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE16
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (16 + 1)
#endif /* SPECENUM_VALUE16 */

#ifdef SPECENUM_VALUE17
  SPECENUM_VALUE17 = SPECENUM_VALUE(17),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE17
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE17
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (17 + 1)
#endif /* SPECENUM_VALUE17 */

#ifdef SPECENUM_VALUE18
  SPECENUM_VALUE18 = SPECENUM_VALUE(18),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE18
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE18
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (18 + 1)
#endif /* SPECENUM_VALUE18 */

#ifdef SPECENUM_VALUE19
  SPECENUM_VALUE19 = SPECENUM_VALUE(19),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE19
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE19
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (19 + 1)
#endif /* SPECENUM_VALUE19 */

#ifdef SPECENUM_VALUE20
  SPECENUM_VALUE20 = SPECENUM_VALUE(20),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE20
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE20
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (20 + 1)
#endif /* SPECENUM_VALUE20 */

#ifdef SPECENUM_VALUE21
  SPECENUM_VALUE21 = SPECENUM_VALUE(21),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE21
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE21
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (21 + 1)
#endif /* SPECENUM_VALUE21 */

#ifdef SPECENUM_VALUE22
  SPECENUM_VALUE22 = SPECENUM_VALUE(22),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE22
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE22
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (22 + 1)
#endif /* SPECENUM_VALUE22 */

#ifdef SPECENUM_VALUE23
  SPECENUM_VALUE23 = SPECENUM_VALUE(23),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE23
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE23
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (23 + 1)
#endif /* SPECENUM_VALUE23 */

#ifdef SPECENUM_VALUE24
  SPECENUM_VALUE24 = SPECENUM_VALUE(24),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE24
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE24
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (24 + 1)
#endif /* SPECENUM_VALUE24 */

#ifdef SPECENUM_VALUE25
  SPECENUM_VALUE25 = SPECENUM_VALUE(25),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE25
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE25
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (25 + 1)
#endif /* SPECENUM_VALUE25 */

#ifdef SPECENUM_VALUE26
  SPECENUM_VALUE26 = SPECENUM_VALUE(26),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE26
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE26
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (26 + 1)
#endif /* SPECENUM_VALUE26 */

#ifdef SPECENUM_VALUE27
  SPECENUM_VALUE27 = SPECENUM_VALUE(27),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE27
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE27
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (27 + 1)
#endif /* SPECENUM_VALUE27 */

#ifdef SPECENUM_VALUE28
  SPECENUM_VALUE28 = SPECENUM_VALUE(28),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE28
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE28
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (28 + 1)
#endif /* SPECENUM_VALUE28 */

#ifdef SPECENUM_VALUE29
  SPECENUM_VALUE29 = SPECENUM_VALUE(29),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE29
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE29
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (29 + 1)
#endif /* SPECENUM_VALUE29 */

#ifdef SPECENUM_VALUE30
  SPECENUM_VALUE30 = SPECENUM_VALUE(30),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE30
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE30
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (30 + 1)
#endif /* SPECENUM_VALUE30 */

#ifdef SPECENUM_VALUE31
  SPECENUM_VALUE31 = SPECENUM_VALUE(31),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE31
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE31
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (31 + 1)
#endif /* SPECENUM_VALUE31 */

#ifdef SPECENUM_VALUE32
  SPECENUM_VALUE32 = SPECENUM_VALUE(32),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE32
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE32
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (32 + 1)
#endif /* SPECENUM_VALUE32 */

#ifdef SPECENUM_VALUE33
  SPECENUM_VALUE33 = SPECENUM_VALUE(33),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE33
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE33
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (33 + 1)
#endif /* SPECENUM_VALUE33 */

#ifdef SPECENUM_VALUE34
  SPECENUM_VALUE34 = SPECENUM_VALUE(34),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE34
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE34
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (34 + 1)
#endif /* SPECENUM_VALUE34 */

#ifdef SPECENUM_VALUE35
  SPECENUM_VALUE35 = SPECENUM_VALUE(35),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE35
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE35
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (35 + 1)
#endif /* SPECENUM_VALUE35 */

#ifdef SPECENUM_VALUE36
  SPECENUM_VALUE36 = SPECENUM_VALUE(36),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE36
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE36
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (36 + 1)
#endif /* SPECENUM_VALUE36 */

#ifdef SPECENUM_VALUE37
  SPECENUM_VALUE37 = SPECENUM_VALUE(37),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE37
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE37
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (37 + 1)
#endif /* SPECENUM_VALUE37 */

#ifdef SPECENUM_VALUE38
  SPECENUM_VALUE38 = SPECENUM_VALUE(38),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE38
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE38
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (38 + 1)
#endif /* SPECENUM_VALUE38 */

#ifdef SPECENUM_VALUE39
  SPECENUM_VALUE39 = SPECENUM_VALUE(39),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE39
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE39
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (39 + 1)
#endif /* SPECENUM_VALUE39 */

#ifdef SPECENUM_VALUE40
  SPECENUM_VALUE40 = SPECENUM_VALUE(40),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE40
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE40
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (40 + 1)
#endif /* SPECENUM_VALUE40 */

#ifdef SPECENUM_VALUE41
  SPECENUM_VALUE41 = SPECENUM_VALUE(41),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE41
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE41
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (41 + 1)
#endif /* SPECENUM_VALUE41 */

#ifdef SPECENUM_VALUE42
  SPECENUM_VALUE42 = SPECENUM_VALUE(42),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE42
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE42
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (42 + 1)
#endif /* SPECENUM_VALUE42 */

#ifdef SPECENUM_VALUE43
  SPECENUM_VALUE43 = SPECENUM_VALUE(43),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE43
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE43
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (43 + 1)
#endif /* SPECENUM_VALUE43 */

#ifdef SPECENUM_VALUE44
  SPECENUM_VALUE44 = SPECENUM_VALUE(44),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE44
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE44
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (44 + 1)
#endif /* SPECENUM_VALUE44 */

#ifdef SPECENUM_VALUE45
  SPECENUM_VALUE45 = SPECENUM_VALUE(45),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE45
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE45
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (45 + 1)
#endif /* SPECENUM_VALUE45 */

#ifdef SPECENUM_VALUE46
  SPECENUM_VALUE46 = SPECENUM_VALUE(46),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE46
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE46
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (46 + 1)
#endif /* SPECENUM_VALUE46 */

#ifdef SPECENUM_VALUE47
  SPECENUM_VALUE47 = SPECENUM_VALUE(47),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE47
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE47
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (47 + 1)
#endif /* SPECENUM_VALUE47 */

#ifdef SPECENUM_VALUE48
  SPECENUM_VALUE48 = SPECENUM_VALUE(48),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE48
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE48
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (48 + 1)
#endif /* SPECENUM_VALUE48 */

#ifdef SPECENUM_VALUE49
  SPECENUM_VALUE49 = SPECENUM_VALUE(49),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE49
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE49
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (49 + 1)
#endif /* SPECENUM_VALUE49 */

#ifdef SPECENUM_VALUE50
  SPECENUM_VALUE50 = SPECENUM_VALUE(50),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE50
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE50
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (50 + 1)
#endif /* SPECENUM_VALUE50 */

#ifdef SPECENUM_VALUE51
  SPECENUM_VALUE51 = SPECENUM_VALUE(51),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE51
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE51
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (51 + 1)
#endif /* SPECENUM_VALUE51 */

#ifdef SPECENUM_VALUE52
  SPECENUM_VALUE52 = SPECENUM_VALUE(52),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE52
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE52
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (52 + 1)
#endif /* SPECENUM_VALUE52 */

#ifdef SPECENUM_VALUE53
  SPECENUM_VALUE53 = SPECENUM_VALUE(53),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE53
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE53
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (53 + 1)
#endif /* SPECENUM_VALUE53 */

#ifdef SPECENUM_VALUE54
  SPECENUM_VALUE54 = SPECENUM_VALUE(54),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE54
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE54
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (54 + 1)
#endif /* SPECENUM_VALUE54 */

#ifdef SPECENUM_VALUE55
  SPECENUM_VALUE55 = SPECENUM_VALUE(55),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE55
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE55
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (55 + 1)
#endif /* SPECENUM_VALUE55 */

#ifdef SPECENUM_VALUE56
  SPECENUM_VALUE56 = SPECENUM_VALUE(56),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE56
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE56
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (56 + 1)
#endif /* SPECENUM_VALUE56 */

#ifdef SPECENUM_VALUE57
  SPECENUM_VALUE57 = SPECENUM_VALUE(57),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE57
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE57
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (57 + 1)
#endif /* SPECENUM_VALUE57 */

#ifdef SPECENUM_VALUE58
  SPECENUM_VALUE58 = SPECENUM_VALUE(58),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE58
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE58
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (58 + 1)
#endif /* SPECENUM_VALUE58 */

#ifdef SPECENUM_VALUE59
  SPECENUM_VALUE59 = SPECENUM_VALUE(59),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE59
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE59
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (59 + 1)
#endif /* SPECENUM_VALUE59 */

#ifdef SPECENUM_VALUE60
  SPECENUM_VALUE60 = SPECENUM_VALUE(60),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE60
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE60
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (60 + 1)
#endif /* SPECENUM_VALUE60 */

#ifdef SPECENUM_VALUE61
  SPECENUM_VALUE61 = SPECENUM_VALUE(61),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE61
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE61
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (61 + 1)
#endif /* SPECENUM_VALUE61 */

#ifdef SPECENUM_VALUE62
  SPECENUM_VALUE62 = SPECENUM_VALUE(62),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE62
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE62
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (62 + 1)
#endif /* SPECENUM_VALUE62 */

#ifdef SPECENUM_VALUE63
  SPECENUM_VALUE63 = SPECENUM_VALUE(63),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE63
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE63
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (63 + 1)
#endif /* SPECENUM_VALUE63 */

#ifdef SPECENUM_VALUE64
  SPECENUM_VALUE64 = SPECENUM_VALUE(64),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE64
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE64
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (64 + 1)
#endif /* SPECENUM_VALUE64 */

#ifdef SPECENUM_VALUE65
  SPECENUM_VALUE65 = SPECENUM_VALUE(65),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE65
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE65
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (65 + 1)
#endif /* SPECENUM_VALUE65 */

#ifdef SPECENUM_VALUE66
  SPECENUM_VALUE66 = SPECENUM_VALUE(66),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE66
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE66
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (66 + 1)
#endif /* SPECENUM_VALUE66 */

#ifdef SPECENUM_VALUE67
  SPECENUM_VALUE67 = SPECENUM_VALUE(67),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE67
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE67
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (67 + 1)
#endif /* SPECENUM_VALUE67 */

#ifdef SPECENUM_VALUE68
  SPECENUM_VALUE68 = SPECENUM_VALUE(68),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE68
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE68
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (68 + 1)
#endif /* SPECENUM_VALUE68 */

#ifdef SPECENUM_VALUE69
  SPECENUM_VALUE69 = SPECENUM_VALUE(69),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE69
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE69
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (69 + 1)
#endif /* SPECENUM_VALUE69 */

#ifdef SPECENUM_VALUE70
  SPECENUM_VALUE70 = SPECENUM_VALUE(70),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE70
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE70
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (70 + 1)
#endif /* SPECENUM_VALUE70 */

#ifdef SPECENUM_VALUE71
  SPECENUM_VALUE71 = SPECENUM_VALUE(71),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE71
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE71
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (71 + 1)
#endif /* SPECENUM_VALUE71 */

#ifdef SPECENUM_VALUE72
  SPECENUM_VALUE72 = SPECENUM_VALUE(72),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE72
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE72
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (72 + 1)
#endif /* SPECENUM_VALUE72 */

#ifdef SPECENUM_VALUE73
  SPECENUM_VALUE73 = SPECENUM_VALUE(73),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE73
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE73
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (73 + 1)
#endif /* SPECENUM_VALUE73 */

#ifdef SPECENUM_VALUE74
  SPECENUM_VALUE74 = SPECENUM_VALUE(74),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE74
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE74
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (74 + 1)
#endif /* SPECENUM_VALUE74 */

#ifdef SPECENUM_VALUE75
  SPECENUM_VALUE75 = SPECENUM_VALUE(75),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE75
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE75
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (75 + 1)
#endif /* SPECENUM_VALUE75 */

#ifdef SPECENUM_VALUE76
  SPECENUM_VALUE76 = SPECENUM_VALUE(76),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE76
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE76
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (76 + 1)
#endif /* SPECENUM_VALUE76 */

#ifdef SPECENUM_VALUE77
  SPECENUM_VALUE77 = SPECENUM_VALUE(77),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE77
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE77
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (77 + 1)
#endif /* SPECENUM_VALUE77 */

#ifdef SPECENUM_VALUE78
  SPECENUM_VALUE78 = SPECENUM_VALUE(78),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE78
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE78
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (78 + 1)
#endif /* SPECENUM_VALUE78 */

#ifdef SPECENUM_VALUE79
  SPECENUM_VALUE79 = SPECENUM_VALUE(79),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE79
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE79
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (79 + 1)
#endif /* SPECENUM_VALUE79 */

#ifdef SPECENUM_VALUE80
  SPECENUM_VALUE80 = SPECENUM_VALUE(80),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE80
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE80
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (80 + 1)
#endif /* SPECENUM_VALUE80 */

#ifdef SPECENUM_VALUE81
  SPECENUM_VALUE81 = SPECENUM_VALUE(81),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE81
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE81
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (81 + 1)
#endif /* SPECENUM_VALUE81 */

#ifdef SPECENUM_VALUE82
  SPECENUM_VALUE82 = SPECENUM_VALUE(82),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE82
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE82
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (82 + 1)
#endif /* SPECENUM_VALUE82 */

#ifdef SPECENUM_VALUE83
  SPECENUM_VALUE83 = SPECENUM_VALUE(83),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE83
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE83
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (83 + 1)
#endif /* SPECENUM_VALUE83 */

#ifdef SPECENUM_VALUE84
  SPECENUM_VALUE84 = SPECENUM_VALUE(84),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE84
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE84
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (84 + 1)
#endif /* SPECENUM_VALUE84 */

#ifdef SPECENUM_VALUE85
  SPECENUM_VALUE85 = SPECENUM_VALUE(85),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE85
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE85
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (85 + 1)
#endif /* SPECENUM_VALUE85 */

#ifdef SPECENUM_VALUE86
  SPECENUM_VALUE86 = SPECENUM_VALUE(86),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE86
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE86
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (86 + 1)
#endif /* SPECENUM_VALUE86 */

#ifdef SPECENUM_VALUE87
  SPECENUM_VALUE87 = SPECENUM_VALUE(87),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE87
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE87
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (87 + 1)
#endif /* SPECENUM_VALUE87 */

#ifdef SPECENUM_VALUE88
  SPECENUM_VALUE88 = SPECENUM_VALUE(88),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE88
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE88
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (88 + 1)
#endif /* SPECENUM_VALUE88 */

#ifdef SPECENUM_VALUE89
  SPECENUM_VALUE89 = SPECENUM_VALUE(89),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE89
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE89
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (89 + 1)
#endif /* SPECENUM_VALUE89 */

#ifdef SPECENUM_VALUE90
  SPECENUM_VALUE90 = SPECENUM_VALUE(90),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE90
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE90
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (90 + 1)
#endif /* SPECENUM_VALUE90 */

#ifdef SPECENUM_VALUE91
  SPECENUM_VALUE91 = SPECENUM_VALUE(91),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE91
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE91
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (91 + 1)
#endif /* SPECENUM_VALUE91 */

#ifdef SPECENUM_VALUE92
  SPECENUM_VALUE92 = SPECENUM_VALUE(92),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE92
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE92
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (92 + 1)
#endif /* SPECENUM_VALUE92 */

#ifdef SPECENUM_VALUE93
  SPECENUM_VALUE93 = SPECENUM_VALUE(93),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE93
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE93
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (93 + 1)
#endif /* SPECENUM_VALUE93 */

#ifdef SPECENUM_VALUE94
  SPECENUM_VALUE94 = SPECENUM_VALUE(94),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE94
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE94
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (94 + 1)
#endif /* SPECENUM_VALUE94 */

#ifdef SPECENUM_VALUE95
  SPECENUM_VALUE95 = SPECENUM_VALUE(95),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE95
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE95
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (95 + 1)
#endif /* SPECENUM_VALUE95 */

#ifdef SPECENUM_VALUE96
  SPECENUM_VALUE96 = SPECENUM_VALUE(96),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE96
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE96
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (96 + 1)
#endif /* SPECENUM_VALUE96 */

#ifdef SPECENUM_VALUE97
  SPECENUM_VALUE97 = SPECENUM_VALUE(97),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE97
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE97
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (97 + 1)
#endif /* SPECENUM_VALUE97 */

#ifdef SPECENUM_VALUE98
  SPECENUM_VALUE98 = SPECENUM_VALUE(98),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE98
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE98
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (98 + 1)
#endif /* SPECENUM_VALUE98 */

#ifdef SPECENUM_VALUE99
  SPECENUM_VALUE99 = SPECENUM_VALUE(99),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE99
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE99
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (99 + 1)
#endif /* SPECENUM_VALUE99 */

#ifdef SPECENUM_VALUE100
  SPECENUM_VALUE100 = SPECENUM_VALUE(100),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE100
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE100
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (100 + 1)
#endif /* SPECENUM_VALUE100 */

#ifdef SPECENUM_VALUE101
  SPECENUM_VALUE101 = SPECENUM_VALUE(101),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE101
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE101
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (101 + 1)
#endif /* SPECENUM_VALUE101 */

#ifdef SPECENUM_VALUE102
  SPECENUM_VALUE102 = SPECENUM_VALUE(102),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE102
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE102
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (102 + 1)
#endif /* SPECENUM_VALUE102 */

#ifdef SPECENUM_VALUE103
  SPECENUM_VALUE103 = SPECENUM_VALUE(103),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE103
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE103
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (103 + 1)
#endif /* SPECENUM_VALUE103 */

#ifdef SPECENUM_VALUE104
  SPECENUM_VALUE104 = SPECENUM_VALUE(104),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE104
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE104
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (104 + 1)
#endif /* SPECENUM_VALUE104 */

#ifdef SPECENUM_VALUE105
  SPECENUM_VALUE105 = SPECENUM_VALUE(105),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE105
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE105
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (105 + 1)
#endif /* SPECENUM_VALUE105 */

#ifdef SPECENUM_VALUE106
  SPECENUM_VALUE106 = SPECENUM_VALUE(106),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE106
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE106
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (106 + 1)
#endif /* SPECENUM_VALUE106 */

#ifdef SPECENUM_VALUE107
  SPECENUM_VALUE107 = SPECENUM_VALUE(107),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE107
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE107
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (107 + 1)
#endif /* SPECENUM_VALUE107 */

#ifdef SPECENUM_VALUE108
  SPECENUM_VALUE108 = SPECENUM_VALUE(108),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE108
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE108
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (108 + 1)
#endif /* SPECENUM_VALUE108 */

#ifdef SPECENUM_VALUE109
  SPECENUM_VALUE109 = SPECENUM_VALUE(109),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE109
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE109
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (109 + 1)
#endif /* SPECENUM_VALUE109 */

#ifdef SPECENUM_VALUE110
  SPECENUM_VALUE110 = SPECENUM_VALUE(110),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE110
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE110
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (110 + 1)
#endif /* SPECENUM_VALUE110 */

#ifdef SPECENUM_VALUE111
  SPECENUM_VALUE111 = SPECENUM_VALUE(111),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE111
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE111
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (111 + 1)
#endif /* SPECENUM_VALUE111 */

#ifdef SPECENUM_VALUE112
  SPECENUM_VALUE112 = SPECENUM_VALUE(112),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE112
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE112
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (112 + 1)
#endif /* SPECENUM_VALUE112 */

#ifdef SPECENUM_VALUE113
  SPECENUM_VALUE113 = SPECENUM_VALUE(113),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE113
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE113
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (113 + 1)
#endif /* SPECENUM_VALUE113 */

#ifdef SPECENUM_VALUE114
  SPECENUM_VALUE114 = SPECENUM_VALUE(114),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE114
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE114
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (114 + 1)
#endif /* SPECENUM_VALUE114 */

#ifdef SPECENUM_VALUE115
  SPECENUM_VALUE115 = SPECENUM_VALUE(115),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE115
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE115
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (115 + 1)
#endif /* SPECENUM_VALUE115 */

#ifdef SPECENUM_VALUE116
  SPECENUM_VALUE116 = SPECENUM_VALUE(116),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE116
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE116
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (116 + 1)
#endif /* SPECENUM_VALUE116 */

#ifdef SPECENUM_VALUE117
  SPECENUM_VALUE117 = SPECENUM_VALUE(117),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE117
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE117
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (117 + 1)
#endif /* SPECENUM_VALUE117 */

#ifdef SPECENUM_VALUE118
  SPECENUM_VALUE118 = SPECENUM_VALUE(118),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE118
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE118
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (118 + 1)
#endif /* SPECENUM_VALUE118 */

#ifdef SPECENUM_VALUE119
  SPECENUM_VALUE119 = SPECENUM_VALUE(119),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE119
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE119
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (119 + 1)
#endif /* SPECENUM_VALUE119 */

#ifdef SPECENUM_VALUE120
  SPECENUM_VALUE120 = SPECENUM_VALUE(120),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE120
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE120
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (120 + 1)
#endif /* SPECENUM_VALUE120 */

#ifdef SPECENUM_VALUE121
  SPECENUM_VALUE121 = SPECENUM_VALUE(121),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE121
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE121
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (121 + 1)
#endif /* SPECENUM_VALUE121 */

#ifdef SPECENUM_VALUE122
  SPECENUM_VALUE122 = SPECENUM_VALUE(122),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE122
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE122
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (122 + 1)
#endif /* SPECENUM_VALUE122 */

#ifdef SPECENUM_VALUE123
  SPECENUM_VALUE123 = SPECENUM_VALUE(123),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE123
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE123
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (123 + 1)
#endif /* SPECENUM_VALUE123 */

#ifdef SPECENUM_VALUE124
  SPECENUM_VALUE124 = SPECENUM_VALUE(124),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE124
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE124
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (124 + 1)
#endif /* SPECENUM_VALUE124 */

#ifdef SPECENUM_VALUE125
  SPECENUM_VALUE125 = SPECENUM_VALUE(125),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE125
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE125
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (125 + 1)
#endif /* SPECENUM_VALUE125 */

#ifdef SPECENUM_VALUE126
  SPECENUM_VALUE126 = SPECENUM_VALUE(126),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE126
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE126
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (126 + 1)
#endif /* SPECENUM_VALUE126 */

#ifdef SPECENUM_VALUE127
  SPECENUM_VALUE127 = SPECENUM_VALUE(127),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE127
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE127
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (127 + 1)
#endif /* SPECENUM_VALUE127 */

#ifdef SPECENUM_VALUE128
  SPECENUM_VALUE128 = SPECENUM_VALUE(128),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE128
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE128
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (128 + 1)
#endif /* SPECENUM_VALUE128 */

#ifdef SPECENUM_VALUE129
  SPECENUM_VALUE129 = SPECENUM_VALUE(129),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE129
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE129
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (129 + 1)
#endif /* SPECENUM_VALUE129 */

#ifdef SPECENUM_VALUE130
  SPECENUM_VALUE130 = SPECENUM_VALUE(130),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE130
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE130
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (130 + 1)
#endif /* SPECENUM_VALUE130 */

#ifdef SPECENUM_VALUE131
  SPECENUM_VALUE131 = SPECENUM_VALUE(131),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE131
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE131
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (131 + 1)
#endif /* SPECENUM_VALUE131 */

#ifdef SPECENUM_VALUE132
  SPECENUM_VALUE132 = SPECENUM_VALUE(132),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE132
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE132
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (132 + 1)
#endif /* SPECENUM_VALUE132 */

#ifdef SPECENUM_VALUE133
  SPECENUM_VALUE133 = SPECENUM_VALUE(133),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE133
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE133
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (133 + 1)
#endif /* SPECENUM_VALUE133 */

#ifdef SPECENUM_VALUE134
  SPECENUM_VALUE134 = SPECENUM_VALUE(134),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE134
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE134
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (134 + 1)
#endif /* SPECENUM_VALUE134 */

#ifdef SPECENUM_VALUE135
  SPECENUM_VALUE135 = SPECENUM_VALUE(135),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE135
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE135
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (135 + 1)
#endif /* SPECENUM_VALUE135 */

#ifdef SPECENUM_VALUE136
  SPECENUM_VALUE136 = SPECENUM_VALUE(136),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE136
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE136
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (136 + 1)
#endif /* SPECENUM_VALUE136 */

#ifdef SPECENUM_VALUE137
  SPECENUM_VALUE137 = SPECENUM_VALUE(137),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE137
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE137
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (137 + 1)
#endif /* SPECENUM_VALUE137 */

#ifdef SPECENUM_VALUE138
  SPECENUM_VALUE138 = SPECENUM_VALUE(138),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE138
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE138
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (138 + 1)
#endif /* SPECENUM_VALUE138 */

#ifdef SPECENUM_VALUE139
  SPECENUM_VALUE139 = SPECENUM_VALUE(139),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE139
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE139
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (139 + 1)
#endif /* SPECENUM_VALUE139 */

#ifdef SPECENUM_VALUE140
  SPECENUM_VALUE140 = SPECENUM_VALUE(140),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE140
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE140
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (140 + 1)
#endif /* SPECENUM_VALUE140 */

#ifdef SPECENUM_VALUE141
  SPECENUM_VALUE141 = SPECENUM_VALUE(141),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE141
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE141
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (141 + 1)
#endif /* SPECENUM_VALUE141 */

#ifdef SPECENUM_VALUE142
  SPECENUM_VALUE142 = SPECENUM_VALUE(142),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE142
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE142
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (142 + 1)
#endif /* SPECENUM_VALUE142 */

#ifdef SPECENUM_VALUE143
  SPECENUM_VALUE143 = SPECENUM_VALUE(143),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE143
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE143
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (143 + 1)
#endif /* SPECENUM_VALUE143 */

#ifdef SPECENUM_VALUE144
  SPECENUM_VALUE144 = SPECENUM_VALUE(144),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE144
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE144
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (144 + 1)
#endif /* SPECENUM_VALUE144 */

#ifdef SPECENUM_VALUE145
  SPECENUM_VALUE145 = SPECENUM_VALUE(145),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE145
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE145
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (145 + 1)
#endif /* SPECENUM_VALUE145 */

#ifdef SPECENUM_VALUE146
  SPECENUM_VALUE146 = SPECENUM_VALUE(146),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE146
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE146
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (146 + 1)
#endif /* SPECENUM_VALUE146 */

#ifdef SPECENUM_VALUE147
  SPECENUM_VALUE147 = SPECENUM_VALUE(147),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE147
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE147
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (147 + 1)
#endif /* SPECENUM_VALUE147 */

#ifdef SPECENUM_VALUE148
  SPECENUM_VALUE148 = SPECENUM_VALUE(148),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE148
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE148
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (148 + 1)
#endif /* SPECENUM_VALUE148 */

#ifdef SPECENUM_VALUE149
  SPECENUM_VALUE149 = SPECENUM_VALUE(149),
#  ifndef SPECENUM_MIN_VALUE
#    define SPECENUM_MIN_VALUE SPECENUM_VALUE149
#  endif
#  ifdef SPECENUM_MAX_VALUE
#    undef SPECENUM_MAX_VALUE
#  endif
#  define SPECENUM_MAX_VALUE SPECENUM_VALUE149
#  ifdef SPECENUM_SIZE
#    undef SPECENUM_SIZE
#  endif
#  define SPECENUM_SIZE (149 + 1)
#endif /* SPECENUM_VALUE149 */

#ifdef SPECENUM_COUNT
  SPECENUM_COUNT = (SPECENUM_MAX_VALUE + 1),
#endif /* SPECENUM_COUNT */
};

/**********************************************************************//**
  Returns TRUE if this enumeration is in bitwise mode.
**************************************************************************/
fc__attribute((const))
static inline bool SPECENUM_FOO(_is_bitwise)(void)
{
#ifdef SPECENUM_BITWISE
  return TRUE;
#else
  return FALSE;
#endif
}

/**********************************************************************//**
  Returns the value of the minimal enumerator.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_min)(void)
{
  return SPECENUM_MIN_VALUE;
}

/**********************************************************************//**
  Returns the value of the maximal enumerator.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_max)(void)
{
  return SPECENUM_MAX_VALUE;
}

/**********************************************************************//**
  Returns TRUE if this enumerator was defined.
**************************************************************************/
fc__attribute((const))
static inline bool SPECENUM_FOO(_is_valid)(enum SPECENUM_NAME enumerator)
{
#ifdef SPECENUM_BITWISE
  static const unsigned long valid = (
    0
#  ifdef SPECENUM_VALUE0
    | SPECENUM_VALUE0
#  endif
#  ifdef SPECENUM_VALUE1
    | SPECENUM_VALUE1
#  endif
#  ifdef SPECENUM_VALUE2
    | SPECENUM_VALUE2
#  endif
#  ifdef SPECENUM_VALUE3
    | SPECENUM_VALUE3
#  endif
#  ifdef SPECENUM_VALUE4
    | SPECENUM_VALUE4
#  endif
#  ifdef SPECENUM_VALUE5
    | SPECENUM_VALUE5
#  endif
#  ifdef SPECENUM_VALUE6
    | SPECENUM_VALUE6
#  endif
#  ifdef SPECENUM_VALUE7
    | SPECENUM_VALUE7
#  endif
#  ifdef SPECENUM_VALUE8
    | SPECENUM_VALUE8
#  endif
#  ifdef SPECENUM_VALUE9
    | SPECENUM_VALUE9
#  endif
#  ifdef SPECENUM_VALUE10
    | SPECENUM_VALUE10
#  endif
#  ifdef SPECENUM_VALUE11
    | SPECENUM_VALUE11
#  endif
#  ifdef SPECENUM_VALUE12
    | SPECENUM_VALUE12
#  endif
#  ifdef SPECENUM_VALUE13
    | SPECENUM_VALUE13
#  endif
#  ifdef SPECENUM_VALUE14
    | SPECENUM_VALUE14
#  endif
#  ifdef SPECENUM_VALUE15
    | SPECENUM_VALUE15
#  endif
#  ifdef SPECENUM_VALUE16
    | SPECENUM_VALUE16
#  endif
#  ifdef SPECENUM_VALUE17
    | SPECENUM_VALUE17
#  endif
#  ifdef SPECENUM_VALUE18
    | SPECENUM_VALUE18
#  endif
#  ifdef SPECENUM_VALUE19
    | SPECENUM_VALUE19
#  endif
#  ifdef SPECENUM_VALUE20
    | SPECENUM_VALUE20
#  endif
#  ifdef SPECENUM_VALUE21
    | SPECENUM_VALUE21
#  endif
#  ifdef SPECENUM_VALUE22
    | SPECENUM_VALUE22
#  endif
#  ifdef SPECENUM_VALUE23
    | SPECENUM_VALUE23
#  endif
#  ifdef SPECENUM_VALUE24
    | SPECENUM_VALUE24
#  endif
#  ifdef SPECENUM_VALUE25
    | SPECENUM_VALUE25
#  endif
#  ifdef SPECENUM_VALUE26
    | SPECENUM_VALUE26
#  endif
#  ifdef SPECENUM_VALUE27
    | SPECENUM_VALUE27
#  endif
#  ifdef SPECENUM_VALUE28
    | SPECENUM_VALUE28
#  endif
#  ifdef SPECENUM_VALUE29
    | SPECENUM_VALUE29
#  endif
#  ifdef SPECENUM_VALUE30
    | SPECENUM_VALUE30
#  endif
#  ifdef SPECENUM_VALUE31
    | SPECENUM_VALUE31
#  endif
#  ifdef SPECENUM_VALUE32
    | SPECENUM_VALUE32
#  endif
#  ifdef SPECENUM_VALUE33
    | SPECENUM_VALUE33
#  endif
#  ifdef SPECENUM_VALUE34
    | SPECENUM_VALUE34
#  endif
#  ifdef SPECENUM_VALUE35
    | SPECENUM_VALUE35
#  endif
#  ifdef SPECENUM_VALUE36
    | SPECENUM_VALUE36
#  endif
#  ifdef SPECENUM_VALUE37
    | SPECENUM_VALUE37
#  endif
#  ifdef SPECENUM_VALUE38
    | SPECENUM_VALUE38
#  endif
#  ifdef SPECENUM_VALUE39
    | SPECENUM_VALUE39
#  endif
#  ifdef SPECENUM_VALUE40
    | SPECENUM_VALUE40
#  endif
#  ifdef SPECENUM_VALUE41
    | SPECENUM_VALUE41
#  endif
#  ifdef SPECENUM_VALUE42
    | SPECENUM_VALUE42
#  endif
#  ifdef SPECENUM_VALUE43
    | SPECENUM_VALUE43
#  endif
#  ifdef SPECENUM_VALUE44
    | SPECENUM_VALUE44
#  endif
#  ifdef SPECENUM_VALUE45
    | SPECENUM_VALUE45
#  endif
#  ifdef SPECENUM_VALUE46
    | SPECENUM_VALUE46
#  endif
#  ifdef SPECENUM_VALUE47
    | SPECENUM_VALUE47
#  endif
#  ifdef SPECENUM_VALUE48
    | SPECENUM_VALUE48
#  endif
#  ifdef SPECENUM_VALUE49
    | SPECENUM_VALUE49
#  endif
#  ifdef SPECENUM_VALUE50
    | SPECENUM_VALUE50
#  endif
#  ifdef SPECENUM_VALUE51
    | SPECENUM_VALUE51
#  endif
#  ifdef SPECENUM_VALUE52
    | SPECENUM_VALUE52
#  endif
#  ifdef SPECENUM_VALUE53
    | SPECENUM_VALUE53
#  endif
#  ifdef SPECENUM_VALUE54
    | SPECENUM_VALUE54
#  endif
#  ifdef SPECENUM_VALUE55
    | SPECENUM_VALUE55
#  endif
#  ifdef SPECENUM_VALUE56
    | SPECENUM_VALUE56
#  endif
#  ifdef SPECENUM_VALUE57
    | SPECENUM_VALUE57
#  endif
#  ifdef SPECENUM_VALUE58
    | SPECENUM_VALUE58
#  endif
#  ifdef SPECENUM_VALUE59
    | SPECENUM_VALUE59
#  endif
#  ifdef SPECENUM_VALUE60
    | SPECENUM_VALUE60
#  endif
#  ifdef SPECENUM_VALUE61
    | SPECENUM_VALUE61
#  endif
#  ifdef SPECENUM_VALUE62
    | SPECENUM_VALUE62
#  endif
#  ifdef SPECENUM_VALUE63
    | SPECENUM_VALUE63
#  endif
#  ifdef SPECENUM_VALUE64
    | SPECENUM_VALUE64
#  endif
#  ifdef SPECENUM_VALUE65
    | SPECENUM_VALUE65
#  endif
#  ifdef SPECENUM_VALUE66
    | SPECENUM_VALUE66
#  endif
#  ifdef SPECENUM_VALUE67
    | SPECENUM_VALUE67
#  endif
#  ifdef SPECENUM_VALUE68
    | SPECENUM_VALUE68
#  endif
#  ifdef SPECENUM_VALUE69
    | SPECENUM_VALUE69
#  endif
#  ifdef SPECENUM_VALUE70
    | SPECENUM_VALUE70
#  endif
#  ifdef SPECENUM_VALUE71
    | SPECENUM_VALUE71
#  endif
#  ifdef SPECENUM_VALUE72
    | SPECENUM_VALUE72
#  endif
#  ifdef SPECENUM_VALUE73
    | SPECENUM_VALUE73
#  endif
#  ifdef SPECENUM_VALUE74
    | SPECENUM_VALUE74
#  endif
#  ifdef SPECENUM_VALUE75
    | SPECENUM_VALUE75
#  endif
#  ifdef SPECENUM_VALUE76
    | SPECENUM_VALUE76
#  endif
#  ifdef SPECENUM_VALUE77
    | SPECENUM_VALUE77
#  endif
#  ifdef SPECENUM_VALUE78
    | SPECENUM_VALUE78
#  endif
#  ifdef SPECENUM_VALUE79
    | SPECENUM_VALUE79
#  endif
#  ifdef SPECENUM_VALUE80
    | SPECENUM_VALUE80
#  endif
#  ifdef SPECENUM_VALUE81
    | SPECENUM_VALUE81
#  endif
#  ifdef SPECENUM_VALUE82
    | SPECENUM_VALUE82
#  endif
#  ifdef SPECENUM_VALUE83
    | SPECENUM_VALUE83
#  endif
#  ifdef SPECENUM_VALUE84
    | SPECENUM_VALUE84
#  endif
#  ifdef SPECENUM_VALUE85
    | SPECENUM_VALUE85
#  endif
#  ifdef SPECENUM_VALUE86
    | SPECENUM_VALUE86
#  endif
#  ifdef SPECENUM_VALUE87
    | SPECENUM_VALUE87
#  endif
#  ifdef SPECENUM_VALUE88
    | SPECENUM_VALUE88
#  endif
#  ifdef SPECENUM_VALUE89
    | SPECENUM_VALUE89
#  endif
#  ifdef SPECENUM_VALUE90
    | SPECENUM_VALUE90
#  endif
#  ifdef SPECENUM_VALUE91
    | SPECENUM_VALUE91
#  endif
#  ifdef SPECENUM_VALUE92
    | SPECENUM_VALUE92
#  endif
#  ifdef SPECENUM_VALUE93
    | SPECENUM_VALUE93
#  endif
#  ifdef SPECENUM_VALUE94
    | SPECENUM_VALUE94
#  endif
#  ifdef SPECENUM_VALUE95
    | SPECENUM_VALUE95
#  endif
#  ifdef SPECENUM_VALUE96
    | SPECENUM_VALUE96
#  endif
#  ifdef SPECENUM_VALUE97
    | SPECENUM_VALUE97
#  endif
#  ifdef SPECENUM_VALUE98
    | SPECENUM_VALUE98
#  endif
#  ifdef SPECENUM_VALUE99
    | SPECENUM_VALUE99
#  endif
#  ifdef SPECENUM_VALUE100
    | SPECENUM_VALUE100
#  endif
#  ifdef SPECENUM_VALUE101
    | SPECENUM_VALUE101
#  endif
#  ifdef SPECENUM_VALUE102
    | SPECENUM_VALUE102
#  endif
#  ifdef SPECENUM_VALUE103
    | SPECENUM_VALUE103
#  endif
#  ifdef SPECENUM_VALUE104
    | SPECENUM_VALUE104
#  endif
#  ifdef SPECENUM_VALUE105
    | SPECENUM_VALUE105
#  endif
#  ifdef SPECENUM_VALUE106
    | SPECENUM_VALUE106
#  endif
#  ifdef SPECENUM_VALUE107
    | SPECENUM_VALUE107
#  endif
#  ifdef SPECENUM_VALUE108
    | SPECENUM_VALUE108
#  endif
#  ifdef SPECENUM_VALUE109
    | SPECENUM_VALUE109
#  endif
#  ifdef SPECENUM_VALUE110
    | SPECENUM_VALUE110
#  endif
#  ifdef SPECENUM_VALUE111
    | SPECENUM_VALUE111
#  endif
#  ifdef SPECENUM_VALUE112
    | SPECENUM_VALUE112
#  endif
#  ifdef SPECENUM_VALUE113
    | SPECENUM_VALUE113
#  endif
#  ifdef SPECENUM_VALUE114
    | SPECENUM_VALUE114
#  endif
#  ifdef SPECENUM_VALUE115
    | SPECENUM_VALUE115
#  endif
#  ifdef SPECENUM_VALUE116
    | SPECENUM_VALUE116
#  endif
#  ifdef SPECENUM_VALUE117
    | SPECENUM_VALUE117
#  endif
#  ifdef SPECENUM_VALUE118
    | SPECENUM_VALUE118
#  endif
#  ifdef SPECENUM_VALUE119
    | SPECENUM_VALUE119
#  endif
#  ifdef SPECENUM_VALUE120
    | SPECENUM_VALUE120
#  endif
#  ifdef SPECENUM_VALUE121
    | SPECENUM_VALUE121
#  endif
#  ifdef SPECENUM_VALUE122
    | SPECENUM_VALUE122
#  endif
#  ifdef SPECENUM_VALUE123
    | SPECENUM_VALUE123
#  endif
#  ifdef SPECENUM_VALUE124
    | SPECENUM_VALUE124
#  endif
#  ifdef SPECENUM_VALUE125
    | SPECENUM_VALUE125
#  endif
#  ifdef SPECENUM_VALUE126
    | SPECENUM_VALUE126
#  endif
#  ifdef SPECENUM_VALUE127
    | SPECENUM_VALUE127
#  endif
#  ifdef SPECENUM_VALUE128
    | SPECENUM_VALUE128
#  endif
#  ifdef SPECENUM_VALUE129
    | SPECENUM_VALUE129
#  endif
#  ifdef SPECENUM_VALUE130
    | SPECENUM_VALUE130
#  endif
#  ifdef SPECENUM_VALUE131
    | SPECENUM_VALUE131
#  endif
#  ifdef SPECENUM_VALUE132
    | SPECENUM_VALUE132
#  endif
#  ifdef SPECENUM_VALUE133
    | SPECENUM_VALUE133
#  endif
#  ifdef SPECENUM_VALUE134
    | SPECENUM_VALUE134
#  endif
#  ifdef SPECENUM_VALUE135
    | SPECENUM_VALUE135
#  endif
#  ifdef SPECENUM_VALUE136
    | SPECENUM_VALUE136
#  endif
#  ifdef SPECENUM_VALUE137
    | SPECENUM_VALUE137
#  endif
#  ifdef SPECENUM_VALUE138
    | SPECENUM_VALUE138
#  endif
#  ifdef SPECENUM_VALUE139
    | SPECENUM_VALUE139
#  endif
#  ifdef SPECENUM_VALUE140
    | SPECENUM_VALUE140
#  endif
#  ifdef SPECENUM_VALUE141
    | SPECENUM_VALUE141
#  endif
#  ifdef SPECENUM_VALUE142
    | SPECENUM_VALUE142
#  endif
#  ifdef SPECENUM_VALUE143
    | SPECENUM_VALUE143
#  endif
#  ifdef SPECENUM_VALUE144
    | SPECENUM_VALUE144
#  endif
#  ifdef SPECENUM_VALUE145
    | SPECENUM_VALUE145
#  endif
#  ifdef SPECENUM_VALUE146
    | SPECENUM_VALUE146
#  endif
#  ifdef SPECENUM_VALUE147
    | SPECENUM_VALUE147
#  endif
#  ifdef SPECENUM_VALUE148
    | SPECENUM_VALUE148
#  endif
#  ifdef SPECENUM_VALUE149
    | SPECENUM_VALUE149
#  endif
  );

  FC_STATIC_ASSERT(sizeof(valid) * 8 >= SPECENUM_SIZE,
                   valid_sizeof_check);

#  ifdef SPECENUM_ZERO
  if (enumerator == SPECENUM_ZERO) {
    return TRUE;
  }
#  endif
  return (enumerator & valid) == enumerator;
#else
  static const bool valid[] = {
#  if 0 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE0
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 1 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE1
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 2 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE2
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 3 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE3
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 4 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE4
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 5 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE5
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 6 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE6
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 7 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE7
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 8 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE8
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 9 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE9
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 10 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE10
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 11 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE11
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 12 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE12
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 13 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE13
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 14 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE14
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 15 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE15
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 16 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE16
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 17 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE17
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 18 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE18
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 19 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE19
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 20 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE20
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 21 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE21
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 22 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE22
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 23 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE23
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 24 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE24
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 25 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE25
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 26 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE26
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 27 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE27
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 28 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE28
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 29 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE29
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 30 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE30
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 31 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE31
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 32 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE32
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 33 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE33
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 34 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE34
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 35 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE35
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 36 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE36
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 37 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE37
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 38 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE38
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 39 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE39
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 40 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE40
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 41 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE41
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 42 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE42
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 43 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE43
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 44 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE44
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 45 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE45
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 46 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE46
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 47 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE47
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 48 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE48
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 49 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE49
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 50 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE50
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 51 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE51
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 52 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE52
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 53 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE53
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 54 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE54
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 55 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE55
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 56 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE56
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 57 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE57
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 58 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE58
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 59 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE59
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 60 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE60
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 61 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE61
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 62 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE62
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 63 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE63
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 64 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE64
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 65 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE65
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 66 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE66
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 67 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE67
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 68 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE68
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 69 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE69
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 70 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE70
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 71 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE71
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 72 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE72
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 73 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE73
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 74 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE74
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 75 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE75
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 76 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE76
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 77 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE77
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 78 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE78
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 79 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE79
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 80 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE80
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 81 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE81
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 82 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE82
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 83 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE83
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 84 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE84
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 85 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE85
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 86 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE86
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 87 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE87
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 88 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE88
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 89 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE89
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 90 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE90
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 91 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE91
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 92 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE92
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 93 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE93
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 94 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE94
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 95 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE95
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 96 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE96
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 97 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE97
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 98 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE98
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 99 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE99
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 100 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE100
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 101 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE101
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 102 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE102
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 103 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE103
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 104 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE104
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 105 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE105
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 106 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE106
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 107 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE107
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 108 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE108
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 109 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE109
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 110 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE110
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 111 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE111
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 112 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE112
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 113 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE113
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 114 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE114
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 115 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE115
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 116 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE116
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 117 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE117
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 118 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE118
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 119 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE119
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 120 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE120
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 121 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE121
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 122 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE122
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 123 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE123
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 124 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE124
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 125 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE125
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 126 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE126
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 127 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE127
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 128 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE128
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 129 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE129
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 130 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE130
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 131 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE131
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 132 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE132
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 133 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE133
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 134 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE134
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 135 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE135
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 136 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE136
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 137 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE137
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 138 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE138
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 139 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE139
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 140 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE140
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 141 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE141
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 142 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE142
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 143 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE143
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 144 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE144
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 145 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE145
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 146 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE146
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 147 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE147
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 148 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE148
       TRUE,
#    else
       FALSE,
#    endif
#  endif
#  if 149 < SPECENUM_SIZE
#    ifdef SPECENUM_VALUE149
       TRUE,
#    else
       FALSE,
#    endif
#  endif
  };

  FC_STATIC_ASSERT(ARRAY_SIZE(valid) == SPECENUM_SIZE,
                   valid_array_size_check);

  return ((unsigned)enumerator < ARRAY_SIZE(valid)
          && valid[enumerator]);
#endif /* SPECENUM_BITWISE */
}

/**********************************************************************//**
  Returns an invalid enumerator value.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_invalid)(void)
{
  fc_assert(!SPECENUM_FOO(_is_valid(SPECENUM_INVALID)));
  return SPECENUM_INVALID;
}

/**********************************************************************//**
  Beginning of the iteration of the enumerators.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_begin)(void)
{
  return SPECENUM_FOO(_min)();
}

/**********************************************************************//**
  End of the iteration of the enumerators.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_end)(void)
{
  return SPECENUM_FOO(_invalid)();
}

/**********************************************************************//**
  Find the next valid enumerator value.
**************************************************************************/
fc__attribute((const))
static inline enum SPECENUM_NAME SPECENUM_FOO(_next)(enum SPECENUM_NAME e)
{
  do {
#ifdef SPECENUM_BITWISE
    e = (enum SPECENUM_NAME)(e << 1);
#else
    e = (enum SPECENUM_NAME)(e + 1);
#endif

    if (e > SPECENUM_FOO(_max)()) {
      /* End of the iteration. */
      return SPECENUM_FOO(_invalid)();
    }
  } while (!SPECENUM_FOO(_is_valid)(e));

  return e;
}

#ifdef SPECENUM_NAMEOVERRIDE
const char *SPECENUM_FOO(_name_cb)(enum SPECENUM_NAME value);
#endif /* SPECENUM_NAMEOVERRIDE */

/**********************************************************************//**
  Returns the name of the enumerator.
**************************************************************************/
#ifndef SPECENUM_NAMEOVERRIDE
fc__attribute((const))
#endif
static inline const char *SPECENUM_FOO(_name)(enum SPECENUM_NAME enumerator)
{
#ifdef SPECENUM_COUNT
  static const char *names[SPECENUM_SIZE + 1];
#else
  static const char *names[SPECENUM_SIZE];
#endif
  static bool initialized = FALSE;

#ifdef SPECENUM_NAMEOVERRIDE
  {
    const char *name = SPECENUM_FOO(_name_cb)(enumerator);

    if (name != NULL) {
      return Qn_(name);
    }
  }
#endif /* SPECENUM_NAMEOVERRIDE */

  if (!initialized) {
#if 0 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE0
     names[0] = NULL;
#  elif defined(SPECENUM_VALUE0NAME)
     names[0] = Qn_(SPECENUM_VALUE0NAME);
#  else
     names[0] = SPECENUM_STRING(SPECENUM_VALUE0);
#  endif
#endif
#if 1 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE1
     names[1] = NULL;
#  elif defined(SPECENUM_VALUE1NAME)
     names[1] = Qn_(SPECENUM_VALUE1NAME);
#  else
     names[1] = SPECENUM_STRING(SPECENUM_VALUE1);
#  endif
#endif
#if 2 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE2
     names[2] = NULL;
#  elif defined(SPECENUM_VALUE2NAME)
     names[2] = Qn_(SPECENUM_VALUE2NAME);
#  else
     names[2] = SPECENUM_STRING(SPECENUM_VALUE2);
#  endif
#endif
#if 3 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE3
     names[3] = NULL;
#  elif defined(SPECENUM_VALUE3NAME)
     names[3] = Qn_(SPECENUM_VALUE3NAME);
#  else
     names[3] = SPECENUM_STRING(SPECENUM_VALUE3);
#  endif
#endif
#if 4 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE4
     names[4] = NULL;
#  elif defined(SPECENUM_VALUE4NAME)
     names[4] = Qn_(SPECENUM_VALUE4NAME);
#  else
     names[4] = SPECENUM_STRING(SPECENUM_VALUE4);
#  endif
#endif
#if 5 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE5
     names[5] = NULL;
#  elif defined(SPECENUM_VALUE5NAME)
     names[5] = Qn_(SPECENUM_VALUE5NAME);
#  else
     names[5] = SPECENUM_STRING(SPECENUM_VALUE5);
#  endif
#endif
#if 6 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE6
     names[6] = NULL;
#  elif defined(SPECENUM_VALUE6NAME)
     names[6] = Qn_(SPECENUM_VALUE6NAME);
#  else
     names[6] = SPECENUM_STRING(SPECENUM_VALUE6);
#  endif
#endif
#if 7 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE7
     names[7] = NULL;
#  elif defined(SPECENUM_VALUE7NAME)
     names[7] = Qn_(SPECENUM_VALUE7NAME);
#  else
     names[7] = SPECENUM_STRING(SPECENUM_VALUE7);
#  endif
#endif
#if 8 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE8
     names[8] = NULL;
#  elif defined(SPECENUM_VALUE8NAME)
     names[8] = Qn_(SPECENUM_VALUE8NAME);
#  else
     names[8] = SPECENUM_STRING(SPECENUM_VALUE8);
#  endif
#endif
#if 9 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE9
     names[9] = NULL;
#  elif defined(SPECENUM_VALUE9NAME)
     names[9] = Qn_(SPECENUM_VALUE9NAME);
#  else
     names[9] = SPECENUM_STRING(SPECENUM_VALUE9);
#  endif
#endif
#if 10 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE10
     names[10] = NULL;
#  elif defined(SPECENUM_VALUE10NAME)
     names[10] = Qn_(SPECENUM_VALUE10NAME);
#  else
     names[10] = SPECENUM_STRING(SPECENUM_VALUE10);
#  endif
#endif
#if 11 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE11
     names[11] = NULL;
#  elif defined(SPECENUM_VALUE11NAME)
     names[11] = Qn_(SPECENUM_VALUE11NAME);
#  else
     names[11] = SPECENUM_STRING(SPECENUM_VALUE11);
#  endif
#endif
#if 12 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE12
     names[12] = NULL;
#  elif defined(SPECENUM_VALUE12NAME)
     names[12] = Qn_(SPECENUM_VALUE12NAME);
#  else
     names[12] = SPECENUM_STRING(SPECENUM_VALUE12);
#  endif
#endif
#if 13 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE13
     names[13] = NULL;
#  elif defined(SPECENUM_VALUE13NAME)
     names[13] = Qn_(SPECENUM_VALUE13NAME);
#  else
     names[13] = SPECENUM_STRING(SPECENUM_VALUE13);
#  endif
#endif
#if 14 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE14
     names[14] = NULL;
#  elif defined(SPECENUM_VALUE14NAME)
     names[14] = Qn_(SPECENUM_VALUE14NAME);
#  else
     names[14] = SPECENUM_STRING(SPECENUM_VALUE14);
#  endif
#endif
#if 15 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE15
     names[15] = NULL;
#  elif defined(SPECENUM_VALUE15NAME)
     names[15] = Qn_(SPECENUM_VALUE15NAME);
#  else
     names[15] = SPECENUM_STRING(SPECENUM_VALUE15);
#  endif
#endif
#if 16 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE16
     names[16] = NULL;
#  elif defined(SPECENUM_VALUE16NAME)
     names[16] = Qn_(SPECENUM_VALUE16NAME);
#  else
     names[16] = SPECENUM_STRING(SPECENUM_VALUE16);
#  endif
#endif
#if 17 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE17
     names[17] = NULL;
#  elif defined(SPECENUM_VALUE17NAME)
     names[17] = Qn_(SPECENUM_VALUE17NAME);
#  else
     names[17] = SPECENUM_STRING(SPECENUM_VALUE17);
#  endif
#endif
#if 18 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE18
     names[18] = NULL;
#  elif defined(SPECENUM_VALUE18NAME)
     names[18] = Qn_(SPECENUM_VALUE18NAME);
#  else
     names[18] = SPECENUM_STRING(SPECENUM_VALUE18);
#  endif
#endif
#if 19 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE19
     names[19] = NULL;
#  elif defined(SPECENUM_VALUE19NAME)
     names[19] = Qn_(SPECENUM_VALUE19NAME);
#  else
     names[19] = SPECENUM_STRING(SPECENUM_VALUE19);
#  endif
#endif
#if 20 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE20
     names[20] = NULL;
#  elif defined(SPECENUM_VALUE20NAME)
     names[20] = Qn_(SPECENUM_VALUE20NAME);
#  else
     names[20] = SPECENUM_STRING(SPECENUM_VALUE20);
#  endif
#endif
#if 21 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE21
     names[21] = NULL;
#  elif defined(SPECENUM_VALUE21NAME)
     names[21] = Qn_(SPECENUM_VALUE21NAME);
#  else
     names[21] = SPECENUM_STRING(SPECENUM_VALUE21);
#  endif
#endif
#if 22 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE22
     names[22] = NULL;
#  elif defined(SPECENUM_VALUE22NAME)
     names[22] = Qn_(SPECENUM_VALUE22NAME);
#  else
     names[22] = SPECENUM_STRING(SPECENUM_VALUE22);
#  endif
#endif
#if 23 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE23
     names[23] = NULL;
#  elif defined(SPECENUM_VALUE23NAME)
     names[23] = Qn_(SPECENUM_VALUE23NAME);
#  else
     names[23] = SPECENUM_STRING(SPECENUM_VALUE23);
#  endif
#endif
#if 24 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE24
     names[24] = NULL;
#  elif defined(SPECENUM_VALUE24NAME)
     names[24] = Qn_(SPECENUM_VALUE24NAME);
#  else
     names[24] = SPECENUM_STRING(SPECENUM_VALUE24);
#  endif
#endif
#if 25 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE25
     names[25] = NULL;
#  elif defined(SPECENUM_VALUE25NAME)
     names[25] = Qn_(SPECENUM_VALUE25NAME);
#  else
     names[25] = SPECENUM_STRING(SPECENUM_VALUE25);
#  endif
#endif
#if 26 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE26
     names[26] = NULL;
#  elif defined(SPECENUM_VALUE26NAME)
     names[26] = Qn_(SPECENUM_VALUE26NAME);
#  else
     names[26] = SPECENUM_STRING(SPECENUM_VALUE26);
#  endif
#endif
#if 27 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE27
     names[27] = NULL;
#  elif defined(SPECENUM_VALUE27NAME)
     names[27] = Qn_(SPECENUM_VALUE27NAME);
#  else
     names[27] = SPECENUM_STRING(SPECENUM_VALUE27);
#  endif
#endif
#if 28 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE28
     names[28] = NULL;
#  elif defined(SPECENUM_VALUE28NAME)
     names[28] = Qn_(SPECENUM_VALUE28NAME);
#  else
     names[28] = SPECENUM_STRING(SPECENUM_VALUE28);
#  endif
#endif
#if 29 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE29
     names[29] = NULL;
#  elif defined(SPECENUM_VALUE29NAME)
     names[29] = Qn_(SPECENUM_VALUE29NAME);
#  else
     names[29] = SPECENUM_STRING(SPECENUM_VALUE29);
#  endif
#endif
#if 30 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE30
     names[30] = NULL;
#  elif defined(SPECENUM_VALUE30NAME)
     names[30] = Qn_(SPECENUM_VALUE30NAME);
#  else
     names[30] = SPECENUM_STRING(SPECENUM_VALUE30);
#  endif
#endif
#if 31 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE31
     names[31] = NULL;
#  elif defined(SPECENUM_VALUE31NAME)
     names[31] = Qn_(SPECENUM_VALUE31NAME);
#  else
     names[31] = SPECENUM_STRING(SPECENUM_VALUE31);
#  endif
#endif
#if 32 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE32
     names[32] = NULL;
#  elif defined(SPECENUM_VALUE32NAME)
     names[32] = Qn_(SPECENUM_VALUE32NAME);
#  else
     names[32] = SPECENUM_STRING(SPECENUM_VALUE32);
#  endif
#endif
#if 33 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE33
     names[33] = NULL;
#  elif defined(SPECENUM_VALUE33NAME)
     names[33] = Qn_(SPECENUM_VALUE33NAME);
#  else
     names[33] = SPECENUM_STRING(SPECENUM_VALUE33);
#  endif
#endif
#if 34 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE34
     names[34] = NULL;
#  elif defined(SPECENUM_VALUE34NAME)
     names[34] = Qn_(SPECENUM_VALUE34NAME);
#  else
     names[34] = SPECENUM_STRING(SPECENUM_VALUE34);
#  endif
#endif
#if 35 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE35
     names[35] = NULL;
#  elif defined(SPECENUM_VALUE35NAME)
     names[35] = Qn_(SPECENUM_VALUE35NAME);
#  else
     names[35] = SPECENUM_STRING(SPECENUM_VALUE35);
#  endif
#endif
#if 36 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE36
     names[36] = NULL;
#  elif defined(SPECENUM_VALUE36NAME)
     names[36] = Qn_(SPECENUM_VALUE36NAME);
#  else
     names[36] = SPECENUM_STRING(SPECENUM_VALUE36);
#  endif
#endif
#if 37 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE37
     names[37] = NULL;
#  elif defined(SPECENUM_VALUE37NAME)
     names[37] = Qn_(SPECENUM_VALUE37NAME);
#  else
     names[37] = SPECENUM_STRING(SPECENUM_VALUE37);
#  endif
#endif
#if 38 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE38
     names[38] = NULL;
#  elif defined(SPECENUM_VALUE38NAME)
     names[38] = Qn_(SPECENUM_VALUE38NAME);
#  else
     names[38] = SPECENUM_STRING(SPECENUM_VALUE38);
#  endif
#endif
#if 39 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE39
     names[39] = NULL;
#  elif defined(SPECENUM_VALUE39NAME)
     names[39] = Qn_(SPECENUM_VALUE39NAME);
#  else
     names[39] = SPECENUM_STRING(SPECENUM_VALUE39);
#  endif
#endif
#if 40 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE40
     names[40] = NULL;
#  elif defined(SPECENUM_VALUE40NAME)
     names[40] = Qn_(SPECENUM_VALUE40NAME);
#  else
     names[40] = SPECENUM_STRING(SPECENUM_VALUE40);
#  endif
#endif
#if 41 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE41
     names[41] = NULL;
#  elif defined(SPECENUM_VALUE41NAME)
     names[41] = Qn_(SPECENUM_VALUE41NAME);
#  else
     names[41] = SPECENUM_STRING(SPECENUM_VALUE41);
#  endif
#endif
#if 42 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE42
     names[42] = NULL;
#  elif defined(SPECENUM_VALUE42NAME)
     names[42] = Qn_(SPECENUM_VALUE42NAME);
#  else
     names[42] = SPECENUM_STRING(SPECENUM_VALUE42);
#  endif
#endif
#if 43 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE43
     names[43] = NULL;
#  elif defined(SPECENUM_VALUE43NAME)
     names[43] = Qn_(SPECENUM_VALUE43NAME);
#  else
     names[43] = SPECENUM_STRING(SPECENUM_VALUE43);
#  endif
#endif
#if 44 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE44
     names[44] = NULL;
#  elif defined(SPECENUM_VALUE44NAME)
     names[44] = Qn_(SPECENUM_VALUE44NAME);
#  else
     names[44] = SPECENUM_STRING(SPECENUM_VALUE44);
#  endif
#endif
#if 45 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE45
     names[45] = NULL;
#  elif defined(SPECENUM_VALUE45NAME)
     names[45] = Qn_(SPECENUM_VALUE45NAME);
#  else
     names[45] = SPECENUM_STRING(SPECENUM_VALUE45);
#  endif
#endif
#if 46 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE46
     names[46] = NULL;
#  elif defined(SPECENUM_VALUE46NAME)
     names[46] = Qn_(SPECENUM_VALUE46NAME);
#  else
     names[46] = SPECENUM_STRING(SPECENUM_VALUE46);
#  endif
#endif
#if 47 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE47
     names[47] = NULL;
#  elif defined(SPECENUM_VALUE47NAME)
     names[47] = Qn_(SPECENUM_VALUE47NAME);
#  else
     names[47] = SPECENUM_STRING(SPECENUM_VALUE47);
#  endif
#endif
#if 48 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE48
     names[48] = NULL;
#  elif defined(SPECENUM_VALUE48NAME)
     names[48] = Qn_(SPECENUM_VALUE48NAME);
#  else
     names[48] = SPECENUM_STRING(SPECENUM_VALUE48);
#  endif
#endif
#if 49 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE49
     names[49] = NULL;
#  elif defined(SPECENUM_VALUE49NAME)
     names[49] = Qn_(SPECENUM_VALUE49NAME);
#  else
     names[49] = SPECENUM_STRING(SPECENUM_VALUE49);
#  endif
#endif
#if 50 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE50
     names[50] = NULL;
#  elif defined(SPECENUM_VALUE50NAME)
     names[50] = Qn_(SPECENUM_VALUE50NAME);
#  else
     names[50] = SPECENUM_STRING(SPECENUM_VALUE50);
#  endif
#endif
#if 51 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE51
     names[51] = NULL;
#  elif defined(SPECENUM_VALUE51NAME)
     names[51] = Qn_(SPECENUM_VALUE51NAME);
#  else
     names[51] = SPECENUM_STRING(SPECENUM_VALUE51);
#  endif
#endif
#if 52 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE52
     names[52] = NULL;
#  elif defined(SPECENUM_VALUE52NAME)
     names[52] = Qn_(SPECENUM_VALUE52NAME);
#  else
     names[52] = SPECENUM_STRING(SPECENUM_VALUE52);
#  endif
#endif
#if 53 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE53
     names[53] = NULL;
#  elif defined(SPECENUM_VALUE53NAME)
     names[53] = Qn_(SPECENUM_VALUE53NAME);
#  else
     names[53] = SPECENUM_STRING(SPECENUM_VALUE53);
#  endif
#endif
#if 54 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE54
     names[54] = NULL;
#  elif defined(SPECENUM_VALUE54NAME)
     names[54] = Qn_(SPECENUM_VALUE54NAME);
#  else
     names[54] = SPECENUM_STRING(SPECENUM_VALUE54);
#  endif
#endif
#if 55 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE55
     names[55] = NULL;
#  elif defined(SPECENUM_VALUE55NAME)
     names[55] = Qn_(SPECENUM_VALUE55NAME);
#  else
     names[55] = SPECENUM_STRING(SPECENUM_VALUE55);
#  endif
#endif
#if 56 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE56
     names[56] = NULL;
#  elif defined(SPECENUM_VALUE56NAME)
     names[56] = Qn_(SPECENUM_VALUE56NAME);
#  else
     names[56] = SPECENUM_STRING(SPECENUM_VALUE56);
#  endif
#endif
#if 57 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE57
     names[57] = NULL;
#  elif defined(SPECENUM_VALUE57NAME)
     names[57] = Qn_(SPECENUM_VALUE57NAME);
#  else
     names[57] = SPECENUM_STRING(SPECENUM_VALUE57);
#  endif
#endif
#if 58 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE58
     names[58] = NULL;
#  elif defined(SPECENUM_VALUE58NAME)
     names[58] = Qn_(SPECENUM_VALUE58NAME);
#  else
     names[58] = SPECENUM_STRING(SPECENUM_VALUE58);
#  endif
#endif
#if 59 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE59
     names[59] = NULL;
#  elif defined(SPECENUM_VALUE59NAME)
     names[59] = Qn_(SPECENUM_VALUE59NAME);
#  else
     names[59] = SPECENUM_STRING(SPECENUM_VALUE59);
#  endif
#endif
#if 60 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE60
     names[60] = NULL;
#  elif defined(SPECENUM_VALUE60NAME)
     names[60] = Qn_(SPECENUM_VALUE60NAME);
#  else
     names[60] = SPECENUM_STRING(SPECENUM_VALUE60);
#  endif
#endif
#if 61 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE61
     names[61] = NULL;
#  elif defined(SPECENUM_VALUE61NAME)
     names[61] = Qn_(SPECENUM_VALUE61NAME);
#  else
     names[61] = SPECENUM_STRING(SPECENUM_VALUE61);
#  endif
#endif
#if 62 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE62
     names[62] = NULL;
#  elif defined(SPECENUM_VALUE62NAME)
     names[62] = Qn_(SPECENUM_VALUE62NAME);
#  else
     names[62] = SPECENUM_STRING(SPECENUM_VALUE62);
#  endif
#endif
#if 63 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE63
     names[63] = NULL;
#  elif defined(SPECENUM_VALUE63NAME)
     names[63] = Qn_(SPECENUM_VALUE63NAME);
#  else
     names[63] = SPECENUM_STRING(SPECENUM_VALUE63);
#  endif
#endif
#if 64 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE64
     names[64] = NULL;
#  elif defined(SPECENUM_VALUE64NAME)
     names[64] = Qn_(SPECENUM_VALUE64NAME);
#  else
     names[64] = SPECENUM_STRING(SPECENUM_VALUE64);
#  endif
#endif
#if 65 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE65
     names[65] = NULL;
#  elif defined(SPECENUM_VALUE65NAME)
     names[65] = Qn_(SPECENUM_VALUE65NAME);
#  else
     names[65] = SPECENUM_STRING(SPECENUM_VALUE65);
#  endif
#endif
#if 66 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE66
     names[66] = NULL;
#  elif defined(SPECENUM_VALUE66NAME)
     names[66] = Qn_(SPECENUM_VALUE66NAME);
#  else
     names[66] = SPECENUM_STRING(SPECENUM_VALUE66);
#  endif
#endif
#if 67 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE67
     names[67] = NULL;
#  elif defined(SPECENUM_VALUE67NAME)
     names[67] = Qn_(SPECENUM_VALUE67NAME);
#  else
     names[67] = SPECENUM_STRING(SPECENUM_VALUE67);
#  endif
#endif
#if 68 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE68
     names[68] = NULL;
#  elif defined(SPECENUM_VALUE68NAME)
     names[68] = Qn_(SPECENUM_VALUE68NAME);
#  else
     names[68] = SPECENUM_STRING(SPECENUM_VALUE68);
#  endif
#endif
#if 69 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE69
     names[69] = NULL;
#  elif defined(SPECENUM_VALUE69NAME)
     names[69] = Qn_(SPECENUM_VALUE69NAME);
#  else
     names[69] = SPECENUM_STRING(SPECENUM_VALUE69);
#  endif
#endif
#if 70 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE70
     names[70] = NULL;
#  elif defined(SPECENUM_VALUE70NAME)
     names[70] = Qn_(SPECENUM_VALUE70NAME);
#  else
     names[70] = SPECENUM_STRING(SPECENUM_VALUE70);
#  endif
#endif
#if 71 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE71
     names[71] = NULL;
#  elif defined(SPECENUM_VALUE71NAME)
     names[71] = Qn_(SPECENUM_VALUE71NAME);
#  else
     names[71] = SPECENUM_STRING(SPECENUM_VALUE71);
#  endif
#endif
#if 72 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE72
     names[72] = NULL;
#  elif defined(SPECENUM_VALUE72NAME)
     names[72] = Qn_(SPECENUM_VALUE72NAME);
#  else
     names[72] = SPECENUM_STRING(SPECENUM_VALUE72);
#  endif
#endif
#if 73 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE73
     names[73] = NULL;
#  elif defined(SPECENUM_VALUE73NAME)
     names[73] = Qn_(SPECENUM_VALUE73NAME);
#  else
     names[73] = SPECENUM_STRING(SPECENUM_VALUE73);
#  endif
#endif
#if 74 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE74
     names[74] = NULL;
#  elif defined(SPECENUM_VALUE74NAME)
     names[74] = Qn_(SPECENUM_VALUE74NAME);
#  else
     names[74] = SPECENUM_STRING(SPECENUM_VALUE74);
#  endif
#endif
#if 75 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE75
     names[75] = NULL;
#  elif defined(SPECENUM_VALUE75NAME)
     names[75] = Qn_(SPECENUM_VALUE75NAME);
#  else
     names[75] = SPECENUM_STRING(SPECENUM_VALUE75);
#  endif
#endif
#if 76 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE76
     names[76] = NULL;
#  elif defined(SPECENUM_VALUE76NAME)
     names[76] = Qn_(SPECENUM_VALUE76NAME);
#  else
     names[76] = SPECENUM_STRING(SPECENUM_VALUE76);
#  endif
#endif
#if 77 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE77
     names[77] = NULL;
#  elif defined(SPECENUM_VALUE77NAME)
     names[77] = Qn_(SPECENUM_VALUE77NAME);
#  else
     names[77] = SPECENUM_STRING(SPECENUM_VALUE77);
#  endif
#endif
#if 78 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE78
     names[78] = NULL;
#  elif defined(SPECENUM_VALUE78NAME)
     names[78] = Qn_(SPECENUM_VALUE78NAME);
#  else
     names[78] = SPECENUM_STRING(SPECENUM_VALUE78);
#  endif
#endif
#if 79 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE79
     names[79] = NULL;
#  elif defined(SPECENUM_VALUE79NAME)
     names[79] = Qn_(SPECENUM_VALUE79NAME);
#  else
     names[79] = SPECENUM_STRING(SPECENUM_VALUE79);
#  endif
#endif
#if 80 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE80
     names[80] = NULL;
#  elif defined(SPECENUM_VALUE80NAME)
     names[80] = Qn_(SPECENUM_VALUE80NAME);
#  else
     names[80] = SPECENUM_STRING(SPECENUM_VALUE80);
#  endif
#endif
#if 81 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE81
     names[81] = NULL;
#  elif defined(SPECENUM_VALUE81NAME)
     names[81] = Qn_(SPECENUM_VALUE81NAME);
#  else
     names[81] = SPECENUM_STRING(SPECENUM_VALUE81);
#  endif
#endif
#if 82 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE82
     names[82] = NULL;
#  elif defined(SPECENUM_VALUE82NAME)
     names[82] = Qn_(SPECENUM_VALUE82NAME);
#  else
     names[82] = SPECENUM_STRING(SPECENUM_VALUE82);
#  endif
#endif
#if 83 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE83
     names[83] = NULL;
#  elif defined(SPECENUM_VALUE83NAME)
     names[83] = Qn_(SPECENUM_VALUE83NAME);
#  else
     names[83] = SPECENUM_STRING(SPECENUM_VALUE83);
#  endif
#endif
#if 84 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE84
     names[84] = NULL;
#  elif defined(SPECENUM_VALUE84NAME)
     names[84] = Qn_(SPECENUM_VALUE84NAME);
#  else
     names[84] = SPECENUM_STRING(SPECENUM_VALUE84);
#  endif
#endif
#if 85 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE85
     names[85] = NULL;
#  elif defined(SPECENUM_VALUE85NAME)
     names[85] = Qn_(SPECENUM_VALUE85NAME);
#  else
     names[85] = SPECENUM_STRING(SPECENUM_VALUE85);
#  endif
#endif
#if 86 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE86
     names[86] = NULL;
#  elif defined(SPECENUM_VALUE86NAME)
     names[86] = Qn_(SPECENUM_VALUE86NAME);
#  else
     names[86] = SPECENUM_STRING(SPECENUM_VALUE86);
#  endif
#endif
#if 87 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE87
     names[87] = NULL;
#  elif defined(SPECENUM_VALUE87NAME)
     names[87] = Qn_(SPECENUM_VALUE87NAME);
#  else
     names[87] = SPECENUM_STRING(SPECENUM_VALUE87);
#  endif
#endif
#if 88 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE88
     names[88] = NULL;
#  elif defined(SPECENUM_VALUE88NAME)
     names[88] = Qn_(SPECENUM_VALUE88NAME);
#  else
     names[88] = SPECENUM_STRING(SPECENUM_VALUE88);
#  endif
#endif
#if 89 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE89
     names[89] = NULL;
#  elif defined(SPECENUM_VALUE89NAME)
     names[89] = Qn_(SPECENUM_VALUE89NAME);
#  else
     names[89] = SPECENUM_STRING(SPECENUM_VALUE89);
#  endif
#endif
#if 90 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE90
     names[90] = NULL;
#  elif defined(SPECENUM_VALUE90NAME)
     names[90] = Qn_(SPECENUM_VALUE90NAME);
#  else
     names[90] = SPECENUM_STRING(SPECENUM_VALUE90);
#  endif
#endif
#if 91 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE91
     names[91] = NULL;
#  elif defined(SPECENUM_VALUE91NAME)
     names[91] = Qn_(SPECENUM_VALUE91NAME);
#  else
     names[91] = SPECENUM_STRING(SPECENUM_VALUE91);
#  endif
#endif
#if 92 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE92
     names[92] = NULL;
#  elif defined(SPECENUM_VALUE92NAME)
     names[92] = Qn_(SPECENUM_VALUE92NAME);
#  else
     names[92] = SPECENUM_STRING(SPECENUM_VALUE92);
#  endif
#endif
#if 93 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE93
     names[93] = NULL;
#  elif defined(SPECENUM_VALUE93NAME)
     names[93] = Qn_(SPECENUM_VALUE93NAME);
#  else
     names[93] = SPECENUM_STRING(SPECENUM_VALUE93);
#  endif
#endif
#if 94 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE94
     names[94] = NULL;
#  elif defined(SPECENUM_VALUE94NAME)
     names[94] = Qn_(SPECENUM_VALUE94NAME);
#  else
     names[94] = SPECENUM_STRING(SPECENUM_VALUE94);
#  endif
#endif
#if 95 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE95
     names[95] = NULL;
#  elif defined(SPECENUM_VALUE95NAME)
     names[95] = Qn_(SPECENUM_VALUE95NAME);
#  else
     names[95] = SPECENUM_STRING(SPECENUM_VALUE95);
#  endif
#endif
#if 96 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE96
     names[96] = NULL;
#  elif defined(SPECENUM_VALUE96NAME)
     names[96] = Qn_(SPECENUM_VALUE96NAME);
#  else
     names[96] = SPECENUM_STRING(SPECENUM_VALUE96);
#  endif
#endif
#if 97 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE97
     names[97] = NULL;
#  elif defined(SPECENUM_VALUE97NAME)
     names[97] = Qn_(SPECENUM_VALUE97NAME);
#  else
     names[97] = SPECENUM_STRING(SPECENUM_VALUE97);
#  endif
#endif
#if 98 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE98
     names[98] = NULL;
#  elif defined(SPECENUM_VALUE98NAME)
     names[98] = Qn_(SPECENUM_VALUE98NAME);
#  else
     names[98] = SPECENUM_STRING(SPECENUM_VALUE98);
#  endif
#endif
#if 99 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE99
     names[99] = NULL;
#  elif defined(SPECENUM_VALUE99NAME)
     names[99] = Qn_(SPECENUM_VALUE99NAME);
#  else
     names[99] = SPECENUM_STRING(SPECENUM_VALUE99);
#  endif
#endif
#if 100 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE100
     names[100] = NULL;
#  elif defined(SPECENUM_VALUE100NAME)
     names[100] = Qn_(SPECENUM_VALUE100NAME);
#  else
     names[100] = SPECENUM_STRING(SPECENUM_VALUE100);
#  endif
#endif
#if 101 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE101
     names[101] = NULL;
#  elif defined(SPECENUM_VALUE101NAME)
     names[101] = Qn_(SPECENUM_VALUE101NAME);
#  else
     names[101] = SPECENUM_STRING(SPECENUM_VALUE101);
#  endif
#endif
#if 102 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE102
     names[102] = NULL;
#  elif defined(SPECENUM_VALUE102NAME)
     names[102] = Qn_(SPECENUM_VALUE102NAME);
#  else
     names[102] = SPECENUM_STRING(SPECENUM_VALUE102);
#  endif
#endif
#if 103 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE103
     names[103] = NULL;
#  elif defined(SPECENUM_VALUE103NAME)
     names[103] = Qn_(SPECENUM_VALUE103NAME);
#  else
     names[103] = SPECENUM_STRING(SPECENUM_VALUE103);
#  endif
#endif
#if 104 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE104
     names[104] = NULL;
#  elif defined(SPECENUM_VALUE104NAME)
     names[104] = Qn_(SPECENUM_VALUE104NAME);
#  else
     names[104] = SPECENUM_STRING(SPECENUM_VALUE104);
#  endif
#endif
#if 105 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE105
     names[105] = NULL;
#  elif defined(SPECENUM_VALUE105NAME)
     names[105] = Qn_(SPECENUM_VALUE105NAME);
#  else
     names[105] = SPECENUM_STRING(SPECENUM_VALUE105);
#  endif
#endif
#if 106 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE106
     names[106] = NULL;
#  elif defined(SPECENUM_VALUE106NAME)
     names[106] = Qn_(SPECENUM_VALUE106NAME);
#  else
     names[106] = SPECENUM_STRING(SPECENUM_VALUE106);
#  endif
#endif
#if 107 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE107
     names[107] = NULL;
#  elif defined(SPECENUM_VALUE107NAME)
     names[107] = Qn_(SPECENUM_VALUE107NAME);
#  else
     names[107] = SPECENUM_STRING(SPECENUM_VALUE107);
#  endif
#endif
#if 108 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE108
     names[108] = NULL;
#  elif defined(SPECENUM_VALUE108NAME)
     names[108] = Qn_(SPECENUM_VALUE108NAME);
#  else
     names[108] = SPECENUM_STRING(SPECENUM_VALUE108);
#  endif
#endif
#if 109 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE109
     names[109] = NULL;
#  elif defined(SPECENUM_VALUE109NAME)
     names[109] = Qn_(SPECENUM_VALUE109NAME);
#  else
     names[109] = SPECENUM_STRING(SPECENUM_VALUE109);
#  endif
#endif
#if 110 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE110
     names[110] = NULL;
#  elif defined(SPECENUM_VALUE110NAME)
     names[110] = Qn_(SPECENUM_VALUE110NAME);
#  else
     names[110] = SPECENUM_STRING(SPECENUM_VALUE110);
#  endif
#endif
#if 111 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE111
     names[111] = NULL;
#  elif defined(SPECENUM_VALUE111NAME)
     names[111] = Qn_(SPECENUM_VALUE111NAME);
#  else
     names[111] = SPECENUM_STRING(SPECENUM_VALUE111);
#  endif
#endif
#if 112 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE112
     names[112] = NULL;
#  elif defined(SPECENUM_VALUE112NAME)
     names[112] = Qn_(SPECENUM_VALUE112NAME);
#  else
     names[112] = SPECENUM_STRING(SPECENUM_VALUE112);
#  endif
#endif
#if 113 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE113
     names[113] = NULL;
#  elif defined(SPECENUM_VALUE113NAME)
     names[113] = Qn_(SPECENUM_VALUE113NAME);
#  else
     names[113] = SPECENUM_STRING(SPECENUM_VALUE113);
#  endif
#endif
#if 114 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE114
     names[114] = NULL;
#  elif defined(SPECENUM_VALUE114NAME)
     names[114] = Qn_(SPECENUM_VALUE114NAME);
#  else
     names[114] = SPECENUM_STRING(SPECENUM_VALUE114);
#  endif
#endif
#if 115 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE115
     names[115] = NULL;
#  elif defined(SPECENUM_VALUE115NAME)
     names[115] = Qn_(SPECENUM_VALUE115NAME);
#  else
     names[115] = SPECENUM_STRING(SPECENUM_VALUE115);
#  endif
#endif
#if 116 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE116
     names[116] = NULL;
#  elif defined(SPECENUM_VALUE116NAME)
     names[116] = Qn_(SPECENUM_VALUE116NAME);
#  else
     names[116] = SPECENUM_STRING(SPECENUM_VALUE116);
#  endif
#endif
#if 117 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE117
     names[117] = NULL;
#  elif defined(SPECENUM_VALUE117NAME)
     names[117] = Qn_(SPECENUM_VALUE117NAME);
#  else
     names[117] = SPECENUM_STRING(SPECENUM_VALUE117);
#  endif
#endif
#if 118 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE118
     names[118] = NULL;
#  elif defined(SPECENUM_VALUE118NAME)
     names[118] = Qn_(SPECENUM_VALUE118NAME);
#  else
     names[118] = SPECENUM_STRING(SPECENUM_VALUE118);
#  endif
#endif
#if 119 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE119
     names[119] = NULL;
#  elif defined(SPECENUM_VALUE119NAME)
     names[119] = Qn_(SPECENUM_VALUE119NAME);
#  else
     names[119] = SPECENUM_STRING(SPECENUM_VALUE119);
#  endif
#endif
#if 120 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE120
     names[120] = NULL;
#  elif defined(SPECENUM_VALUE120NAME)
     names[120] = Qn_(SPECENUM_VALUE120NAME);
#  else
     names[120] = SPECENUM_STRING(SPECENUM_VALUE120);
#  endif
#endif
#if 121 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE121
     names[121] = NULL;
#  elif defined(SPECENUM_VALUE121NAME)
     names[121] = Qn_(SPECENUM_VALUE121NAME);
#  else
     names[121] = SPECENUM_STRING(SPECENUM_VALUE121);
#  endif
#endif
#if 122 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE122
     names[122] = NULL;
#  elif defined(SPECENUM_VALUE122NAME)
     names[122] = Qn_(SPECENUM_VALUE122NAME);
#  else
     names[122] = SPECENUM_STRING(SPECENUM_VALUE122);
#  endif
#endif
#if 123 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE123
     names[123] = NULL;
#  elif defined(SPECENUM_VALUE123NAME)
     names[123] = Qn_(SPECENUM_VALUE123NAME);
#  else
     names[123] = SPECENUM_STRING(SPECENUM_VALUE123);
#  endif
#endif
#if 124 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE124
     names[124] = NULL;
#  elif defined(SPECENUM_VALUE124NAME)
     names[124] = Qn_(SPECENUM_VALUE124NAME);
#  else
     names[124] = SPECENUM_STRING(SPECENUM_VALUE124);
#  endif
#endif
#if 125 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE125
     names[125] = NULL;
#  elif defined(SPECENUM_VALUE125NAME)
     names[125] = Qn_(SPECENUM_VALUE125NAME);
#  else
     names[125] = SPECENUM_STRING(SPECENUM_VALUE125);
#  endif
#endif
#if 126 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE126
     names[126] = NULL;
#  elif defined(SPECENUM_VALUE126NAME)
     names[126] = Qn_(SPECENUM_VALUE126NAME);
#  else
     names[126] = SPECENUM_STRING(SPECENUM_VALUE126);
#  endif
#endif
#if 127 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE127
     names[127] = NULL;
#  elif defined(SPECENUM_VALUE127NAME)
     names[127] = Qn_(SPECENUM_VALUE127NAME);
#  else
     names[127] = SPECENUM_STRING(SPECENUM_VALUE127);
#  endif
#endif
#if 128 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE128
     names[128] = NULL;
#  elif defined(SPECENUM_VALUE128NAME)
     names[128] = Qn_(SPECENUM_VALUE128NAME);
#  else
     names[128] = SPECENUM_STRING(SPECENUM_VALUE128);
#  endif
#endif
#if 129 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE129
     names[129] = NULL;
#  elif defined(SPECENUM_VALUE129NAME)
     names[129] = Qn_(SPECENUM_VALUE129NAME);
#  else
     names[129] = SPECENUM_STRING(SPECENUM_VALUE129);
#  endif
#endif
#if 130 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE130
     names[130] = NULL;
#  elif defined(SPECENUM_VALUE130NAME)
     names[130] = Qn_(SPECENUM_VALUE130NAME);
#  else
     names[130] = SPECENUM_STRING(SPECENUM_VALUE130);
#  endif
#endif
#if 131 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE131
     names[131] = NULL;
#  elif defined(SPECENUM_VALUE131NAME)
     names[131] = Qn_(SPECENUM_VALUE131NAME);
#  else
     names[131] = SPECENUM_STRING(SPECENUM_VALUE131);
#  endif
#endif
#if 132 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE132
     names[132] = NULL;
#  elif defined(SPECENUM_VALUE132NAME)
     names[132] = Qn_(SPECENUM_VALUE132NAME);
#  else
     names[132] = SPECENUM_STRING(SPECENUM_VALUE132);
#  endif
#endif
#if 133 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE133
     names[133] = NULL;
#  elif defined(SPECENUM_VALUE133NAME)
     names[133] = Qn_(SPECENUM_VALUE133NAME);
#  else
     names[133] = SPECENUM_STRING(SPECENUM_VALUE133);
#  endif
#endif
#if 134 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE134
     names[134] = NULL;
#  elif defined(SPECENUM_VALUE134NAME)
     names[134] = Qn_(SPECENUM_VALUE134NAME);
#  else
     names[134] = SPECENUM_STRING(SPECENUM_VALUE134);
#  endif
#endif
#if 135 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE135
     names[135] = NULL;
#  elif defined(SPECENUM_VALUE135NAME)
     names[135] = Qn_(SPECENUM_VALUE135NAME);
#  else
     names[135] = SPECENUM_STRING(SPECENUM_VALUE135);
#  endif
#endif
#if 136 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE136
     names[136] = NULL;
#  elif defined(SPECENUM_VALUE136NAME)
     names[136] = Qn_(SPECENUM_VALUE136NAME);
#  else
     names[136] = SPECENUM_STRING(SPECENUM_VALUE136);
#  endif
#endif
#if 137 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE137
     names[137] = NULL;
#  elif defined(SPECENUM_VALUE137NAME)
     names[137] = Qn_(SPECENUM_VALUE137NAME);
#  else
     names[137] = SPECENUM_STRING(SPECENUM_VALUE137);
#  endif
#endif
#if 138 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE138
     names[138] = NULL;
#  elif defined(SPECENUM_VALUE138NAME)
     names[138] = Qn_(SPECENUM_VALUE138NAME);
#  else
     names[138] = SPECENUM_STRING(SPECENUM_VALUE138);
#  endif
#endif
#if 139 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE139
     names[139] = NULL;
#  elif defined(SPECENUM_VALUE139NAME)
     names[139] = Qn_(SPECENUM_VALUE139NAME);
#  else
     names[139] = SPECENUM_STRING(SPECENUM_VALUE139);
#  endif
#endif
#if 140 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE140
     names[140] = NULL;
#  elif defined(SPECENUM_VALUE140NAME)
     names[140] = Qn_(SPECENUM_VALUE140NAME);
#  else
     names[140] = SPECENUM_STRING(SPECENUM_VALUE140);
#  endif
#endif
#if 141 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE141
     names[141] = NULL;
#  elif defined(SPECENUM_VALUE141NAME)
     names[141] = Qn_(SPECENUM_VALUE141NAME);
#  else
     names[141] = SPECENUM_STRING(SPECENUM_VALUE141);
#  endif
#endif
#if 142 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE142
     names[142] = NULL;
#  elif defined(SPECENUM_VALUE142NAME)
     names[142] = Qn_(SPECENUM_VALUE142NAME);
#  else
     names[142] = SPECENUM_STRING(SPECENUM_VALUE142);
#  endif
#endif
#if 143 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE143
     names[143] = NULL;
#  elif defined(SPECENUM_VALUE143NAME)
     names[143] = Qn_(SPECENUM_VALUE143NAME);
#  else
     names[143] = SPECENUM_STRING(SPECENUM_VALUE143);
#  endif
#endif
#if 144 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE144
     names[144] = NULL;
#  elif defined(SPECENUM_VALUE144NAME)
     names[144] = Qn_(SPECENUM_VALUE144NAME);
#  else
     names[144] = SPECENUM_STRING(SPECENUM_VALUE144);
#  endif
#endif
#if 145 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE145
     names[145] = NULL;
#  elif defined(SPECENUM_VALUE145NAME)
     names[145] = Qn_(SPECENUM_VALUE145NAME);
#  else
     names[145] = SPECENUM_STRING(SPECENUM_VALUE145);
#  endif
#endif
#if 146 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE146
     names[146] = NULL;
#  elif defined(SPECENUM_VALUE146NAME)
     names[146] = Qn_(SPECENUM_VALUE146NAME);
#  else
     names[146] = SPECENUM_STRING(SPECENUM_VALUE146);
#  endif
#endif
#if 147 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE147
     names[147] = NULL;
#  elif defined(SPECENUM_VALUE147NAME)
     names[147] = Qn_(SPECENUM_VALUE147NAME);
#  else
     names[147] = SPECENUM_STRING(SPECENUM_VALUE147);
#  endif
#endif
#if 148 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE148
     names[148] = NULL;
#  elif defined(SPECENUM_VALUE148NAME)
     names[148] = Qn_(SPECENUM_VALUE148NAME);
#  else
     names[148] = SPECENUM_STRING(SPECENUM_VALUE148);
#  endif
#endif
#if 149 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE149
     names[149] = NULL;
#  elif defined(SPECENUM_VALUE149NAME)
     names[149] = Qn_(SPECENUM_VALUE149NAME);
#  else
     names[149] = SPECENUM_STRING(SPECENUM_VALUE149);
#  endif
#endif
#ifdef SPECENUM_COUNT
#  ifdef SPECENUM_COUNTNAME
  names[SPECENUM_COUNT] = Qn_(SPECENUM_COUNTNAME);
#  else
  names[SPECENUM_COUNT] = SPECENUM_STRING(SPECENUM_COUNT);
#  endif
#endif
    initialized = TRUE;
  }

#ifdef SPECENUM_BITWISE
#  ifdef SPECENUM_ZERO
  if (enumerator == SPECENUM_ZERO) {
#    ifdef SPECENUM_ZERONAME
    return Qn_(SPECENUM_ZERONAME);
#    else
    return SPECENUM_STRING(SPECENUM_ZERO);
#    endif
  }
#  endif
  {
    size_t i;

    for (i = 0; i < ARRAY_SIZE(names); i++) {
      if (1 << i == enumerator) {
        return names[i];
      }
    }
  }
#else
  if ((unsigned)enumerator < ARRAY_SIZE(names)) {
    return names[enumerator];
  }
#endif /* SPECENUM_BITWISE */
  return NULL;
}

/**********************************************************************//**
  Returns the enumerator for the name or *_invalid() if not found.
**************************************************************************/
static inline enum SPECENUM_NAME SPECENUM_FOO(_by_name)
    (const char *name, int (*strcmp_func)(const char *, const char *))
{
  enum SPECENUM_NAME e;
  const char *enum_name;
  const char *current_name = name;

#ifdef SPECENUM_NAME_UPDATER
  current_name = SPECENUM_FOO(_name_update_cb)(name);
#endif

  for (e = SPECENUM_FOO(_begin)(); e != SPECENUM_FOO(_end)();
       e = SPECENUM_FOO(_next)(e)) {
    if ((enum_name = SPECENUM_FOO(_name)(e))
        && 0 == strcmp_func(current_name, enum_name)) {
      return e;
    }
  }

  return SPECENUM_FOO(_invalid)();
}

/**********************************************************************//**
  Returns the translated name of the enumerator.
**************************************************************************/
#ifndef SPECENUM_NAMEOVERRIDE
fc__attribute((const))
#endif
static inline const char *
SPECENUM_FOO(_translated_name)(enum SPECENUM_NAME enumerator)
{
#ifdef SPECENUM_COUNT
  static const char *names[SPECENUM_SIZE + 1];
#else
  static const char *names[SPECENUM_SIZE];
#endif
  static bool initialized = FALSE;

#ifdef SPECENUM_NAMEOVERRIDE
  {
    const char *name = SPECENUM_FOO(_name_cb)(enumerator);

    if (name != NULL) {
      return Q_(name);
    }
  }
#endif /* SPECENUM_NAMEOVERRIDE */

  if (!initialized) {
#if 0 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE0
     names[0] = NULL;
#  elif defined(SPECENUM_VALUE0NAME)
     names[0] = Q_(SPECENUM_VALUE0NAME);
#  else
     names[0] = SPECENUM_STRING(SPECENUM_VALUE0);
#  endif
#endif
#if 1 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE1
     names[1] = NULL;
#  elif defined(SPECENUM_VALUE1NAME)
     names[1] = Q_(SPECENUM_VALUE1NAME);
#  else
     names[1] = SPECENUM_STRING(SPECENUM_VALUE1);
#  endif
#endif
#if 2 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE2
     names[2] = NULL;
#  elif defined(SPECENUM_VALUE2NAME)
     names[2] = Q_(SPECENUM_VALUE2NAME);
#  else
     names[2] = SPECENUM_STRING(SPECENUM_VALUE2);
#  endif
#endif
#if 3 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE3
     names[3] = NULL;
#  elif defined(SPECENUM_VALUE3NAME)
     names[3] = Q_(SPECENUM_VALUE3NAME);
#  else
     names[3] = SPECENUM_STRING(SPECENUM_VALUE3);
#  endif
#endif
#if 4 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE4
     names[4] = NULL;
#  elif defined(SPECENUM_VALUE4NAME)
     names[4] = Q_(SPECENUM_VALUE4NAME);
#  else
     names[4] = SPECENUM_STRING(SPECENUM_VALUE4);
#  endif
#endif
#if 5 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE5
     names[5] = NULL;
#  elif defined(SPECENUM_VALUE5NAME)
     names[5] = Q_(SPECENUM_VALUE5NAME);
#  else
     names[5] = SPECENUM_STRING(SPECENUM_VALUE5);
#  endif
#endif
#if 6 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE6
     names[6] = NULL;
#  elif defined(SPECENUM_VALUE6NAME)
     names[6] = Q_(SPECENUM_VALUE6NAME);
#  else
     names[6] = SPECENUM_STRING(SPECENUM_VALUE6);
#  endif
#endif
#if 7 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE7
     names[7] = NULL;
#  elif defined(SPECENUM_VALUE7NAME)
     names[7] = Q_(SPECENUM_VALUE7NAME);
#  else
     names[7] = SPECENUM_STRING(SPECENUM_VALUE7);
#  endif
#endif
#if 8 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE8
     names[8] = NULL;
#  elif defined(SPECENUM_VALUE8NAME)
     names[8] = Q_(SPECENUM_VALUE8NAME);
#  else
     names[8] = SPECENUM_STRING(SPECENUM_VALUE8);
#  endif
#endif
#if 9 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE9
     names[9] = NULL;
#  elif defined(SPECENUM_VALUE9NAME)
     names[9] = Q_(SPECENUM_VALUE9NAME);
#  else
     names[9] = SPECENUM_STRING(SPECENUM_VALUE9);
#  endif
#endif
#if 10 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE10
     names[10] = NULL;
#  elif defined(SPECENUM_VALUE10NAME)
     names[10] = Q_(SPECENUM_VALUE10NAME);
#  else
     names[10] = SPECENUM_STRING(SPECENUM_VALUE10);
#  endif
#endif
#if 11 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE11
     names[11] = NULL;
#  elif defined(SPECENUM_VALUE11NAME)
     names[11] = Q_(SPECENUM_VALUE11NAME);
#  else
     names[11] = SPECENUM_STRING(SPECENUM_VALUE11);
#  endif
#endif
#if 12 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE12
     names[12] = NULL;
#  elif defined(SPECENUM_VALUE12NAME)
     names[12] = Q_(SPECENUM_VALUE12NAME);
#  else
     names[12] = SPECENUM_STRING(SPECENUM_VALUE12);
#  endif
#endif
#if 13 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE13
     names[13] = NULL;
#  elif defined(SPECENUM_VALUE13NAME)
     names[13] = Q_(SPECENUM_VALUE13NAME);
#  else
     names[13] = SPECENUM_STRING(SPECENUM_VALUE13);
#  endif
#endif
#if 14 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE14
     names[14] = NULL;
#  elif defined(SPECENUM_VALUE14NAME)
     names[14] = Q_(SPECENUM_VALUE14NAME);
#  else
     names[14] = SPECENUM_STRING(SPECENUM_VALUE14);
#  endif
#endif
#if 15 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE15
     names[15] = NULL;
#  elif defined(SPECENUM_VALUE15NAME)
     names[15] = Q_(SPECENUM_VALUE15NAME);
#  else
     names[15] = SPECENUM_STRING(SPECENUM_VALUE15);
#  endif
#endif
#if 16 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE16
     names[16] = NULL;
#  elif defined(SPECENUM_VALUE16NAME)
     names[16] = Q_(SPECENUM_VALUE16NAME);
#  else
     names[16] = SPECENUM_STRING(SPECENUM_VALUE16);
#  endif
#endif
#if 17 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE17
     names[17] = NULL;
#  elif defined(SPECENUM_VALUE17NAME)
     names[17] = Q_(SPECENUM_VALUE17NAME);
#  else
     names[17] = SPECENUM_STRING(SPECENUM_VALUE17);
#  endif
#endif
#if 18 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE18
     names[18] = NULL;
#  elif defined(SPECENUM_VALUE18NAME)
     names[18] = Q_(SPECENUM_VALUE18NAME);
#  else
     names[18] = SPECENUM_STRING(SPECENUM_VALUE18);
#  endif
#endif
#if 19 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE19
     names[19] = NULL;
#  elif defined(SPECENUM_VALUE19NAME)
     names[19] = Q_(SPECENUM_VALUE19NAME);
#  else
     names[19] = SPECENUM_STRING(SPECENUM_VALUE19);
#  endif
#endif
#if 20 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE20
     names[20] = NULL;
#  elif defined(SPECENUM_VALUE20NAME)
     names[20] = Q_(SPECENUM_VALUE20NAME);
#  else
     names[20] = SPECENUM_STRING(SPECENUM_VALUE20);
#  endif
#endif
#if 21 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE21
     names[21] = NULL;
#  elif defined(SPECENUM_VALUE21NAME)
     names[21] = Q_(SPECENUM_VALUE21NAME);
#  else
     names[21] = SPECENUM_STRING(SPECENUM_VALUE21);
#  endif
#endif
#if 22 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE22
     names[22] = NULL;
#  elif defined(SPECENUM_VALUE22NAME)
     names[22] = Q_(SPECENUM_VALUE22NAME);
#  else
     names[22] = SPECENUM_STRING(SPECENUM_VALUE22);
#  endif
#endif
#if 23 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE23
     names[23] = NULL;
#  elif defined(SPECENUM_VALUE23NAME)
     names[23] = Q_(SPECENUM_VALUE23NAME);
#  else
     names[23] = SPECENUM_STRING(SPECENUM_VALUE23);
#  endif
#endif
#if 24 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE24
     names[24] = NULL;
#  elif defined(SPECENUM_VALUE24NAME)
     names[24] = Q_(SPECENUM_VALUE24NAME);
#  else
     names[24] = SPECENUM_STRING(SPECENUM_VALUE24);
#  endif
#endif
#if 25 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE25
     names[25] = NULL;
#  elif defined(SPECENUM_VALUE25NAME)
     names[25] = Q_(SPECENUM_VALUE25NAME);
#  else
     names[25] = SPECENUM_STRING(SPECENUM_VALUE25);
#  endif
#endif
#if 26 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE26
     names[26] = NULL;
#  elif defined(SPECENUM_VALUE26NAME)
     names[26] = Q_(SPECENUM_VALUE26NAME);
#  else
     names[26] = SPECENUM_STRING(SPECENUM_VALUE26);
#  endif
#endif
#if 27 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE27
     names[27] = NULL;
#  elif defined(SPECENUM_VALUE27NAME)
     names[27] = Q_(SPECENUM_VALUE27NAME);
#  else
     names[27] = SPECENUM_STRING(SPECENUM_VALUE27);
#  endif
#endif
#if 28 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE28
     names[28] = NULL;
#  elif defined(SPECENUM_VALUE28NAME)
     names[28] = Q_(SPECENUM_VALUE28NAME);
#  else
     names[28] = SPECENUM_STRING(SPECENUM_VALUE28);
#  endif
#endif
#if 29 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE29
     names[29] = NULL;
#  elif defined(SPECENUM_VALUE29NAME)
     names[29] = Q_(SPECENUM_VALUE29NAME);
#  else
     names[29] = SPECENUM_STRING(SPECENUM_VALUE29);
#  endif
#endif
#if 30 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE30
     names[30] = NULL;
#  elif defined(SPECENUM_VALUE30NAME)
     names[30] = Q_(SPECENUM_VALUE30NAME);
#  else
     names[30] = SPECENUM_STRING(SPECENUM_VALUE30);
#  endif
#endif
#if 31 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE31
     names[31] = NULL;
#  elif defined(SPECENUM_VALUE31NAME)
     names[31] = Q_(SPECENUM_VALUE31NAME);
#  else
     names[31] = SPECENUM_STRING(SPECENUM_VALUE31);
#  endif
#endif
#if 32 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE32
     names[32] = NULL;
#  elif defined(SPECENUM_VALUE32NAME)
     names[32] = Q_(SPECENUM_VALUE32NAME);
#  else
     names[32] = SPECENUM_STRING(SPECENUM_VALUE32);
#  endif
#endif
#if 33 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE33
     names[33] = NULL;
#  elif defined(SPECENUM_VALUE33NAME)
     names[33] = Q_(SPECENUM_VALUE33NAME);
#  else
     names[33] = SPECENUM_STRING(SPECENUM_VALUE33);
#  endif
#endif
#if 34 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE34
     names[34] = NULL;
#  elif defined(SPECENUM_VALUE34NAME)
     names[34] = Q_(SPECENUM_VALUE34NAME);
#  else
     names[34] = SPECENUM_STRING(SPECENUM_VALUE34);
#  endif
#endif
#if 35 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE35
     names[35] = NULL;
#  elif defined(SPECENUM_VALUE35NAME)
     names[35] = Q_(SPECENUM_VALUE35NAME);
#  else
     names[35] = SPECENUM_STRING(SPECENUM_VALUE35);
#  endif
#endif
#if 36 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE36
     names[36] = NULL;
#  elif defined(SPECENUM_VALUE36NAME)
     names[36] = Q_(SPECENUM_VALUE36NAME);
#  else
     names[36] = SPECENUM_STRING(SPECENUM_VALUE36);
#  endif
#endif
#if 37 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE37
     names[37] = NULL;
#  elif defined(SPECENUM_VALUE37NAME)
     names[37] = Q_(SPECENUM_VALUE37NAME);
#  else
     names[37] = SPECENUM_STRING(SPECENUM_VALUE37);
#  endif
#endif
#if 38 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE38
     names[38] = NULL;
#  elif defined(SPECENUM_VALUE38NAME)
     names[38] = Q_(SPECENUM_VALUE38NAME);
#  else
     names[38] = SPECENUM_STRING(SPECENUM_VALUE38);
#  endif
#endif
#if 39 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE39
     names[39] = NULL;
#  elif defined(SPECENUM_VALUE39NAME)
     names[39] = Q_(SPECENUM_VALUE39NAME);
#  else
     names[39] = SPECENUM_STRING(SPECENUM_VALUE39);
#  endif
#endif
#if 40 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE40
     names[40] = NULL;
#  elif defined(SPECENUM_VALUE40NAME)
     names[40] = Q_(SPECENUM_VALUE40NAME);
#  else
     names[40] = SPECENUM_STRING(SPECENUM_VALUE40);
#  endif
#endif
#if 41 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE41
     names[41] = NULL;
#  elif defined(SPECENUM_VALUE41NAME)
     names[41] = Q_(SPECENUM_VALUE41NAME);
#  else
     names[41] = SPECENUM_STRING(SPECENUM_VALUE41);
#  endif
#endif
#if 42 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE42
     names[42] = NULL;
#  elif defined(SPECENUM_VALUE42NAME)
     names[42] = Q_(SPECENUM_VALUE42NAME);
#  else
     names[42] = SPECENUM_STRING(SPECENUM_VALUE42);
#  endif
#endif
#if 43 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE43
     names[43] = NULL;
#  elif defined(SPECENUM_VALUE43NAME)
     names[43] = Q_(SPECENUM_VALUE43NAME);
#  else
     names[43] = SPECENUM_STRING(SPECENUM_VALUE43);
#  endif
#endif
#if 44 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE44
     names[44] = NULL;
#  elif defined(SPECENUM_VALUE44NAME)
     names[44] = Q_(SPECENUM_VALUE44NAME);
#  else
     names[44] = SPECENUM_STRING(SPECENUM_VALUE44);
#  endif
#endif
#if 45 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE45
     names[45] = NULL;
#  elif defined(SPECENUM_VALUE45NAME)
     names[45] = Q_(SPECENUM_VALUE45NAME);
#  else
     names[45] = SPECENUM_STRING(SPECENUM_VALUE45);
#  endif
#endif
#if 46 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE46
     names[46] = NULL;
#  elif defined(SPECENUM_VALUE46NAME)
     names[46] = Q_(SPECENUM_VALUE46NAME);
#  else
     names[46] = SPECENUM_STRING(SPECENUM_VALUE46);
#  endif
#endif
#if 47 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE47
     names[47] = NULL;
#  elif defined(SPECENUM_VALUE47NAME)
     names[47] = Q_(SPECENUM_VALUE47NAME);
#  else
     names[47] = SPECENUM_STRING(SPECENUM_VALUE47);
#  endif
#endif
#if 48 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE48
     names[48] = NULL;
#  elif defined(SPECENUM_VALUE48NAME)
     names[48] = Q_(SPECENUM_VALUE48NAME);
#  else
     names[48] = SPECENUM_STRING(SPECENUM_VALUE48);
#  endif
#endif
#if 49 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE49
     names[49] = NULL;
#  elif defined(SPECENUM_VALUE49NAME)
     names[49] = Q_(SPECENUM_VALUE49NAME);
#  else
     names[49] = SPECENUM_STRING(SPECENUM_VALUE49);
#  endif
#endif
#if 50 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE50
     names[50] = NULL;
#  elif defined(SPECENUM_VALUE50NAME)
     names[50] = Q_(SPECENUM_VALUE50NAME);
#  else
     names[50] = SPECENUM_STRING(SPECENUM_VALUE50);
#  endif
#endif
#if 51 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE51
     names[51] = NULL;
#  elif defined(SPECENUM_VALUE51NAME)
     names[51] = Q_(SPECENUM_VALUE51NAME);
#  else
     names[51] = SPECENUM_STRING(SPECENUM_VALUE51);
#  endif
#endif
#if 52 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE52
     names[52] = NULL;
#  elif defined(SPECENUM_VALUE52NAME)
     names[52] = Q_(SPECENUM_VALUE52NAME);
#  else
     names[52] = SPECENUM_STRING(SPECENUM_VALUE52);
#  endif
#endif
#if 53 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE53
     names[53] = NULL;
#  elif defined(SPECENUM_VALUE53NAME)
     names[53] = Q_(SPECENUM_VALUE53NAME);
#  else
     names[53] = SPECENUM_STRING(SPECENUM_VALUE53);
#  endif
#endif
#if 54 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE54
     names[54] = NULL;
#  elif defined(SPECENUM_VALUE54NAME)
     names[54] = Q_(SPECENUM_VALUE54NAME);
#  else
     names[54] = SPECENUM_STRING(SPECENUM_VALUE54);
#  endif
#endif
#if 55 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE55
     names[55] = NULL;
#  elif defined(SPECENUM_VALUE55NAME)
     names[55] = Q_(SPECENUM_VALUE55NAME);
#  else
     names[55] = SPECENUM_STRING(SPECENUM_VALUE55);
#  endif
#endif
#if 56 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE56
     names[56] = NULL;
#  elif defined(SPECENUM_VALUE56NAME)
     names[56] = Q_(SPECENUM_VALUE56NAME);
#  else
     names[56] = SPECENUM_STRING(SPECENUM_VALUE56);
#  endif
#endif
#if 57 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE57
     names[57] = NULL;
#  elif defined(SPECENUM_VALUE57NAME)
     names[57] = Q_(SPECENUM_VALUE57NAME);
#  else
     names[57] = SPECENUM_STRING(SPECENUM_VALUE57);
#  endif
#endif
#if 58 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE58
     names[58] = NULL;
#  elif defined(SPECENUM_VALUE58NAME)
     names[58] = Q_(SPECENUM_VALUE58NAME);
#  else
     names[58] = SPECENUM_STRING(SPECENUM_VALUE58);
#  endif
#endif
#if 59 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE59
     names[59] = NULL;
#  elif defined(SPECENUM_VALUE59NAME)
     names[59] = Q_(SPECENUM_VALUE59NAME);
#  else
     names[59] = SPECENUM_STRING(SPECENUM_VALUE59);
#  endif
#endif
#if 60 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE60
     names[60] = NULL;
#  elif defined(SPECENUM_VALUE60NAME)
     names[60] = Q_(SPECENUM_VALUE60NAME);
#  else
     names[60] = SPECENUM_STRING(SPECENUM_VALUE60);
#  endif
#endif
#if 61 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE61
     names[61] = NULL;
#  elif defined(SPECENUM_VALUE61NAME)
     names[61] = Q_(SPECENUM_VALUE61NAME);
#  else
     names[61] = SPECENUM_STRING(SPECENUM_VALUE61);
#  endif
#endif
#if 62 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE62
     names[62] = NULL;
#  elif defined(SPECENUM_VALUE62NAME)
     names[62] = Q_(SPECENUM_VALUE62NAME);
#  else
     names[62] = SPECENUM_STRING(SPECENUM_VALUE62);
#  endif
#endif
#if 63 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE63
     names[63] = NULL;
#  elif defined(SPECENUM_VALUE63NAME)
     names[63] = Q_(SPECENUM_VALUE63NAME);
#  else
     names[63] = SPECENUM_STRING(SPECENUM_VALUE63);
#  endif
#endif
#if 64 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE64
     names[64] = NULL;
#  elif defined(SPECENUM_VALUE64NAME)
     names[64] = Q_(SPECENUM_VALUE64NAME);
#  else
     names[64] = SPECENUM_STRING(SPECENUM_VALUE64);
#  endif
#endif
#if 65 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE65
     names[65] = NULL;
#  elif defined(SPECENUM_VALUE65NAME)
     names[65] = Q_(SPECENUM_VALUE65NAME);
#  else
     names[65] = SPECENUM_STRING(SPECENUM_VALUE65);
#  endif
#endif
#if 66 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE66
     names[66] = NULL;
#  elif defined(SPECENUM_VALUE66NAME)
     names[66] = Q_(SPECENUM_VALUE66NAME);
#  else
     names[66] = SPECENUM_STRING(SPECENUM_VALUE66);
#  endif
#endif
#if 67 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE67
     names[67] = NULL;
#  elif defined(SPECENUM_VALUE67NAME)
     names[67] = Q_(SPECENUM_VALUE67NAME);
#  else
     names[67] = SPECENUM_STRING(SPECENUM_VALUE67);
#  endif
#endif
#if 68 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE68
     names[68] = NULL;
#  elif defined(SPECENUM_VALUE68NAME)
     names[68] = Q_(SPECENUM_VALUE68NAME);
#  else
     names[68] = SPECENUM_STRING(SPECENUM_VALUE68);
#  endif
#endif
#if 69 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE69
     names[69] = NULL;
#  elif defined(SPECENUM_VALUE69NAME)
     names[69] = Q_(SPECENUM_VALUE69NAME);
#  else
     names[69] = SPECENUM_STRING(SPECENUM_VALUE69);
#  endif
#endif
#if 70 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE70
     names[70] = NULL;
#  elif defined(SPECENUM_VALUE70NAME)
     names[70] = Q_(SPECENUM_VALUE70NAME);
#  else
     names[70] = SPECENUM_STRING(SPECENUM_VALUE70);
#  endif
#endif
#if 71 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE71
     names[71] = NULL;
#  elif defined(SPECENUM_VALUE71NAME)
     names[71] = Q_(SPECENUM_VALUE71NAME);
#  else
     names[71] = SPECENUM_STRING(SPECENUM_VALUE71);
#  endif
#endif
#if 72 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE72
     names[72] = NULL;
#  elif defined(SPECENUM_VALUE72NAME)
     names[72] = Q_(SPECENUM_VALUE72NAME);
#  else
     names[72] = SPECENUM_STRING(SPECENUM_VALUE72);
#  endif
#endif
#if 73 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE73
     names[73] = NULL;
#  elif defined(SPECENUM_VALUE73NAME)
     names[73] = Q_(SPECENUM_VALUE73NAME);
#  else
     names[73] = SPECENUM_STRING(SPECENUM_VALUE73);
#  endif
#endif
#if 74 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE74
     names[74] = NULL;
#  elif defined(SPECENUM_VALUE74NAME)
     names[74] = Q_(SPECENUM_VALUE74NAME);
#  else
     names[74] = SPECENUM_STRING(SPECENUM_VALUE74);
#  endif
#endif
#if 75 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE75
     names[75] = NULL;
#  elif defined(SPECENUM_VALUE75NAME)
     names[75] = Q_(SPECENUM_VALUE75NAME);
#  else
     names[75] = SPECENUM_STRING(SPECENUM_VALUE75);
#  endif
#endif
#if 76 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE76
     names[76] = NULL;
#  elif defined(SPECENUM_VALUE76NAME)
     names[76] = Q_(SPECENUM_VALUE76NAME);
#  else
     names[76] = SPECENUM_STRING(SPECENUM_VALUE76);
#  endif
#endif
#if 77 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE77
     names[77] = NULL;
#  elif defined(SPECENUM_VALUE77NAME)
     names[77] = Q_(SPECENUM_VALUE77NAME);
#  else
     names[77] = SPECENUM_STRING(SPECENUM_VALUE77);
#  endif
#endif
#if 78 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE78
     names[78] = NULL;
#  elif defined(SPECENUM_VALUE78NAME)
     names[78] = Q_(SPECENUM_VALUE78NAME);
#  else
     names[78] = SPECENUM_STRING(SPECENUM_VALUE78);
#  endif
#endif
#if 79 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE79
     names[79] = NULL;
#  elif defined(SPECENUM_VALUE79NAME)
     names[79] = Q_(SPECENUM_VALUE79NAME);
#  else
     names[79] = SPECENUM_STRING(SPECENUM_VALUE79);
#  endif
#endif
#if 80 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE80
     names[80] = NULL;
#  elif defined(SPECENUM_VALUE80NAME)
     names[80] = Q_(SPECENUM_VALUE80NAME);
#  else
     names[80] = SPECENUM_STRING(SPECENUM_VALUE80);
#  endif
#endif
#if 81 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE81
     names[81] = NULL;
#  elif defined(SPECENUM_VALUE81NAME)
     names[81] = Q_(SPECENUM_VALUE81NAME);
#  else
     names[81] = SPECENUM_STRING(SPECENUM_VALUE81);
#  endif
#endif
#if 82 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE82
     names[82] = NULL;
#  elif defined(SPECENUM_VALUE82NAME)
     names[82] = Q_(SPECENUM_VALUE82NAME);
#  else
     names[82] = SPECENUM_STRING(SPECENUM_VALUE82);
#  endif
#endif
#if 83 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE83
     names[83] = NULL;
#  elif defined(SPECENUM_VALUE83NAME)
     names[83] = Q_(SPECENUM_VALUE83NAME);
#  else
     names[83] = SPECENUM_STRING(SPECENUM_VALUE83);
#  endif
#endif
#if 84 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE84
     names[84] = NULL;
#  elif defined(SPECENUM_VALUE84NAME)
     names[84] = Q_(SPECENUM_VALUE84NAME);
#  else
     names[84] = SPECENUM_STRING(SPECENUM_VALUE84);
#  endif
#endif
#if 85 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE85
     names[85] = NULL;
#  elif defined(SPECENUM_VALUE85NAME)
     names[85] = Q_(SPECENUM_VALUE85NAME);
#  else
     names[85] = SPECENUM_STRING(SPECENUM_VALUE85);
#  endif
#endif
#if 86 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE86
     names[86] = NULL;
#  elif defined(SPECENUM_VALUE86NAME)
     names[86] = Q_(SPECENUM_VALUE86NAME);
#  else
     names[86] = SPECENUM_STRING(SPECENUM_VALUE86);
#  endif
#endif
#if 87 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE87
     names[87] = NULL;
#  elif defined(SPECENUM_VALUE87NAME)
     names[87] = Q_(SPECENUM_VALUE87NAME);
#  else
     names[87] = SPECENUM_STRING(SPECENUM_VALUE87);
#  endif
#endif
#if 88 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE88
     names[88] = NULL;
#  elif defined(SPECENUM_VALUE88NAME)
     names[88] = Q_(SPECENUM_VALUE88NAME);
#  else
     names[88] = SPECENUM_STRING(SPECENUM_VALUE88);
#  endif
#endif
#if 89 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE89
     names[89] = NULL;
#  elif defined(SPECENUM_VALUE89NAME)
     names[89] = Q_(SPECENUM_VALUE89NAME);
#  else
     names[89] = SPECENUM_STRING(SPECENUM_VALUE89);
#  endif
#endif
#if 90 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE90
     names[90] = NULL;
#  elif defined(SPECENUM_VALUE90NAME)
     names[90] = Q_(SPECENUM_VALUE90NAME);
#  else
     names[90] = SPECENUM_STRING(SPECENUM_VALUE90);
#  endif
#endif
#if 91 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE91
     names[91] = NULL;
#  elif defined(SPECENUM_VALUE91NAME)
     names[91] = Q_(SPECENUM_VALUE91NAME);
#  else
     names[91] = SPECENUM_STRING(SPECENUM_VALUE91);
#  endif
#endif
#if 92 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE92
     names[92] = NULL;
#  elif defined(SPECENUM_VALUE92NAME)
     names[92] = Q_(SPECENUM_VALUE92NAME);
#  else
     names[92] = SPECENUM_STRING(SPECENUM_VALUE92);
#  endif
#endif
#if 93 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE93
     names[93] = NULL;
#  elif defined(SPECENUM_VALUE93NAME)
     names[93] = Q_(SPECENUM_VALUE93NAME);
#  else
     names[93] = SPECENUM_STRING(SPECENUM_VALUE93);
#  endif
#endif
#if 94 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE94
     names[94] = NULL;
#  elif defined(SPECENUM_VALUE94NAME)
     names[94] = Q_(SPECENUM_VALUE94NAME);
#  else
     names[94] = SPECENUM_STRING(SPECENUM_VALUE94);
#  endif
#endif
#if 95 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE95
     names[95] = NULL;
#  elif defined(SPECENUM_VALUE95NAME)
     names[95] = Q_(SPECENUM_VALUE95NAME);
#  else
     names[95] = SPECENUM_STRING(SPECENUM_VALUE95);
#  endif
#endif
#if 96 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE96
     names[96] = NULL;
#  elif defined(SPECENUM_VALUE96NAME)
     names[96] = Q_(SPECENUM_VALUE96NAME);
#  else
     names[96] = SPECENUM_STRING(SPECENUM_VALUE96);
#  endif
#endif
#if 97 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE97
     names[97] = NULL;
#  elif defined(SPECENUM_VALUE97NAME)
     names[97] = Q_(SPECENUM_VALUE97NAME);
#  else
     names[97] = SPECENUM_STRING(SPECENUM_VALUE97);
#  endif
#endif
#if 98 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE98
     names[98] = NULL;
#  elif defined(SPECENUM_VALUE98NAME)
     names[98] = Q_(SPECENUM_VALUE98NAME);
#  else
     names[98] = SPECENUM_STRING(SPECENUM_VALUE98);
#  endif
#endif
#if 99 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE99
     names[99] = NULL;
#  elif defined(SPECENUM_VALUE99NAME)
     names[99] = Q_(SPECENUM_VALUE99NAME);
#  else
     names[99] = SPECENUM_STRING(SPECENUM_VALUE99);
#  endif
#endif
#if 100 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE100
     names[100] = NULL;
#  elif defined(SPECENUM_VALUE100NAME)
     names[100] = Q_(SPECENUM_VALUE100NAME);
#  else
     names[100] = SPECENUM_STRING(SPECENUM_VALUE100);
#  endif
#endif
#if 101 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE101
     names[101] = NULL;
#  elif defined(SPECENUM_VALUE101NAME)
     names[101] = Q_(SPECENUM_VALUE101NAME);
#  else
     names[101] = SPECENUM_STRING(SPECENUM_VALUE101);
#  endif
#endif
#if 102 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE102
     names[102] = NULL;
#  elif defined(SPECENUM_VALUE102NAME)
     names[102] = Q_(SPECENUM_VALUE102NAME);
#  else
     names[102] = SPECENUM_STRING(SPECENUM_VALUE102);
#  endif
#endif
#if 103 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE103
     names[103] = NULL;
#  elif defined(SPECENUM_VALUE103NAME)
     names[103] = Q_(SPECENUM_VALUE103NAME);
#  else
     names[103] = SPECENUM_STRING(SPECENUM_VALUE103);
#  endif
#endif
#if 104 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE104
     names[104] = NULL;
#  elif defined(SPECENUM_VALUE104NAME)
     names[104] = Q_(SPECENUM_VALUE104NAME);
#  else
     names[104] = SPECENUM_STRING(SPECENUM_VALUE104);
#  endif
#endif
#if 105 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE105
     names[105] = NULL;
#  elif defined(SPECENUM_VALUE105NAME)
     names[105] = Q_(SPECENUM_VALUE105NAME);
#  else
     names[105] = SPECENUM_STRING(SPECENUM_VALUE105);
#  endif
#endif
#if 106 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE106
     names[106] = NULL;
#  elif defined(SPECENUM_VALUE106NAME)
     names[106] = Q_(SPECENUM_VALUE106NAME);
#  else
     names[106] = SPECENUM_STRING(SPECENUM_VALUE106);
#  endif
#endif
#if 107 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE107
     names[107] = NULL;
#  elif defined(SPECENUM_VALUE107NAME)
     names[107] = Q_(SPECENUM_VALUE107NAME);
#  else
     names[107] = SPECENUM_STRING(SPECENUM_VALUE107);
#  endif
#endif
#if 108 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE108
     names[108] = NULL;
#  elif defined(SPECENUM_VALUE108NAME)
     names[108] = Q_(SPECENUM_VALUE108NAME);
#  else
     names[108] = SPECENUM_STRING(SPECENUM_VALUE108);
#  endif
#endif
#if 109 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE109
     names[109] = NULL;
#  elif defined(SPECENUM_VALUE109NAME)
     names[109] = Q_(SPECENUM_VALUE109NAME);
#  else
     names[109] = SPECENUM_STRING(SPECENUM_VALUE109);
#  endif
#endif
#if 110 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE110
     names[110] = NULL;
#  elif defined(SPECENUM_VALUE110NAME)
     names[110] = Q_(SPECENUM_VALUE110NAME);
#  else
     names[110] = SPECENUM_STRING(SPECENUM_VALUE110);
#  endif
#endif
#if 111 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE111
     names[111] = NULL;
#  elif defined(SPECENUM_VALUE111NAME)
     names[111] = Q_(SPECENUM_VALUE111NAME);
#  else
     names[111] = SPECENUM_STRING(SPECENUM_VALUE111);
#  endif
#endif
#if 112 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE112
     names[112] = NULL;
#  elif defined(SPECENUM_VALUE112NAME)
     names[112] = Q_(SPECENUM_VALUE112NAME);
#  else
     names[112] = SPECENUM_STRING(SPECENUM_VALUE112);
#  endif
#endif
#if 113 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE113
     names[113] = NULL;
#  elif defined(SPECENUM_VALUE113NAME)
     names[113] = Q_(SPECENUM_VALUE113NAME);
#  else
     names[113] = SPECENUM_STRING(SPECENUM_VALUE113);
#  endif
#endif
#if 114 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE114
     names[114] = NULL;
#  elif defined(SPECENUM_VALUE114NAME)
     names[114] = Q_(SPECENUM_VALUE114NAME);
#  else
     names[114] = SPECENUM_STRING(SPECENUM_VALUE114);
#  endif
#endif
#if 115 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE115
     names[115] = NULL;
#  elif defined(SPECENUM_VALUE115NAME)
     names[115] = Q_(SPECENUM_VALUE115NAME);
#  else
     names[115] = SPECENUM_STRING(SPECENUM_VALUE115);
#  endif
#endif
#if 116 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE116
     names[116] = NULL;
#  elif defined(SPECENUM_VALUE116NAME)
     names[116] = Q_(SPECENUM_VALUE116NAME);
#  else
     names[116] = SPECENUM_STRING(SPECENUM_VALUE116);
#  endif
#endif
#if 117 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE117
     names[117] = NULL;
#  elif defined(SPECENUM_VALUE117NAME)
     names[117] = Q_(SPECENUM_VALUE117NAME);
#  else
     names[117] = SPECENUM_STRING(SPECENUM_VALUE117);
#  endif
#endif
#if 118 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE118
     names[118] = NULL;
#  elif defined(SPECENUM_VALUE118NAME)
     names[118] = Q_(SPECENUM_VALUE118NAME);
#  else
     names[118] = SPECENUM_STRING(SPECENUM_VALUE118);
#  endif
#endif
#if 119 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE119
     names[119] = NULL;
#  elif defined(SPECENUM_VALUE119NAME)
     names[119] = Q_(SPECENUM_VALUE119NAME);
#  else
     names[119] = SPECENUM_STRING(SPECENUM_VALUE119);
#  endif
#endif
#if 120 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE120
     names[120] = NULL;
#  elif defined(SPECENUM_VALUE120NAME)
     names[120] = Q_(SPECENUM_VALUE120NAME);
#  else
     names[120] = SPECENUM_STRING(SPECENUM_VALUE120);
#  endif
#endif
#if 121 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE121
     names[121] = NULL;
#  elif defined(SPECENUM_VALUE121NAME)
     names[121] = Q_(SPECENUM_VALUE121NAME);
#  else
     names[121] = SPECENUM_STRING(SPECENUM_VALUE121);
#  endif
#endif
#if 122 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE122
     names[122] = NULL;
#  elif defined(SPECENUM_VALUE122NAME)
     names[122] = Q_(SPECENUM_VALUE122NAME);
#  else
     names[122] = SPECENUM_STRING(SPECENUM_VALUE122);
#  endif
#endif
#if 123 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE123
     names[123] = NULL;
#  elif defined(SPECENUM_VALUE123NAME)
     names[123] = Q_(SPECENUM_VALUE123NAME);
#  else
     names[123] = SPECENUM_STRING(SPECENUM_VALUE123);
#  endif
#endif
#if 124 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE124
     names[124] = NULL;
#  elif defined(SPECENUM_VALUE124NAME)
     names[124] = Q_(SPECENUM_VALUE124NAME);
#  else
     names[124] = SPECENUM_STRING(SPECENUM_VALUE124);
#  endif
#endif
#if 125 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE125
     names[125] = NULL;
#  elif defined(SPECENUM_VALUE125NAME)
     names[125] = Q_(SPECENUM_VALUE125NAME);
#  else
     names[125] = SPECENUM_STRING(SPECENUM_VALUE125);
#  endif
#endif
#if 126 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE126
     names[126] = NULL;
#  elif defined(SPECENUM_VALUE126NAME)
     names[126] = Q_(SPECENUM_VALUE126NAME);
#  else
     names[126] = SPECENUM_STRING(SPECENUM_VALUE126);
#  endif
#endif
#if 127 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE127
     names[127] = NULL;
#  elif defined(SPECENUM_VALUE127NAME)
     names[127] = Q_(SPECENUM_VALUE127NAME);
#  else
     names[127] = SPECENUM_STRING(SPECENUM_VALUE127);
#  endif
#endif
#if 128 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE128
     names[128] = NULL;
#  elif defined(SPECENUM_VALUE128NAME)
     names[128] = Q_(SPECENUM_VALUE128NAME);
#  else
     names[128] = SPECENUM_STRING(SPECENUM_VALUE128);
#  endif
#endif
#if 129 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE129
     names[129] = NULL;
#  elif defined(SPECENUM_VALUE129NAME)
     names[129] = Q_(SPECENUM_VALUE129NAME);
#  else
     names[129] = SPECENUM_STRING(SPECENUM_VALUE129);
#  endif
#endif
#if 130 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE130
     names[130] = NULL;
#  elif defined(SPECENUM_VALUE130NAME)
     names[130] = Q_(SPECENUM_VALUE130NAME);
#  else
     names[130] = SPECENUM_STRING(SPECENUM_VALUE130);
#  endif
#endif
#if 131 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE131
     names[131] = NULL;
#  elif defined(SPECENUM_VALUE131NAME)
     names[131] = Q_(SPECENUM_VALUE131NAME);
#  else
     names[131] = SPECENUM_STRING(SPECENUM_VALUE131);
#  endif
#endif
#if 132 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE132
     names[132] = NULL;
#  elif defined(SPECENUM_VALUE132NAME)
     names[132] = Q_(SPECENUM_VALUE132NAME);
#  else
     names[132] = SPECENUM_STRING(SPECENUM_VALUE132);
#  endif
#endif
#if 133 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE133
     names[133] = NULL;
#  elif defined(SPECENUM_VALUE133NAME)
     names[133] = Q_(SPECENUM_VALUE133NAME);
#  else
     names[133] = SPECENUM_STRING(SPECENUM_VALUE133);
#  endif
#endif
#if 134 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE134
     names[134] = NULL;
#  elif defined(SPECENUM_VALUE134NAME)
     names[134] = Q_(SPECENUM_VALUE134NAME);
#  else
     names[134] = SPECENUM_STRING(SPECENUM_VALUE134);
#  endif
#endif
#if 135 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE135
     names[135] = NULL;
#  elif defined(SPECENUM_VALUE135NAME)
     names[135] = Q_(SPECENUM_VALUE135NAME);
#  else
     names[135] = SPECENUM_STRING(SPECENUM_VALUE135);
#  endif
#endif
#if 136 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE136
     names[136] = NULL;
#  elif defined(SPECENUM_VALUE136NAME)
     names[136] = Q_(SPECENUM_VALUE136NAME);
#  else
     names[136] = SPECENUM_STRING(SPECENUM_VALUE136);
#  endif
#endif
#if 137 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE137
     names[137] = NULL;
#  elif defined(SPECENUM_VALUE137NAME)
     names[137] = Q_(SPECENUM_VALUE137NAME);
#  else
     names[137] = SPECENUM_STRING(SPECENUM_VALUE137);
#  endif
#endif
#if 138 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE138
     names[138] = NULL;
#  elif defined(SPECENUM_VALUE138NAME)
     names[138] = Q_(SPECENUM_VALUE138NAME);
#  else
     names[138] = SPECENUM_STRING(SPECENUM_VALUE138);
#  endif
#endif
#if 139 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE139
     names[139] = NULL;
#  elif defined(SPECENUM_VALUE139NAME)
     names[139] = Q_(SPECENUM_VALUE139NAME);
#  else
     names[139] = SPECENUM_STRING(SPECENUM_VALUE139);
#  endif
#endif
#if 140 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE140
     names[140] = NULL;
#  elif defined(SPECENUM_VALUE140NAME)
     names[140] = Q_(SPECENUM_VALUE140NAME);
#  else
     names[140] = SPECENUM_STRING(SPECENUM_VALUE140);
#  endif
#endif
#if 141 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE141
     names[141] = NULL;
#  elif defined(SPECENUM_VALUE141NAME)
     names[141] = Q_(SPECENUM_VALUE141NAME);
#  else
     names[141] = SPECENUM_STRING(SPECENUM_VALUE141);
#  endif
#endif
#if 142 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE142
     names[142] = NULL;
#  elif defined(SPECENUM_VALUE142NAME)
     names[142] = Q_(SPECENUM_VALUE142NAME);
#  else
     names[142] = SPECENUM_STRING(SPECENUM_VALUE142);
#  endif
#endif
#if 143 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE143
     names[143] = NULL;
#  elif defined(SPECENUM_VALUE143NAME)
     names[143] = Q_(SPECENUM_VALUE143NAME);
#  else
     names[143] = SPECENUM_STRING(SPECENUM_VALUE143);
#  endif
#endif
#if 144 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE144
     names[144] = NULL;
#  elif defined(SPECENUM_VALUE144NAME)
     names[144] = Q_(SPECENUM_VALUE144NAME);
#  else
     names[144] = SPECENUM_STRING(SPECENUM_VALUE144);
#  endif
#endif
#if 145 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE145
     names[145] = NULL;
#  elif defined(SPECENUM_VALUE145NAME)
     names[145] = Q_(SPECENUM_VALUE145NAME);
#  else
     names[145] = SPECENUM_STRING(SPECENUM_VALUE145);
#  endif
#endif
#if 146 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE146
     names[146] = NULL;
#  elif defined(SPECENUM_VALUE146NAME)
     names[146] = Q_(SPECENUM_VALUE146NAME);
#  else
     names[146] = SPECENUM_STRING(SPECENUM_VALUE146);
#  endif
#endif
#if 147 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE147
     names[147] = NULL;
#  elif defined(SPECENUM_VALUE147NAME)
     names[147] = Q_(SPECENUM_VALUE147NAME);
#  else
     names[147] = SPECENUM_STRING(SPECENUM_VALUE147);
#  endif
#endif
#if 148 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE148
     names[148] = NULL;
#  elif defined(SPECENUM_VALUE148NAME)
     names[148] = Q_(SPECENUM_VALUE148NAME);
#  else
     names[148] = SPECENUM_STRING(SPECENUM_VALUE148);
#  endif
#endif
#if 149 < SPECENUM_SIZE
#  ifndef SPECENUM_VALUE149
     names[149] = NULL;
#  elif defined(SPECENUM_VALUE149NAME)
     names[149] = Q_(SPECENUM_VALUE149NAME);
#  else
     names[149] = SPECENUM_STRING(SPECENUM_VALUE149);
#  endif
#endif
#ifdef SPECENUM_COUNT
#  ifdef SPECENUM_COUNTNAME
  names[SPECENUM_COUNT] = Q_(SPECENUM_COUNTNAME);
#  else
  names[SPECENUM_COUNT] = SPECENUM_STRING(SPECENUM_COUNT);
#  endif
#endif
    initialized = TRUE;
  }

#ifdef SPECENUM_BITWISE
#  ifdef SPECENUM_ZERO
  if (enumerator == SPECENUM_ZERO) {
#    ifdef SPECENUM_ZERONAME
    return Q_(SPECENUM_ZERONAME);
#    else
    return SPECENUM_STRING(SPECENUM_ZERO);
#    endif
  }
#  endif
  {
    size_t i;

    for (i = 0; i < ARRAY_SIZE(names); i++) {
      if (1 << i == enumerator) {
        return names[i];
      }
    }
  }
#else
  if ((unsigned)enumerator < ARRAY_SIZE(names)) {
    return names[enumerator];
  }
#endif /* SPECENUM_BITWISE */
  return NULL;
}

#ifdef SPECENUM_BITVECTOR
BV_DEFINE(SPECENUM_BITVECTOR, (SPECENUM_MAX_VALUE + 1));
#endif /* SPECENUM_BITVECTOR */

#undef SPECENUM_NAME
#undef SPECENUM_PASTE_
#undef SPECENUM_PASTE
#undef SPECENUM_STRING_
#undef SPECENUM_STRING
#undef SPECENUM_FOO
#undef SPECENUM_INVALID
#undef SPECENUM_BITWISE
#undef SPECENUM_VALUE
#undef SPECENUM_ZERO
#undef SPECENUM_MIN_VALUE
#undef SPECENUM_MAX_VALUE
#undef SPECENUM_SIZE
#undef SPECENUM_NAMEOVERRIDE
#undef SPECENUM_NAME_UPDATER
#undef SPECENUM_BITVECTOR
#undef SPECENUM_COUNT
#undef SPECENUM_VALUE0
#undef SPECENUM_VALUE1
#undef SPECENUM_VALUE2
#undef SPECENUM_VALUE3
#undef SPECENUM_VALUE4
#undef SPECENUM_VALUE5
#undef SPECENUM_VALUE6
#undef SPECENUM_VALUE7
#undef SPECENUM_VALUE8
#undef SPECENUM_VALUE9
#undef SPECENUM_VALUE10
#undef SPECENUM_VALUE11
#undef SPECENUM_VALUE12
#undef SPECENUM_VALUE13
#undef SPECENUM_VALUE14
#undef SPECENUM_VALUE15
#undef SPECENUM_VALUE16
#undef SPECENUM_VALUE17
#undef SPECENUM_VALUE18
#undef SPECENUM_VALUE19
#undef SPECENUM_VALUE20
#undef SPECENUM_VALUE21
#undef SPECENUM_VALUE22
#undef SPECENUM_VALUE23
#undef SPECENUM_VALUE24
#undef SPECENUM_VALUE25
#undef SPECENUM_VALUE26
#undef SPECENUM_VALUE27
#undef SPECENUM_VALUE28
#undef SPECENUM_VALUE29
#undef SPECENUM_VALUE30
#undef SPECENUM_VALUE31
#undef SPECENUM_VALUE32
#undef SPECENUM_VALUE33
#undef SPECENUM_VALUE34
#undef SPECENUM_VALUE35
#undef SPECENUM_VALUE36
#undef SPECENUM_VALUE37
#undef SPECENUM_VALUE38
#undef SPECENUM_VALUE39
#undef SPECENUM_VALUE40
#undef SPECENUM_VALUE41
#undef SPECENUM_VALUE42
#undef SPECENUM_VALUE43
#undef SPECENUM_VALUE44
#undef SPECENUM_VALUE45
#undef SPECENUM_VALUE46
#undef SPECENUM_VALUE47
#undef SPECENUM_VALUE48
#undef SPECENUM_VALUE49
#undef SPECENUM_VALUE50
#undef SPECENUM_VALUE51
#undef SPECENUM_VALUE52
#undef SPECENUM_VALUE53
#undef SPECENUM_VALUE54
#undef SPECENUM_VALUE55
#undef SPECENUM_VALUE56
#undef SPECENUM_VALUE57
#undef SPECENUM_VALUE58
#undef SPECENUM_VALUE59
#undef SPECENUM_VALUE60
#undef SPECENUM_VALUE61
#undef SPECENUM_VALUE62
#undef SPECENUM_VALUE63
#undef SPECENUM_VALUE64
#undef SPECENUM_VALUE65
#undef SPECENUM_VALUE66
#undef SPECENUM_VALUE67
#undef SPECENUM_VALUE68
#undef SPECENUM_VALUE69
#undef SPECENUM_VALUE70
#undef SPECENUM_VALUE71
#undef SPECENUM_VALUE72
#undef SPECENUM_VALUE73
#undef SPECENUM_VALUE74
#undef SPECENUM_VALUE75
#undef SPECENUM_VALUE76
#undef SPECENUM_VALUE77
#undef SPECENUM_VALUE78
#undef SPECENUM_VALUE79
#undef SPECENUM_VALUE80
#undef SPECENUM_VALUE81
#undef SPECENUM_VALUE82
#undef SPECENUM_VALUE83
#undef SPECENUM_VALUE84
#undef SPECENUM_VALUE85
#undef SPECENUM_VALUE86
#undef SPECENUM_VALUE87
#undef SPECENUM_VALUE88
#undef SPECENUM_VALUE89
#undef SPECENUM_VALUE90
#undef SPECENUM_VALUE91
#undef SPECENUM_VALUE92
#undef SPECENUM_VALUE93
#undef SPECENUM_VALUE94
#undef SPECENUM_VALUE95
#undef SPECENUM_VALUE96
#undef SPECENUM_VALUE97
#undef SPECENUM_VALUE98
#undef SPECENUM_VALUE99
#undef SPECENUM_VALUE100
#undef SPECENUM_VALUE101
#undef SPECENUM_VALUE102
#undef SPECENUM_VALUE103
#undef SPECENUM_VALUE104
#undef SPECENUM_VALUE105
#undef SPECENUM_VALUE106
#undef SPECENUM_VALUE107
#undef SPECENUM_VALUE108
#undef SPECENUM_VALUE109
#undef SPECENUM_VALUE110
#undef SPECENUM_VALUE111
#undef SPECENUM_VALUE112
#undef SPECENUM_VALUE113
#undef SPECENUM_VALUE114
#undef SPECENUM_VALUE115
#undef SPECENUM_VALUE116
#undef SPECENUM_VALUE117
#undef SPECENUM_VALUE118
#undef SPECENUM_VALUE119
#undef SPECENUM_VALUE120
#undef SPECENUM_VALUE121
#undef SPECENUM_VALUE122
#undef SPECENUM_VALUE123
#undef SPECENUM_VALUE124
#undef SPECENUM_VALUE125
#undef SPECENUM_VALUE126
#undef SPECENUM_VALUE127
#undef SPECENUM_VALUE128
#undef SPECENUM_VALUE129
#undef SPECENUM_VALUE130
#undef SPECENUM_VALUE131
#undef SPECENUM_VALUE132
#undef SPECENUM_VALUE133
#undef SPECENUM_VALUE134
#undef SPECENUM_VALUE135
#undef SPECENUM_VALUE136
#undef SPECENUM_VALUE137
#undef SPECENUM_VALUE138
#undef SPECENUM_VALUE139
#undef SPECENUM_VALUE140
#undef SPECENUM_VALUE141
#undef SPECENUM_VALUE142
#undef SPECENUM_VALUE143
#undef SPECENUM_VALUE144
#undef SPECENUM_VALUE145
#undef SPECENUM_VALUE146
#undef SPECENUM_VALUE147
#undef SPECENUM_VALUE148
#undef SPECENUM_VALUE149
#undef SPECENUM_VALUE0NAME
#undef SPECENUM_VALUE1NAME
#undef SPECENUM_VALUE2NAME
#undef SPECENUM_VALUE3NAME
#undef SPECENUM_VALUE4NAME
#undef SPECENUM_VALUE5NAME
#undef SPECENUM_VALUE6NAME
#undef SPECENUM_VALUE7NAME
#undef SPECENUM_VALUE8NAME
#undef SPECENUM_VALUE9NAME
#undef SPECENUM_VALUE10NAME
#undef SPECENUM_VALUE11NAME
#undef SPECENUM_VALUE12NAME
#undef SPECENUM_VALUE13NAME
#undef SPECENUM_VALUE14NAME
#undef SPECENUM_VALUE15NAME
#undef SPECENUM_VALUE16NAME
#undef SPECENUM_VALUE17NAME
#undef SPECENUM_VALUE18NAME
#undef SPECENUM_VALUE19NAME
#undef SPECENUM_VALUE20NAME
#undef SPECENUM_VALUE21NAME
#undef SPECENUM_VALUE22NAME
#undef SPECENUM_VALUE23NAME
#undef SPECENUM_VALUE24NAME
#undef SPECENUM_VALUE25NAME
#undef SPECENUM_VALUE26NAME
#undef SPECENUM_VALUE27NAME
#undef SPECENUM_VALUE28NAME
#undef SPECENUM_VALUE29NAME
#undef SPECENUM_VALUE30NAME
#undef SPECENUM_VALUE31NAME
#undef SPECENUM_VALUE32NAME
#undef SPECENUM_VALUE33NAME
#undef SPECENUM_VALUE34NAME
#undef SPECENUM_VALUE35NAME
#undef SPECENUM_VALUE36NAME
#undef SPECENUM_VALUE37NAME
#undef SPECENUM_VALUE38NAME
#undef SPECENUM_VALUE39NAME
#undef SPECENUM_VALUE40NAME
#undef SPECENUM_VALUE41NAME
#undef SPECENUM_VALUE42NAME
#undef SPECENUM_VALUE43NAME
#undef SPECENUM_VALUE44NAME
#undef SPECENUM_VALUE45NAME
#undef SPECENUM_VALUE46NAME
#undef SPECENUM_VALUE47NAME
#undef SPECENUM_VALUE48NAME
#undef SPECENUM_VALUE49NAME
#undef SPECENUM_VALUE50NAME
#undef SPECENUM_VALUE51NAME
#undef SPECENUM_VALUE52NAME
#undef SPECENUM_VALUE53NAME
#undef SPECENUM_VALUE54NAME
#undef SPECENUM_VALUE55NAME
#undef SPECENUM_VALUE56NAME
#undef SPECENUM_VALUE57NAME
#undef SPECENUM_VALUE58NAME
#undef SPECENUM_VALUE59NAME
#undef SPECENUM_VALUE60NAME
#undef SPECENUM_VALUE61NAME
#undef SPECENUM_VALUE62NAME
#undef SPECENUM_VALUE63NAME
#undef SPECENUM_VALUE64NAME
#undef SPECENUM_VALUE65NAME
#undef SPECENUM_VALUE66NAME
#undef SPECENUM_VALUE67NAME
#undef SPECENUM_VALUE68NAME
#undef SPECENUM_VALUE69NAME
#undef SPECENUM_VALUE70NAME
#undef SPECENUM_VALUE71NAME
#undef SPECENUM_VALUE72NAME
#undef SPECENUM_VALUE73NAME
#undef SPECENUM_VALUE74NAME
#undef SPECENUM_VALUE75NAME
#undef SPECENUM_VALUE76NAME
#undef SPECENUM_VALUE77NAME
#undef SPECENUM_VALUE78NAME
#undef SPECENUM_VALUE79NAME
#undef SPECENUM_VALUE80NAME
#undef SPECENUM_VALUE81NAME
#undef SPECENUM_VALUE82NAME
#undef SPECENUM_VALUE83NAME
#undef SPECENUM_VALUE84NAME
#undef SPECENUM_VALUE85NAME
#undef SPECENUM_VALUE86NAME
#undef SPECENUM_VALUE87NAME
#undef SPECENUM_VALUE88NAME
#undef SPECENUM_VALUE89NAME
#undef SPECENUM_VALUE90NAME
#undef SPECENUM_VALUE91NAME
#undef SPECENUM_VALUE92NAME
#undef SPECENUM_VALUE93NAME
#undef SPECENUM_VALUE94NAME
#undef SPECENUM_VALUE95NAME
#undef SPECENUM_VALUE96NAME
#undef SPECENUM_VALUE97NAME
#undef SPECENUM_VALUE98NAME
#undef SPECENUM_VALUE99NAME
#undef SPECENUM_VALUE100NAME
#undef SPECENUM_VALUE101NAME
#undef SPECENUM_VALUE102NAME
#undef SPECENUM_VALUE103NAME
#undef SPECENUM_VALUE104NAME
#undef SPECENUM_VALUE105NAME
#undef SPECENUM_VALUE106NAME
#undef SPECENUM_VALUE107NAME
#undef SPECENUM_VALUE108NAME
#undef SPECENUM_VALUE109NAME
#undef SPECENUM_VALUE110NAME
#undef SPECENUM_VALUE111NAME
#undef SPECENUM_VALUE112NAME
#undef SPECENUM_VALUE113NAME
#undef SPECENUM_VALUE114NAME
#undef SPECENUM_VALUE115NAME
#undef SPECENUM_VALUE116NAME
#undef SPECENUM_VALUE117NAME
#undef SPECENUM_VALUE118NAME
#undef SPECENUM_VALUE119NAME
#undef SPECENUM_VALUE120NAME
#undef SPECENUM_VALUE121NAME
#undef SPECENUM_VALUE122NAME
#undef SPECENUM_VALUE123NAME
#undef SPECENUM_VALUE124NAME
#undef SPECENUM_VALUE125NAME
#undef SPECENUM_VALUE126NAME
#undef SPECENUM_VALUE127NAME
#undef SPECENUM_VALUE128NAME
#undef SPECENUM_VALUE129NAME
#undef SPECENUM_VALUE130NAME
#undef SPECENUM_VALUE131NAME
#undef SPECENUM_VALUE132NAME
#undef SPECENUM_VALUE133NAME
#undef SPECENUM_VALUE134NAME
#undef SPECENUM_VALUE135NAME
#undef SPECENUM_VALUE136NAME
#undef SPECENUM_VALUE137NAME
#undef SPECENUM_VALUE138NAME
#undef SPECENUM_VALUE139NAME
#undef SPECENUM_VALUE140NAME
#undef SPECENUM_VALUE141NAME
#undef SPECENUM_VALUE142NAME
#undef SPECENUM_VALUE143NAME
#undef SPECENUM_VALUE144NAME
#undef SPECENUM_VALUE145NAME
#undef SPECENUM_VALUE146NAME
#undef SPECENUM_VALUE147NAME
#undef SPECENUM_VALUE148NAME
#undef SPECENUM_VALUE149NAME
#undef SPECENUM_COUNTNAME
#undef SPECENUM_ZERONAME
#undef SPECENUM_VALUE0NAME
#undef SPECENUM_VALUE1NAME
#undef SPECENUM_VALUE2NAME
#undef SPECENUM_VALUE3NAME
#undef SPECENUM_VALUE4NAME
#undef SPECENUM_VALUE5NAME
#undef SPECENUM_VALUE6NAME
#undef SPECENUM_VALUE7NAME
#undef SPECENUM_VALUE8NAME
#undef SPECENUM_VALUE9NAME
#undef SPECENUM_VALUE10NAME
#undef SPECENUM_VALUE11NAME
#undef SPECENUM_VALUE12NAME
#undef SPECENUM_VALUE13NAME
#undef SPECENUM_VALUE14NAME
#undef SPECENUM_VALUE15NAME
#undef SPECENUM_VALUE16NAME
#undef SPECENUM_VALUE17NAME
#undef SPECENUM_VALUE18NAME
#undef SPECENUM_VALUE19NAME
#undef SPECENUM_VALUE20NAME
#undef SPECENUM_VALUE21NAME
#undef SPECENUM_VALUE22NAME
#undef SPECENUM_VALUE23NAME
#undef SPECENUM_VALUE24NAME
#undef SPECENUM_VALUE25NAME
#undef SPECENUM_VALUE26NAME
#undef SPECENUM_VALUE27NAME
#undef SPECENUM_VALUE28NAME
#undef SPECENUM_VALUE29NAME
#undef SPECENUM_VALUE30NAME
#undef SPECENUM_VALUE31NAME
#undef SPECENUM_VALUE32NAME
#undef SPECENUM_VALUE33NAME
#undef SPECENUM_VALUE34NAME
#undef SPECENUM_VALUE35NAME
#undef SPECENUM_VALUE36NAME
#undef SPECENUM_VALUE37NAME
#undef SPECENUM_VALUE38NAME
#undef SPECENUM_VALUE39NAME
#undef SPECENUM_VALUE40NAME
#undef SPECENUM_VALUE41NAME
#undef SPECENUM_VALUE42NAME
#undef SPECENUM_VALUE43NAME
#undef SPECENUM_VALUE44NAME
#undef SPECENUM_VALUE45NAME
#undef SPECENUM_VALUE46NAME
#undef SPECENUM_VALUE47NAME
#undef SPECENUM_VALUE48NAME
#undef SPECENUM_VALUE49NAME
#undef SPECENUM_VALUE50NAME
#undef SPECENUM_VALUE51NAME
#undef SPECENUM_VALUE52NAME
#undef SPECENUM_VALUE53NAME
#undef SPECENUM_VALUE54NAME
#undef SPECENUM_VALUE55NAME
#undef SPECENUM_VALUE56NAME
#undef SPECENUM_VALUE57NAME
#undef SPECENUM_VALUE58NAME
#undef SPECENUM_VALUE59NAME
#undef SPECENUM_VALUE60NAME
#undef SPECENUM_VALUE61NAME
#undef SPECENUM_VALUE62NAME
#undef SPECENUM_VALUE63NAME
#undef SPECENUM_VALUE64NAME
#undef SPECENUM_VALUE65NAME
#undef SPECENUM_VALUE66NAME
#undef SPECENUM_VALUE67NAME
#undef SPECENUM_VALUE68NAME
#undef SPECENUM_VALUE69NAME
#undef SPECENUM_VALUE70NAME
#undef SPECENUM_VALUE71NAME
#undef SPECENUM_VALUE72NAME
#undef SPECENUM_VALUE73NAME
#undef SPECENUM_VALUE74NAME
#undef SPECENUM_VALUE75NAME
#undef SPECENUM_VALUE76NAME
#undef SPECENUM_VALUE77NAME
#undef SPECENUM_VALUE78NAME
#undef SPECENUM_VALUE79NAME
#undef SPECENUM_VALUE80NAME
#undef SPECENUM_VALUE81NAME
#undef SPECENUM_VALUE82NAME
#undef SPECENUM_VALUE83NAME
#undef SPECENUM_VALUE84NAME
#undef SPECENUM_VALUE85NAME
#undef SPECENUM_VALUE86NAME
#undef SPECENUM_VALUE87NAME
#undef SPECENUM_VALUE88NAME
#undef SPECENUM_VALUE89NAME
#undef SPECENUM_VALUE90NAME
#undef SPECENUM_VALUE91NAME
#undef SPECENUM_VALUE92NAME
#undef SPECENUM_VALUE93NAME
#undef SPECENUM_VALUE94NAME
#undef SPECENUM_VALUE95NAME
#undef SPECENUM_VALUE96NAME
#undef SPECENUM_VALUE97NAME
#undef SPECENUM_VALUE98NAME
#undef SPECENUM_VALUE99NAME
#undef SPECENUM_VALUE100NAME
#undef SPECENUM_VALUE101NAME
#undef SPECENUM_VALUE102NAME
#undef SPECENUM_VALUE103NAME
#undef SPECENUM_VALUE104NAME
#undef SPECENUM_VALUE105NAME
#undef SPECENUM_VALUE106NAME
#undef SPECENUM_VALUE107NAME
#undef SPECENUM_VALUE108NAME
#undef SPECENUM_VALUE109NAME
#undef SPECENUM_VALUE110NAME
#undef SPECENUM_VALUE111NAME
#undef SPECENUM_VALUE112NAME
#undef SPECENUM_VALUE113NAME
#undef SPECENUM_VALUE114NAME
#undef SPECENUM_VALUE115NAME
#undef SPECENUM_VALUE116NAME
#undef SPECENUM_VALUE117NAME
#undef SPECENUM_VALUE118NAME
#undef SPECENUM_VALUE119NAME
#undef SPECENUM_VALUE120NAME
#undef SPECENUM_VALUE121NAME
#undef SPECENUM_VALUE122NAME
#undef SPECENUM_VALUE123NAME
#undef SPECENUM_VALUE124NAME
#undef SPECENUM_VALUE125NAME
#undef SPECENUM_VALUE126NAME
#undef SPECENUM_VALUE127NAME
#undef SPECENUM_VALUE128NAME
#undef SPECENUM_VALUE129NAME
#undef SPECENUM_VALUE130NAME
#undef SPECENUM_VALUE131NAME
#undef SPECENUM_VALUE132NAME
#undef SPECENUM_VALUE133NAME
#undef SPECENUM_VALUE134NAME
#undef SPECENUM_VALUE135NAME
#undef SPECENUM_VALUE136NAME
#undef SPECENUM_VALUE137NAME
#undef SPECENUM_VALUE138NAME
#undef SPECENUM_VALUE139NAME
#undef SPECENUM_VALUE140NAME
#undef SPECENUM_VALUE141NAME
#undef SPECENUM_VALUE142NAME
#undef SPECENUM_VALUE143NAME
#undef SPECENUM_VALUE144NAME
#undef SPECENUM_VALUE145NAME
#undef SPECENUM_VALUE146NAME
#undef SPECENUM_VALUE147NAME
#undef SPECENUM_VALUE148NAME
#undef SPECENUM_VALUE149NAME

#ifdef __cplusplus
}
#endif /* __cplusplus */
