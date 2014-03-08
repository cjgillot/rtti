#include "multifile.hpp"

#include "mmethod/implement.hpp"

struct bar
: foo
, implement_rtti<bar, foo>
{
  int g();
};

int bar::g() { return 42; }
foo* make_bar() { return new bar; }

IMPLEMENT_MMETHOD(f1, int, (bar& a)) { return a.g(); }
