//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../classes.hpp"
#include <boost/test/unit_test.hpp>

#include "mmethod/smart_ptr/boost_shared_ptr.hpp"
#define PTR(T)    boost::shared_ptr<T>
#define TESTNAME  test_smp_boost_shared_ptr
#define CALL(f,x) f(x)
#include "generic.hpp"
#undef TESTNAME
#undef CALL
#undef PTR

#include "mmethod/smart_ptr/boost_unique_ptr.hpp"
#define PTR(T) boost::movelib::unique_ptr<T>
#define TESTNAME test_smp_boost_unique_ptr
#define CALL(f,x) f(boost::move(x))
#include "generic.hpp"
#undef TESTNAME
#undef CALL
#undef PTR

#ifndef BOOST_NO_CXX11_SMART_PTR

#include "mmethod/smart_ptr/stl_shared_ptr.hpp"
#define PTR(T) std::shared_ptr<T>
#define TESTNAME test_smp_stl_shared_ptr
#define CALL(f,x) f(x)
#include "generic.hpp"
#undef TESTNAME
#undef CALL
#undef PTR

#include "mmethod/smart_ptr/stl_unique_ptr.hpp"
#define PTR(T) std::unique_ptr<T>
#define TESTNAME test_smp_stl_unique_ptr
#define CALL(f,x) f(std::move(x))
#include "generic.hpp"
#undef TESTNAME
#undef CALL
#undef PTR

#endif
