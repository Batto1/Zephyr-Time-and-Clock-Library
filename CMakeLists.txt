# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(time_and_clock_utils)

target_sources(app PRIVATE src/main.c)


target_include_directories(app PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_sources            (app PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/src/time_and_clock_utils.c)
