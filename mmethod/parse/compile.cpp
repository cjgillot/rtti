#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "archive.hpp"
#include "archive.tpp"

using std::string;
typedef string const& strr;

void output_preabule(arch_declaration& decl, strr name, strr nspace, size_t arity, size_t fullarity, strr tags);
void output_overload(arch_declaration& decl, string& hashes);

void do_compile(std::istream& ifile, std::ostream& ofile)
{
  std::unordered_map<std::string, arch_declaration> registry;

  /// read compiler output
  for(std::string line; std::getline(ifile, line); )
  {
    /// parse error line
    static const boost::regex re { R"(.*\[with T = rtti::mmethod::mmethod_(\w+)<((\w+::)*)(\w+),\s*(\d+)ul\s*,\s*(\d+)ul\s*,\s*(.*)>\].*)" };
    boost::smatch result;
    if( !boost::regex_match(line, result, re) )
      continue;

    std::string
      predicate = result[1]
    , nspace    = result[2]
    , name      = result[4]
    , hashes    = result[7];
    std::size_t vsize = boost::lexical_cast<std::size_t>(result[5]);
    std::size_t fullarity = boost::lexical_cast<std::size_t>(result[6]);

    std::string key = "::" + nspace + name;

    auto& decl = registry[key];

    if( predicate == "declare" )
      output_preabule( decl, name, nspace, vsize, fullarity, hashes );
    else if( predicate == "implement" )
      output_overload( decl, hashes );
  }

  {
    oarchive_t arch { ofile, arch_flags };
    std::size_t sz = registry.size();
    arch << sz;
    for(auto& p : registry)
      arch << p.first << p.second;
  }
}
