# Set compiler flags for Debug builds.
# Only has an effect on GCC/Clang >= 5.0.

set(tmp_CXXFLAGS "")
set(tmp_LDFLAGS "")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"
    AND NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5")
  list(APPEND tmp_CXXFLAGS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-Wuninitialized"
    "-Wshadow"
    "-Wnon-virtual-dtor"
    "-Wconversion"
    "-Wsign-conversion"
    "-Wold-style-cast"
    "-Wzero-as-null-pointer-constant"
    "-Wmissing-declarations"
    "-Wcast-align"
    "-Wunused"
    "-Woverloaded-virtual"
    "-Wdouble-promotion"
    "-Wformat=2"
    "-ftrapv"
    "-fsanitize=undefined"
    "-Og"
    "-fno-omit-frame-pointer")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND tmp_CXXFLAGS
      "-Wlogical-op"
      "-Wuseless-cast")
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6")
      list(APPEND tmp_CXXFLAGS
        "-Wmisleading-indentation"
        "-Wduplicated-cond"
        "-Wnull-dereference")
      if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7")
        list(APPEND tmp_CXXFLAGS
          "-Wduplicated-branches")
      endif()
    endif()
  endif()
  add_compile_options("$<$<CONFIG:Debug>:${tmp_CXXFLAGS}>")

  list(APPEND tmp_LDFLAGS
    "-fsanitize=undefined")
  # add_link_options was introduced in version 3.13.
  if(${CMAKE_VERSION} VERSION_LESS 3.13)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${tmp_LDFLAGS}")
  else()
    add_link_options("$<$<CONFIG:Debug>:${tmp_LDFLAGS}>")
  endif()
else()
  message(STATUS
    "No additional compiler flags were set, "
    "because your compiler was not anticipated.")
endif()

unset(tmp_CXXFLAGS)
unset(tmp_LDFLAGS)
