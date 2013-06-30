#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <stack>

#include "hierarchy.hpp"

// readability improvement
#define pole subtype[0]

// ----- klass ----- //

inline klass_t::klass_t()
: hash(0ul)
, base(nullptr)
, subtype(1)
, sig(nullptr) {}

void klass_t::shrink() {
  /// requires : hierarchy root is a pole
  while(base && !base->pole)
    base = base->base;
}

// ----- hierarchy ----- //

void hierarchy_t::shrink() {
  for(auto& p : dict)
    p.second.shrink();

  for(auto it = dict.begin(), en = dict.end(); it != en; ++it)
    if(! it->second.pole)
      it = dict.erase(it);

  for(auto& p : dict)
    p.second.subtype.resize(dict.size());
}

const klass_t* hierarchy_t::add(hashvec_t const& vec) {
  klass_t* first = nullptr;
  klass_t* old = nullptr;
  { // first path item : current pole
    std::size_t hash = vec.front();

    // one may test on k.hash to choose whether newly inserted or not
    klass_t& k = dict[hash];
    k.hash = hash; k.pole = true;
    first = old = &k;
  }

  for(auto it = vec.begin() + 1, en = vec.end(); it != en; ++it)
  { // rest of path
    std::size_t hash = *it;

    klass_t& k = dict[hash];
    k.hash = hash;
    old->base = &k;
    old = &k;
  }

  return first;
}

template<typename Sequence>
void hierarchy_t::order(Sequence& seq) {
  /// hierarchy must be shrinked already
  std::vector<klass_t*> stack; stack.reserve(dict.size());
  std::transform(
    dict.begin(), dict.end(),
    std::back_inserter(stack),
    [](dict_t::reference p) { return &p.second; }
  );

  /// rank == 0 for root
  std::size_t rank = 0;
  while(!stack.empty()) {
    klass_t* top = stack.back();
    if(!top->pole) {
      stack.pop_back();
      continue;
    }

    klass_t* base = top->base;
    while(base && base->pole)
    { stack.push_back(top); top = base; base = top->base; }

    seq.push_back(top);
    top->rank = rank;

    if(base) top->subtype = base->subtype;
    top->subtype.set(rank);

    top->pole = false;
    stack.pop_back(); ++rank;
  }

  for(klass_t const* k : seq)
    const_cast<klass_t*>(k)->pole = true;
}

// template klass_t const* hierarchy_t::add<boost::tokenizer<>::iterator>(boost::tokenizer<>::iterator, boost::tokenizer<>::iterator const&);
template void hierarchy_t::order<>(std::vector<klass_t const*>&);
