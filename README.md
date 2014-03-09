# rtti

Implementation of multimethods in C++11.

## RTTI library

This library creates a efficient replacement of default C++ RTTI.

Using a ISO C++ construct, RTTI provides access to a unique integer per class in the hierarchy.
For each class, its bases' ids are also exposed as a linked list. This feature
allows dynamic introspection through the hierarchy.

Note : ids are garanteed to be constructed when main() starts. Using those
before entering main() results in undefined behaviour.

See *include/rtti/* for further information.
See *examples/* folder for examples using this library.

## MMETHOD library

This library implements open multi-methods in ISO C++.

The external interface fits in two files :
- *include/mmethod/mmethod.hpp*
- *include/mmethod/implement.hpp*

*mmethod.hpp* exports everything needed to declare
and invoke mmethods. *mmethod.hpp*

*implement.hpp* exports everything needed to implement
formerly declared mmethods.

See *include/mmethod/* for further information.
See *examples/* folder for examples using this library.

## Supported compilers

GCC 4.8 and CLANG 3.3 have been succesfully tested.
