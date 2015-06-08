//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[pp_predator_prey
/*`
  The canonical example using __multimethods__ is the predator/prey model.

  [pp_predators]
  [pp_preys]

  [pp_mm_declaration]
  [pp_mm_implement]

  [pp_use]
  [pp_bind]
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
/*`
  First, we need a set of predators.

  The root class in the predator hierarchy
  must be registered by __rtti__.
  For this, we just need to inherit
  [^base_rtti<['class-name]>].
  The `base_rtti` template will inject the necessary code
  to have __rtti__ understand the new class hierarchy.
 */
// Base class for predators
class Predator_abstract
: public base_rtti<Predator_abstract> // Register class hierarchy
, private boost::noncopyable          // Make non-copyable objects
{
public:
  Predator_abstract() {/**/}
  virtual ~Predator_abstract() {/**/}

  // Name of the animal
  virtual std::string i_am_a() const = 0;
};

/*`
  Then, all the derived classes must
  inherit from [^implement_rtti<['class-name], ['bases]>].
  /bases/ is a __sequence__ of classes
  that are already registered with __rtti__.

  The `implement_rtti` class will fetch
  the configuration options given to the actual base classes,
  and register the new class into the hierarchy.
 */
using boost::mpl::vector;

class Lion
: public Predator_abstract
, public implement_rtti<Lion, vector<Predator_abstract> >
{
public:
  std::string i_am_a() const { return "Lion"; }
};

class Anaconda
: public Predator_abstract
, public implement_rtti<Anaconda, vector<Predator_abstract> >
{
public:
  std::string i_am_a() const { return "Anaconda"; }
};

class Bear
: public Predator_abstract
, public implement_rtti<Bear, vector<Predator_abstract> >
{
public:
  std::string i_am_a() const { return "Bear"; }
};
//]

//[pp_preys
/*`
  Now we have a proper set of predators,
  we can define a set of preys the same way.
 */
class Prey_abstract
: public base_rtti<Prey_abstract>
, private boost::noncopyable
{
public:
  Prey_abstract() {/**/}
  virtual ~Prey_abstract() {/**/}

  virtual std::string i_am_a() const = 0;
};

class Gazelle
: public Prey_abstract
, public implement_rtti<Gazelle, vector<Prey_abstract> >
{
public:
  std::string i_am_a() const { return "Gazelle"; }
};

class Giraffe
: public Prey_abstract
, public implement_rtti<Giraffe, vector<Prey_abstract> >
{
public:
  std::string i_am_a() const { return "Giraffe"; }
};
//]

//[pp_mm_declaration
/*`
  We are ready to declare our first __multimethod__.
  This task is made simple by the use of
  the [^DECLARE_MMETHOD] macro,
  taking the following arguments:

  * the name of the __multimethod__
  * its return type
  * a tagged argument list, marking the dispatched arguments
 */
typedef void ResultType;

// Tag marking the arguments whose type
// will dictate the dispatch.
using tags::_v;

// Declare using a dedicated macro
DECLARE_MMETHOD(hunter, ResultType, (_v<Predator_abstract const&>, _v<Prey_abstract const&>));
//]

//[pp_mm_implement
/*`
  A macro is provided to implement the __multimethod__.
  Each implementation must be made with the
  [^IMPLEMENT_MMETHOD] macro, taking :

  * the __multimethod__ name (the name given to [^DECLARE_MMETHOD])
  * its return type
  * its argument list

  The types appearing in the argument list are the actual types
  expected by the __multimethod__ implementation.
 */
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

//` We can catch all the preys using a `Prey_abstract` argument.
IMPLEMENT_MMETHOD(hunter, ResultType, ( Bear const&, Prey_abstract const& prey ))
{ /*<-*/(void)prey;/*->*/
  //=std::cout<<"Bear mauls "<<prey.i_am_a()<<"\n";
}
//]

BOOST_AUTO_TEST_CASE(test_predator_prey)
{
  //[pp_use
  /*`
    The `hunter` __multimethod__ is a function object,
    it can be called directly as a function.
   */
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
  /*`
    It may also be passed as a STL function object
    to STL and __boost__ algorithms.
   */
  auto hunter_lion_prey    = boost::bind(hunter, boost::cref(anaconda), _1);
  hunter_lion_prey(prey1);

  auto hunter_lion_giraffe = boost::bind(hunter, boost::cref(bear), boost::cref(giraffe));
  hunter_lion_giraffe();

  auto hunter_pred_giraffe = boost::bind(hunter, _1, boost::cref(gazelle));
  hunter_pred_giraffe(pred1);
  //]
#endif
}
