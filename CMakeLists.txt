cmake_minimum_required(VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_PREFIX_PATH "/home/siarshai/Programming/Qt5.12.11/5.12.11/gcc_64/lib/cmake")

set(PROJECT_NAME same_game_plus)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -Wall -Werror -std=c++11 -fstrict-aliasing -pedantic-errors -pedantic -Wno-deprecated-declarations -Wno-unused-variable")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

add_library(common STATIC
        common/boom_block.h
        common/boom_block.cpp)
target_link_libraries(common PUBLIC Qt5::Core)

add_library(scores_sorting STATIC
        scoreboard/scores_sorting.h
        scoreboard/scores_sorting.cpp)
target_link_libraries(scores_sorting PUBLIC Qt5::Core)
add_library(scoreboard STATIC
        scoreboard/scoreboard.cpp
        scoreboard/scoreboard.h
        scoreboard/iscoreboard.h
        scoreboard/scoreboard_factory.h
        scoreboard/scores_context.h
        scoreboard/scores_context.cpp)
target_link_libraries(scoreboard PRIVATE scores_sorting)

add_library(game_board STATIC
            game_board.h
            game_board.cpp)
target_link_libraries(game_board PUBLIC common Qt5::Core)

add_library(level_loader STATIC
            level_loader/level_loader.cpp
            level_loader/level_loader.h
            level_loader/ilevel_loader.h)
target_link_libraries(level_loader PUBLIC common Qt5::Core)


set(MAIN_SOURCES
        main.cpp
        game_backend.cpp
        game_backend.h)
qt5_add_resources(QT_RESOURCES qml.qrc)
add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${QT_RESOURCES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
            Qt5::Core Qt5::Widgets Qt5::Quick stdc++fs
            game_board level_loader common
            scores_sorting scoreboard)


add_executable(${PROJECT_NAME}_test
        main_test_all.cpp
        game_board_test.cpp
        scoreboard/scores_sorting_test.cpp)

target_link_libraries(
        ${PROJECT_NAME}_test
        Qt5::Core gtest gtest_main gmock pthread
        scores_sorting game_board)

