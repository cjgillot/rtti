private:
  typedef typename boost::mpl::transform<Args, tags::unwrap>::type unwrapped_args;

#define BOOST_PP_FILENAME_1 "mmethod/trampoline/call_template.hpp"
#define BOOST_PP_ITERATION_LIMITS (0, 2)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS