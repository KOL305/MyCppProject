cmake_minimum_required(VERSION 3.10)
project(MyCppProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###### WILL BE DIFFERENT DEPENDING ON YOUR SYSTEM ######
# Specify the path to the vcpkg toolchain file
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/Development/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file" FORCE)
endif()

###### WILL BE DIFFERENT DEPENDING ON YOUR SYSTEM ######
# Manually specify GLPK, GMP, MPFR, and Coin-Or directories 
set(GLPK_INCLUDE_DIR "C:/Development/vcpkg/installed/x64-windows/include")
set(GLPK_LIBRARY "C:/Development/vcpkg/installed/x64-windows/lib")

set(GMP_INCLUDE_DIR "C:/Development/vcpkg/installed/x64-windows/include")
set(GMP_LIBRARY "C:/Development/vcpkg/installed/x64-windows/lib")

set(MPFR_INCLUDE_DIR "C:/Development/vcpkg/installed/x64-windows/include")
set(MPFR_LIBRARY "C:/Development/vcpkg/installed/x64-windows/lib")

#set(CPLEX_INCLUDE_DIR "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/include")
#set(CPLEX_LIBRARY_DIR "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/lib/x64_windows_msvc14/stat_mdd")

#set(CONCERT_INCLUDE_DIR "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/concert/include")
#set(CONCERT_LIBRARY_DIR "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/concert/lib/x64_windows_msvc14/stat_mdd")

# Add include directories
#include_directories(${GLPK_INCLUDE_DIR} ${GMP_INCLUDE_DIR} ${MPFR_INCLUDE_DIR} ${CPLEX_INCLUDE_DIR} ${CONCERT_INCLUDE_DIR})
include_directories(${GLPK_INCLUDE_DIR} ${GMP_INCLUDE_DIR} ${MPFR_INCLUDE_DIR})

# Add library directories
#link_directories(${GLPK_LIBRARY} ${GMP_LIBRARY} ${MPFR_LIBRARY} ${CPLEX_LIBRARY_DIR} ${CONCERT_LIBRARY_DIR} ${CONCERT_LIBRARY_DIR})
link_directories(${GLPK_LIBRARY} ${GMP_LIBRARY} ${MPFR_LIBRARY})

# Add executable
add_executable(MyCppProject src/main.cpp)

# Link libraries
#target_link_libraries(MyCppProject PRIVATE glpk gmp mpfr cplex2211 ilocplex concert)
target_link_libraries(MyCppProject PRIVATE glpk gmp mpfr)
