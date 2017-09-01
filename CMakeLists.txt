cmake_minimum_required(VERSION 2.8.7)

project(avm_lib_test)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
set(USE_FOLDERS TRUE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(AVM_LIB_TEST_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(externals/avm)

add_definitions(-DANY_TASK_FIBER)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/avm/src/inc)

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(osp ${HEADERS} ${SOURCES})
target_link_libraries(osp avm)