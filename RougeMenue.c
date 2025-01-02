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
void init(){
    initscr();
    noecho();
    cbreak();
    curs_set(0);
}
User users[1000];
int userCount=0;
void showMenu(){
    clear();
    printw("---Main menu---\n");
    printw("1.Create New User\n");
    printw("2.    Login\n");
    printw("3.     Exit\n");
    printw("Select An Option: ");

    refresh();
}
int ValidPassword(const char* password) {
    if (strlen(password) < 7) return 0;
    int Digit = 0, Upper = 0, Lower = 0;
    for (int i = 0; password[i] !=\0; i++) {
        if (digit(password[i])) Digit = 1;
        else if (upper(password[i])) Upper = 1;
        else if (lower(password[i])) Lower = 1;
    }
    return Digit && Upper && Lower;
}
int ValidEmail(const char* email) {
    const char* atSign = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    if(!atSign || !dot || atSign>=dot){
        return 0;
    }
    if(atSign == email){
        return 0;
    }
    if(strlen(dot+1)<1){
        return 0;
    }
    return 1;

}
void generateRandomPassword(char* password,) {
    const char* characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int numCharacters = strlen(characters);
    srand(time(NULL)); 
    for (int i = 0; i < 10; i++) {
        password[i] = characters[rand() % numCharacters];
    }
    password[10] = '\0'; 
}
void CreateUser(){
    clear();
    char username[50];
    char password[50];
    char email[50];

    printw("---Create A New User---\n");
    printw("Enter Username: ");
    echo();
    curs_set(1);
    getstr(username);
    noecho();
    curs_set(0);
    for(int i=0; i<userCount; i++){
        if(strcmp(users[i].username,username)==0){
            printw("Username is already exist!\n");
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
    if(!ValidEmail(email)){
        printw("Invalid email format; The format must be xxx@y.zzz\n");
        printw("Press any key to return to the menu...");
        getch();
        return;
    }
    printw("1.Enter Password normally:\n");
    printw("2.Generate random password\n");
    printw("Select an option\n");
    choice=getch()-'0';
    if(choice==1){
   printw("Enter Password: ");
    echo();
    curs_set(1);
    getstr(password);
    noecho();
    curs_set(0); 
    if(!ValidPassword(password)){
        printw("Invalid password! Password must have at least 7 characters include a number, uppercase and lowercase");
    }
    }
    else if(choice==2){
        GenerateRandomPassword(password);
        printw("\nGenerated password: %s\n",password);
    }
    else{
        printw("Invalid option\n");
        printw("Press any key to return to the menu...");
        getch();
        return;
    }
    User newUser;
    strcpy(newUser.username,username);
    strcpy(newUser.email,email);
    strcpy(newUser.password,password);
    save_user_to_file (newUser);
    users[userCount]=newUser;
    userCount++;
    printw("User successfully created\n");
    printw("Press any key to return to the menu...");
    getch();
}
void save_user_to_file(User user) {
    FILE *file = fopen(“users.dat”, "ab");
    if (!file) {
        printw("Error: Unable to open file for saving user data.\n");
        return;
    }
    fwrite(user,sizeof(User),1,file);

    fclose(file);
}
int loginUser(){
    clear();
    char username[50];
    char password[50];
    char email[50];
    printw("1.login with username:\n");
    printw("2.login with email: \n");
    printw("Select an option\n");
    choice=getch()-'0';
    if(choice==1){
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
    for(int i=0; i<userCount; i++){
        if(strcmp(users[i].username, username)==0 && strcmp(users[i].password, password)==0){
            printw("Logged in successfully ! Welcome, %s",username);
            printw("press any key to continue...");
            getch();
            return 1;

        }
    }
        printw("Invalid username or password!\n");
        printw("press any key to return to the menu...");
        getch();
        return 0;
    }
     if(choice==2){
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
    for(int i=0; i<userCount; i++){
        if(strcmp(users[i].email, email)==0 && strcmp(users[i].password, password)==0){
            printw("Logged in successfully ! Welcome, %s",users[i].username);
            printw("press any key to continue...");
            getch();
            return 1;
            }
    }
        printw("Invalid email or password!\n");
        printw("press any key to return to the menu...");
        getch();
        return 0;
    }
}
int main() {
    User newUser;
    init();
    int logged_in=0;
    int choice;
    while(1){
        showMenu();
        choice=getch()-'0';
        switch(choice){
            case 1:
             CreateUser();
             break;
             case 2:
             logged_in=loginUser();
             if(logged_in){
                play_game;
             }
             break;
             case 3:
             endwin();
            return 0;
            default:
            printw("Invalid choice! press any key to try again\n");
            getch();
        }
    }
    refresh();
    endwin();
    return 0;
}