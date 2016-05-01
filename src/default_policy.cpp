//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/policy.hpp"

#include <boost/move/unique_ptr.hpp>
#include <boost/throw_exception.hpp>

using namespace rtti;
using namespace rtti::mmethod::ambiguity;

typedef boost::movelib::unique_ptr<duplicator> dup_ptr;

class default_duplicator
: public duplicator
{
public:
  void load(size_t, rtti_hierarchy*) {}
  bool next() { return false; }
};

dup_ptr
default_policy::make_duplicate() {
  dup_ptr ret ( new default_duplicator );
  return BOOST_MOVE_RET(dup_ptr, ret);
}

void default_policy::bad_dispatch() {
  BOOST_THROW_EXCEPTION( rtti::bad_dispatch() );
}

action_t
default_policy::ambiguity_handler(size_t, rtti_hierarchy const*) {
  return action_t::NOTHING;
}

