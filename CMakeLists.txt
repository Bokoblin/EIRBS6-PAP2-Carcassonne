cmake_minimum_required(VERSION 2.8)

#----------------------------
#       PROJECT CONFIG
#----------------------------

set(PROJECT_NAME EIRBS6-ProjetCarcasonne)
project(${PROJECT_NAME})
set(APP_EXECUTABLE server)
set(TEST_EXECUTABLE app_test)

set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99 -g -O0")
set(CMAKE_C_STANDARD 99)

#----------------------------
#       ENABLE BUILD TARGETS
#----------------------------
option(ENABLE_TESTS "Enable unit tests" ON)
option(ENABLE_UI "Enable server UI" OFF)

#----------------------------
#       SERVER SOURCES
#----------------------------

file(GLOB SERVER_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/common/ADT/*.h"
        "src/common/ADT/*.c"
        "src/server/*.h"
        "src/server/*.c"
        )


#----------------------------
#       CLIENTS SOURCES
#----------------------------

file(GLOB CLIENT1_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/client/common_clients_functions/*.h"
        "src/client/common_clients_functions/*.c"
        "src/client/client_4410_random_valid.c"
        )

file(GLOB CLIENT2_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/client/common_clients_functions/*.h"
        "src/client/common_clients_functions/*.c"
        "src/client/client_4410_always_first.c"
        )

file(GLOB CLIENT3_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/client/common_clients_functions/*.h"
        "src/client/common_clients_functions/*.c"
        "src/client/client_4410_random_valid_2_tmp.c"
        )


#----------------------------
#       CONFIGURE BUILDING
#----------------------------

# Client
add_library(client1 SHARED ${CLIENT1_SOURCE_FILES})
add_library(client2 SHARED ${CLIENT2_SOURCE_FILES})
add_library(client3 SHARED ${CLIENT3_SOURCE_FILES})

# Server
add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES})
target_link_libraries(${APP_EXECUTABLE} ${CMAKE_DL_LIBS})
target_link_libraries(${APP_EXECUTABLE} client1)
target_link_libraries(${APP_EXECUTABLE} client2)
target_link_libraries(${APP_EXECUTABLE} client3)


if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tst)
endif()
if(ENABLE_UI)
    add_subdirectory(src/ui)
endif()

