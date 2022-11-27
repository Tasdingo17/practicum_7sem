cmake_minimum_required(VERSION 3.14)
project(7semTask2)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
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

set(Functions src/Functions.h src/Functions.cpp
              src/FuncFabric.h src/FuncFabric.cpp
   )

add_library( Task2 ${Functions})


option(USER_TEST "Compile test.cpp file" OFF)

if(USER_TEST)
  add_executable(test_task2 test.cpp)
  target_link_libraries(test_task2 PUBLIC Task2)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
  #target_include_directories(test_task2  PUBLIC
  #                         "${PROJECT_BINARY_DIR}"
  #                          )
endif()

#add_executable(Function_test tests/ tests/)
#target_link_libraries(Function_test Task2 GTest::gtest GTest::gtest_main)
#add_test(NAME test_functions COMMAND Function_test)