
#include <ostream>
#include <vector>

#include "hierarchy.hpp"
#include "overloads.hpp"

#include "../dynamic/early.hpp"

#define USE_SMALLARRAY 0


void order_poles(
  pole_table_t& pole_table
, std::vector<hierarchy_t>& hierarchies
) {
  std::size_t const arity = pole_table.size();

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::reference t = pole_table[i];
    hierarchy_t& h = hierarchies[i];

    t.reserve( h.size() );
    h.order(t);
  }
}


#if 0

void print_initializer(
  std::ostream& ofile
, arch_declaration const& decl
) {
  for(std::size_t i : decl.argpos) {
    ofile <<
        "template<> template<> detail::poles_map_type register_base<MMETHOD>::poles<" << i << ">::array {\n"
#if USE_SMALLARRAY
        "#if MMETHOD_USE_SMALLARRAY\n"
        "\tTAG(__protectns)::_impl_smallarray" << i << "\n"
        "#endif\n"
#endif
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
#endif

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

struct print_insert {
  poles_map_type& a;
  std::size_t i;
  std::size_t arity;

  void operator()(klass_t const* k) {
    // insert expects 2-aligned values
    if(arity == 1) {
      // assert code is 2-aligned
      BOOST_ASSERT( k->rankhash & 1 == 0 );
      a.insert(k->hash, k->rankhash);
    }
    else
      a.insert(k->hash, 2 * k->rankhash);
  }
};

template<typename Seq>
static void print_map(seal_table_type& output, std::size_t i, Seq const& t, std::size_t arity) {
  /// split \c t between static and dynamic id
  std::vector<klass_t const*> dynamics ( boost::begin(t), boost::end(t) );

  poles_map_type& a = *output.poles[i];

  a.create( dynamics.size() );

  print_insert ins = { a, i, arity };
  std::for_each(dynamics.begin(), dynamics.end(), ins);
}

void print_poles(
  seal_table_type& output
, early_bindings_type const& decl
, pole_table_t& pole_table
) {
  std::size_t const arity = decl.arity;

  for(std::size_t i = 0; i < arity; ++i) {
    pole_table_t::reference t = pole_table[i];
    print_map(output, i, t, arity);
  }
}
