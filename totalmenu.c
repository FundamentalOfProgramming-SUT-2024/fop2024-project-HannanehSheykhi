#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

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
   char title [20];
   int experience;
} Player;

Player players[100];
User users[100];
int playerCount;
int userCount;

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

// Function prototypes
void displayPage(Player players[], int start, int end, int count);
int ValidPassword(const char* password);
int ValidEmail(const char* email);
void saveUser(const User* user);
void generateRandomPassword(char* password, int length);
void recoverPassword(const char* username);
void showPreGameMenu(int option);
void CreateUser();
void showScoreboard(Player players[], int count);
void setting();
void GameLevel();
void ChangeColor();
void ChooseMusic();

void CreateUser() {
    User newUser;
    clear();
    mvprintw(0, 0, "Create a New User");
    mvprintw(2, 0, "Enter Username: ");
    echo();
    getstr(newUser.username);
    noecho();

    mvprintw(3, 0, "Enter Password: ");
    echo();
    getstr(newUser.password);
    noecho();

    while (!ValidPassword(newUser.password)) {
        mvprintw(4, 0, "Invalid Password! Must be at least 7 characters with a digit, uppercase, and lowercase.");
        echo();
        getstr(newUser.password);
        noecho();
    }
    mvprintw(5, 0, "Enter Email: ");
    echo();
    getstr(newUser.email);
    noecho();

    while (!ValidEmail(newUser.email)) {
        mvprintw(6, 0, "Invalid Email! The true form: example@example.com.");
        echo();
        getstr(newUser.email);
        noecho();
    }
    saveUser(&newUser);
    mvprintw(7, 0, "User created successfully!");
    mvprintw(8, 0, "Try random password? (y/n): ");
    char choice = getch();
    if (choice == 'y' || choice == 'Y') {
        generateRandomPassword(newUser.password, 10);
        mvprintw(9, 0, "Generated password: %s", newUser.password);
    }
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

void recoverPassword(const char* username) {
    // Placeholder for password recovery logic
}

void showPreGameMenu(int option) {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 10, "---Welcome to the game!---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(4));

    mvprintw(4, 10, (option == 0 ? "> " : "  "));
    printw("Start New Game\n");

    mvprintw(5, 10, (option == 1 ? "> " : "  "));
    printw("Continue Last Game\n");

    mvprintw(6, 10, (option == 2 ? "> " : "  "));
    printw("View Scoreboard\n");

    mvprintw(7, 10, (option == 3 ? "> " : "  "));
    printw("Settings\n");

    mvprintw(8, 10, (option == 4 ? "> " : "  "));
    printw("Profile\n");

    mvprintw(9, 10, (option == 5 ? "> " : "  "));
    printw("Back to Main Menu\n");

    attroff(COLOR_PAIR(4));
    refresh();
}

void setting() {
    clear();
    mvprintw(2, 10, "Choose an option:\n");
    mvprintw(4, 10, "1. Game Level\n");
    mvprintw(5, 10, "2. Change Color\n");
    mvprintw(6, 10, "3. Choose Music\n");
    mvprintw(7, 10, "4. Exit\n");
    refresh();

    int choice = getch() - '0';
    switch (choice) {
        case 1:
            GameLevel();
            break;
        case 2:
            ChangeColor();
            break;
        case 3:
            ChooseMusic();
            break;
        default:
            mvprintw(9, 10, "Invalid choice! Press any key to try again.");
            getch();
            setting();
    }
}

void GameLevel() {
    clear();
    mvprintw(2, 10, "Select Game Level:\n");
    mvprintw(4, 10, "1. Easy\n");
    mvprintw(5, 10, "2. Medium\n");
    mvprintw(6, 10, "3. Hard\n");
    refresh();
    getch();
}

void ChangeColor() {
    clear();
    mvprintw(2, 10, "Change Color:\n");
    mvprintw(4, 10, "1. Red\n");
    mvprintw(5, 10, "2. Green\n");
    mvprintw(6, 10, "3. Blue\n");
    refresh();
    getch();
}

void ChooseMusic() {
    clear();
    mvprintw(2, 10, "Choose Music:\n");
    mvprintw(4, 10, "1. Rock\n");
    mvprintw(5, 10, "2. Jazz\n");
    mvprintw(6, 10, "3. Classical\n");
    refresh();
    getch();
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

int main() {
    setlocale(LC_ALL, ""); // Ensure proper locale settings
    init();
    int choice = 0;
    showPreGameMenu(choice);
    while (1) {
        int ch = getch();
        if (ch == KEY_UP && choice > 0) {
            choice--;
        } else if (ch == KEY_DOWN && choice < 5) {
            choice++;
        } else if (ch == '\n') {
            break;
        }
        showPreGameMenu(choice);
    }
    switch (choice) {
        case 0:
            clear();
            mvprintw(10, 10, "Starting New Game...");
            break;
        case 1:
            clear();
            mvprintw(10, 10, "Continuing Previous Game...");
            break;
        case 2:
            showScoreboard(players, playerCount);
            break;
        case 3:
            setting();
            break;
        case 4:
            // Profile
            break;
        case 5:
            endwin();
            return 0;
        default:
            mvprintw(10, 10, "Invalid option! Please try again ");
            getch();
    }
    refresh();
    endwin();
    return 0;
}

