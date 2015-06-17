//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_AMBIGUITY_DUPLICATE_HPP
#define MMETHOD_AMBIGUITY_DUPLICATE_HPP

#include "mmethod/rttifwd.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace mmethod {
namespace ambiguity {

/*!\brief Interface of a duplicator object
 * Objects following this interface are generated
 * by the policy classes to duplicate multimethod
 * overloads.
 * The use is the following:
 * - it is fed an array of \c rtti_node
 * - we loop on \c next() to transform inplace the array
 * Example usage:
 * <code>
 * duplicator* d = policy::make_duplicator();
 *
 * d->load(my_arity, my_array);
 *
 * do {
 *   // read my_array
 * }
 * while(d->next());
 *
 * delete d;
 * </code>
 */
class duplicator {
public:
  //!\brief Destructor
  virtual ~duplicator() {}

  //!\brief Load a C array of \c rtti_node
  //!\param arity the size of the array
  //!\param array pointer to the array
  //! This array is to be modified in place by \c next().
  virtual void load(size_t arity, rtti_hierarchy* array) = 0;

  //!\brief Mutate the array to the next overload
  //!\return \c true if the array is to be used, \c false otherwise
  virtual bool next() = 0;
};

} // namespace ambiguity

using ambiguity::duplicator;

}} // namespace rtti::mmethod

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
