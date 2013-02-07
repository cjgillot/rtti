#include <vector>

#include "rtti/hash.hpp"

#include "hierarchy.hpp"
#include "overloads.hpp"

#include "archive.hpp"

// from dispatch.cpp
extern void dispatch(
  const overloads_t& overloads,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
);

// from gen_mph.cpp/gperf.cc
extern void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch,
  const arch_declaration& decl,
  std::ostream& f
);

// from order_poles.cpp
extern void order_poles(
  pole_table_t& pole_table,
  std::vector<hierarchy_t>& hierarchies
);
extern void print_poles(
  std::ostream&,
  arch_declaration const& decl,
  pole_table_t& pole_table
);
extern void print_initializer(
  std::ostream& ofile
, arch_declaration const& decl
);

static void print_preamble(std::ostream&, arch_declaration const& decl);
static void print_epilogue(std::ostream&, arch_declaration const& decl);

void process_declaration(std::ostream& ofile, arch_declaration const& decl)
{
  std::size_t arity = decl.fullarity;

  /// print preamble
  print_preamble(ofile, decl);

  /// hierarchy stuff
  std::vector<hierarchy_t> hierarchies ( arity );

  /// parsing input
  overloads_t overloads { arity };
  for(arch_overload const& over : decl.overloads) {
    // for static types, this just inputs { 0 }
    for(std::size_t i = 0; i < over.hashes.size(); ++i)
      overloads.get_back(i) = hierarchies[i].add( over.hashes[i] );

    overloads.save();
  }
  overloads.finish();

  /// shrink hierarchies
  for(auto& hier : hierarchies)
    hier.shrink();

  /// order poles
  pole_table_t pole_table { arity };
  order_poles(pole_table, hierarchies);

  /// order methods
  overloads.sort();

  /// fill up dispatch table
  dispatch_t dispatch_table;
  dispatch(overloads, pole_table, dispatch_table);

  /// prepare poles for output : link each pole to a signature in which it appears
  /// \warning This code must be after any change to \c overloads
  for(const signature_t& sig : overloads.array())
    for(const klass_t* k : sig.array())
      const_cast<signature_t const*&>( k->sig ) = &sig;

  ofile << "namespace { namespace TAG(__protectns) {" << std::endl << std::endl;

  /// create minimal perfect hash function
  gen_mph(pole_table, dispatch_table, decl, ofile);

  /// print pole tables
  print_poles(ofile, decl, pole_table);

  ofile << "}} // <anonymous>::TAG(__protectns)" << std::endl << std::endl;

  /// print initialization code
  print_initializer(ofile, decl);

  print_epilogue(ofile, decl);
}

void print_preamble(std::ostream& ofile, arch_declaration const& decl)
{
  static const char structure [] =
    "struct STRUCTNAME {\n"
    "\ttemplate<typename> struct overload {\n"
    "\t\ttemplate<std::size_t> struct poles {\n"
    "\t\t\tstatic rtti_node const* node;\n"
    "\t\t};\n"
      // mmethod/implement.hpp ensures this is statically initialized
    "\t\tstatic const invoker_t address;\n"
    "\t};\n"
    "};\n"
  ;

  /// debugging comment
  ofile << "/****** COMPILING FOR MMETHOD : " << decl.name << " ******/" << std::endl;

  /// arity
  ofile << "#define ARITY " << decl.vsize << std::endl;

  /// namespace-qualified mmethod name
  ofile << "#define MMETHOD /**/";
  for(std::string const& ns : decl.nspath)
    ofile << "::" << ns;
  ofile << "::" << decl.name << std::endl;
  ofile << std::endl;

  /// create namespace declarations for structure
  for(std::string const& ns : decl.nspath)
    ofile << "namespace " << ns << " { ";

  /// mmethod structure name
  ofile << "#define STRUCTNAME " << decl.name << std::endl;
  ofile << structure << std::endl;
  ofile << "#undef STRUCTNAME" << std::endl;

  /// close namespace
  for(std::size_t count = decl.nspath.size(); count; --count)
    ofile << "}";
  ofile << std::endl << std::endl;

  /// unique name for structure
  ofile << "#define TAGNAME __MMTAG_";
  for(std::string const& ns : decl.nspath)
    ofile << "_N" << ns.size() << ns;
  ofile << "_S" << decl.name.size() << decl.name << std::endl;
  ofile << std::endl;

  ofile << "namespace rtti { namespace mmethod {\n" << std::endl;
}

void print_epilogue(std::ostream& ofile, arch_declaration const& /*decl*/)
{
  ofile << "}}\n" << std::endl; // namespace rtti::mmethod

  ofile << "#undef TAGNAME\n#undef ARITY\n#undef MMETHOD\n" << std::endl;
}
