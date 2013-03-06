#ifndef POLE_MAP_HPP
#define POLE_MAP_HPP

#include "rtti_allhash/rttifwd.hpp"
#include "util/attribute.hpp"
#include "util/assert.hpp"

#include <array>
#include <memory>
#include <limits>

// All functions declared in this file
// are defined in rtti/multimethod/poles.cpp

namespace rtti {
namespace mmethod {
namespace detail {

typedef void(*invoker_t)();
static_assert(sizeof(std::size_t) == sizeof(invoker_t), "Error");

inline constexpr std::size_t
next_2exp(std::size_t n)
{
  return (n == 0)
    ? 1
    : (next_2exp(n >> 1) << 1)
    ;
}

class map_type_base {
public:
  struct bucket_t {
    enum { key_size = std::numeric_limits<std::size_t>::digits - 1 };
    bool empty : 1;
    std::size_t key : key_size;
    std::size_t value;

    constexpr bucket_t()
    : empty(1), key(0), value(0) {}
    constexpr bucket_t(bool e, std::size_t k, std::size_t v)
    : empty(e), key(k), value(v) {}
  };

  typedef bucket_t* iterator;
  typedef std::pair<std::size_t, std::size_t> pair_type;
  typedef std::initializer_list<bucket_t> initializer_list_type;

protected:
  std::size_t m_smallcount;
  bucket_t* m_smallarray;

  std::size_t m_bucket_count = 0;
  bucket_t* m_array = nullptr;

  std::size_t mask = 0;
  bucket_t badbucket {};

public:
  template<std::size_t N>
  constexpr map_type_base(bucket_t(&small)[N])
  : m_smallcount(N), m_smallarray(small) {}
  ~map_type_base();

public:
  // initialization by generated code
  void assign(initializer_list_type const& array);

  // use by fetch_pole
  iterator find(std::size_t key) const;
  void insert_at(iterator it, std::size_t key, std::size_t value);

private:
  void create(std::size_t sz);
  void insert(std::size_t key, std::size_t value);
  void insert_need_resize(std::size_t index, std::size_t value);
};

template<std::size_t N>
class map_type {
/*
public:
  using map_type_base::iterator;
  using map_type_base::pair_type;
  using map_type_base::initializer_list_type;

  bucket_t m_small [ N ];

public:
  template<typename... Types>
  constexpr map_type(Types&& ...init) noexcept
  : m_small{std::forward<Types>(init)...} {}
  ~map_type() noexcept {}
*/
};

}}} // namespace rtti::mmethod::detail

#endif
