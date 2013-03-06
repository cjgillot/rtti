#include "rtti_allhash/multimethod/register.hpp"

#include "rtti_allhash/multimethod/print.hpp"
#include <rtti_allhash/hash.hpp>

#include <rtti_allhash/macro.hpp>

#include <time.h>
#include <iostream>

struct foo       { DECLARE_RTTI(foo, 100); public: virtual void func() {} virtual ~foo() {} };
struct bar : foo { STATIC_RTTI(bar, foo, 1); public: };

#define DECL(name, args) DECLARE_MMETHOD((void), name, args)
#define IMPL(name, args) IMPLEMENT_MMETHOD((void), name, args)

DECL(vf1, (virtual(foo&)()))
IMPL(vf1, (virtual(foo&)())) {}

DECL(vf2, (virtual(foo&)())(virtual(foo&)()))
IMPL(vf2, (virtual(foo&)())(virtual(foo&)())) {}
IMPL(vf2, (virtual(bar&)())(virtual(foo&)())) {}

DECL(vf3, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)()))
IMPL(vf3, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)())) {}

DECL(vf4, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)())(virtual(foo&)()))
IMPL(vf4, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)())(virtual(foo&)())) {}

#define N 300000000

void
test( foo& f )
{
        std::clog<<"starting benchmark" << N << std::endl;
        clock_t c1, c2;
        double t1, t2;

        std::clog<<"calling virtual function 600M times... ";
        c1 = clock();
        for ( int i = 0; i < N; ++i ) f.func();
        c2 = clock();
        t1 = double( c2 - c1 ) / CLOCKS_PER_SEC;
        std::clog<<t1<<" secs\n";

        std::clog<<"calling function with one virtual arg 600M times... ";
        c1 = clock();
        for ( int i = 0; i < N; ++i ) vf1( f );
        c2 = clock();
        t2 = double( c2 - c1 ) / CLOCKS_PER_SEC;
        std::clog<<t2<<" secs\nratio: "<<( t2 / t1 )<<std::endl;

        std::clog<<"calling function with two virtual args 300M times... ";
        c1 = clock();
        for ( int i = 0; i < N/2; ++i ) vf2( f, f );
        c2 = clock();
        t2 = double( c2 - c1 ) / CLOCKS_PER_SEC;
        std::clog<<t2<<" secs\nratio: "<<( t2 / t1 )<<std::endl;

        std::clog<<"calling function with three virtual args 200M times... ";
        c1 = clock();
        for ( int i = 0; i < N/3; ++i ) vf3( f, f, f );
        c2 = clock();
        t2 = double( c2 - c1 ) / CLOCKS_PER_SEC;
        std::clog<<t2<<" secs\nratio: "<<( t2 / t1 )<<std::endl;

        std::clog<<"calling function with four virtual args 150M times... ";
        c1 = clock();
        for ( int i = 0; i < N/4; ++i ) vf4( f, f, f, f );
        c2 = clock();
        t2 = double( c2 - c1 ) / CLOCKS_PER_SEC;
        std::clog<<t2<<" secs\nratio: "<<( t2 / t1 )<<std::endl;
}

int
main()
{
        foo f;
        test( f );

        return 0;
}
