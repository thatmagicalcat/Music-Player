#include "SideBar.h"

SideBar::SideBar():
    m_win(newwin(W_HEIGHT, W_WIDTH, 0, 0)),
    m_scroll(0),
    m_focus(false),
    m_selector(0) {

    refresh();
    update();
}

void SideBar::scrollMenu(SideBar::Direction axis) {
    if (axis == DOWN && m_vec.size() > LINES - 5 && m_scroll != m_vec.size() - 5) m_scroll++;
    if (axis == UP && m_scroll != 0) m_scroll--;

    update();
}

void SideBar::update() {
    constexpr static auto margin_y = 2;
    const static auto max_x = 3 * (W_WIDTH / 4);

    wclear(m_win);
    printHeader();

    if (m_focus) wattron(m_win, COLOR_PAIR(1));
    box(m_win, 0, 0);
    wattroff(m_win, COLOR_PAIR(1));

    if (!m_focus) wattron(m_win, COLOR_PAIR(1));
    mvwprintw(m_win, W_HEIGHT - 1, W_WIDTH / 2 - 2, "<s>");
    wattroff(m_win, COLOR_PAIR(1));

    // Print the menu
    for (auto c = m_scroll; c < LINES - 5 + m_scroll && c < m_vec.size(); ++c) {
        std::string current = m_vec[c];
        if (current.size() > max_x)
            current = current.substr(0, max_x - 3) + "...";

        // Selector stuff
        if (c - m_scroll == m_selector) {
            wattron(m_win, A_STANDOUT);
            for (auto i = max_x - 2; i > current.size(); --i) mvwaddch(m_win, c + margin_y - m_scroll, i, ' ');
        }

        // Print the actual menu item
        mvwprintw(m_win, c + margin_y - m_scroll, 1, "%s", current.c_str());

        // More selector stuff
        wattroff(m_win, A_STANDOUT);

        // Print the length
        mvwprintw(m_win, c + margin_y - m_scroll, max_x + 1, "%s", m_length[c].c_str());
    }

    wrefresh(m_win);
}

void SideBar::setFocus(bool b) {
    m_focus = b;
    update();
}

void SideBar::moveSelector(Direction dir) {
    if (!m_focus) return;

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

const std::string &SideBar::getCurrentItem() {
    return m_vec.at(m_selector + m_scroll);
}

void SideBar::printHeader() {
    wattron(m_win, A_BOLD | A_ITALIC);

    mvwprintw(m_win, 1, 1, "Name");
    mvwprintw(m_win, 1, 3 * (W_WIDTH / 4) + 1, "Length");

    wattroff(m_win, A_BOLD | A_ITALIC);
}

void SideBar::addItem(std::string item, std::string length) {
    m_vec.push_back(std::move(item));
    m_length.push_back(std::move(length));

    update();
}