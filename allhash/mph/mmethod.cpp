#include <iostream>
#include <fstream>
#include <limits>

#include <set>
#include <vector>
#include <sstream>

#include "rtti/hash.hpp"
#include "rtti/mph/hierarchy.hpp"
#include "rtti/mph/overloads.hpp"

#include <boost/dynamic_bitset.hpp>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

#include "archive.hpp"

// from dispatch.cpp
extern void dispatch(
  const overloads_t& overloads,
  const pole_table_t &pole_table,
  dispatch_t &dispatch
);

// from gen_mph.cpp
extern void gen_mph(
  const pole_table_t& pole_table,
  const dispatch_t& dispatch, size_t arity,
  std::ostream& f
);

static const char function [] =
  "invoker_t TAGNAME::fetch(size_t* spec) {\n"
  "\tsize_t index = hash((unsigned char*)spec);\n"
  "\treturn invoker_table[index-MIN_HASH_VALUE];\n"
  "}\n"
;


static void print_preamble(std::ostream&, arch_declaration const& decl);
static void order_poles(std::ostream&, arch_declaration const& decl, pole_table_t& pole_table, std::vector<hierarchy_t>& hierarchies);

static void process(std::string fn, std::string fo)
{
  std::ifstream ifile { fn, std::ios_base::in|std::ios_base::binary };
  std::ofstream ofile { fo.c_str(), std::ios_base::out };

  /// get file header
  arch_declaration decl;
  {
  iarchive_t arch ( ifile, arch_flags );
  arch_type type;
  arch >> type; assert(type == ARCH_DECL);
  arch >> decl;
  }

  std::size_t arity = decl.arity;

  /// print preamble
  print_preamble(ofile, decl);

  /// hierarchy stuff
  std::vector<hierarchy_t> hierarchies ( arity );

  /// parsing input
  overloads_t overloads { arity };
  for(;;)
  {
    arch_overload over;

    try {
      iarchive_t arch ( ifile, boost::archive::no_header );
      arch_type type;
      arch >> type;

      if(type == ARCH_DECL)
      {
        arch_declaration tmp;
        arch >> tmp;
        continue;
      }

      arch >> over;
    }
    catch(boost::archive::archive_exception& exn) {
      break;
    }

    std::size_t index = 0;
    for(auto& vec : over.hashes)
    {
      overloads.get_back(index) = hierarchies[index].add(vec);
      ++index;
    }

    overloads.save();
  }
  overloads.finish();

  /// shrink hierarchies
  for(auto& hier : hierarchies)
    hier.shrink();

  /// prepare poles : link each pole to a signature in which it appears
  for(const signature_t& sig : overloads.array())
    for(const klass_t* k : sig.array())
      const_cast<klass_t*>(k)->sig = &sig;

  /// order poles
  pole_table_t pole_table { arity };
  order_poles(ofile, decl, pole_table, hierarchies);

  /// order methods
  overloads.sort();

  /// fill up dispatch table
  dispatch_t dispatch_table;
  dispatch(overloads, pole_table, dispatch_table);

  /// create minimal perfect hash function
  gen_mph(pole_table, dispatch_table, arity, ofile);

  /// output main dispatch function
  if(arity != 1)
    ofile << function << std::endl;
}

int main(int argc, char* argv[])
{
  if(argc < 3) return 1;
  process(argv[1], argv[2]);
  return 0;
}

void print_preamble(std::ostream& ofile, arch_declaration const& decl)
{
  static const char includes [] =
    "#include <stddef.h>\n"
    "#include \"rtti_allhash/multimethod/poles.hpp\"\n"
    "typedef void(*invoker_t)();\n"
    "using ::rtti::rtti_node;\n"
    "using ::rtti::mmethod::detail::map_type_base;\n"
    "using bucket_t = ::rtti::mmethod::detail::map_type_base::bucket_t;\n"
    "template<size_t N> using map_type = ::rtti::mmethod::detail::map_type<N>;\n"
  ;

  static const char structure [] =
    "struct TAGNAME {\n"
    "\ttemplate<size_t...> struct overload {\n"
    "\t\ttemplate<size_t> struct poles {\n"
    "\t\t\tstatic rtti_node& node;\n"
    "\t\t};\n"
    "\t\tstatic const invoker_t address;\n"
    "\t};\n"
    "\ttemplate<size_t> struct poles {\n"
    "\t\tstatic map_type_base map;\n"
    "\t};\n"
    "\tstatic void do_initialize();\n"
    "\tstatic invoker_t fetch(size_t spec[ARITY]);\n"
    "};\n"
  ;

  ofile << includes << std::endl;

  /// arity
  ofile << "#define ARITY " << decl.arity << std::endl;

  /// mmethod structure name
  ofile << "#define TAGNAME __rtti_mmethod_tags__" << decl.name << std::endl;

  /// namespace-qualified mmethod name
  ofile << "#define MMETHOD ";
  for(std::string const& ns : decl.nspath)
    ofile << "::" << ns;
  ofile << "::TAGNAME" << std::endl;
  ofile << std::endl;;

  /// create namespace declarations for structure
  for(std::string const& ns : decl.nspath)
    ofile << "namespace " << ns << " { ";
  ofile << std::endl;

  ofile << structure << std::endl;

  for(size_t count = decl.nspath.size(); count; --count)
    ofile << "}";
  ofile << std::endl << std::endl;

  ofile << "using MMETHOD;\n" << std::endl;
}

void order_poles(std::ostream& ofile, arch_declaration const& decl, pole_table_t& pole_table, std::vector<hierarchy_t>& hierarchies)
{
  std::size_t const arity = pole_table.size();

  for(std::size_t i = 0; i < arity; ++i)
  {
    auto& t = pole_table[i]; auto& h = hierarchies[i];

    t.reserve( h.size() );
    h.order(t);

    // name for the statically initialized array
    std::string tempname; {
      std::ostringstream stream;
      stream << "__map_temp_" << i;
      tempname = std::move( stream.str() );
    }

    // add hashes base first -> forward iterators
    ofile << "static bucket_t " << tempname << " [] = {" << std::endl;
    std::size_t current = 0;
    for(const klass_t* k : t)
    {
      if( !RTTI_HASH_IS_ID(k->hash) )
        continue;

      // fill empty slots
      for(; current < k->hash; ++current)
        ofile << "\t\t{ true,  " << current << ", 0 },\n";

      ofile << "\t\t{ false, ";

      // part 1 : hash
      ofile << k->hash << ", ";

      // part2 : rank
      if(arity == 1)
      {
        // special case : input invoker directly
        ofile << "(std::size_t)TAGNAME::overload<0";
        for(const klass_t* k2 : k->sig->array())
          ofile << ", " << k2->hash << "ul";
        ofile << ">::address";
      }
      else
      {
        ofile << k->rank;
      }
      ofile << " }," << std::endl;

      ++current;
    }
    ofile << "\t};" << std::endl;

    ofile
      << "template<> map_type_base TAGNAME::poles<"
        << decl.argpos[i]
      << ">::map { "
      << tempname
      << " };" << std::endl << std::endl;
  }

  ofile << "void TAGNAME::do_initialize() {\n";

  for(std::size_t i = 0; i < arity; ++i)
  {
    auto& t = pole_table[i];

    // add hashes base first -> forward iterators
    ofile << "\tTAGNAME::poles<" << decl.argpos[i] << ">::map.assign( {" << std::endl;
    for(const klass_t* k : t)
    {
      if( RTTI_HASH_IS_ID(k->hash) )
        continue;

      ofile << "\t\tbucket_t{ ";
      // part 1 : hash
      ofile << k->hash;

      ofile << ", ";
      // part2 : rank
      if(arity == 1)
      {
        // special case : input invoker directly
        ofile << "(std::size_t)TAGNAME::overload<0";
        for(const klass_t* k2 : k->sig->array())
          ofile << ", " << k2->hash << "ul";
        ofile << ">::address";
      }
      else
      {
        ofile << k->rank;
      }
      ofile << " }," << std::endl;
    }
    ofile << "\t} );" << std::endl << std::endl;
  }

  ofile << "}\n" << std::endl;
}
