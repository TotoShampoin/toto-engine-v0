set(STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendors/stb)
file(GLOB_RECURSE STB_HEADERS ${STB_INCLUDE_DIR}/*.h)

add_library(stb INTERFACE)

target_include_directories(stb INTERFACE ${STB_INCLUDE_DIR})
