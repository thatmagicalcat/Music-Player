#include "BottomBar.h"

BottomBar::BottomBar():
    m_mute(false),
    m_win(newwin(3, COLS, LINES - 3, 0)),
    m_focus(false),
    m_isPlaying(false) {

    refresh();
    update();
}

void BottomBar::setTitle(const std::string &title) {
    if (title.size() > COLS / 2) m_title = title.substr(0, COLS / 2 - 3) + "...";
    else m_title = title;

    update();
}

void BottomBar::update() {
    wclear(m_win);

    if (m_focus) wattron(m_win, COLOR_PAIR(1));
    box(m_win, 0, 0);
    wattroff(m_win, COLOR_PAIR(1));

    if (!m_focus) wattron(m_win, COLOR_PAIR(1));
    mvwprintw(m_win, 2, COLS / 2 - 2, "<p>");
    wattroff(m_win, COLOR_PAIR(1));

    mvwprintw(m_win, 1, 1, "%s", m_title.value_or("").c_str());
    mvwprintw(m_win, 1, COLS - 3, m_isPlaying && m_title.has_value() ? "||":"|>");

    if (m_mute) wattron(m_win, A_STANDOUT);
    mvwprintw(m_win, 1, COLS - 8, "mute");
    wattroff(m_win, A_STANDOUT);

    wrefresh(m_win);
}

void BottomBar::setFocus(bool focus) {
    m_focus = focus;

    update();
}

void BottomBar::play(const std::string &title) {
    setTitle(title);
    m_isPlaying = true;

    update();
}

void BottomBar::pause() {
    m_isPlaying = false;
    update();
}

bool BottomBar::isPlaying() const {
    return m_isPlaying;
}

void BottomBar::updateTime(std::chrono::milliseconds originalTime, std::chrono::milliseconds currentTime) {
    auto original_seconds = std::chrono::duration_cast<std::chrono::seconds>(originalTime);
    auto original_minutes = std::chrono::duration_cast<std::chrono::minutes>(original_seconds);

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);

    mvwprintw(m_win, 1, COLS - 25, "%02ld:%02ld / %02ld:%02ld", minutes.count(), seconds.count() % 60, original_minutes.count(), original_seconds.count() % 60);
    wrefresh(m_win);
}

void BottomBar::finishPlaying() {
    m_title = {};
    m_isPlaying = false;
    update();
}

void BottomBar::setMute(bool mute) {
    if (!m_focus) return;

    m_mute = mute;
    update();
}

bool BottomBar::isMute() const {
    return m_mute;
}
