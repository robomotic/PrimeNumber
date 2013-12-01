#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8)
#Name your project here
project(PrimeNumber)



find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

IF(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions("-Wall")
	#Sends the -std=c99 flag to the gcc compiler
	add_definitions(-std=c++11)
	add_definitions(-Ofast)
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
ENDIF()


# Boost
# For automatic linking, uses the static version of the libraries.
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif(WIN32)
set(Boost_ADDITIONAL_VERSIONS "1.49.0" )
if ( WIN32 )
find_package(Boost)
  #find_package(Boost REQUIRED COMPONENTS system date_time filesystem
#thread iostreams regex serialization bzip2 zlib)
else ( WIN32 )
find_package(Boost)
  #find_package(Boost REQUIRED COMPONENTS system date_time filesystem
#thread iostreams regex serialization)
endif ( WIN32 )
if ( Boost_FOUND )
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
  # Remember to include ${Boost_LIBRARIES} in the target_link_libraries() statement
  # This is needed for debug builds in windows in boost.
  link_directories(${Boost_LIBRARY_DIRS})
endif ( Boost_FOUND )


#This tells CMake to fib.c and name it fibonacci
add_executable(primes main.cpp)
