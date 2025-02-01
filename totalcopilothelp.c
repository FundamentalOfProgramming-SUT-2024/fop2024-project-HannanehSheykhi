        curs_set(0);
        printw("Enter Password: ");
        echo();
        curs_set(1);
        getstr(password);
        noecho();
        curs_set(0);
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0) {
                printw("Logged in successfully! Welcome, %s", users[i].username);
                printw("Press any key to continue...");
                getch();
                return 1;
            }
        }
        printw("Invalid email or password!\n");
        printw("Press any key to return to the menu...");
        getch();
        return 0;
    } else {
        printw("Invalid option\n");
        printw("Press any key to return to the menu...");
        attroff(COLOR_PAIR(4));
        getch();
        return 0;
    }
}

// Pre-Game Menu
void showPreGameMenu() {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 10, "---Welcome to the game!---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(4));
    mvprintw(4, 10, "1. Start New Game\n");
    mvprintw(5, 10, "2. Continue Last Game\n");
    mvprintw(6, 10, "3. View Scoreboard\n");
    mvprintw(7, 10, "4. Settings\n");
    mvprintw(8, 10, "5. Profile\n");
    mvprintw(9, 10, "6. Back to Main Menu\n");
    mvprintw(10, 10, "Select an option: ");
    attroff(COLOR_PAIR(4));
    refresh();
}

// Sort Players by Score
void sortPlayersByScore(Player players[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].score < players[j + 1].score || 
                (players[j].score == players[j + 1].score && strcmp(players[j].username, players[j + 1].username) > 0)) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }
    if (count > 0) strcpy(players[0].title, "GOAT");
    if (count > 1) strcpy(players[1].title, "Legend");
    if (count > 2) strcpy(players[2].title, "Champion");
}

// Add Player
void addPlayer(Player players[], int *count, const char *username) {
    snprintf(players[*count].username, sizeof(players[*count].username), "%s", username);
    players[*count].score = 0;
    players[*count].gold = 0;
    players[*count].gameplayed = 0;
    players[*count].experience = 0;
    strcpy(players[*count].title, "Player");
    (*count)++;
}

// Update Player Score
void updateScore(Player players[], int count, const char *username, int newScore) {
    for (int i = 0; i < count; i++) {
        if (strcmp(players[i].username, username) == 0) {
            players[i].score += newScore;
            players[i].gameplayed++;
            break;
        }
    }
}

// Display Page
void displayPage(Player players[], int start, int end, int count) {
    clear();
    mvprintw(2, 10, "Scoreboard - Page %d", start / 10 + 1);
    mvprintw(4, 2, "Rank\tUsername\tScore\tGold\tGames Played\tExperience\tTitle");

    for (int i = start; i < end; i++) {
        if (i < 3) {
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
        } else {
            attron(COLOR_PAIR(3));
        }
        mvprintw(6 + i - start, 2, "%d\t%s\t\t%d\t%d\t%d\t\t%d\t%s", 
                 i + 1, players[i].username, players[i].score, 
                 players[i].gold, players[i].gameplayed, 
                 players[i].experience, players[i].title);

        if (i < 3) {
            attroff(A_BOLD);
            attroff(COLOR_PAIR(1));
        } else {
            attroff(COLOR_PAIR(3));
        }
    }

    attron(COLOR_PAIR(4));
    mvprintw(6, 70, "🏆");
    mvprintw(7, 70, "🥈");
    mvprintw(8, 70, "🥉");
    attroff(COLOR_PAIR(4));

    mvprintw(18, 10, "Press LEFT/RIGHT to scroll, 'q' to return to menu...");
    refresh();
}

// Show Scoreboard
void showScoreboard(Player players[], int count) {
    int page = 0;
    int totalPages = (count + 10 - 1) / 10;
    while (1) {
        int start = page * 10;
        int end = start + 10;

        if (end > count) {
            end = count;
        }

        displayPage(players, start, end, count);

        int ch = getch();
        if (ch == KEY_RIGHT && page < totalPages - 1) {
            page++;
        } else if (ch == KEY_LEFT && page > 0) {
            page--;
        } else if (ch == 'q') {
            break;
        }
    }
}

// Music Handling Functions
void play_music(const char *track) {
    if ((music_pid = fork()) == 0) {
        execlp("vlc", "vlc", track, NULL);
        exit(0);
    }
}

void stop_music() {
    if (music_pid > 0) {
        kill(music_pid, SIGKILL);
        music_pid = 0;
    }
}

// Handle Difficulty Settings
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
                    mvprintw(22, 0, "Difficulty set to Easy.");
                } else if (strcmp(difficulty_choices[choice - 1], "Medium") == 0) {
                    game_difficulty = 2; // Medium
                    mvprintw(22, 0, "Difficulty set to Medium.");
                } else if (strcmp(difficulty_choices[choice - 1], "Hard") == 0) {
                    game_difficulty = 3; // Hard
                    mvprintw(22, 0, "Difficulty set to Hard.");
                } else if (strcmp(difficulty_choices[choice - 1], "Back") == 0) {
                    delwin(diff_win);
                    return;
                }
                refresh();
                break;
            default:
                break;
        }
    }
}

// Handle Music Settings
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
                    stop[_{{{CITATION{{{_1{](https://github.com/cornerstonecollege/401_02/tree/ccbfa3e822b913021d90d2d17bfd8825aec4d41d/PacMan%2Fmain.c)