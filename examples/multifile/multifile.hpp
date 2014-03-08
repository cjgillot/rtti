#ifndef MULTIFILE_HPP
#define MULTIFILE_HPP

#include "rtti/rtti.hpp"

#include "mmethod/mmethod.hpp"

using namespace rtti;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f();
};

template<typename T> using v_ = tags::virtual_<T>;
DECLARE_MMETHOD(f1, int, (v_<foo>&));

foo* make_foo();
foo* make_bar();
foo* make_baz();

#endif
