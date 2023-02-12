#ifndef MUSIC_PLAYER_SETTINGS_H
#define MUSIC_PLAYER_SETTINGS_H

#include <string>
#include <vector>

#include <ncurses.h>

class Settings {
public:
    Settings();

    enum Direction {
        UP, DOWN,
    };

    enum Options {
        SongsPath,
    };

public:
    void moveSelector(Direction dir);
    void scrollMenu(Direction dir);
    void performAction();

private:
    void printMenu();
    void update();

private:
    int m_selector, m_scroll;

    std::vector<std::string> m_vec = {
        "Edit Song Path",
    };

    WINDOW *m_win;
};


#endif // !MUSIC_PLAYER_SETTINGS_H