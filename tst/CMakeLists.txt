#----------------------------
#       SET TEST EXECUTABLES
#----------------------------

set(TEST_BOARD test_board)
set(TEST_CARD test_card)
set(TEST_DECK test_deck)
set(TEST_STACK test_stack)
set(TEST_QUEUE test_queue)
set(TEST_SET test_set)


#----------------------------
#       SET TEST SOURCES
#----------------------------
file(GLOB TEST_BOARD_SOURCE_FILES
        "test_utils.h"
        "test_board.c"
        "../src/server/board.h"
        "../src/server/board.c"
        "../src/server/function_pointers.h"
        "../src/server/function_pointers.c"
        "../src/server/player.h"
        "../src/server/player.c"
        "../src/common/card.h"
        "../src/common/card.c"
        "../src/common/card_type.h"
        "../src/common/card_type.c"
        "../src/common/common_interface.h"
        "../src/common/ADT/set.h"
        "../src/common/ADT/set.c"
        "../src/common/ADT/stack.h"
        "../src/common/ADT/stack.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        )

file(GLOB TEST_CARD_SOURCE_FILES
        "test_utils.h"
        "test_card.c"
        "../src/common/card.c"
        "../src/common/card.h"
        "../src/common/card_type.h"
        "../src/common/card_type.c"
        "../src/common/card.c"
        "../src/common/ADT/stack.h"
        "../src/common/ADT/stack.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/common_interface.h"
        )

file(GLOB TEST_DECK_SOURCE_FILES
        "test_utils.h"
        "test_deck.c"
        "../src/common/deck.h"
        "../src/common/deck.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/ADT/stack.h"
        "../src/common/ADT/stack.c"
        "../src/common/common_interface.h"
        )

file(GLOB TEST_STACK_SOURCE_FILES
        "test_utils.h"
        "test_stack.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/ADT/stack.h"
        "../src/common/ADT/stack.c"
        )

file(GLOB TEST_QUEUE_SOURCE_FILES
        "test_utils.h"
        "test_queue.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/ADT/queue.h"
        "../src/common/ADT/queue.c"
        )

file(GLOB TEST_SET_SOURCE_FILES
        "test_utils.h"
        "test_set.c"
        "../src/common/utils.h"
        "../src/common/utils.c"
        "../src/common/ADT/set.h"
        "../src/common/ADT/set.c"
        )
        
#----------------------------
#       CONFIGURE BUILDING
#----------------------------

add_executable(${TEST_BOARD} ${TEST_BOARD_SOURCE_FILES})
target_link_libraries(${TEST_BOARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_CARD} ${TEST_CARD_SOURCE_FILES})
target_link_libraries(${TEST_CARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_DECK} ${TEST_DECK_SOURCE_FILES})
target_link_libraries(${TEST_DECK} ${CMAKE_DL_LIBS})
add_executable(${TEST_STACK} ${TEST_STACK_SOURCE_FILES})
target_link_libraries(${TEST_STACK} ${CMAKE_DL_LIBS})
add_executable(${TEST_QUEUE} ${TEST_QUEUE_SOURCE_FILES})
target_link_libraries(${TEST_QUEUE} ${CMAKE_DL_LIBS})
add_executable(${TEST_SET} ${TEST_SET_SOURCE_FILES})
target_link_libraries(${TEST_SET} ${CMAKE_DL_LIBS})

#----------------------------
#       CONFIGURE TESTS
#----------------------------

add_test(NAME ${TEST_BOARD} COMMAND ./${TEST_BOARD})
add_test(NAME ${TEST_CARD} COMMAND ./${TEST_CARD})
add_test(NAME ${TEST_DECK} COMMAND ./${TEST_DECK})
add_test(NAME ${TEST_STACK} COMMAND ./${TEST_STACK})
add_test(NAME ${TEST_QUEUE} COMMAND ./${TEST_QUEUE})
add_test(NAME ${TEST_SET} COMMAND ./${TEST_SET})
