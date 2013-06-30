#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include <cstddef>
#include <vector>
#include <string>
#include <list>

typedef std::vector<std::size_t> hashvec_t;

struct arch_overload {
  std::vector<hashvec_t> hashes;
};

struct arch_declaration {
  std::string name;
  std::vector<std::string> nspath; // excludes function name
  std::size_t vsize, fullarity;
  std::vector<std::size_t> argpos;
  std::list<arch_overload> overloads;
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
