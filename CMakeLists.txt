cmake_minimum_required(VERSION 3.31)
project(life)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED)
#find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(src)

# Create the life library
set ( ${PROJECT_NAME}_LIFE_HEADERS ./src/life.hpp src/Stats.hpp)
set ( ${PROJECT_NAME}_LIFE_SOURCE ./src/life.cpp src/Stats.cpp)
add_library( ${PROJECT_NAME}_LIFE SHARED ${${PROJECT_NAME}_LIFE_HEADERS} ${${PROJECT_NAME}_LIFE_SOURCE})

add_executable(
        ${PROJECT_NAME}
        src/main.cpp
)

target_link_libraries(life LINK_PUBLIC SDL3::SDL3 ${PROJECT_NAME}_LIFE)

# Testing - LIFE
include(GoogleTest)
enable_testing()
# add_executable(life_tests src/life_test.cpp)
# target_link_libraries(life_tests ${PROJECT_NAME}_LIFE)
add_test(NAME life_unit_tests COMMAND life_tests)
add_executable(life_gtests src/life_gtests.cpp)
target_link_libraries(life_gtests ${PROJECT_NAME}_LIFE GTest::gtest_main)
gtest_discover_tests(life_gtests)

# Testing - STATS
include(GoogleTest)
enable_testing()
add_executable(stats_tests src/stats_test.cpp)
target_link_libraries(stats_tests ${PROJECT_NAME}_LIFE GTest::gtest_main)
gtest_discover_tests(stats_tests)
