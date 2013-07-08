#include "impl.hpp"

#include <boost/optional.hpp>

#if USE_SMALLINT_FUNCTOR
using nocache_value = std::pair<std::size_t, value_type>;

// struct nocache_hasher {
//   std::hash<functor_t> ff;
//   std::size_t operator()(const nocache_value& v) const noexcept {
//     return v.first ^ ff(v.second);
//   }
// };

inline static value_type functor(nocache_value v) { return v.second; }
#else
using nocache_value = value_type;

// using nocache_hasher = std::hash<functor_t>;
inline static value_type functor(nocache_value v) { return v; }
#endif

using nocache_set   = std::set<nocache_value>;
using nocache_map   = std::unordered_map<rtti_type, nocache_set>;
using nocache_table = std::vector<nocache_map>;

struct invoker_table_type::poles_table {
  explicit poles_table(std::size_t arity)
  : table(arity) {}

  nocache_table table;
  
#if USE_SMALLINT_FUNCTOR
  std::size_t unique_id_counter;
#endif
};

static nocache_set const*
lookup_nocache_onevar(
  nocache_map const& map
, rtti_hierarchy hier
) {
  nocache_map::const_iterator it;
  for(; hier; hier = hier->base) {
    it = map.find(hier->id);
    
    if(it != map.end() && it->second.size() != 0)
      return &it->second;
  }

  return nullptr;
}

static unsigned ATTRIBUTE_PURE
clog2(unsigned long u) {
  unsigned ret = 0;
  while(u) { u >>= 1; ++ret; }
  return ret;
}

using nocache_value_opt = boost::optional<nocache_value>;

nocache_value_opt
static lookup_nocache_intersect(
  std::size_t arity
, std::vector<nocache_set const*>&& sets
) {
  ASSERT(arity > 0); ASSERT(sets.size() == arity);
  // easy case
  if(arity == 1)
    if(sets[0]->size() == 1)
      return *sets[0]->begin();
    else
      return boost::none;

  // sort according to set size
  std::sort(
    sets.begin(), sets.end()
  , [](nocache_set const* a, nocache_set const* b) {
      ASSERT(a && b);
      return a->size() < b->size();
  });

  // buffer for intersection
  std::vector<nocache_value> ret ( sets[0]->size() );
  std::copy(sets[0]->begin(), sets[0]->end(), ret.begin());

  for(std::size_t i = 1; i < arity; ++i) {
    if(ret.size() == 0)
      break;
    
    nocache_set const& s = *sets[i];

    std::vector<nocache_value> tmp; tmp.reserve( ret.size() );
    if(ret.size() + clog2(s.size()) > s.size()) {
      // linear scan
      std::set_intersection(
        ret.begin(), ret.end()
      , s.begin(), s.end()
      , std::back_inserter(tmp)
      );
    }
    
    else {
      // logarithmic scan
      std::copy_if(
        ret.begin(), ret.end()
      , std::back_inserter(tmp)
      , [&s](nocache_value f) ATTRIBUTE_PURE {
          return s.find(f) != s.end();
      });
    }

    ret = std::move(tmp);
  }

  if(ret.size() == 1)
    return ret[0];
  
  return boost::none;
}

nocache_value_opt
static lookup_nocache_help(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
) {
  std::vector<nocache_set const*> sets (arity);
  
  for(std::size_t i = 0; i < arity; ++i) {
    nocache_set const* s = lookup_nocache_onevar(table.root->table[i], hiers[i]);
    if(!s) return boost::none;
    sets[i] = s;
  }
  
  return lookup_nocache_intersect(arity, std::move(sets));
}

void init_nocache(
  std::size_t arity
, invoker_table_type& tbl
) {
  tbl.root = new invoker_table_type::poles_table(arity);
}

functor_t lookup_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
) {
  ASSERT(table.root);
  nocache_value_opt ret = lookup_nocache_help(arity, table, hiers);
  return ret ? functor(*ret) : nullptr;
}

void insert_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
, functor_t f
) {
  ASSERT(table.root);
#if USE_SMALLINT_FUNCTOR
  nocache_value val ( table.root->unique_id_counter++, f );
#else
  nocache_value val = f;
#endif
  for(std::size_t i = 0; i < arity; ++i) {
    nocache_map& map = table.root->table[i];
    rtti_hierarchy h = hiers[i];
    map[h->id].insert(val);
  }
}

void retract_nocache(
  std::size_t arity
, invoker_table_type& table
, rtti_hierarchy* hiers
, functor_t f
) {
  ASSERT(table.root);
#if USE_SMALLINT_FUNCTOR
  nocache_value_opt obj_opt = lookup_nocache_help(arity, table, hiers);
  if(!obj_opt) return;
  nocache_value obj = obj_opt;
  (void)f;
#else
  nocache_value obj = f;
#endif
  ASSERT( functor(obj) == f );

  for(std::size_t i = 0; i < arity; ++i) {
    nocache_map& map = table.root->table[i];
    rtti_hierarchy h = hiers[i];
    map[h->id].erase(obj);
  }
}
