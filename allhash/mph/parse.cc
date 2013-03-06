#include <string>
#include <fstream>
#include <unordered_map>

#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/algorithm/string.hpp>

#include "archive.hpp"

using std::string;
typedef string const& strr;

static void do_compile(std::istream&);

int main(int argc, char* argv[])
{
  if(argc == 1)
    do_compile(std::cin);
  else
  {
    std::ifstream ifile { argv[1], std::ios_base::in };
    do_compile(ifile);
  }
  return 0;
}

static void output_preabule(std::ostream& os, strr name, strr nspace, size_t arity, string& tags);
static void output_overload(std::ostream& os, string& hashes);

void do_compile(std::istream& ifile)
{
  std::vector<std::unique_ptr<std::ofstream>> folder;
  std::unordered_map<std::string, std::ofstream*> store;

  for(std::string line; std::getline(ifile, line); )
  {
    /// tee input
    std::cerr << line << std::endl;

    /// parse error line
    static const boost::regex re ( R"(^[^:]+:[^\[]+\[with T = rtti::mmethod::mmethod_(\w+)<((\w+::)*)__rtti_mmethod_tags__(\w+),\s*(\d+)ul\s*(.*)>\].*:)" );
    boost::smatch result;
    if( !boost::regex_match(line, result, re) )
      continue;

    std::string
      predicate = result[1]
    , nspace = result[2]
    , name = result[4]
    , hashes = result[6];
    size_t arity = boost::lexical_cast<size_t>(result[5]);

    std::string key = "::" + nspace + name;

    if( predicate == "declare" )
    {
      std::string filenm = nspace + name + ".mm";
      boost::replace_all(filenm, "::", "-");

      {
        folder.emplace_back(new std::ofstream( filenm, std::ios_base::out|std::ios_base::binary|std::ios_base::app ));
        store.insert(std::make_pair( key, folder.back().get() ));
      }
      std::ofstream& file = *folder.back().get();

      output_preabule( file, name, nspace, arity, hashes );
    }
    else if( predicate == "implement" )
    {
      std::ostream& file = *store.at(key);

      output_overload( file, hashes );
    }
  }

//   for(auto& p : store)
//   {
//     oarchive_t arch ( *p.second, arch_flags );
//     arch << ARCH_END;
//   }
}

template<typename Pred>
inline void erase_if(string& s, Pred p)
{
  boost::algorithm::find_format_all(
    s,
    boost::algorithm::token_finder(p),
    boost::algorithm::empty_formatter(s)
  );
}

void output_preabule(std::ostream& os, strr name, strr nspace, size_t arity, string& tags)
{
  {
  boost::replace_all(tags, "true", "1");
  boost::replace_all(tags, "false", "0");

  erase_if(tags, !boost::is_digit());
  }

  boost::char_separator<char> sep { ":" };
  boost::tokenizer<decltype(sep)> tok ( nspace, sep );

  arch_declaration decl = {
    name,
    { tok.begin(), tok.end() },
    arity,
    {}
  };

  for(size_t i = 0; i < tags.size(); ++i)
    if(tags[i] == '1')
      decl.argpos.push_back(i);

  oarchive_t arch ( os, arch_flags );
  arch << ARCH_DECL;
  arch << decl;
}

void output_overload(std::ostream& os, string& hashes)
{
  boost::erase_all(hashes, "rtti::mmethod::mmethod_hpath_end");
  boost::erase_all(hashes, "rtti::mmethod::mmethod_hpath");

  typedef boost::algorithm::split_iterator<string::const_iterator> split_it;

  std::vector<string> pathes;
  for(split_it en, it = split_it(hashes, boost::algorithm::first_finder(">, <"));
      it != en; ++it)
    pathes.emplace_back(it->begin(), it->end());

  arch_overload over { {} };

  for(string& path : pathes)
  {
    std::vector<std::size_t> vec;

    boost::char_separator<char> sep { ", <>ul" };
    boost::tokenizer<decltype(sep)> tok ( path, sep );

    for(std::string const& str : tok)
      vec.push_back(boost::lexical_cast<std::size_t>(str));

    over.hashes.push_back( std::move(vec) );
  }

  oarchive_t arch ( os, arch_flags );
  arch << ARCH_OVER;
  arch << over;
}
