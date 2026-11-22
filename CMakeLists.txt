cmake_minimum_required(VERSION 3.16)
project(codex_naturalis CXX)

# Norme volontairement ancienne : seules les notions vues en cours sont utilisees.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

file(GLOB_RECURSE CODEX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM CODEX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(codex_core STATIC ${CODEX_SOURCES})
target_include_directories(codex_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(codex_naturalis src/main.cpp)
target_link_libraries(codex_naturalis PRIVATE codex_core)

file(GLOB_RECURSE CODEX_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
if (CODEX_TESTS)
  add_executable(codex_tests ${CODEX_TESTS})
  target_link_libraries(codex_tests PRIVATE codex_core)
  enable_testing()
  add_test(NAME tests_unitaire COMMAND codex_tests)
endif()
