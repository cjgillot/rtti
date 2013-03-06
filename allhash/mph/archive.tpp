#ifndef ARCHIVE_TPP
#define ARCHIVE_TPP

#include "archive.hpp"

#include <boost/serialization/vector.hpp>

template<typename Archive>
inline void serialize(Archive& ar, arch_overload& over, const unsigned)
{
  ar & over.hashes;
}

template<typename Archive>
inline void serialize(Archive& ar, arch_declaration& decl, const unsigned)
{
  ar & decl.name;
  ar & decl.nspath;
  ar & decl.arity;
  ar & decl.argpos;
}

template<typename Archive>
inline void serialize(Archive& ar, arch_type& type, const unsigned)
{
  unsigned char t = (unsigned char)type;
  ar & t;
  type = (arch_type)t;
}
template<typename Archive>
inline Archive& operator<<(Archive& ar, arch_type t)
{ return ar.operator<<(t); }

#endif
