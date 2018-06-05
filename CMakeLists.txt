cmake_minimum_required(VERSION 2.8)

#--------------------------------------------------------
#       PROJECT CONFIG
#--------------------------------------------------------

set(PROJECT_NAME EIRBS6-ProjetCarcasonne)
project(${PROJECT_NAME})

set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -g -O0")
set(CMAKE_C_STANDARD 99)

option(ENABLE_TESTS "Enable unit tests"     ON)
option(ENABLE_UI    "Enable user interface" ON)

#--------------------------------------------------------
#       SUBDIRS ADDING
#--------------------------------------------------------

add_subdirectory(src)
add_subdirectory(tst)
