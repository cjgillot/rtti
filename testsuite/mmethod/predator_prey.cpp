//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[pp_predator_prey
/*`
  The canonical example using __multimethods__ is the predator/prey model.

  First, we need a set of predators
  [pp_predators]
  and a set of preys
  [pp_preys]

  Then we can declare a __multimethod__ [pp_mm_declaration]
  And then implement it for selected type couples [pp_mm_implement]

  The `hunter` __multimethod__ object is a function object,
  it can be called directly as a function [pp_use]
  It may also be passed as a STL function object to STL and __boost__ wrappers [pp_bind]
 */
//]

//<-
#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/noncopyable.hpp>
#include <boost/test/unit_test.hpp>

using namespace rtti;
using boost::mpl::vector;
//->

//[pp_predators
// Base class for predators
struct Predator_abstract
: base_rtti<Predator_abstract>  // rtti hierarchy
, boost::noncopyable
{
  Predator_abstract() {/**/}    // Ctor

  virtual std::string
  i_am_a() const = 0;           // Name of the animal
};

struct Lion
: public Predator_abstract      // classical inheritance
, public implement_rtti<Lion, vector<Predator_abstract> >       // declare hierarchy
{
  std::string i_am_a() const { return "Lion"; }
};

struct Anaconda
: public Predator_abstract
, public implement_rtti<Anaconda, vector<Predator_abstract> >
{
  std::string i_am_a() const { return "Anaconda"; }
};

struct Bear
: public Predator_abstract
, public implement_rtti<Bear, vector<Predator_abstract> >
{
  std::string i_am_a() const { return "Bear"; }
};
//]

//[pp_preys
struct Prey_abstract
: base_rtti<Prey_abstract>      // rtti hierarchy declaration
, boost::noncopyable
{
  Prey_abstract() {/**/}        // Ctor

  virtual std::string
  i_am_a() const = 0;           // Name of the animal
};

struct Gazelle
: public Prey_abstract
, public implement_rtti<Gazelle, vector<Prey_abstract> >
{
  std::string i_am_a() const { return "Gazelle"; }
};

struct Giraffe
: public Prey_abstract
, public implement_rtti<Giraffe, vector<Prey_abstract> >
{
  std::string i_am_a() const { return "Giraffe"; }
};
//]

//[pp_mm_declaration
typedef void ResultType; // return type declaration

using rtti::tags::_v; // tag for flagging dispatch arguments

// declare using a dedicated macro
DECLARE_MMETHOD(hunter, ResultType, (_v<Predator_abstract const&>, _v<Prey_abstract const&>));
//]

//[pp_mm_implement
// implement using a dedicated macro
IMPLEMENT_MMETHOD(hunter, ResultType, ( Lion const&, Gazelle const& ))
{
  //=std::cout<<"Lion jumps on Gazelle and bites its neck.\n";
}

IMPLEMENT_MMETHOD(hunter, ResultType, ( Lion const&, Giraffe const& ))
{
  //=std::cout<<"Lion jumps on Giraffe and bites its ass.\n";
}

IMPLEMENT_MMETHOD(hunter, ResultType, ( Anaconda const&, Gazelle const& ))
{
  //=std::cout<<"Anaconda injects venom into Gazelle.\n";
}

IMPLEMENT_MMETHOD(hunter, ResultType, ( Anaconda const&, Giraffe const& ))
{
  //=std::cout<<"Anaconda ignores Giraffe.\n";
}

IMPLEMENT_MMETHOD(hunter, ResultType, ( Bear const&, Prey_abstract const& prey ))
{
  //=std::cout<<"Bear mauls "<<prey.i_am_a()<<"\n";
}
//]

BOOST_AUTO_TEST_CASE(predator_prey)
{
  //[pp_use
  Bear bear;
  Lion lion;
  Anaconda anaconda;

  Gazelle gazelle;
  Giraffe giraffe;

  Predator_abstract& pred1 = lion;
  Prey_abstract& prey1 = giraffe;

  hunter(pred1, prey1);
  //]

#ifndef BOOST_NO_CXX11_AUTO_DECLARATIONS
  //[pp_bind
  auto hunter_lion_prey=boost::bind(hunter, boost::cref(anaconda), _1);
  hunter_lion_prey(prey1);
  auto hunter_lion_giraffe=boost::bind(hunter, boost::cref(bear), boost::cref(giraffe));
  hunter_lion_giraffe();
  auto hunter_pred_giraffe=boost::bind(hunter, _1, boost::cref(gazelle));
  hunter_pred_giraffe(pred1);
  //]
#endif
}
