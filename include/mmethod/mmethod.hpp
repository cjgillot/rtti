//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HPP
#define RTTI_MMETHOD_HPP

#include "mmethod/rtti.hpp"
#include "mmethod/invoke.hpp"
#include "mmethod/declare.hpp"
#include "mmethod/implement.hpp"

/*! \page RTTI.MMethod library

  This library implements open multi-methods in ISO C++.

  The external interface fits in three files :
  - \c mmethod.hpp
  - \c declare.hpp
  - \c implement.hpp

  \c declare.hpp exports everything needed to declare mmethods.
  It does not provide code used for calling those. \see declare.hpp

  \c mmethod.hpp exports everything needed to declare
  and invoke mmethods. \see mmethod.hpp

  \c implement.hpp exports everything needed to implement
  formerly declared mmethods. \see implement.hpp

*/

/*! \file declare.hpp

  The declaration of a mmethod passes by the \c MMETHOD_DECLARE macro.
  It takes following arguments :
  - the name of the mmethod
  - the return type
  - the argument list

  The argument list uses \c rtti::tags::_v to discriminate between the arguments
  involved in the runtime dispatch and the others. This tag should appear directly on
  the polymorphic class type.
  All class types used as virtual in mmethod must be RTTI-enabled. \see rtti.hpp

  \code
class foo
  // unspecified
;
class bar
  // unspecified
;

using rtti::tags::_v;

// f1 is unary : only the first argument, of type [foo*], is used in resolution
MMETHOD_DECLARE(f1, void, (_v<foo*>, bar&));

// f2 is binary : both arguments are used in resolution
MMETHOD_DECLARE(f2, int, (_v<foo*>, _v<bar&>));

// f3 does not compile : the tag must be the outmost template
// MMETHOD_DECLARE(f3, float, (_v<foo>* ));
  \endcode

  Arguments to a mmethod may be pointer or references.
  In the future, a traits template will be provided to
  use other pointer-like types.

  Then, the defined mmethod may be called like regular functions.
  You need to include \see mmethod.hpp if you want those calls to be inlined.
  \code
  foo f; bar b;

  f1(&f, b);
  f2(&f, b);
  \endcode

 */

/*! \file mmethod.hpp

  This file loads the necessary includes to call mmethods efficiently.

  \see declare.hpp for information about declaring mmethods.
 */

/*! \file implement.hpp

  The definition of a mmethod passes by the \c MMETHOD_IMPLEMENT macro.
  It takes following arguments :
  - the name of the mmethod
  - the return type
  - the argument list

  and it expects the function body to follow.

  The argument list must not be tagged as in \c MMETHOD_DECLARE.

  \code
class foo
  // unspecified
;
class bar
  // unspecified, inherits from foo
;

template<typename T> using _v = rtti::tags::virtual_<T>;

// f1 is unary
MMETHOD_DECLARE(f1, void, (_v<foo>* ));

// first case
MMETHOD_IMPLEMENT(f1, void, (foo* a)) { a->do_something(); }

// second case
MMETHOD_IMPLEMENT(f1, void, (bar* a)) { a->do_something_else(); }
  \endcode

  MMethod implement may be spread across translation units.
  The dispatch table is generated at the first call to the mmethod,
  It can be computed earlier by a call to <tt>f1.generate()</tt>.

  If an ambiguity is found (no-match or several-match) in the dispatch
  table generation, the corresponding case will throw \c rtti::bad_dispatch.
 */

#endif
