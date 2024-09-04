cmake_minimum_required(VERSION 3.12)
project(TotoEngine)

# includes
include(utils/CMakeUtils/files_and_folders.cmake)
include(utils/files_and_folders2.cmake)
    # graphics
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm REQUIRED)
    include(vendors/imgui.cmake)
    include(vendors/stb.cmake)
    # audio
    find_package(OpenAL REQUIRED)

# variables
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})
set(${PROJECT_NAME}_OUTPUT_BIN ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
set(${PROJECT_NAME}_OUTPUT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/dist/lib)
set(${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(${PROJECT_NAME}_VENDORS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendors)

set(${PROJECT_NAME}_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(${PROJECT_NAME}_TESPERIMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tesperiment)

# files
file(GLOB_RECURSE SOURCES ${${PROJECT_NAME}_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${${PROJECT_NAME}_INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE EMBEDDED ${${PROJECT_NAME}_SOURCE_DIR}/impl/*.hpp)
file(GLOB_RECURSE TESTS ${${PROJECT_NAME}_TESTS_DIR}/*.cpp)

# toto engine
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw OpenGL GLEW glm stb openal)
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_VENDORS_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_LIB})
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_LIB})
Cool__target_copy_folder(${PROJECT_NAME} ${${PROJECT_NAME}_INCLUDE_DIR})

# tesperiment
file(GLOB TESPERIMENT_FOLDERS ${${PROJECT_NAME}_TESPERIMENT_DIR}/*)
foreach(TESPERIMENT_FOLDER ${TESPERIMENT_FOLDERS})
    if(IS_DIRECTORY ${TESPERIMENT_FOLDER})
        get_filename_component(TESPERIMENT_FOLDER_NAME ${TESPERIMENT_FOLDER} NAME)
        message(STATUS "Adding exe: ${PROJECT_NAME}_test_${TESPERIMENT_FOLDER_NAME}")
        file(GLOB_RECURSE TESPERIMENT_SOURCES ${TESPERIMENT_FOLDER}/*.cpp)
        file(GLOB_RECURSE TESPERIMENT_HEADERS ${TESPERIMENT_FOLDER}/*.hpp)
        set(TESPERIMENT_OUTPUT ${PROJECT_NAME}_test_${TESPERIMENT_FOLDER_NAME})
        add_executable(${TESPERIMENT_OUTPUT} ${TESPERIMENT_SOURCES} ${TESPERIMENT_HEADERS})
        target_link_libraries(${TESPERIMENT_OUTPUT} PUBLIC ${PROJECT_NAME} imgui stb)
        target_include_directories(${TESPERIMENT_OUTPUT} PUBLIC ${${PROJECT_NAME}_INCLUDE_DIR} ${TESPERIMENT_FOLDER})
        set_target_properties(${TESPERIMENT_OUTPUT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_OUTPUT_BIN}/${PROJECT_NAME}_test_${TESPERIMENT_FOLDER_NAME})
        Toto__target_copy_folder_absolute_paths(${TESPERIMENT_OUTPUT} ${TESPERIMENT_FOLDER}/assets ${${PROJECT_NAME}_OUTPUT_BIN}/${PROJECT_NAME}_test_${TESPERIMENT_FOLDER_NAME}/assets)
        if (MSVC)
            target_compile_options(${TESPERIMENT_OUTPUT} PRIVATE /W3)
        else()
            target_compile_options(${TESPERIMENT_OUTPUT} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
        endif()
    endif()
endforeach()
