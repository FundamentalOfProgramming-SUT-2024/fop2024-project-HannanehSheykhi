#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h> 
#include <signal.h>
//#include "common.h"
typedef struct {
    char username[50];
    char password[50];
    char email[100];
} User;

typedef struct {
    char username[50];
    int score;
    int gold;
    int gameplayed;
    char title[20];
    int experience;
} Player;

Player players[100];
User users[100];
int playerCount;
int userCount;

// Function Prototypes
void init();
void displayPage(Player players[], int start, int end, int count);
int ValidPassword(const char* password);
int ValidEmail(const char* email);
void saveUser(const User* user);
void generateRandomPassword(char* password, int length);
void showPreGameMenu(int option);
void CreateUser();
void CreateGuestUser();
void showScoreboard(Player players[], int count);
void setting();
//void GameLevel();
void ChangeColor();
void ChooseMusic();
void sortPlayersByScore(Player players[], int count);
void addPlayer(Player players[], int *count, const char *username);
void updateScore(Player players[], int count, const char *username, int newScore);
void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices);
void play_music(const char *track);
void stop_music();
void handle_music_settings();
void handle_difficulty_settings();
void startNewGame();
void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices);

void init() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); // Enable function and arrow keys
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Define color pair for normal text
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Define color pair for highlighted text
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
}

void CreateUser() {
    User newUser;
    int isValid;
    char choice;

    // Username
    do {
        clear();
        mvprintw(0, 0, "Create a New User");
        mvprintw(2, 0, "Enter Username: ");
        move(2, 15); // Position cursor for username input
        echo();
        getstr(newUser.username);
        noecho();
        // You can add additional validation for username if needed
        isValid = strlen(newUser.username) > 0; // Basic non-empty validation
        if (!isValid) {
            mvprintw(4, 0, "Invalid Username! Press any key to retry...");
            getch();
        }
    } while (!isValid);

    // Password
    do {
        clear();
        mvprintw(0, 0, "Create a New User");
        mvprintw(2, 0, "Username: %s", newUser.username);
        mvprintw(3, 0, "Would you like to enter your own password or generate a random one?");
        mvprintw(4, 0, "Enter 'y' for your own password, 'r' for random: ");
        move(4, 56); // Position cursor for choice input
        choice = getch();
        if (choice == 'r') {
            generateRandomPassword(newUser.password, 10);
            mvprintw(5, 0, "Generated password: %s", newUser.password);
            isValid = 1;
        } else if (choice == 'y') {
            mvprintw(6, 0, "Enter Password: ");
            move(6, 15); // Position cursor for password input
            echo();
            getstr(newUser.password);
            noecho();
            isValid = ValidPassword(newUser.password);
            if (!isValid) {
                mvprintw(7, 0, "Invalid Password! Must be at least 7 characters with a digit, uppercase, and lowercase. Press any key to retry...");
                getch();
            }
        } else {
            mvprintw(5, 0, "Invalid choice! Press any key to retry...");
            getch();
            isValid = 0;
        }
    } while (!isValid);

    // Email
    do {
        clear();
        mvprintw(0, 0, "Create a New User");
        mvprintw(2, 0, "Username: %s", newUser.username);
        mvprintw(3, 0, "Password: %s", newUser.password);
        mvprintw(4, 0, "Enter Email: ");
        move(4, 12); // Position cursor for email input
        echo();
        getstr(newUser.email);
        noecho();
        isValid = ValidEmail(newUser.email);
        if (!isValid) {
            mvprintw(5, 0, "Invalid Email! The true form: example@example.com. Press any key to retry...");
            getch();
        }
    } while (!isValid);

    // Save User
    saveUser(&newUser);
    mvprintw(6, 0, "User created successfully!");
    refresh();
    getch();
}

void CreateGuestUser() {
    User guestUser;
    int isValid;
    // Username
    do {
        clear();
        mvprintw(0, 0, "Create a Guest User");
        mvprintw(2, 0, "Enter Guest Username: ");
        move(2, 22); // Position cursor for username input
        echo();
        getstr(guestUser.username);
        noecho();
        // You can add additional validation for username if needed
        isValid = strlen(guestUser.username) > 0; // Basic non-empty validation
        if (!isValid) {
            mvprintw(4, 0, "Invalid Username! Press any key to retry...");
            getch();
        }
    } while (!isValid);
    strcpy(guestUser.password, "");
    strcpy(guestUser.email, "");
    addPlayer(players, &playerCount, guestUser.username);
    mvprintw(6, 0, "Guest user created successfully!");
    refresh();
    getch();
}

