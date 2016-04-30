#!/bin/bash
#          Copyright Camille Gillot 2012 - 2015.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

tar xjf /tmp/boost-1.60.tbz2

BOOST_PREFIX="$(pwd)/boost-prefix/"
mkdir -p $BOOST_PREFIX

pushd boost_1_60_0
    ./bootstrap.sh --prefix=$BOOST_PREFIX --with-libraries=test
    ./b2 install -d0 -j4
popd
pushd boost_1_60_0/tools/build
    ./bootstrap.sh
    ./b2 install -d0 -j4 --prefix=$BOOST_PREFIX
popd

export PATH="$BOOST_PREFIX/bin:$PATH"

cat > user-config.jam <<EOF

project
  :
  : requirements <include>$BOOST_PREFIX/include
  ;

lib unit_test_framework : : <name>boost_unit_test_framework <search>$BOOST_PREFIX ;

EOF

