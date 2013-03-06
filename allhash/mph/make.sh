BUILDDIR="../../../build/src/rtti_allhash/mph/"
FLAGS="-std=c++11 -I../.. -O3 -g -DNDEBUG -fomit-frame-pointer -funroll-loops"
NAME=vf*
# rm $NAME.mm* ../multimethod/*.o
for n in $(ls ../multimethod/*.cpp); do
  gcc -c $n -o $n.o $FLAGS 2>&1 | $BUILDDIR/parse_allhash
done
for n in $(ls $NAME.mm); do
  echo $n
  $BUILDDIR/mph_allhash $n $n.cc
  gcc $FLAGS -c $n.cc -o $n.o
done
g++ ../multimethod/*.cpp.o $NAME.mm.o $FLAGS -o exe
