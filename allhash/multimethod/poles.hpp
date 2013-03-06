#ifndef RTTI_MMETHOD_POLES_HPP
#define RTTI_MMETHOD_POLES_HPP

#include "rtti_allhash/rttifwd.hpp"
#include "util/attribute.hpp"
#include "util/assert.hpp"

#include <memory>
#include <limits>

#include "rtti_allhash/multimethod/pole_map.hpp"

// All functions declared in this file
// are defined in rtti/multimethod/poles.cpp

namespace rtti {
namespace mmethod {
namespace detail {

typedef void(*invoker_t)();
static_assert(sizeof(std::size_t) == sizeof(invoker_t), "Error");

#if 0
inline constexpr std::size_t
next_2exp(std::size_t n)
{
  return (n == 0)
    ? 1
    : (next_2exp(n >> 1) << 1)
    ;
}

class map_type {
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

private:
  std::size_t m_bucket_count = 0;
  std::size_t mask = 0;
  bucket_t badbucket {};
  bucket_t* m_array = nullptr;

public:
  constexpr map_type() {}
  ~map_type();

  iterator find(std::size_t key) const;

  void create(std::size_t sz);
  void insert(std::size_t key, std::size_t value);
  void assign(std::initializer_list<std::pair<std::size_t, std::size_t>> const& array);

  void insert_at(iterator it, std::size_t key, std::size_t value);

private:
  void insert_need_resize(std::size_t index, std::size_t value);
};
#endif

//! \brief Invoker retrieval function
extern std::size_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2)
fetch_pole(
  map_type_base& map
, const ::rtti::detail::rtti_node* rt
) throw();

}}} // namespace rtti::mmethod::detail

#endif
