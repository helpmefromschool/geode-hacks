cmake_minimum_required(VERSION 3.13)

project(OpenHack)

find_package(Geode REQUIRED)

# This tells Geode to use the newest "v3" system
add_geode_mod(
    ${PROJECT_NAME}
    SOURCES src/main.cpp 
)
