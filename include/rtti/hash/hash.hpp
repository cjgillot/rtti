#ifndef RTTI_HASH_HPP
#define RTTI_HASH_HPP

#include <cstdlib>
#include <limits>

#include <boost/preprocessor/stringize.hpp>

#include "rtti/holder/getter.hpp"

// this file is highly difficult to read : some readability macros -> [ if(...) then ... else ...  ] syntax
#define if /* */
#define then ?
#define else :

namespace rtti {
namespace hash {
namespace {

template<std::size_t N>
constexpr std::size_t load(const char (&str)[N], std::size_t i, std::size_t len)
{
  return
    if(len <= 0)
    then 0
    else if(len == 1)
    then str[i]
    else
#define hlen (len/2)
#define half_bits (len/2 * std::numeric_limits<char>::digits)
      (load(str, i, len - hlen) << half_bits) | load(str, i + hlen, hlen);
#undef half_bits
#undef hlen
}

template<std::size_t SZ>
struct hash_string_fnv
{ static const std::size_t m = 131; };

template<>
struct hash_string_fnv<4>
{ static const std::size_t m = 16777619UL; };

template<>
struct hash_string_fnv<8>
{ static const size_t m = 1099511628211ULL; };

template<std::size_t SZ>
struct hash_string_base
{
private:
  template<std::size_t N>
  static constexpr std::size_t hash_loop(const char (&str)[N], std::size_t i, std::size_t seed)
  {
    return
      if(i == N)
      then seed
      else
#define step ((seed * hash_string_fnv<SZ>::m) + str[i])
        hash_loop(str, i+1, step);
#undef step
  }

public:
  template<std::size_t N>
  static constexpr std::size_t hash(const char (&str)[N], std::size_t seed)
  {
    return hash_loop(str, 0, seed);
  }

  template<std::size_t N>
  static constexpr std::size_t fnv_hash(const char (&str)[N], std::size_t seed)
  { return hash(str, seed); }
};

template<std::size_t SZ>
struct hash_string
: public hash_string_base<SZ>
{};

template<>
struct hash_string<4>
{
private:
  static const std::size_t m = 0x5bd1e995;

  static constexpr std::size_t mix(std::size_t k, std::size_t h)
  { return (hrot(k * m, 24) * m) ^ (h * m); }

  static constexpr std::size_t hrot(std::size_t h, std::size_t n)
  { return h ^ (h >> n); }

  static constexpr std::size_t final(std::size_t h)
  { return hrot(hrot(h, 13) * m, 15); }

  template<std::size_t N>
  static constexpr std::size_t hash_loop(const char (&str)[N], std::size_t i, std::size_t h0)
  {
    return
      if(i == N)
      then final(h0)
      else if(i+4 > N)
      then hash_loop(str, N, (h0 ^ load(str, i, N-i)) * m)
      else hash_loop(str, i+4, mix(load(str, i, 4), h0));
  }

public:
  template<std::size_t N>
  static constexpr std::size_t hash(const char (&str)[N], std::size_t h0)
  {
    return hash_loop(str, 0, h0);
  }

  template<std::size_t N>
  static constexpr std::size_t fnv_hash(const char (&str)[N], std::size_t h0)
  { return hash_string_base<4>::fnv_hash(str, h0); }
};

template<>
struct hash_string<8>
{
private:
  static const std::size_t m = 0xc6a4a7935bd1e995ULL;

  static constexpr std::size_t mix(std::size_t k, std::size_t h)
  { return (hrot(k * m) * m) ^ (h * m); }

  static constexpr std::size_t hrot(std::size_t h, std::size_t n = 47)
  { return h ^ (h >> n); }

  static constexpr std::size_t final(std::size_t h)
  { return hrot(hrot(h, 13) * m, 15); }

  template<std::size_t N>
  static constexpr std::size_t hash_loop(const char (&str)[N], std::size_t i, std::size_t h0)
  {
    return
      if(i == N)
      then final(h0)
      else if(i+8 > N)
      then hash_loop(str, N, (h0 ^ load(str, i, N-i)) * m)
      else hash_loop(str, i+8, mix(load(str, i, 8), h0));
  }

public:
  template<std::size_t N>
  static constexpr std::size_t hash(const char (&str)[N], std::size_t h0)
  {
    return hash_loop(str, 0, h0 ^(N * m));
  }

  template<std::size_t N>
  static constexpr std::size_t fnv_hash(const char (&str)[N], std::size_t h0)
  { return hash_string_base<8>::fnv_hash(str, h0); }
};

} // anonymous namespace

struct hash_type
{
private:
  typedef hash_string<sizeof(std::size_t)> hash_type_base;

public:
  template<unsigned N>
  static constexpr std::size_t hash(const char(&t)[N], bool stat, unsigned char id)
  {
    return
      if(stat /*& (id < 0x100)*/)
      then id
      else
        // static id is retrieved from hash by testing whether <= CHAR_MAX
          hash_type_base::hash( t, 0 )
        | 0x100;
  }

  static constexpr bool hash_is_id(std::size_t h)
  { return h <= 0xff; }
};

#undef if
#undef then
#undef else

}} // namespace rtti::hash

#define __RTTI_HASH_TYPE( klass, static, id ) \
  (::rtti::hash::hash_type::hash( BOOST_PP_STRINGIZE(klass), static, id ))

#define RTTI_HASH_IS_ID(Hash) (::rtti::hash::hash_type::hash_is_id(Hash))

#endif
