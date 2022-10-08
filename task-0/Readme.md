## First (0th) task of practicum 7sem 2022

Simple library for working with
- Rational numbers
- Complex numbers
- Sparse matrices

### Usage

In plans: install library so it will be able just with inclusion of "Task0.h"
Now: meh...

### Building
cmake required

Can compile test.cpp file if -DUSER_TEST=ON option provided to cmake

TODO: optional test compiling support 
```
mkdir build
cd build
cmake -S ../ -B .
make
```

### Documentation and tests.

Documentation (require doxygen):

`doxygen Doxyfile`

Will create docs/html directory with documentation (open some html file)

Tests are built currently in build directory. You can recognize them with template "ClassName_test" (just run those files if you want)
