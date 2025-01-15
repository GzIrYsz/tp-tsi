cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME lib_name)
string(TOUPPER "${lib_name}" lib_name_upper)

FILE(GLOB LIB_SOURCES src/*.c)

add_library(${lib_name} STATIC ${LIB_SOURCES})
target_include_directories(${lib_name} PUBLIC includes)

add_executable("${lib_name}_example" examples/example.c)
target_link_libraries("${lib_name}_example" PRIVATE ${lib_name})