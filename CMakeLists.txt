cmake_minimum_required(VERSION 3.20)
project(sound-level-meter VERSION 1.0.0 LANGUAGES CXX C)

enable_testing()

include_directories(${CMAKE_BINARY_DIR}/coeffs)

if (NATIVE_BUILD)
  find_package(cxxopts REQUIRED)
  find_package(SndFile REQUIRED)
endif()

add_subdirectory(src)

