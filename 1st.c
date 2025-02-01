#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    char username[50];
    char password[50];
    char email[100];
} User;

int ValidPassword(const char* password);
int ValidEmail(const char* email);
void saveUser(const User* user);
void generateRandomPassword(char* password, int length);
void recoverPassword(const char* username);

int main() {
    User newUser;
    initscr();
    noecho();
    cbreak();

    mvprintw(0, 0, "Create a New User");
    mvprintw(2, 0, "Enter Username: ");
    getstr(newUser.username);

    mvprintw(3, 0, "Enter Password: ");
    getstr(newUser.password);
    while (!ValidPassword(newUser.password)) {
        mvprintw(4, 0, "Invalid Password! Must be at least 7 characters with a digit, uppercase, and lowercase.");
        getstr(newUser.password);
    }
    mvprintw(5, 0, "Enter Email: ");
    getstr(newUser.email);
    while (!ValidEmail(newUser.email)) {
        mvprintw(6, 0, "Invalid Email! The true form: example@example.com.");
        getstr(newUser.email);
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
    endwin();
    return 0;
}

// Implementations for validation and saving functions
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
    FILE* file = fopen("users.dat", "rb");
    if (!file) {
        mvprintw(10, 0, "Error: Cannot open user data file.");
        return;
    }

    User user;
    int found = 0;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        char newPassword[50];
        generateRandomPassword(newPassword, 10);
        strcpy(user.password, newPassword);

        file = fopen("users.dat", "r+b");
        if (file) {
            while (fread(&user, sizeof(User), 1, file)) {
                if (strcmp(user.username, username) == 0) {
                    fseek(file, -sizeof(User), SEEK_CUR);
                    fwrite(&user, sizeof(User), 1, file);
                    break;
                }
            }
            fclose(file);
        }
        mvprintw(11, 0, "Password recovered successfully! New password: %s", newPassword);
    } else {
        mvprintw(11, 0, "User not found.");
    }
}
void recoverPassword(const char* username) {
    FILE* file = fopen("users.dat", "rb");
    if (!file) {
        mvprintw(10, 0, "Error: Cannot open user data file.");
        return;
    }

    User user;
    int found = 0;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (found) {
        char newPassword[50];
        generateRandomPassword(newPassword, 10);
        strcpy(user.password, newPassword);

        file = fopen("users.dat", "r+b");
        if (file) {
            while (fread(&user, sizeof(User), 1, file)) {
                if (strcmp(user.username, username) == 0) {
                    fseek(file, -sizeof(User), SEEK_CUR);
                    fwrite(&user, sizeof(User), 1, file);
                    break;
                }
            }
            fclose(file);
        }
        mvprintw(11, 0, "Password recovered successfully! New password: %s", newPassword);
    } else {
        mvprintw(11, 0, "User not found.");
    }
}
