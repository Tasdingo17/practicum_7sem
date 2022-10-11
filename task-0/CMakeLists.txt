cmake_minimum_required(VERSION 3.14)
project(7semTask0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
  
enable_testing()
include(GoogleTest)
#gtest_discover_tests(target)

set(Rational_number rational/ClassRationalNumber.h
                    rational/ClassRationalNumber.cpp
   )

set(Exceptions exceptions/CommonExceptions.hpp
               exceptions/ComplNumbersExceptions.hpp
               exceptions/RatNumbersExceptions.hpp
               exceptions/ParserExceptions.hpp
   )

set(Complex    complex/ClassComplex.h
   )

set(Matrix     matrix/ClassMatrix.h
               matrix/Matrix_coords.h
               matrix/Matrix_coords.cpp
               matrix/Matrix_proxy.hpp
   )

set(Vector     vector/ClassVector.hpp
   )

set(Parsers    parsers/Parser.h
               parsers/Parser.cpp
   )

add_library( Task0 ${Rational_number} ${Complex} ${Matrix} ${Vector} ${Exceptions} ${Parsers})


option(USER_TEST "Compile test.cpp file" OFF)

if(USER_TEST)
  add_executable(test_task0 test.cpp)
  target_link_libraries(test_task0 PUBLIC Task0)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
  target_include_directories(test_task0  PUBLIC
                           "${PROJECT_BINARY_DIR}"
                            )
  install(TARGETS test_task0 DESTINATION bin)
endif()

add_executable(Rational_number_test tests/rational/RatNumbersConstrTests.cpp tests/rational/RatNumbersOperatorsTest.cpp)
target_link_libraries(Rational_number_test Task0 GTest::gtest GTest::gtest_main)
add_test(NAME Rational_number_test COMMAND Rational_number_test)

#add_executable(Complex_number_test test/Complex_number/Complex_number_constructor_test.cpp test/Complex_number/Complex_number_operators_test.cpp)
#target_link_libraries(Rational_number_test Task0 gtests_main)
#add_test(NAME Complex_number_test COMMAND Complex_number_test)

#add_executable(Matrix_test test/Matrix/Matrix_test.cpp)
#target_link_libraries(Rational_number_test Task0 gtests_main)
#add_test(NAME Matrix_test COMMAND Matrix_test)