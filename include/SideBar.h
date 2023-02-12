#ifndef MUSIC_PLAYER_SIDEBAR_H
#define MUSIC_PLAYER_SIDEBAR_H

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <string>

#include <ncurses.h>

#define W_HEIGHT (LINES - 3)
#define W_WIDTH COLS

class SideBar {
public:
    SideBar();

    enum Direction { UP, DOWN };

public:
    void setFocus(bool b);
    void addItem(std::string item, std::string length);
    void moveSelector(Direction dir);
    const std::string &getCurrentItem();

private:
    void scrollMenu(Direction axis);
    void update();
    void printHeader();

private:
    uint32_t m_scroll, m_selector;
    bool m_focus;

    std::vector<std::string> m_vec, m_length;

    WINDOW *m_win;
};


#endif // !MUSIC_PLAYER_SIDEBAR_H
