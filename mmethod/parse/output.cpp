#include <string>

#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/algorithm/string.hpp>

#include "archive.hpp"

using std::string;
typedef string const& strr;

template<typename Pred>
extern inline void erase_if(string& s, Pred p)
{
  boost::algorithm::find_format_all(
    s,
    boost::algorithm::token_finder(p),
    boost::algorithm::empty_formatter(s)
  );
}

void output_preabule(arch_declaration& decl, strr name, strr nspace,
                     std::size_t vsize, std::size_t fullarity, strr tags)
{
  static const boost::regex re { R"(\s*(\d+)ul\s*)" };
  boost::smatch result;
  boost::regex_match(tags, result, re);

  std::size_t tagging = boost::lexical_cast<std::size_t>( result[1] );

  boost::char_separator<char> sep { ":" };
  boost::tokenizer<decltype(sep)> tok { nspace, sep };

  decl.name = name;
  decl.nspath.assign( tok.begin(), tok.end() );
  decl.vsize = vsize;
  decl.fullarity = fullarity;

  for(std::size_t i = 0; i < tags.size(); ++i)
    if(tagging & (1 << i))
      decl.argpos.push_back(i);
}

void output_overload(arch_declaration& decl, string& hashes)
{
  boost::erase_all(hashes, "rtti::mpl::mplpack");
  boost::erase_all(hashes, "rtti::hash::path_end");
  boost::erase_all(hashes, "rtti::hash::path_node");

  typedef boost::algorithm::split_iterator<string::const_iterator> split_it;

  std::vector<string> pathes;
  for(split_it en, it = split_it(hashes, boost::algorithm::first_finder(">, <"));
      it != en; ++it)
    pathes.emplace_back(it->begin(), it->end());

  decl.overloads.emplace_back();
  arch_overload& over = decl.overloads.back();

  for(string& path : pathes)
  {
    std::vector<std::size_t> vec;

    boost::char_separator<char> sep { ", <>ul" };
    boost::tokenizer<decltype(sep)> tok ( path, sep );

    for(std::string const& str : tok)
      vec.push_back(boost::lexical_cast<std::size_t>(str));

    over.hashes.push_back( std::move(vec) );
  }
}
