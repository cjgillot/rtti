rtti
====

Implementation of multimethods in C++11.

This library implements 2 multimethod dispatch mechanisms in C++11 :
- static table :
    The compilation command must be echo'ed to a outer program
    that generates the dispatch table and efficient lookup
    routines.

- dynamic table :
    The dispatch table is filled at runtime.
