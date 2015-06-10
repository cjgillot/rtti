//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_LINK_TABLE_HPP
#define RTTI_MPH_LINK_TABLE_HPP

#include "forward.hpp"

#include <boost/variant.hpp>
#include <boost/noncopyable.hpp>
#include <boost/unordered_set.hpp>
#include <boost/dynamic_bitset.hpp>

class link_table
: private boost::noncopyable
{
public:
  typedef boost::variant<invoker_t, signature_t> variant_t;
  typedef std::pair<signature_t, variant_t> link_t;
  typedef boost::optional<link_t> optional_link_t;
  typedef boost::unordered_map<signature_t, optional_link_t> dispatch_link_t;

  typedef boost::unordered_set<signature_t> visited_t;

private:
  dispatch_link_t links;
  visited_t visited;

  dispatch_t& dispatch;
  pole_table_t const& pole_table;

public:
  link_table(dispatch_t& d, const pole_table_t& pt);

  void resolve();

public:
  bool has(const signature_t& s) const
  { return links.find(s) != links.end(); }

  optional_link_t const& at(const signature_t& s) const {
    BOOST_ASSERT(has(s));
    return links.at(s);
  }

  void insert(const signature_t& s, link_t const& l) {
    links.insert(std::make_pair(s, l));
  }
  void insert_none(const signature_t& s) {
    links.insert(std::make_pair(s, boost::none));
  }
  void insert_link(const signature_t& s, const signature_t& t) {
    insert(s, std::make_pair(s, variant_t(t)));
  }

private:
  invoker_t resolve_once(const signature_t& sig);
};

#endif
