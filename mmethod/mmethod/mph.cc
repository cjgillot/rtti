#include "overloads.hpp"
#include "archive.hpp"

static std::size_t
rerank(
  pole_table_t const& table
) {
  std::size_t incr = 1;

  for(auto const& h : table) {
    std::size_t current = 0;

    for(klass_t const* k : h) {
      const_cast<klass_t*>( k )->rankhash = current;
      current += incr;
    }

    incr = current;
  }

  return incr;
}

typedef std::pair<std::size_t, std::string> ass_t;

template<typename Sig, typename Inv>
static ass_t
make_assignment(
  Sig& sig,
  Inv& inv
) {
  std::size_t const index = std::accumulate(
    sig.begin(), sig.end(), 0,
    [](std::size_t acc, klass_t const* k)
    { return acc + k->rankhash; }
  );

  std::ostringstream oss;

  oss << "\t_impl_invoker_table[" << index << "] = MMETHOD::overload<rtti::mpl::mplpack_c<0";
  for(klass_t const* k : inv) oss << ", " << k->hash << "ul";
  oss << ">>::address";

  oss << "/* ";
  for(klass_t const* k : sig) oss << k->rankhash << " ";
  oss << "*/";

  oss << ";" << std::endl;

  return { index, oss.str() };
}

/* Generates the hash function and the key word recognizer function.  */
void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const arch_declaration& decl,
  std::ostream& os
) {
  if(decl.vsize == 1) return;

  os << "enum { MIN_HASH_VALUE = 0 };\n";

  { // invoker_table
    std::size_t const max_index = rerank(pole_table);
    os << "static invoker_t _impl_invoker_table[] = {\n";
    std::fill_n(std::ostream_iterator<std::string>(os), max_index, "\t&_rtti_bad_dispatch,\n");
    os << "};" << std::endl;
  }
  os << "static void _impl_inittable() {" << std::endl;

  /* Generate an array of reserved words at appropriate locations.  */
  std::vector<ass_t> assignments;
  for(auto const& p : dispatch)
    if(p.second)
      assignments.push_back( make_assignment(p.first.array(), p.second->array()) );

  std::sort(assignments.begin(), assignments.end());
  std::transform(
    assignments.begin(), assignments.end(),
    std::ostream_iterator<std::string>(os),
    [](ass_t& a) { return a.second; }
  );

  os << "}" << std::endl;
}
