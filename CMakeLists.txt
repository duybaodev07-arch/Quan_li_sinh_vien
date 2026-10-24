cmake_minimum_required(VERSION 3.16)
project(QuanLySinhVien LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(QuanLySinhVien ${PROJECT_SOURCES})
target_include_directories(QuanLySinhVien PRIVATE include)

if(MSVC)
    target_compile_options(QuanLySinhVien PRIVATE /W4 /utf-8)
else()
    target_compile_options(QuanLySinhVien PRIVATE -Wall -Wextra -Wpedantic)
endif()
