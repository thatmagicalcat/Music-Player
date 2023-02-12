cmake_minimum_required(VERSION 3.24)
project(music_player)

set(CMAKE_CXX_STANDARD 20)

add_executable(music_player src/main.cpp src/SideBar.cpp include/SideBar.h src/BottomBar.cpp include/BottomBar.h src/Application.cpp include/Application.h)

target_include_directories(music_player PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(music_player vlc ncurses)
