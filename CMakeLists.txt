# IMPORTED target CURL::libcurl needs 3.12.
cmake_minimum_required(VERSION 3.12...3.18)

project(gitea2rss
  VERSION 0.7.0
  DESCRIPTION "Generates RSS feeds from Gitea releases or tags."
  LANGUAGES CXX)

option(WITH_MAN "Compile and install manpage." YES)
option(WITH_TESTS "Compile tests." NO)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/debug_flags.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(jsoncpp REQUIRED IMPORTED_TARGET jsoncpp)
find_package(CURL 7.56 REQUIRED)

add_subdirectory(src)

if (WITH_MAN)
  add_subdirectory(man)
endif()

if(WITH_TESTS)
  add_subdirectory(tests)
endif()
