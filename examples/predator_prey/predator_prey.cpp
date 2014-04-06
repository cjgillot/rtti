//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

typedef void ResultType; 
 
struct object
//Super class of all classes.
{
      virtual
    std::string 
  i_am_a()const=0
  //Returns the most derived class that
  //object is a member of.
  ;
  object()
  {}
#define OBJECT_IS_COPYABLE
#ifndef OBJECT_IS_COPYABLE
 private:
  object(object const&)
  //Ensure that no copies are made when calling
  //functions. with object as arg.
  {}
#endif  
}; 
 
//Predators:
  struct Lion;
  struct Anaconda;
  struct Bear;
 
struct Predator_abstract;
 
//Preys:
  struct Gazelle;
  struct Girrafe;
   
struct Prey_abstract;  
 
#define ACCEPT_CONSTANCY
#define VISIT_CONSTANCY const

struct Predator_abstract: object
, base_rtti<Predator_abstract>
{};
 
struct Lion: public Predator_abstract
, public implement_rtti<Lion, vector<Predator_abstract> >
{
  std::string i_am_a() const { return "Lion"; }
};
 
struct Anaconda: public Predator_abstract
, public implement_rtti<Anaconda, vector<Predator_abstract> >
{
  std::string i_am_a() const { return "Anaconda"; }
};
 
struct Bear: public Predator_abstract
, public implement_rtti<Bear, vector<Predator_abstract> >
{
  std::string i_am_a() const { return "Bear"; }
};

//--------------------------
struct Prey_abstract: object
, base_rtti<Prey_abstract>
{};
 
struct Gazelle: public Prey_abstract
, public implement_rtti<Gazelle, vector<Prey_abstract> >
{
  std::string i_am_a() const { return "Gazelle"; }
};
 
struct Girrafe: public Prey_abstract
, public implement_rtti<Girrafe, vector<Prey_abstract> >
{
  std::string i_am_a() const { return "Gazelle"; }
};

//----------------------
using boost::mmethod::tags::_v;

BOOST_MMETHOD_DECLARE(huntor, ResultType, (_v<Predator_abstract const&>, _v<Prey_abstract const&>));

BOOST_MMETHOD_IMPLEMENT(huntor, ResultType, ( Lion const&, Gazelle const& ))
{
  std::cout<<"Lion jumps on Gazelle and bites its neck.\n";
}
 
BOOST_MMETHOD_IMPLEMENT(huntor, ResultType, ( Lion const&, Girrafe const& ))
{
  std::cout<<"Lion jumps on Girrafe and bites its ass.\n";
}

BOOST_MMETHOD_IMPLEMENT(huntor, ResultType, ( Anaconda const&, Gazelle const& ))
{
  std::cout<<"Anaconda injects venom into Gazelle.\n";
}

BOOST_MMETHOD_IMPLEMENT(huntor, ResultType, ( Anaconda const&, Girrafe const& ))
{
  std::cout<<"Anaconda ignores Girraffe.\n";
}

BOOST_MMETHOD_IMPLEMENT(huntor, ResultType, ( Bear const&, Prey_abstract const& prey ))
{
  std::cout<<"Bear mauls "<<prey.i_am_a()<<"\n";
}
 
void test()
{
    Lion lion;
    Girrafe girrafe;

    Predator_abstract ACCEPT_CONSTANCY& pred1=lion;
    Prey_abstract ACCEPT_CONSTANCY& prey1=girrafe;

    huntor(pred1, prey1);

  #ifdef OBJECT_IS_COPYABLE
    auto huntor_lion_prey=boost::bind(huntor,Anaconda(),_1);
    huntor_lion_prey(prey1);
    auto huntor_lion_girrafe=boost::bind(huntor,Bear(),Girrafe());
    huntor_lion_girrafe();
    auto huntor_pred_girrafe=boost::bind(huntor,_1,Gazelle());
    huntor_pred_girrafe(pred1);
  #endif
}
 
int main(void)
{
    test();
    return 0;
}
