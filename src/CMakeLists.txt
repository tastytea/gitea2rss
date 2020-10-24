include(GNUInstallDirs)

# Write version in header
configure_file("version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")

file(GLOB sources *.cpp)
add_executable(${PROJECT_NAME} "${sources}" "gitea2rss.hpp")

target_include_directories(${PROJECT_NAME}
  PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(${PROJECT_NAME}
  PRIVATE PkgConfig::jsoncpp CURL::libcurl)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
