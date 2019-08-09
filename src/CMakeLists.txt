include(GNUInstallDirs)

# Write version in header
configure_file("version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")

file(GLOB sources *.cpp)
add_executable(${PROJECT_NAME} "${sources}" "gitea2rss.hpp")

target_include_directories(${PROJECT_NAME}
  PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(${PROJECT_NAME}
  PRIVATE jsoncpp_lib)

# If no Poco*Config.cmake recipes are found, look for headers in standard dirs.
if(PocoNetSSL_FOUND)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE Poco::Foundation Poco::Net Poco::NetSSL)
else()
  find_file(Poco_h NAMES "Poco/Poco.h"
    PATHS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

  if("${Poco_h}" STREQUAL "Poco_h-NOTFOUND")
    message(FATAL_ERROR "Could not find POCO.")
  else()
    message(WARNING
      "Your distribution of POCO doesn't contain the *Config.cmake recipes, "
      "but the files seem to be in the standard directories. "
      "Let's hope this works.")
    target_link_libraries(${PROJECT_NAME}
      PRIVATE PocoFoundation PocoNet PocoNetSSL)
  endif()
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
