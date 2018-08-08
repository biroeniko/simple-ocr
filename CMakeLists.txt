cmake_minimum_required(VERSION 2.8)

project(ocr)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_${lang}_FLAGS_RELEASE_INIT "-O3 -DNDEBUG")

set(SOURCES
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
