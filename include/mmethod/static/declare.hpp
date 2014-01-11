#ifndef RTTI_MMETHOD_DECLARE_HPP
#define RTTI_MMETHOD_DECLARE_HPP

#include "mmethod/static/common.hpp"
#include "mmethod/static/dispatch.hpp"

#define MMETHOD_NSPACE mmethod
#include "mmethod/templates/declare.hpp"
#undef MMETHOD_NSPACE

namespace rtti { namespace mmethod {

template<typename Tag, typename Sig>
struct mmethod_register
: register_base<Tag>
, detail::make_declare<Tag, Sig> {
private:
  typedef detail::make_declare<Tag, Sig> decl_maker;

public:
  using decl_maker::operator();
};

}} // namespace rtti::mmethod

#define __MMETHOD_TAG(name) BOOST_PP_CAT(__rtti_mmethod_tags__, name)

#define DECLARE_MMETHOD(name, ret, sig) \
struct __MMETHOD_TAG(name)              \
: rtti::mmethod::mmethod_register<      \
  __MMETHOD_TAG(name)                   \
, ret sig   > {                         \
  template<typename> struct overload;   \
}; \
static __MMETHOD_TAG(name) name /* ; */

#include "mmethod/static/declare.tpp"

#endif
