#include "Settings.h"

Settings::Settings():
    m_scroll(0),
    m_selector(0),
    m_win(newwin(LINES - 1, COLS, 0, 0)) {

    refresh();
    update();
}

void Settings::update() {
    wclear(m_win);
    box(m_win, 0, 0);

    printMenu();
    wrefresh(m_win);
}

void Settings::printMenu() {
    for (int c = m_scroll; c < LINES - 2 && c < m_vec.size(); ++c) {
        auto current = m_vec[c];

        if (c - m_scroll == m_selector) {
            wattron(m_win, A_STANDOUT);
            for (auto i = COLS - 2; i > current.size(); --i) mvwaddch(m_win, c + 1 - m_scroll, i, ' ');
        }

        mvwprintw(m_win, c + 1, 1, "%s", current.c_str());
        wattroff(m_win, A_STANDOUT);
    }
}

void Settings::moveSelector(Settings::Direction dir) {
    if (dir == UP) {
        if (m_selector != 0) -- m_selector;
        else if (m_scroll != 0) scrollMenu(UP);
    } if (dir == DOWN && m_selector != m_vec.size() - m_scroll - 1 && m_selector <= LINES - 6) {
        if (m_selector == LINES - 8)
            scrollMenu(DOWN);
        else ++ m_selector;
    }

    update();
}

void Settings::scrollMenu(Settings::Direction dir) {
    if (dir == DOWN && m_vec.size() > LINES - 2 && m_scroll != m_vec.size() - 5) m_scroll++;
    if (dir == UP && m_scroll != 0) m_scroll--;

    update();
}

void Settings::performAction() {
    // Get the current item

    // Edit song path
//    if (m_selector == 0) {
//        wclear(m_win);
//    }
}
