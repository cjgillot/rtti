//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/policy/default_policy.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/throw_exception.hpp>

using namespace rtti;
using namespace rtti::mmethod::ambiguity;

class default_duplicator
: public duplicator
{
public:
  void load(size_t, rtti_hierarchy*) {}
  bool next() { return false; }
};

boost::shared_ptr<duplicator>
default_policy::make_duplicate() {
  static boost::shared_ptr<duplicator> ret =
      boost::make_shared<default_duplicator>();

  return ret;
}

void default_policy::bad_dispatch() {
  BOOST_THROW_EXCEPTION( rtti::bad_dispatch() );
}

void default_policy::ambiguity_handler(size_t, rtti_hierarchy const*)
{}