int ValidPassword(const char* password) {
    if (strlen(password) < 7) return 0;
    int Digit = 0, Upper = 0, Lower = 0;
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) Digit = 1;
        else if (isupper(password[i])) Upper = 1;
        else if (islower(password[i])) Lower = 1;
    }
    return Digit && Upper && Lower;
}

int ValidEmail(const char* email) {
    const char* atSign = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    return atSign && dot && (dot > atSign);
}

void saveUser(const User* user) {
    FILE* file = fopen("users.dat", "ab");
    if (file) {
        fwrite(user, sizeof(User), 1, file);
        fclose(file);
    }
}

void generateRandomPassword(char* password, int length) {
    const char* characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int numCharacters = strlen(characters);

    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < length; i++) {
        password[i] = characters[rand() % numCharacters];
    }
    password[length] = '\0'; // Null-terminate the string
}

void startNewGame() {
    CreateUser();
    clear();
    mvprintw(10, 10, "Starting New Game...");
    refresh();
    getch();
}

void startNewGuestGame() {
    CreateGuestUser();
    clear();
    mvprintw(10, 10, "Starting New Game as Guest...");
    refresh();
    getch();
}

void showPreGameMenu(int option) {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 10, "---Welcome to the game!---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(4));

    mvprintw(4, 10, (option == 0 ? "> " : "  "));
    printw("Start New Game as Regular User\n");

    mvprintw(5, 10, (option == 1 ? "> " : "  "));
    printw("Start New Game as Guest\n");

    mvprintw(6, 10, (option == 2 ? "> " : "  "));
    printw("Continue Last Game\n");

    mvprintw(7, 10, (option == 3 ? "> " : "  "));
    printw("View Scoreboard\n");

    mvprintw(8, 10, (option == 4 ? "> " : "  "));
    printw("Settings\n");

    mvprintw(9, 10, (option == 5 ? "> " : "  "));
    printw("Profile\n");

        mvprintw(10, 10, (option == 6 ? "> " : "  "));
    printw("Back to Main Menu\n");

    attroff(COLOR_PAIR(4));
    refresh();
}

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

void addPlayer(Player players[], int *count, const char *username) {
    snprintf(players[*count].username, sizeof(players[*count].username), "%s", username);
    players[*count].score = 0;
    players[*count].gold = 0;
    players[*count].gameplayed = 0;
    players[*count].experience = 0;
    strcpy(players[*count].title, "Player");
    (*count)++;
}

void updateScore(Player players[], int count, const char *username, int newScore) {
    for (int i = 0; i < count; i++) {
        if (strcmp(players[i].username, username) == 0) {
            players[i].score += newScore;
            players[i].gameplayed++;
            break;
        }
    }
}

void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices) {
    int x = 2, y = 2, i;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i) {
        if(highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        y++;
    }
    wrefresh(menu_win);
}

int main() {
    setlocale(LC_ALL, ""); // Ensure proper locale settings
    init();
    int choice = 0;
    showPreGameMenu(choice);
    while (1) {
        int ch = getch();
        if (ch == KEY_UP && choice > 0) {
            choice--;
        } else if (ch == KEY_DOWN && choice < 6) {
            choice++;
        } else if (ch == '\n') {
            break;
        }
        showPreGameMenu(choice);
    }
    switch (choice) {
        case 0:
            startNewGame();
            break;
        case 1:
            startNewGuestGame();
            break;
        case 2:
            clear();
            mvprintw(10, 10, "Continuing Previous Game...");
            refresh();
            getch();
            // Continue Previous Game Code
            break;
        case 3:
            showScoreboard(players, playerCount);
            break;
        case 4:
            clear();
            mvprintw(10, 10, "Opening Settings...");
            refresh();
            getch();
            // Settings Code
            break;
        case 5:
            clear();
            mvprintw(10, 10, "Opening Profile...");
            refresh();
            getch();
            // Profile Code
            break;
        case 6:
            clear();
            mvprintw(10, 10, "Returning to Main Menu...");
            refresh();
            getch();
            // Main Menu Code
            break;
        default:
            mvprintw(10, 10, "Invalid option! Please try again.");
            getch();
    }
    refresh();
    endwin();
    return 0;
}


