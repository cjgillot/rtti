#ifndef HEAD_HPP
#define HEAD_HPP

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"

#include <memory>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual void func();
  virtual ~foo();
};

struct bar
: foo
, implement_rtti<bar, vector<foo> > {
  virtual void func();
};

struct baz
: foo
, implement_rtti<baz, vector<foo> > {
  virtual void func();
};

using tags::_v;

DECLARE_MMETHOD(vf1, void, (_v<foo*>, int));
DECLARE_MMETHOD(vf2, void, (_v<foo*>, _v<foo&>));

#endif
