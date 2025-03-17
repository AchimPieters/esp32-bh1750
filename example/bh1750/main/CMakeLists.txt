cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)

idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES freertos driver bh1750
)
