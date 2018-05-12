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
        "src/server/board.h"
        "src/server/board.c"
        "src/client/client_4410_1.c"
        "src/client/client.h"
        "src/client/auxfunctions.c"
)

file(GLOB CLIENT2_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/server/board.h"
        "src/server/board.c"
        "src/client/client_4410_2.c"
        "src/client/client.h"
        "src/client/auxfunctions.c"
)

file(GLOB CLIENT3_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/server/board.h"
        "src/server/board.c"
        "src/client/client_4410_3.c"
        "src/client/client.h"
        "src/client/auxfunctions.c"
)

file(GLOB CLIENT4_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/server/board.h"
        "src/server/board.c"
        "src/client/client_4410_4.c"
        "src/client/client.h"
        "src/client/auxfunctions.c"
)

#----------------------------
#       CONFIGURE BUILDING
#----------------------------

# Client
add_library(client1 SHARED ${CLIENT1_SOURCE_FILES})
add_library(client2 SHARED ${CLIENT2_SOURCE_FILES})
add_library(client3 SHARED ${CLIENT3_SOURCE_FILES})
add_library(client4 SHARED ${CLIENT4_SOURCE_FILES})

# Server
add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES})
target_link_libraries(${APP_EXECUTABLE} ${CMAKE_DL_LIBS})
target_link_libraries(${APP_EXECUTABLE} client1)
target_link_libraries(${APP_EXECUTABLE} client2)
target_link_libraries(${APP_EXECUTABLE} client3)
target_link_libraries(${APP_EXECUTABLE} client4)


if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tst)
endif()

