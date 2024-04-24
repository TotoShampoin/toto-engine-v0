cmake_minimum_required(VERSION 3.12)
project(TotoEngine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 23)

set(${PROJECT_NAME}_OUTPUT_BIN ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
set(${PROJECT_NAME}_OUTPUT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/dist/lib)

set(${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(${PROJECT_NAME}_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

include(utils/CMakeUtils/files_and_folders.cmake)

file(GLOB SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${${PROJECT_NAME}_INCLUDE_DIR}/*.hpp)
file(GLOB EMBEDDED ${${PROJECT_NAME}_SOURCE_DIR}/res/*.hpp)

file(GLOB TESTS ${${PROJECT_NAME}_TESTS_DIR}/*.cpp)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR} ${${PROJECT_NAME}_SOURCE_DIR})

add_executable(${PROJECT_NAME}_tests ${TESTS})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_tests PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_LIB})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_LIB})
set_target_properties(${PROJECT_NAME}_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_BIN})

Cool__target_copy_folder(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDE_DIR})

if (MSVC)
    target_compile_options(${PROJECT_NAME}_tests PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME}_tests PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()
