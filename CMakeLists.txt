cmake_minimum_required(VERSION 3.15)
project(ReleaseInfoCLI VERSION 1.0 LANGUAGES CXX)

# Set default build type if not set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Set C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Conan package handling
include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

find_package(RapidJSON REQUIRED)
find_package(CURL REQUIRED)

add_executable(releaseInfoCLI 
  src/main.cpp 
  src/DataFetcher.cpp
  src/CurlWrapper.cpp
)

# Not clean but RapidJson gives lots of deprecated warnings
target_compile_options(releaseInfoCLI PRIVATE -Wno-deprecated-declarations)

target_include_directories(releaseInfoCLI
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${RapidJSON_INCLUDE_DIRS}  # Conan sets this path automatically
)

target_link_libraries(releaseInfoCLI PRIVATE CURL::libcurl)
