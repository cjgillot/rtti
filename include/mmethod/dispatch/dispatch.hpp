#ifndef RTTI_DMETHOD_DISPATCH_HPP
#define RTTI_DMETHOD_DISPATCH_HPP

#include "mmethod/dynamic/poles.hpp"

#include "mmethod/shared/basic.hpp"
#include "rtti/holder/getter.hpp"

#include <boost/assert.hpp>
#include <functional>

namespace rtti { namespace dmethod { namespace detail {

/// main dispatch function
template<typename Tag, typename Ret>
struct dispatch {
  dispatch() {
    struct initializer_t {
      initializer_t() { Tag::do_initialize(); };
      void touch() {}
    }
    static initializer;
    initializer.touch();
  }

  template<typename Tuple>
  invoker_t ATTRIBUTE_PURE fetch(Tuple const& args) const;

  template<typename Tuple>
  Ret call(Tuple const& args) const {
    this->generate();

    // do actual fetching
    invoker_t f = this->fetch( args );

    BOOST_ASSERT(f);

    typedef typename Tag::traits::trampoline::sig_t func_t;
    return (*reinterpret_cast<func_t>(f))( args );
  }

  template<typename K, typename F>
  void insert(F const& f);

  // calls seal once
  void generate() const {
    struct sealer_t {
      sealer_t(dispatch const* a) { a->seal(); };
      void touch() {}
    }
    static sealer ( this );
    sealer.touch();
  }

  // must only be called by generate
  void seal() const;
};

}}} // rtti::mmethod::detail

#include "mmethod/dispatch/fetch.hpp"
#include "mmethod/dispatch/insert.hpp"

#endif
