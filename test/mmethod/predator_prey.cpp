//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[pp_predator_prey
/*`
  The canonical example using __multimethods__ is the predator/prey model.

  [pp_include]

  [pp_predators]
  [pp_preys]

  [pp_mm_declaration]
  [pp_mm_implement]

  [pp_use]
  [pp_bind]
 */
//]

//[pp_include
/*`
  To access the library, we will need to include this header.
  It loads everything necessary to have working __multimethods__ in no time.
 */
#include "mmethod/mmethod.hpp"

/*`
  To have this example running, we will also need those includes.
 */
#include <iostream>
#include <boost/bind.hpp>
#include <boost/mpl/vector.hpp>
//]

#include <boost/test/unit_test.hpp>

//[pp_predators
/*`
  [heading Defining the objects]
  First, we need a set of predators.

  The root class in the predator hierarchy
  must be registered by __rtti__.
  For this, it just needs to inherit
  [^base_rtti<['class-name]>].
  The `base_rtti` template will inject the necessary code
  to have __rtti__ understand the new class hierarchy.
 */
// For convenience
using namespace rtti;

// Base class for predators
class Predator_abstract
: public base_rtti<Predator_abstract> // Register class hierarchy
{
public:
  Predator_abstract() {/**/}
  virtual ~Predator_abstract() {/**/}

  // Name of the animal
  virtual std::string i_am_a() const = 0;
};

/*`
  Then, all the derived classes must
  inherit from [^implement_rtti<['class-name], ['bases]>],
  where /bases/ is a __sequence__ of already registered classes.
  The `implement_rtti` template will find
  the configuration options given to the base classes,
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
  [heading Declaring the __multimethod__]
  We are ready to declare our first __multimethod__.
  This task is made simple by the use of
  the `DECLARE_MMETHOD` macro,
  taking the following arguments:

  * the name of the __multimethod__
  * its return type
  * a tagged argument list, marking the dispatched arguments
 */
typedef void ResultType;

// Tag marking the arguments whose type will dictate the dispatch.
using tags::_v;

// Declare using a dedicated macro
DECLARE_MMETHOD(hunter, ResultType, (_v<Predator_abstract const&>, _v<Prey_abstract const&>));
/*`
  The tagging here says the following:
  `_v<Predator_abstract const&>` means that the first argument
  to the __multimethod__ is a constant reference to a predator
  (`Predator_abstract const&`),
  and that it is taken into account in the dispatch
  (because of the `_v<>`).
  Likewise for the second argument.

  When an argument is set without the `_v<>` tag,
  the dispatch has to ignore it,
  as in [link mmethod_doc.mmethod.mixed this example].
 */
//]

//[pp_mm_implement
/*`
  [heading Implementing the __multimethod__]
  To implement the __multimethod__,
  we only need to use the `IMPLEMENT_MMETHOD` macro,
  the rest is just writing the function.
  The arguments to `IMPLEMENT_MMETHOD` are:

  * the __multimethod__ name (the name given to `DECLARE_MMETHOD`)
  * its return type
  * its argument list

  The types appearing in the argument list are the actual types
  expected by the __multimethod__ implementation.
 */
// implement using the dedicated macro
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
  std::cout<<"Anaconda ignores Giraffe.\n";
}

/*`
  The overload resolution rules are the same as for static dispatch.
  Therefors, we can catch all the preys using a `Prey_abstract` argument.
 */
IMPLEMENT_MMETHOD(hunter, ResultType, ( Bear const&, Prey_abstract const& prey ))
{
  //=std::cout<<"Bear mauls "<<prey.i_am_a()<<"\n";
}
//]

BOOST_AUTO_TEST_CASE(test_predator_prey)
{
  //[pp_use
  /*`
    [Heading Calling the __multimethod__]
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
