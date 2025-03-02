cmake_minimum_required(VERSION 3.10)
project(pbf)

set(CMAKE_CXX_STANDARD 17)

add_executable(pbf
    src/main.cpp
    src/color_manager.cpp
    src/tag_manager.cpp
    src/file_manager.cpp
)

install(TARGETS pbf DESTINATION bin)
