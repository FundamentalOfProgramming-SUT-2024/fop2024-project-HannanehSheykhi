#include "common2.h"

// Define global variables
int current_track = 0; 
pid_t music_pid = 0;   
int game_difficulty = 1;
wchar_t hero_avatar = L'☺';

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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
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

// Define other functions
