#ifndef MUSIC_PLAYER_APPLICATION_H
#define MUSIC_PLAYER_APPLICATION_H

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>

#include <ncurses.h>
#include <vlcpp/vlc.hpp>

#include "SideBar.h"
#include "BottomBar.h"

namespace fs = std::filesystem;

namespace Application {
    void run();
    bool isMusicFile(const fs::path &ext);
}


#endif // !MUSIC_PLAYER_APPLICATION_H
