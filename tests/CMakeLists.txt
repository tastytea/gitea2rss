include(CTest)

file(GLOB sources_testlib ../src/*.cpp)
add_library(${PROJECT_NAME}_testlib SHARED ${sources_testlib})

target_include_directories(${PROJECT_NAME}_testlib
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
  "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>")

target_link_libraries(${PROJECT_NAME}_testlib
  PRIVATE CURL::libcurl PkgConfig::jsoncpp)

file(GLOB sources_tests test_*.cpp)

find_package(Catch2 CONFIG)
if(Catch2_FOUND)                # Catch 2.x
  include(Catch)
  add_executable(all_tests main.cpp ${sources_tests})
  target_link_libraries(all_tests
    PRIVATE Catch2::Catch2 ${PROJECT_NAME}_testlib)
  target_include_directories(all_tests PRIVATE "/usr/include/catch2")
  catch_discover_tests(all_tests EXTRA_ARGS "${EXTRA_TEST_ARGS}")
else()                          # Catch 1.x
  if(EXISTS "/usr/include/catch.hpp")
    message(STATUS "Catch 1.x found.")
    foreach(src ${sources_tests})
      get_filename_component(bin ${src} NAME_WE)
      add_executable(${bin} main.cpp ${src})
      target_link_libraries(${bin}
        PRIVATE ${PROJECT_NAME}_testlib)
      add_test(${bin} ${bin} "${EXTRA_TEST_ARGS}")
    endforeach()
  else()
    message(FATAL_ERROR
      "Neither Catch 2.x nor Catch 1.x could be found.")
  endif()
endif()
