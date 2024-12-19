cmake_minimum_required(VERSION 3.22)
project(CPP3_BrickGame_v2_0_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if(DEFINED ENV{QTDIR})
    set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(../../brick_game/snake)
include_directories(.)


add_executable(brick_game_desktop
        main.cpp
        
        menu_desktop_view.cpp
        menu_desktop_view.hpp
        menu_desktop_view.ui

        game_desktop_view.hpp
        game_desktop_view.cpp
        game_desktop_view.ui
    )

target_link_libraries(brick_game_desktop
        Qt::Core
        Qt::Gui
        Qt::Widgets
        dl
)
