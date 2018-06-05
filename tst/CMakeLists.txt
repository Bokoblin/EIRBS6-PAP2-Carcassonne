if(ENABLE_TESTS)

#--------------------------------------------------------
#       SET TEST EXECUTABLES
#--------------------------------------------------------

set(TEST_BOARD test_board)
set(TEST_CARD test_card)
set(TEST_DECK test_deck)
set(TEST_MEEPLE test_meeple)
set(TEST_QUEUE test_queue)
set(TEST_SET test_set)
set(TEST_STACK test_stack)
set(TEST_ZONE test_zone)
set(TEST_CLIENT test_client)

#--------------------------------------------------------
#       SET TEST SOURCES
#--------------------------------------------------------
file(GLOB TEST_BOARD_SOURCE_FILES
        "common_tests_utils.c"
        "test_board.c"
        "../src/server/board.c"
        "../src/common/ADT/*.c"
        "../src/common/*.c"
        )

file(GLOB TEST_CARD_SOURCE_FILES
        "common_tests_utils.c"
        "test_card.c"
        "../src/common/ADT/stack.c"
        "../src/common/card.c"
        "../src/common/interface.c"
        "../src/common/utils.c"
        )

file(GLOB TEST_DECK_SOURCE_FILES
        "common_tests_utils.c"
        "test_deck.c"
        "../src/common/ADT/stack.c"
        "../src/common/card.c"
        "../src/common/deck.c"
        "../src/common/interface.c"
        "../src/common/utils.c"
        )

file(GLOB TEST_MEEPLE_SOURCE_FILES
        "common_tests_utils.c"
        "test_meeple.c"
        "../src/common/card.c"
        "../src/common/interface.c"
        "../src/common/meeple.c"
        "../src/common/utils.c"
        "../src/common/ADT/stack.c"
        )

file(GLOB TEST_QUEUE_SOURCE_FILES
        "common_tests_utils.c"
        "test_queue.c"
        "../src/common/ADT/queue.c"
        "../src/common/utils.c"
        )

file(GLOB TEST_SET_SOURCE_FILES
        "common_tests_utils.c"
        "test_set.c"
        "../src/common/ADT/set.c"
        "../src/common/utils.c"
        )

file(GLOB TEST_STACK_SOURCE_FILES
        "common_tests_utils.c"
        "test_stack.c"
        "../src/common/ADT/stack.c"
        "../src/common/utils.c"
        )

file(GLOB TEST_ZONE_SOURCE_FILES
        "common_tests_utils.c"
        "test_zone.c"
        "../src/common/ADT/set.c"
        "../src/common/area_type.c"
        "../src/common/card.c"
        "../src/common/interface.c"
        "../src/common/meeple.c"
        "../src/common/utils.c"
        "../src/common/zone.c"
        )

file(GLOB TEST_CLIENT_SOURCE_FILES
        "common_tests_utils.c"
        "test_client.c"
        "../src/common/ADT/set.c"
        "../src/common/card.c"
        "../src/common/interface.c"
        "../src/common/utils.c"
        "../src/client/common_clients_functions/*.c"
        )

#--------------------------------------------------------
#       CONFIGURE BUILDING
#--------------------------------------------------------

add_executable(${TEST_BOARD} ${TEST_BOARD_SOURCE_FILES})
target_link_libraries(${TEST_BOARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_CARD} ${TEST_CARD_SOURCE_FILES})
target_link_libraries(${TEST_CARD} ${CMAKE_DL_LIBS})
add_executable(${TEST_DECK} ${TEST_DECK_SOURCE_FILES})
target_link_libraries(${TEST_DECK} ${CMAKE_DL_LIBS})
add_executable(${TEST_QUEUE} ${TEST_QUEUE_SOURCE_FILES})
target_link_libraries(${TEST_QUEUE} ${CMAKE_DL_LIBS})
add_executable(${TEST_MEEPLE} ${TEST_MEEPLE_SOURCE_FILES})
target_link_libraries(${TEST_MEEPLE} ${CMAKE_DL_LIBS})
add_executable(${TEST_SET} ${TEST_SET_SOURCE_FILES})
target_link_libraries(${TEST_SET} ${CMAKE_DL_LIBS})
add_executable(${TEST_STACK} ${TEST_STACK_SOURCE_FILES})
target_link_libraries(${TEST_STACK} ${CMAKE_DL_LIBS})
add_executable(${TEST_ZONE} ${TEST_ZONE_SOURCE_FILES})
target_link_libraries(${TEST_ZONE} ${CMAKE_DL_LIBS})
add_executable(${TEST_CLIENT} ${TEST_CLIENT_SOURCE_FILES})
target_link_libraries(${TEST_CLIENT} ${CMAKE_DL_LIBS})

#--------------------------------------------------------
#       CONFIGURE TESTS
#--------------------------------------------------------

add_test(NAME ${TEST_BOARD}     COMMAND make ${TEST_BOARD})
add_test(NAME ${TEST_CARD}      COMMAND make ${TEST_CARD})
add_test(NAME ${TEST_CLIENT}    COMMAND make ${TEST_CLIENT})
add_test(NAME ${TEST_DECK}      COMMAND make ${TEST_DECK})
add_test(NAME ${TEST_MEEPLE}    COMMAND make ${TEST_MEEPLE})
add_test(NAME ${TEST_QUEUE}     COMMAND make ${TEST_QUEUE})
add_test(NAME ${TEST_SET}       COMMAND make ${TEST_SET})
add_test(NAME ${TEST_STACK}     COMMAND make ${TEST_STACK})
add_test(NAME ${TEST_ZONE}      COMMAND make ${TEST_ZONE})

enable_testing()

endif()