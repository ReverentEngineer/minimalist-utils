cmake_minimum_required(VERSION 2.8)

project(minalist-utils C)
set(PROJECT_VERSION "1.0.0")

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

include (CTest)
enable_testing()

include_directories(include)


add_library(minimalist-utils SHARED
  src/graph.c
  src/hash_map.c
  src/map.c
  src/set.c)

install(TARGETS minimalist-utils LIBRARY DESTINATION lib)
install(DIRECTORY include/minimalist DESTINATION include
  FILES_MATCHING PATTERN "*.h")

if (UNIX)
  configure_file(minimalist-utils.pc.in minimalist-utils.pc)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/minimalist-utils.pc
    DESTINATION lib/pkgconfig)
endif()

macro(add_utils_test _NAME)
  add_executable(${_NAME} tests/${_NAME}.c)
  target_link_libraries(${_NAME} minimalist-utils)
  add_test(${_NAME} ${_NAME})
endmacro(add_utils_test)

add_utils_test(test_graph)
add_utils_test(test_map)
add_utils_test(test_hash_map)
