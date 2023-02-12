#ifndef MUSIC_PLAYER_BOTTOMBAR_H
#define MUSIC_PLAYER_BOTTOMBAR_H

#include <chrono>
#include <string>
#include <optional>

#include <ncurses.h>

class BottomBar {
public:
    BottomBar();

public:
    void setFocus(bool focus);
    void play(const std::string &title);
    void pause();
    void updateTime(std::chrono::milliseconds originalTime, std::chrono::milliseconds currentTime);
    void finishPlaying();
    void setMute(bool mute);
    void refreshWindow();

    [[nodiscard]] bool isMute() const;
    [[nodiscard]] bool isPlaying() const;

private:
    void setTitle(const std::string &title);
    void update();

private:
    bool m_mute, m_focus, m_isPlaying;

    std::optional<std::string> m_title;
    WINDOW *m_win;
};

#endif // !MUSIC_PLAYER_BOTTOMBAR_H
