#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    char username[50];
    char password[50];
    char email[100];
    char firstLogin[50];
} User;

void init(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE); // Enable arrow keys
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); // Define color pair for normal text
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Define color pair for highlighted text
}

User users[1000];
int userCount = 0;

void showMenu(int highlight) {
    clear();
    char *choices[] = { "Create New User", "Login", "Exit" };
    int n_choices = sizeof(choices) / sizeof(char *);
    attron(COLOR_PAIR(1) | A_BOLD);
    printw("---Main menu---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    for (int i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            attron(A_REVERSE | COLOR_PAIR(2)); // Highlight the current choice
            mvprintw(i + 1, 0, "%s", choices[i]);
            attroff(A_REVERSE | COLOR_PAIR(2));
        } else {
            mvprintw(i + 1, 0, "%s", choices[i]);
        }
    }
    refresh();
}

int ValidPassword(const char* password) {
    if (strlen(password) < 7) return 0;
    int Digit = 0, Upper = 0, Lower = 0;
    for (int i = 0; password[i] !='\0'; i++) {
        if (isdigit(password[i])) Digit = 1;
        else if (isupper(password[i])) Upper = 1;
        else if (islower(password[i])) Lower = 1;
    }
    return Digit && Upper && Lower;
}

int ValidEmail(const char* email) {
    const char* atSign = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    if (!atSign || !dot || atSign >= dot) {
        return 0;
    }
    if (atSign == email) {
        return 0;
    }
    if (strlen(dot + 1) < 1) {
        return 0;
    }
    return 1;
}

void generateRandomPassword(char* password) {
    const char* characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int numCharacters = strlen(characters);
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        password[i] = characters[rand() % numCharacters];
    }
    password[10] = '\0';
}

void save_user_to_file(User user) {
    FILE *file = fopen("users.dat", "ab");
    if (!file) {
        attron(COLOR_PAIR(4));
        printw("Error: Unable to open file for saving user data.\n");
        attroff(COLOR_PAIR(4));
        return;
    }
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
}

void CreateUser(){
    clear();
    char username[50];
    char password[50];
    char email[50];
    time_t certainTime = time(NULL);
    int choice;
    attron(COLOR_PAIR(1) | A_BOLD);
    printw("---Create A New User---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(4));
    printw("Enter Username: ");
    echo();
    curs_set(1);
    getstr(username);
    noecho();
    curs_set(0);
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            mvprintw(3, 0, "Username is already exist!\n");
            printw("Press any key to return to the menu...");
            getch();
            return;
        }
    }
    printw("Enter Email: ");
    echo();
    curs_set(1);
    getstr(email);
    noecho();
    curs_set(0);
    if (!ValidEmail(email)) {
        printw("Invalid email format; The format must be xxx@y.zzz\n");
        printw("Press any key to return to the menu...");
        getch();
        return;
    }
    printw("1. Enter Password normally:\n");
    printw("2. Generate random password\n");
    printw("Select an option\n");
    choice = getch() - '0';
    if (choice == 1) {
        printw("Enter Password: ");
        echo();
        curs_set(1);
        getstr(password);
        noecho();
        curs_set(0);
        if (!ValidPassword(password)) {
            printw("Invalid password! Password must have at least 7 characters include a number, uppercase and lowercase");
            printw("Press any key to return to the menu...");
            getch();
            return;
        }
    } else if (choice == 2) {
        generateRandomPassword(password);
        printw("\nGenerated password: %s\n", password);
    } else {
        printw("Invalid option\n");
        printw("Press any key to return to the menu...");
        attroff(COLOR_PAIR(4));
        getch();
        return;
    }
    User newUser;
    strcpy(newUser.username, username);
    strcpy(newUser.email, email);
    strcpy(newUser.password, password);
    strcpy(newUser.firstLogin, ctime(&certainTime));
    save_user_to_file(newUser);
    users[userCount] = newUser;
    userCount++;
    attron(COLOR_PAIR(4));
    printw("User successfully created\n");
    printw("Press any key to return to the menu...");
    attroff(COLOR_PAIR(4));
    getch();
}

int loginUser() {
    clear();
    char username[50];
    char password[50];
    char email[50];
    int choice;
    attron(COLOR_PAIR(4));
    printw("1. Login with username:\n");
    printw("2. Login with email: \n");
    printw("Select an option\n");
    choice = getch() - '0';
    if (choice == 1) {
        printw("Enter Username: ");
        echo();
        curs_set(1);
        getstr(username);
        noecho();
        curs_set(0);
        printw("Enter Password: ");
        echo();
        curs_set(1);
        getstr(password);
        noecho();
        curs_set(0);
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                printw("Logged in successfully! Welcome, %s", username);
                printw("Press any key to continue...");
                getch();
                return 1;
            }
        }
        printw("Invalid username or password!\n");
        printw("Press any key to return to the menu...");
        getch();
        return 0;
    } else if (choice == 2) {
        printw("Enter Email: ");
        echo();
        curs_set(1);
        getstr(email);
        noecho();
        curs_set(0);
        printw("Enter password: ");
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
        return 0; // Ensure proper return value
    }
}

void forgotPassword() {
    clear();
    char email[50];
    printw("---Forgot Password---\n");
    printw("Enter Email: ");
    echo();
    curs_set(1);
    getstr(email);
    noecho();
    curs_set(0);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0) {
            char newPassword[50];
            generateRandomPassword(newPassword);
            strcpy(users[i].password, newPassword);
            printw("Your new password has been sent to your email.\n");
            printw("Press any key to return to the menu...");
            getch();
            return;
        }
    }
    printw("Email not found.\n");
    printw("Press any key to return to the menu...");
    getch();
}
int main() {
    User newUser;
    init();
    int logged_in = 0;
    int highlight = 1;
    int ch;
    int choice;

    while (1) {
        showMenu(highlight);
        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = 3;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == 3)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key
                choice = highlight;
                switch (choice) {
                    case 1:
                        CreateUser();
                        break;
                    case 2:
                        logged_in = loginUser();
                        if (logged_in) {
                            // play_game();
                        }
                        break;
                    case 3:
                        endwin();
                        return 0;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    refresh();
    endwin();
    return 0;
}

