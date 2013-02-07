#ifndef ARCHIVE_TPP
#define ARCHIVE_TPP

#include "archive.hpp"

#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>

template<typename Archive>
inline void serialize(Archive& ar, arch_declaration& decl, const unsigned)
{
  ar & decl.name;
  ar & decl.nspath;
  ar & decl.vsize;
  ar & decl.fullarity;
  ar & decl.argpos;
  ar & decl.overloads;
}

template<typename Archive>
inline void serialize(Archive& ar, arch_overload& over, const unsigned)
{
  ar & over.hashes;
}

#endif
