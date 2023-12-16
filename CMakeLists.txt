# project(result)

# Source

#set ( SOURCE_ONE_WEEKEND

#  InOneWeekend/InOneWeekend/color.h
  #InOneWeekend/InOneWeekend/vec3.h

#  InOneWeekend/InOneWeekend/main.cpp
#)

#include_directories(InOneWeekend)

# Executables


#add_executable(result ${SOURCE_ONE_WEEKEND})

cmake_minimum_required ( VERSION 3.1.0...3.27.0 )

project(result)

# Set to C++11
set ( CMAKE_CXX_STANDARD          11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

add_executable(result InOneWeekend/InOneWeekend/main.cpp)