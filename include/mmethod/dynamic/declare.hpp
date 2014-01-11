#ifndef RTTI_DMETHOD_DECLARE_HPP
#define RTTI_DMETHOD_DECLARE_HPP

#include "mmethod/dynamic/common.hpp"
#include "mmethod/dynamic/dispatch.hpp"

#define MMETHOD_NSPACE dmethod
#include "mmethod/templates/declare.hpp"
#undef MMETHOD_NSPACE

namespace rtti { namespace dmethod {

template<typename Tag, typename Sig>
struct dmethod_register
: register_base<Tag>
, detail::make_declare<Tag, Sig> {
private:
  typedef detail::make_declare<Tag, Sig> decl_maker;

public:
  using decl_maker::operator();
  using decl_maker::insert;
  using decl_maker::retract;
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

#include "mmethod/dynamic/declare.tpp"

#endif
