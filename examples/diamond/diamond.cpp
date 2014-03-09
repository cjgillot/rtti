#include "rtti/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <iostream>
#include <boost/mpl/vector.hpp>

using namespace rtti;

/*!\example diamond.cpp
 * 
 * This example demonstrate mmethods with multiple inheritance.
 * 
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 * 
 * \c foo is the base class
 * \c bar has a statically defined id \c bar_id
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};

struct bar
: virtual foo
, implement_rtti<bar, boost::mpl::vector<foo> >
{};

struct baz
: virtual foo
, implement_rtti<baz, boost::mpl::vector<foo> >
{};

struct lap
: bar, baz
, implement_rtti<lap, boost::mpl::vector<bar, baz>>
{};

template<typename T> using v_ = tags::virtual_<T>;
DECLARE_MMETHOD(f1, int, (v_<foo>&));

IMPLEMENT_MMETHOD(f1, int, (foo& a)) { return 5; }
IMPLEMENT_MMETHOD(f1, int, (bar& a)) { return 7; }
IMPLEMENT_MMETHOD(f1, int, (lap& a)) { return 15; }

int main() {
  foo f; bar r; baz z; lap l;

  std::cout << f1(f) << std::endl; // prints 5
  std::cout << f1(r) << std::endl; // prints 42
  std::cout << f1(z) << std::endl; // prints 10
  std::cout << f1(l) << std::endl; // prints 42 (lap is-a bar)
  
  return 0;
}
