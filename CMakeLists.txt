cmake_minimum_required(VERSION 3.22)

PROJECT(nine-strip
LANGUAGES C CXX
VERSION 0.0.1
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for clang-tidy(this enable to find system header files).
if(CMAKE_EXPORT_COMPILE_COMMANDS)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# Enable JUCE. Do not use find_package to prevent from mix up with one globally installed.
add_subdirectory(lib/JUCE)

add_subdirectory(src)