cmake_minimum_required(VERSION 3.10)
project(FileTools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable color output
if(CMAKE_CXX_COMPILER_ID GNU)
    add_compile_options(-fdiagnostics-color=always)
endif()

add_executable(file_tools main.cpp)

# Link pthread on Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(file_tools pthread)
endif()

# Install target
install(TARGETS file_tools DESTINATION bin)
