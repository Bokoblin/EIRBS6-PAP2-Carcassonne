#----------------------------
#       SET TEST EXECUTABLES
#----------------------------

set(TEST_BOARD test_board)
set(TEST_CARD test_card)
set(TEST_STACK test_stack)
set(TEST_QUEUE test_queue)


#----------------------------
#       SET TEST SOURCES
#----------------------------
file(GLOB TEST_BOARD_SOURCE_FILES
        "test_utils.h"
        "test_board.c"
        "../src/common/board.h"
        "../src/common/board.c"
        "../src/common/common_interface.h"
        )

file(GLOB TEST_CARD_SOURCE_FILES
        "test_utils.h"
        "test_card.c"
        "../src/common/card.h"
        "../src/common/card.c"
        "../src/common/common_interface.h"
        )

file(GLOB TEST_STACK_SOURCE_FILES
        "test_utils.h"
        "test_stack.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/stack.h"
        "../src/common/stack.c"
        )

file(GLOB TEST_QUEUE_SOURCE_FILES
        "test_utils.h"
        "test_queue.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/queue.h"
        "../src/common/queue.c"
        )
      
#----------------------------
#       CONFIGURE BUILDING
#----------------------------

add_executable(${TEST_BOARD} ${TEST_BOARD_SOURCE_FILES})
target_link_libraries(${TEST_BOARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_CARD} ${TEST_CARD_SOURCE_FILES})
target_link_libraries(${TEST_CARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_STACK} ${TEST_STACK_SOURCE_FILES})
target_link_libraries(${TEST_STACK} ${CMAKE_DL_LIBS})
add_executable(${TEST_QUEUE} ${TEST_QUEUE_SOURCE_FILES})
target_link_libraries(${TEST_QUEUE} ${CMAKE_DL_LIBS})

#----------------------------
#       CONFIGURE TESTS
#----------------------------

add_test(NAME ${TEST_BOARD} COMMAND ./${TEST_BOARD})
add_test(NAME ${TEST_CARD} COMMAND ./${TEST_CARD})
add_test(NAME ${TEST_STACK} COMMAND ./${TEST_STACK})
add_test(NAME ${TEST_QUEUE} COMMAND ./${TEST_QUEUE})
