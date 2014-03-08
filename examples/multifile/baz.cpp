#include "multifile.hpp"

#include "mmethod/implement.hpp"

struct baz
: foo
, implement_rtti<baz, foo>
{};

foo* make_baz() { return new baz; }

IMPLEMENT_MMETHOD(f1, int, (baz& a)) { return 2 * a.f(); }
