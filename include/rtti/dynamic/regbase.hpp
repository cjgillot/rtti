#ifndef RTTI_DMETHOD_REGBASE_HPP
#define RTTI_DMETHOD_REGBASE_HPP

#include <cstdint>

#include "rtti/rttifwd.hpp"

#include "rtti/holder/getter.hpp"
#include "rtti/holder/holder.hpp"

#include "rtti/shared/basic.hpp"

#include "rtti/dynamic/dispatch.hpp"

#include <boost/mpl/size_t.hpp>

#include "rtti/hash/path.hpp"

namespace rtti {
namespace dmethod {

template<typename TAG, size_t ARITY, size_t FULLARITY, size_t TAGS>
struct mmethod_declare { enum { sz = 5 + TAGS }; };

template<typename TAG, size_t ARITY, size_t FULLARITY, typename HASHES>
struct mmethod_implement { enum { sz = 5 + sizeof(HASHES) }; };

using hash::make_path;

}} // namespace rtti::mmethod

#endif
