//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/mmethod.hpp"
#include "mmethod/smart_ptr/boost_intrusive_ptr.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

using namespace rtti;
using namespace rtti::mmethod;

namespace {

using boost::mpl::vector;

class foo
: public base_rtti<foo>
, public boost::intrusive_ref_counter<foo>
{
public:
  virtual ~foo() {}
};

class bar
: public foo
, public implement_rtti<bar, vector<foo> >
{};

using tags::_v;
DECLARE_MMETHOD(sip, int, (_v<boost::intrusive_ptr<foo> >));

IMPLEMENT_MMETHOD(sip, int, (boost::intrusive_ptr<foo>)) { return  5; }
IMPLEMENT_MMETHOD(sip, int, (boost::intrusive_ptr<bar>)) { return 42; }

} // namespace

BOOST_AUTO_TEST_CASE(TESTNAME) {
  typedef boost::intrusive_ptr<foo> pointer;
  pointer f ( new foo );
  pointer r ( new bar );

  BOOST_CHECK_EQUAL( sip(f),  5 );
  BOOST_CHECK_EQUAL( sip(r), 42 );
}
