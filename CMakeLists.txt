cmake_minimum_required(VERSION 3.26)
project(Backend)

set(CMAKE_CXX_STANDARD 17)


find_package(cpprestsdk CONFIG REQUIRED)

add_executable(Backend main.cpp
        server.cpp
        server.h)

target_link_libraries(Backend PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
