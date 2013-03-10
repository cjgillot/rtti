#include <ostream>
#include <vector>

#include "archive.hpp"
#include "hierarchy.hpp"
#include "overloads.hpp"
#include "rtti/hash/hash.hpp"

static void print_dyn_id(std::ostream& ofile, klass_t const* k, std::size_t i) {
  if(RTTI_HASH_IS_ID(k->hash)) {
    ofile << "rtti_type(" << k->hash << ")";
  }
  else {
    // runtime access id
    ofile << "MMETHOD::overload<rtti::mpl::mplpack_c<0";
    for(const klass_t* k2 : k->sig->array())
      ofile << ", " << k2->hash << "ul";
    assert( k == k->sig->array()[i] );
    ofile << ">>::poles<" << i << ">::node->id";
  }
}

static void print_rank(std::ostream& ofile, klass_t const* k, std::size_t arity, bool runtime) {
  if(arity == 1 && runtime) {
    // special case : input invoker directly
    ofile << "(std::uintptr_t)MMETHOD::overload<rtti::mpl::mplpack_c<0";
    for(const klass_t* k2 : k->sig->array())
      ofile << ", " << k2->hash << "ul";
    ofile << ">>::address";
  }
  else {
    ofile << 2 * k->rankhash << " /* " << k->rank << " */";
  }
}

template<typename Seq>
static void print_insert(std::ostream& ofile, std::size_t i, Seq&& seq, std::size_t arity) {
  for(const klass_t* k : seq)
  {
    ofile << "\ta.insert( ";
      print_dyn_id(ofile, k, i);
    ofile << ", ";
      print_rank(ofile, k, arity, true);
    ofile << " );\n";
  }
}

template<typename Seq>
static void print_map(std::ostream& ofile, std::size_t i, Seq const& t, std::size_t arity) {
  /// split \c t between static and dynamic id
  std::vector<klass_t const*> statics, dynamics;
  std::for_each(
    t.begin(), t.end(),
    [&](klass_t const* k) {
      if(RTTI_HASH_IS_ID(k->hash))
        statics.push_back(k);
      else
        dynamics.push_back(k);
    }
  );

  /// declare smallarray
  ofile << "#if MMETHOD_USE_SMALLARRAY" << std::endl;
  ofile << "static pole_t _impl_smallarray" << i << "[] = {" << std::endl;
  // add hashes base first -> forward iterators
  { // static hashes
    std::size_t lasthash = -1;
    for(const klass_t* k : statics)
    {
      while(++lasthash < k->hash)
        ofile << "\t{ /* FILL */ }," << std::endl;

      ofile << "\t{ ";
        ofile << k->hash;
      ofile << ", ";
        print_rank(ofile, k, arity, false);
      ofile << " }," << std::endl;

      assert( lasthash == k->hash );
    }
  }
  ofile << "};" << std::endl;
  ofile << "#endif" << std::endl;

  ofile << "static void _impl_assignarray" << i << "() {\n";
  ofile << "\tdetail::poles_map_type& a = register_base<MMETHOD>::poles<" << i << ">::array;\n" << std::endl;

  if(arity == 1 && statics.size())
  { // static hashes
    ofile << "#if MMETHOD_USE_SMALLARRAY" << std::endl;
    for(const klass_t* k : statics)
    {
      ofile << "\t_impl_smallarray" << i << "[" << k->hash << "].value = ";
      print_rank(ofile, k, arity, true);
      ofile << ";\n";
    }
    ofile << "#endif" << std::endl << std::endl;
  }

  ofile << "#if MMETHOD_USE_SMALLARRAY" << std::endl;
  ofile << "\ta.create<" << dynamics.size() << ">();\n";
  ofile << "#else" << std::endl;
  ofile << "\ta.create<" << dynamics.size() + statics.size() << ">();\n";
  if( statics[0]->hash != 0 )
    ofile << "\ta.insert( rtti_type(0), " << statics[0]->hash << " );\n";
  print_insert(ofile, i, statics, arity);
  ofile << "#endif" << std::endl;
  print_insert(ofile, i, dynamics, arity);
  ofile << "}" << std::endl;
}

void order_poles(
  pole_table_t& pole_table
, std::vector<hierarchy_t>& hierarchies
) {
  std::size_t const arity = pole_table.size();

  for(std::size_t i = 0; i < arity; ++i) {
    auto& t = pole_table[i];
    auto& h = hierarchies[i];

    t.reserve( h.size() );
    h.order(t);
  }
}

void print_poles(
  std::ostream& ofile
, arch_declaration const& decl
, pole_table_t& pole_table
) {
  std::size_t const arity = decl.vsize;

  for(std::size_t i : decl.argpos) {
    auto& t = pole_table[i];
    print_map(ofile, i, t, arity);
  }

  ofile << std::endl;
}

void print_initializer(
  std::ostream& ofile
, arch_declaration const& decl
) {
  for(std::size_t i : decl.argpos) {
    ofile <<
        "template<> template<> detail::poles_map_type register_base<MMETHOD>::poles<" << i << ">::array {\n"
        "#if MMETHOD_USE_SMALLARRAY\n"
        "\tTAG(__protectns)::_impl_smallarray" << i << "\n"
        "#endif\n"
        "};"
          << std::endl;
  }
  ofile << std::endl;

  if(decl.vsize > 1) {
    ofile << "template<> invoker_t const* const register_base<MMETHOD>::invoker_table = "
          << "TAG(__protectns)::_impl_invoker_table - TAG(__protectns)::MIN_HASH_VALUE;"
          << std::endl << std::endl;
  }

  ofile << "template<> void register_base<MMETHOD>::do_initialize() {" << std::endl;

  if(decl.vsize > 1)
    ofile << "\tTAG(__protectns)::_impl_inittable();" << std::endl;
  for(std::size_t i : decl.argpos)
    ofile << "\tTAG(__protectns)::_impl_assignarray" << i << "();" << std::endl;

  ofile << "}\n" << std::endl;
}
