#--------------------------------------------------------
#       SET EXECUTABLES
#--------------------------------------------------------

set(CLIENT_RANDOM_VALID client_random_valid)
set(CLIENT_LOWER_LEFT   client_lower_left)
set(CLIENT_UPPER_RIGHT  client_upper_right)
set(APP_EXECUTABLE      server)


#--------------------------------------------------------
#       SET SOURCES
#--------------------------------------------------------
file(GLOB CLIENT_SOURCE_FILES
        "common/*.c"
        "client/common_clients_functions/*.c"
        )

file(GLOB SERVER_SOURCE_FILES
        "common/*.c"
        "common/ADT/*.c"
        "server/*.c"
        )

file(GLOB UI_SOURCE_FILES "ui/*.c")

#--------------------------------------------------------
#       CONFIGURE BUILDING
#--------------------------------------------------------
if(ENABLE_UI)
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    if(SDL2_FOUND)
        pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
        set(SDL2_TTF_DIR "${CMAKE_CURRENT_LIST_DIR}/ui/cmake")
        find_package(SDL2_TTF)
        include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
    else()
        set(ENABLE_UI OFF)
    endif()
endif()

#--------------------------------------------------------
#       TARGETS CONFIG
#--------------------------------------------------------

# Client
add_library(${CLIENT_RANDOM_VALID}  SHARED ${CLIENT_SOURCE_FILES} "client/client_4410_choose_always_random_valid.c")
add_library(${CLIENT_LOWER_LEFT}    SHARED ${CLIENT_SOURCE_FILES} "client/client_4410_always_choose_lower_left.c")
add_library(${CLIENT_UPPER_RIGHT}   SHARED ${CLIENT_SOURCE_FILES} "client/client_4410_always_choose_upper_right.c")

# Server
if(ENABLE_UI)
    add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES} ${UI_SOURCE_FILES})
    target_compile_definitions(${APP_EXECUTABLE} PUBLIC USE_SDL=1)
    target_link_libraries(${APP_EXECUTABLE} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
else()
    add_executable(${APP_EXECUTABLE} ${SERVER_SOURCE_FILES})
endif()

target_link_libraries(${APP_EXECUTABLE} ${CMAKE_DL_LIBS})
target_link_libraries(${APP_EXECUTABLE} ${CLIENT_RANDOM_VALID})
target_link_libraries(${APP_EXECUTABLE} ${CLIENT_LOWER_LEFT})
target_link_libraries(${APP_EXECUTABLE} ${CLIENT_UPPER_RIGHT})
