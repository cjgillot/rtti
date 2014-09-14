//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[wg_widget
/*`
  Unary __multimethods__ can be used to extend classes without adding new virtual functions,
  providing a simple replacement to the visitor pattern,
  the latter allowing only closed-world dispatch.

  Suppose we have a hierarchy of widgets, printing various things:
  [wg_classes]

  It could be useful to be able to count the number of printed characters,
  without having to modify the classes.
  We then define the `count` __multimethod__.
  [wg_mm_declaration]

  We can implement it on the several widgets.
  [wg_mm_implement]

  It can now be used like any native function on the widgets.
  [wg_use]
 */
//]

//<-
#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/noncopyable.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;
//->

//[wg_classes
// Base class for widgets
struct Widget
: base_rtti<Widget>  // rtti hierarchy
, boost::noncopyable
{
  Widget() {/**/}
  virtual ~Widget() {/**/}

  virtual void show() const = 0; // Show widget
};

struct WNumber
: public Widget
, public implement_rtti<WNumber, vector<Widget> >
{
  explicit WNumber(int n): number(n) {}

  void show() const { std::cout << number << std::endl; }

  inline int get_number() const { return number; }

private:
  int number;
};

struct WString
: public Widget
, public implement_rtti<WString, vector<Widget> >
{
  explicit WString(std::string const& l): str(l) {}

  void show() const { std::cout << str << std::endl; }

  inline std::string const& get_str() const { return str; }

private:
  std::string str;
};

struct WLetter
: public WString
, public implement_rtti<WLetter, vector<WString> >
{
  explicit WLetter(char l): WString(std::string(1, l)) {}  // Ctor

  void show() const { WString::show(); }

  inline char get_letter() const { return get_str()[0]; }
};
//]

//[wg_mm_declaration
using tags::_v;

BOOST_MMETHOD_DECLARE(count, int, (_v<Widget const&>));
//]

//[wg_mm_implement
BOOST_MMETHOD_IMPLEMENT(count, int, ( WString const& s ))
{
  return s.get_str().size();
}

BOOST_MMETHOD_IMPLEMENT(count, int, ( WLetter const& ))
{
  return 1;
}

BOOST_MMETHOD_IMPLEMENT(count, int, ( WNumber const& n ))
{
  int k = n.get_number();

  // trivial case
  if(k == 0)
    return 1;

  int ret = 0;
  if(k < 0) {
    ret = 1;
    k = -k;
  }

  // count digits
  for(; k != 0; k /= 10)
    ++ret;

  return ret;
}
//]

BOOST_AUTO_TEST_CASE(test_widget)
{
  //[wg_use
  WNumber wn ( 150 );
  WString ws ( "foo" );
  WLetter wl ( 'a' );

  //=std::cout << "Prepare to print " << count(wn) << " characters : ";
  //=  wn.show();
  //=std::cout << std::endl;
  //]

  BOOST_CHECK_EQUAL( count(wn), 3 );
  BOOST_CHECK_EQUAL( count(ws), 3 );
  BOOST_CHECK_EQUAL( count(wl), 1 );
}
