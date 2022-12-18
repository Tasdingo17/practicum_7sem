cmake_minimum_required(VERSION 3.14)
project(7semTask3)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(GenAlg src/Sizes.h src/Crossing.cpp 
              src/Selection.cpp src/GoLEngine.hpp
              src/SurvaivalFunc.cpp src/Mutation.cpp
              src/GeneticAlgo.cpp            
   )

add_library( Task3 ${GenAlg})

option(USER_TEST "Compile test.cpp file" OFF)

if(USER_TEST)
  add_executable(test_task3 test.cpp)
  target_link_libraries(test_task3 PUBLIC Task3 
                                   ${CURSES_LIBRARIES})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
  #target_include_directories(test_task2  PUBLIC
  #                         "${PROJECT_BINARY_DIR}"
  #                          )
endif()

# todo experiment
