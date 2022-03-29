cmake_minimum_required(VERSION 3.21)
project(hwrt C)

set(CMAKE_C_STANDARD 23)


add_executable(hwrt main.c utils.h ${SOURCE_FILES} service.h)

