#include "multifile.hpp"

#include "mmethod/implement.hpp"

int foo::f() { return 5; }
foo* make_foo() { return new foo; }

IMPLEMENT_MMETHOD(f1, int, (foo& a)) { return a.f(); }
