#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include <cstddef>
#include <vector>
#include <string>

struct arch_declaration {
  std::string name;
  std::vector<std::string> nspath; // excludes function name
  std::size_t arity;
  std::vector<std::size_t> argpos;
};

struct arch_overload {
  std::vector<std::vector<std::size_t>> hashes;
};

enum arch_type : unsigned char {
//   ARCH_END,
  ARCH_DECL,
  ARCH_OVER
};

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

typedef boost::archive::binary_iarchive iarchive_t;
typedef boost::archive::binary_oarchive oarchive_t;

constexpr boost::archive::archive_flags arch_flags = boost::archive::no_header;

#endif

#include "archive.tpp"
