cmake_minimum_required(VERSION 3.10)
project(MySDL3Project)
add_definitions( -std=c++17 )

find_package(SDL3 REQUIRED)

# Create the life library
set ( ${PROJECT_NAME}_LIFE_HEADERS ./src/life.hpp )
set ( ${PROJECT_NAME}_LIFE_SOURCE ./src/life.cpp )
add_library( ${PROJECT_NAME}_LIFE SHARED ${${PROJECT_NAME}_LIFE_HEADERS} ${${PROJECT_NAME}_LIFE_SOURCE})

# Create the atoms library from atoms.cpp
set ( ${PROJECT_NAME}_ATOMS_SOURCE ./src/libs/atoms.cpp )
add_library( ${PROJECT_NAME}_ATOMS SHARED ${${PROJECT_NAME}_ATOMS_SOURCE})

# MySDL3App
add_executable(MySDL3App ./src/main.cpp)
target_link_libraries(MySDL3App LINK_PUBLIC SDL3::SDL3 ${PROJECT_NAME}_LIFE)
target_include_directories(MySDL3App PUBLIC ${PROJECT_NAME}_LIFE)

# Testing - LIFE
include(CTest)
enable_testing()
add_executable(life_tests ./src/test_life.cpp)
target_link_libraries(life_tests ${PROJECT_NAME}_LIFE)
add_test(NAME life_unit_tests COMMAND life_tests)