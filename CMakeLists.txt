cmake_minimum_required(VERSION 3.16)
project(QuanLySinhVien LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB APP_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(QuanLySinhVien ${APP_SOURCES})
target_include_directories(QuanLySinhVien PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
    target_compile_options(QuanLySinhVien PRIVATE /W4 /utf-8)
else()
    target_compile_options(QuanLySinhVien PRIVATE -Wall -Wextra -Wpedantic)
endif()

