#include "Application.h"

namespace Application {
    void run() {
        std::unordered_map<std::string, std::string> songMap;

        auto instance = VLC::Instance(0, nullptr);
        auto *media = new VLC::Media(instance, "", VLC::Media::FromType::FromPath);
        auto *mediaPlayer = new VLC::MediaPlayer(*media);

        std::chrono::milliseconds mediaStart;
        std::string songPath;

        initscr();
        noecho();
        cbreak();
        halfdelay(10);
        keypad(stdscr, true);

        start_color();

        // Used by SideBar.h and BottomBar.h
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);

        SideBar sideBar;
        BottomBar bottomBar;

        bool
                isPlaying = false,
                sideBar_focus = true,
                bottomBar_focus = false;

        const auto update_focus = [&]() -> void {
            sideBar.setFocus(sideBar_focus);
            bottomBar.setFocus(bottomBar_focus);
        };

        const auto playSong = [&](const std::string &path) -> void {
            // if the song is paused, and being playing again
            // we don't have to create another player,
            // it should resume playing
            if (songPath == path) {
                if (!mediaPlayer -> isPlaying()) mediaPlayer -> play();
                return;
            }

            // stop the music if its playing
            if (mediaPlayer -> isPlaying()) mediaPlayer -> stop();

            songPath = path;

            // delete and create new media and media player
            delete mediaPlayer;
            delete media;

            media = new VLC::Media(instance, path, VLC::Media::FromPath);
            mediaPlayer = new VLC::MediaPlayer(*media);

            mediaPlayer -> play();
        };

        const auto printHints = [&]() -> void {
            attron(COLOR_PAIR(2));
            mvprintw(LINES - 1, COLS / 2 - 8, " s ");
            attroff(COLOR_PAIR(2));
            mvprintw(LINES - 1, COLS / 2 - 4, "settings");
        };

        const auto getLength = [&](const std::string &song_path) -> std::string {
            static auto tmp_instance = VLC::Instance(0, nullptr);
            auto tmp_media = VLC::Media(tmp_instance, song_path, VLC::Media::FromPath);

            tmp_media.parseWithOptions(VLC::Media::ParseFlags::Local, 10000);

            while (tmp_media.parsedStatus() != VLC::Media::ParsedStatus::Done) {}

            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::milliseconds(tmp_media.duration()));
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);

            return std::to_string(minutes.count()) + ":" + std::to_string(seconds.count() % 60);
        };

        std::ifstream in("path.txt");
        std::string buffer;

        while (std::getline(in, buffer)) {
            if (!fs::exists(buffer)) {
                endwin();
                std::cerr << "Invalid path: '" << buffer << "'\n";
            }

            for (const auto &entry: fs::directory_iterator(buffer)) {
                if (!entry.is_directory() && isMusicFile(entry.path())) {
                    songMap[entry.path().filename().string()] = entry.path().string();
                    sideBar.addItem(entry.path().filename().string(), getLength(entry.path().string()));
                }
            }
        }

        in.close();

        update_focus();

        int ch;
        while ((ch = getch()) != 'q') {
            printHints();

            if (ch == KEY_UP) sideBar.moveSelector(SideBar::UP);
            else if (ch == KEY_DOWN) sideBar.moveSelector(SideBar::DOWN);

            else if (ch == ' ' && bottomBar_focus) {
                if (bottomBar.isPlaying()) {
                    bottomBar.pause();
                    mediaPlayer -> pause();
                } else {
                    bottomBar.play(sideBar.getCurrentItem());
                    playSong(songMap.at(sideBar.getCurrentItem()));
                }
            } else if (ch == '\n' && sideBar_focus) {
                bottomBar.play(sideBar.getCurrentItem());
                playSong(songMap.at(sideBar.getCurrentItem()));
            }

            else if (ch == 'b' || ch == 'B') {
                sideBar_focus = !sideBar_focus;
                if (sideBar_focus) bottomBar_focus = false;

                update_focus();
            } else if (ch == 'p' || ch == 'P') {
                bottomBar_focus = !bottomBar_focus;
                if (bottomBar_focus) sideBar_focus = false;

                update_focus();
            }

            else if (ch == 'm' || ch == 'M') {
                bottomBar.setMute(!bottomBar.isMute());
                mediaPlayer -> setMute(bottomBar.isMute());
            }

            else if (ch == 's' || ch == 'S') {
                clear();
                printHints();

                Settings s;

                char chr;
                while ((chr = getch()) != 's')
                    if (chr == '\n') s.performAction();

                sideBar.refreshWindow();
                bottomBar.refreshWindow();
            }

            if (mediaPlayer -> isValid())
                bottomBar.updateTime(std::chrono::milliseconds(mediaPlayer->time()), std::chrono::milliseconds(mediaPlayer -> length()));

            if (mediaPlayer -> state() == libvlc_Ended) // finished playing
                bottomBar.finishPlaying();
        }


        getch();
        endwin();
    }

    bool isMusicFile(const fs::path &ext) {
        auto str = ext.extension().string();
        return str == ".mp3"
               || str == ".m4a"
               || str == ".ogg";
    }
}