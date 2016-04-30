#!/bin/bash
#          Copyright Camille Gillot 2012 - 2015.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

tar xjf /tmp/boost.tbz2

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

export BOOST_ROOT=$BOOST_PREFIX
export BOOST_INCLUDEDIR=$BOOST_PREFIX/include
export BOOST_LIBRARYDIR=$BOOST_PREFIX/lib
export Boost_NO_SYSTEM_PATHS=ON
