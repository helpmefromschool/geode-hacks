cmake_minimum_required(VERSION 3.13)

# This line tells the builder where the Geode SDK is
list(APPEND CMAKE_PREFIX_PATH $ENV{GEODE_SDK})

project(GeodeHacks)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
