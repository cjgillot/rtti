//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <stack>

#include "hierarchy.hpp"

// ----- klass ----- //

klass_t::klass_t(
  rtti_type hh
, std::size_t arity
)
: id(hh)
, bases(arity)
, pole(NULL)
, subtype(1)
, sig(NULL) {}
