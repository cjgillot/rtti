#ifndef RTTI_DMETHOD_DECLARE_HPP
#define RTTI_DMETHOD_DECLARE_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/shared/tags.hpp"

#include "mmethod/dynamic/common.hpp"
#include "mmethod/dispatch/dispatch.hpp"

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/function_types/components.hpp>

#include "mmethod/declare/helper.hpp"

namespace rtti { namespace dmethod {

template<typename Tag, typename Sig>
struct dmethod_register
: register_base<Tag>
, detail::make_declare<Tag, Sig>::type {
private:
  typedef typename detail::make_declare<Tag, Sig>::type decl_maker;

public:
  using decl_maker::result_type;
  using decl_maker::operator();
  using decl_maker::insert;
  using decl_maker::generate;
};

}} // namespace rtti::mmethod

#define __MMETHOD_TAG(name) BOOST_PP_CAT(__rtti_mmethod_tags__, name)

#define DECLARE_MMETHOD(name, ret, sig) \
struct __MMETHOD_TAG(name)              \
: rtti::dmethod::dmethod_register<      \
  __MMETHOD_TAG(name)                   \
, ret sig   > {                         \
  template<typename> struct overload;   \
}; \
static __MMETHOD_TAG(name) name /* ; */

#include "mmethod/declare/traits.hpp"

#endif
