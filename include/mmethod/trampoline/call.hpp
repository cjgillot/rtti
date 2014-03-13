//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

private:
  typedef typename boost::mpl::transform<Args, tags::unwrap>::type unwrapped_args;

#define BOOST_PP_FILENAME_1 "mmethod/trampoline/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, 2)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS
