#include "archive.hpp"

#include <fstream>

#include <vector>
#include <unordered_map>

#include <boost/program_options.hpp>

using std::istream; using std::ostream;
using std::string;  using std::vector;
namespace po = boost::program_options;

typedef std::unordered_map<string, arch_declaration> registry_t;

static void process_file (istream& ifile, registry_t& registry);
static void archive_file (ostream& ofile, registry_t& registry);
static void generate_file(ostream& ofile, registry_t& registry);

int main(int argc, char* argv[])
{
  string o_file; vector<string> i_file;

  // Declare the supported options.
  po::options_description desc("Options");
  desc.add_options()
    ("help,h",    "Display this information")
    ("link,l",    "Archive the inputs as one output file")
    ("output,o",  po::value<       string >(&o_file)->default_value("a.cc"), "Output file")
    ("input",     po::value<vector<string>>(&i_file)                       ,  "Input file")
  ;
  po::positional_options_description p;
  p.add("input", -1);

  po::variables_map vm;
  po::store(
    po::command_line_parser(argc, argv)
      .options(desc)
      .positional(p)
      .run()
  , vm
  );
  po::notify(vm);

  if( vm.count("help") ) {
    std::cout << desc << std::endl;
    return 1;
  }

  std::unordered_map<string, arch_declaration> registry;

  for(string const& s: i_file) {
    std::ifstream ifile { s, std::ios_base::in|std::ios_base::binary };
    process_file(ifile, registry);
  }

  std::ofstream ofile { o_file, std::ios_base::out|std::ios_base::binary };

  if( vm.count("link") )
    archive_file(ofile, registry);
  else
    generate_file(ofile, registry);

  return 0;
}

void process_file(istream& ifile, registry_t& registry) {
  for(;;) {
    iarchive_t arch ( ifile, arch_flags );

    std::size_t sz; arch >> sz;

    std::string key; arch_declaration decl;

    for(; sz > 0; --sz) {
      arch >> key >> decl;

      auto it = registry.find(key);
      if(it != registry.end()) {
        auto& overlist = it->second.overloads;
        overlist.splice( overlist.begin(), decl.overloads );
      }
      else {
        registry.emplace( key, std::move(decl) );
      }
    }
  }
}

void archive_file(ostream& ofile, registry_t& registry) {
  oarchive_t arch ( ofile, arch_flags );
  std::size_t sz = registry.size();
  arch << sz;
  for(auto& p : registry) {
    arch << p.first << p.second;
  }
}

void process_declaration(std::ostream& ofile, arch_declaration const& decl);
void generate_file(ostream& ofile, registry_t& registry) {
  static const char includes [] =
    "#define MMETHOD_IN_MMETHOD\n"
    "#include \"rtti/mmethod/common.hpp\"\n\n"

    "using ::rtti::rtti_node;\n"
    "using ::rtti::invoker_t;\n"
    "using ::rtti::mmethod::detail::pole_t;\n\n"

    "#define __DO_CAT(a,b) a##b\n"
    "#define __CAT(a,b) __DO_CAT(a,b)\n"
    "#define TAG(a) __CAT(TAGNAME, a)\n\n"
  ;

  ofile << includes << std::endl;
  for(auto& p : registry)
    process_declaration(ofile, p.second);
}
