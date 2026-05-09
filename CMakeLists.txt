cmake_minimum_required(VERSION 3.13)

# This line is the "GPS" for the builder
set(GEODE_SDK $ENV{GEODE_SDK})
list(APPEND CMAKE_PREFIX_PATH ${GEODE_SDK})

project(GeodeHacks)

find_package(Geode REQUIRED)

add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp
)
