# Support version 3.2 and above, but use policy settings up to 3.14.
cmake_minimum_required(VERSION 3.2...3.14)
# Ranges are supported from 3.12, set policy to current for < 3.12.
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(gitea2rss
  VERSION 0.5.0
  LANGUAGES CXX)

# DESCRIPTION was introduced in version 3.9.
if(NOT (${CMAKE_VERSION} VERSION_LESS 3.9))
  set(PROJECT_DESCRIPTION
    "Generates RSS feeds from Gitea releases or tags.")
endif()

option(WITH_MAN "Compile and install manpage." YES)
option(WITH_TESTS "Compile tests." NO)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DEBUG_CXXFLAGS
  "-Wall"
  "-Wextra"
  "-Wpedantic"
  "-ftrapv"
  "-fsanitize=undefined"
  "-g"
  "-Og"
  "-fno-omit-frame-pointer")
set(DEBUG_LDFLAGS
  "-fsanitize=undefined")
add_compile_options("$<$<CONFIG:Debug>:${DEBUG_CXXFLAGS}>")
# add_link_options was introduced in version 3.13.
if(${CMAKE_VERSION} VERSION_LESS 3.13)
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${DEBUG_LDFLAGS}")
else()
  add_link_options("$<$<CONFIG:Debug>:${DEBUG_LDFLAGS}>")
endif()

find_package(jsoncpp CONFIG REQUIRED)
# Some distributions do not contain Poco*Config.cmake recipes.
find_package(Poco COMPONENTS Foundation Net NetSSL CONFIG)

add_subdirectory(src)

if (WITH_MAN)
  add_subdirectory(man)
endif()

if(WITH_TESTS)
  add_subdirectory(tests)
endif()
