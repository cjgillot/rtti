//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "head.hpp"

void test(foo*, std::size_t);

int main(int argc, char** argv) {
  std::size_t N = 3000000000;
  foo f; bar b; baz z;
  foo* ff = &f;

  if(argc >= 2)
    N = std::atoll(argv[1]);
  if(argc >= 3)
    ff =
      (argv[2][0] == '0') ? (foo*)&f :
      (argv[2][0] == '1') ? (foo*)&b :
      (argv[2][0] == '2') ? (foo*)&z : &f;

  test(ff, N);
}

#include <cstdio>
#include <ctime>

void loop_virt(foo& f, size_t N);
void loop_mm1 (foo& f, size_t N);
void loop_mm2 (foo& f, foo& g, size_t N);

template<typename F>
double time(F&& fnc, char const* s) {
  clock_t cl1, cl2;

  std::printf("Calling function %s... ", s); std::fflush(stdout);
  cl1 = std::clock();

  fnc();

  cl2 = std::clock();
  double t1 = (double)(cl2 - cl1) / CLOCKS_PER_SEC;
  std::printf("%f secs.\n", t1); std::fflush(stdout);

  return t1;
}

void test(foo* f, std::size_t N) {
  vf1.generate();
  vf2.generate();

  double t1, t2;
  std::printf("Calling functions %u times :\n", N); std::fflush(stdout);

  t1 = time([=](){ loop_virt(*f, N); }, "virtual");

  t2 = time([=](){ loop_mm1 (*f, N); }, "mmethod1");
  std::printf("ratio : %f\n", t2 / t1);

  t2 = time([=](){ loop_mm2 (*f, *f, N); }, "mmethod2");
  std::printf("ratio : %f\n", t2 / t1);
}

void boost::throw_exception(std::exception const&)
{ std::abort(); }

