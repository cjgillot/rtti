//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "multifile.hpp"

#include "mmethod/implement.hpp"

#include <boost/mpl/vector.hpp>

namespace multifile {

struct baz
: foo
, implement_rtti<baz, boost::mpl::vector<foo> >
{};

foo* make_baz() { return new baz; }

IMPLEMENT_MMETHOD(f1, int, (baz& a)) { return 2 * a.f(); }

} // namespace multifile
