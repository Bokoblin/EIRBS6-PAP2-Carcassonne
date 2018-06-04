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
option(ENABLE_UI "Enable UI" OFF)

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
#       UI SOURCES
#----------------------------

file(GLOB UI_SOURCE_FILES
        "src/ui/*.h"
        "src/ui/*.c"
        )

#----------------------------
#       CLIENTS SOURCES
#----------------------------

file(GLOB CLIENT_SOURCE_FILES
        "src/common/*.h"
        "src/common/*.c"
        "src/client/common_clients_functions/*.h"
        "src/client/common_clients_functions/*.c"
        )

#----------------------------
#       CONFIGURE BUILDING
#----------------------------

# Options
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tst)
endif()
if(ENABLE_UI)
    # SDL
    set(SDL2_TTF_DIR "${CMAKE_CURRENT_LIST_DIR}/src/ui/cmake")
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
    find_package(SDL2_TTF)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
endif()

# Client
add_library(client1 SHARED ${CLIENT_SOURCE_FILES} "src/client/client_4410_choose_always_random_valid.c")
add_library(client2 SHARED ${CLIENT_SOURCE_FILES} "src/client/client_4410_always_choose_lower_left.c")
add_library(client3 SHARED ${CLIENT_SOURCE_FILES} "src/client/client_4410_always_choose_upper_right.c")

# Server
if(ENABLE_UI)
    add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES} ${UI_SOURCE_FILES})
    target_compile_definitions(${APP_EXECUTABLE} PUBLIC USE_SDL=1)
    target_link_libraries(${APP_EXECUTABLE} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
else()
    add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES})
endif()
target_link_libraries(${APP_EXECUTABLE} ${CMAKE_DL_LIBS})
target_link_libraries(${APP_EXECUTABLE} client1)
target_link_libraries(${APP_EXECUTABLE} client2)
target_link_libraries(${APP_EXECUTABLE} client3)
