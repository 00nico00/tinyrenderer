cmake_minimum_required(VERSION 3.24)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./include)

set(HEADERS
    include/tgaimage.h
)

set(SOURCES
    src/tgaimage.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
