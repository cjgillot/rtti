#include <boost/tokenizer.hpp>

#include <boost/lexical_cast.hpp>
#include <stack>

#include "hierarchy.hpp"

// readability improvement
#define pole subtype[0]

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

klass_t const*
hierarchy_t::add(hashvec_t const& vec) {
  klass_t const* base = nullptr;

  for(auto it = vec.rbegin(), en = vec.rend(); it != en; ++it) {
    std::size_t hash = *it;
 
    auto dit = dict.find(hash);
    if(dit != dict.end()) {
      klass_t& k = dit->second;
      
      assert( k.hash == hash );
      
      base = &k;
    }
    else {
      dit = dict.emplace( hash, klass_t{ hash, base } ).first;
      base = &dit->second;
    }
  }
  
  const_cast<klass_t*>(base)->pole = true;
  return base;
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

    klass_t* base = const_cast<klass_t*>(top->base);
    while(base && base->pole)
    { stack.push_back(top); top = base; base = const_cast<klass_t*>(top->base); }

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
