#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <stack>

#include "hierarchy.hpp"

// readability improvement
#define is_pole subtype[0]

// ----- klass ----- //

klass_t::klass_t(
  std::size_t hh
, std::size_t arity
)
: id(hh)
, bases(arity)
, subtype(1)
, pole(nullptr)
, sig(nullptr) {}
