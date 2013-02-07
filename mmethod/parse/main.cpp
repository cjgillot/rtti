#include <fstream>
#include <iostream>

#include <memory>

#include <vector>
#include <string>
#include <boost/program_options.hpp>

extern void do_compile(std::istream&, std::ostream&);

using std::string;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  string i_file, o_file;

  // Declare the supported options.
  po::options_description desc("Options");
  desc.add_options()
    ("help",      "Display this information")
    ("output,o",  po::value<string>(&o_file)->default_value("a.mmo"), "Output file")
    ("input",     po::value<string>(&i_file)->default_value("-")    , "Input file")
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

  std::ofstream output { o_file, std::ios_base::out | std::ios_base::binary };
  std::ifstream  input;

  std::istream* is;
  if( i_file == "-" ) {
    is = &std::cin;
  }
  else {
    input.open( i_file, std::ios_base::in );
    is = &input;
  }
  do_compile( *is, output );

  return 0;
}
