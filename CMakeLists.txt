cmake_minimum_required(VERSION 3.20)
project(sound-level-meter VERSION 1.0.0 LANGUAGES CXX C)

enable_testing()

include_directories(${CMAKE_BINARY_DIR}/coeffs)

add_subdirectory(src)

