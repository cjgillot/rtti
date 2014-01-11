#include <cstdint>

#include "rtti/rttifwd.hpp"

#include "mmethod/shared/basic.hpp"
#include "mmethod/hash/path.hpp"

namespace rtti {
namespace MMETHOD_NSPACE {

template<typename TAG, size_t ARITY, size_t FULLARITY, size_t TAGS>
struct method_declare { enum { sz = 5 + TAGS }; };

template<typename TAG, size_t ARITY, size_t FULLARITY, typename HASHES>
struct method_implement { enum { sz = 5 + sizeof(HASHES) }; };

using hash::make_path;

}} // namespace rtti::mmethod
