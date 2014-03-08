#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <stack>

#include "hierarchy.hpp"

// readability improvement
#define pole subtype[0]

// ----- klass ----- //

klass_t::klass_t(
  std::size_t hh
, klass_t const* kk
)
: hash(hh)
, base(kk)
, subtype(1)
, sig(nullptr) {}

void klass_t::shrink() {
  /// requires : hierarchy root is a pole
  while(base && !base->pole)
    base = base->base;
}
