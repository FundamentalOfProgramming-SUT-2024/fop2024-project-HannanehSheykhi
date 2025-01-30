#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>

const char *music_files[] = {
    "/home/hannaneh/Documents/fop2024-project-HannanehSheykhi/asli.mp3",
    "/home/hannaneh/Documents/fop2024-project-HannanehSheykhi/1.mp3",
    "/home/hannaneh/Documents/fop2024-project-HannanehSheykhi/2.mp3"
};

int current_track = 0; 
pid_t music_pid = 0;   
int game_difficulty = 1;
wchar_t hero_avatar = L'😊'; // Default hero avatar

void play_music(const char *track);
void stop_music();
void handle_music_settings();
void handle_difficulty_settings();
void handle_avatar_settings();
void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices);

int main() {
    setlocale(LC_ALL, ""); 
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // White text on black background
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); 

    int startx = 0, starty = 0;
    int width = 30, height = 10;
    int highlight = 1, choice = 0, c;
    char *choices[] = {
        "Adjust Difficulty",
        "Change Hero Avatar",
        "Music Settings",
        "Exit"
    };
    int n_choices = sizeof(choices) / sizeof(char *);

    clear();
    noecho();
    cbreak();
    startx = (80 - width) / 2;
    starty = (24 - height) / 2;
    WINDOW *menu_win = newwin(height, width, starty, startx);
    keypad(menu_win, TRUE);
    attron(COLOR_PAIR(1));
    bkgd(COLOR_PAIR(1));
    mvprintw(0, 0, "Use arrow keys to navigate. Press Enter to select.");
    refresh();
    
    while (1) {
        display_menu(menu_win, highlight, choices, n_choices);
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: 
                choice = highlight;
                if (strcmp(choices[choice - 1], "Adjust Difficulty") == 0) {
                    handle_difficulty_settings();
                } else if (strcmp(choices[choice - 1], "Change Hero Avatar") == 0) {
                    handle_avatar_settings();
                } else if (strcmp(choices[choice - 1], "Music Settings") == 0) {
                    handle_music_settings();
                } else if (strcmp(choices[choice - 1], "Exit") == 0) {
                    stop_music(); 
                    endwin();
                    return 0;
                }
                break;
            default:
                break;
        }
    }
    endwin();
    return 0;
}

void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices) {
    int x = 2, y = 2, i;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE | COLOR_PAIR(1));
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE | COLOR_PAIR(1));
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        y++;
    }
    wrefresh(menu_win);
}

void handle_difficulty_settings() {
    char *difficulty_choices[] = {
        "Easy",
        "Medium",
        "Hard",
        "Back"
    };
    int n_choices = sizeof(difficulty_choices) / sizeof(char *);
    int highlight = game_difficulty, choice = 0, c; 
    WINDOW *diff_win = newwin(10, 30, 10, 30);
    keypad(diff_win, TRUE);

    while (1) {
        display_menu(diff_win, highlight, difficulty_choices, n_choices);
        c = wgetch(diff_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10: 
                choice = highlight;
                if (strcmp(difficulty_choices[choice - 1], "Easy") == 0) {
                    game_difficulty = 1; // Easy
                    mvprintw(22, 0, "Difficulty set to Easy.            ");
                } else if (strcmp(difficulty_choices[choice - 1], "Medium") == 0) {
                    game_difficulty = 2; // Medium
                    mvprintw(22, 0, "Difficulty set to Medium.          ");
                } else if (strcmp(difficulty_choices[choice - 1], "Hard") == 0) {
                    game_difficulty = 3; // Hard
                    mvprintw(22, 0, "Difficulty set to Hard.            ");
                } else if (strcmp(difficulty_choices[choice - 1], "Back") == 0) {
                    delwin(diff_win);
                    return;
                }
                refresh();
                break;
            default:
                printw("Invalid option ");
                break;
        }
    }
}

void handle_avatar_settings() {
    wchar_t *avatar_choices[] = {
        L"😊",
        L"🙂",
        L"😁",
        L"Back"
    };
    int n_choices = sizeof(avatar_choices) / sizeof(wchar_t *);
    int highlight = 1, choice = 0, c; 
    WINDOW *avatar_win = newwin(10, 30, 10, 30);
    keypad(avatar_win, TRUE);

    while (1) {
        box(avatar_win, 0, 0);
        for (int i = 0; i < n_choices; ++i) {
            if (highlight == i + 1) {
                wattron(avatar_win, A_REVERSE | COLOR_PAIR(1));
                mvwprintw(avatar_win, i + 2, 2, "%ls", avatar_choices[i]);
                wattroff(avatar_win, A_REVERSE | COLOR_PAIR(1));
            } else {
                mvwprintw(avatar_win, i + 2, 2, "%ls", avatar_choices[i]);
            }
        }
        wrefresh(avatar_win);

        c = wgetch(avatar_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: 
                choice = highlight;
                if (highlight != n_choices) {
                    hero_avatar = *avatar_choices[choice - 1];
                    mvprintw(22, 0, "Hero avatar set to %ls", avatar_choices[choice - 1]);
                } else {
                    delwin(avatar_win);
                    return;
                }
                refresh();
                break;
            default:
                break;
        }
    }
}

void handle_music_settings() {
    char *music_choices[] = {
        "Play Current Track",
        "Stop Music",
        "Next Track",
        "Previous Track",
        "Back"
    };
    int n_choices = sizeof(music_choices) / sizeof(char *);
    int highlight = 1, choice = 0, c;
    WINDOW *music_win = newwin(10, 30, 10, 30);
    keypad(music_win, TRUE);

    while (1) {
        display_menu(music_win, highlight, music_choices, n_choices);
        c = wgetch(music_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case 10: 
                choice = highlight;
                if (strcmp(music_choices[choice - 1], "Play Current Track") == 0) {
                    stop_music();
                    play_music(music_files[current_track]);
                } else if (strcmp(music_choices[choice - 1], "Stop Music") == 0) {
                    stop_music();
                } else if (strcmp(music_choices[choice - 1], "Next Track") == 0) {
                    stop_music();
                                    current_track = (current_track + 1) % (sizeof(music_files) / sizeof(char *));
                    play_music(music_files[current_track]);
                } else if (strcmp(music_choices[choice - 1], "Previous Track") == 0) {
                    stop_music();
                    current_track = (current_track - 1 + (sizeof(music_files) / sizeof(char *))) % (sizeof(music_files) / sizeof(char *));
                    play_music(music_files[current_track]);
                } else if (strcmp(music_choices[choice - 1], "Back") == 0) {
                    delwin(music_win);
                    return;
                }
                refresh();
                break;
            default:
                printw("Invalid command ");
                break;
        }
    }
}

void play_music(const char *track) {
    if ((music_pid = fork()) == 0) {
        // Suppress mpg123 output
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        execlp("mpg123", "mpg123", track, NULL); 
        exit(0);
    }
}

void stop_music() {
    if (music_pid > 0) {
        kill(music_pid, SIGTERM);
        music_pid = 0;
    }
}
