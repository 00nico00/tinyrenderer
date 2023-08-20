cmake_minimum_required(VERSION 3.24)
project(tinyrenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include_directories(./include)

set(HEADERS
    include/tgaimage.h
    include/model.h
    include/geometry.h
)

set(SOURCES
    src/tgaimage.cpp
    src/model.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
