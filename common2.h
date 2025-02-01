#ifndef COMMON_H
#define COMMON_H

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAP_WIDTH 180
#define MAP_HEIGHT 50
#define MAX_ENEMIES 5
#define MAX_ITEMS 5
#define MAX_ROOMS 15
#define MAX_SPECIAL_ELEMENTS 20
#define MAX_BULLETS 10
#define MAX_GOLD 20
#define MAX_FLOORS 4
#define MAX_PASSWORD_DOORS 3
#define MAX_SECRET_DOORS 5
#define MAX_CODE_BUTTONS 3

// Struct declarations
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

typedef struct {
    int x, y;
    int health;
    int itemsCollected;
    int gold;
} GamePlayer;

typedef struct {
    int x, y, width, height;
} Room;

typedef struct {
    int x, y;
    int dx, dy;
    int isActive;
    Room *room;
} Bullet;

typedef struct {
    int x, y;
    int type;
    int health;
    int damage;
    int isAlive;
    int canChase;
    int chaseSteps;
    int canShoot;
    Room *room;
} Enemy;

typedef struct {
    int x, y;
    int isCollected;
} Item;

typedef struct {
    int x, y;
    char type;
    int isActive;
    int secretCode;
} SpecialElement;

typedef struct {
    int x, y;
    int value;
    int isCollected;
} Gold;

typedef struct {
    int x, y;
    int isActive;
    int linkedFloor;
} Stair;

typedef struct {
    char map[MAP_HEIGHT][MAP_WIDTH];
    Room rooms[MAX_ROOMS];
    Stair upStair;
    Stair downStair;
} Floor;

typedef struct {
    int x, y;
    int isDiscovered;
} SecretDoor;

typedef struct {
    int x, y;
    int isActive;
    int generatedCode;
    time_t generationTime;
    int isVisible;
    int linkedDoorIndex;
} CodeButton;

typedef struct {
    int x, y;
    int isActive;
    int isUnlocked;
    int requiredCode;
    int isLocked;
    int attemptsLeft;
} PasswordDoor;

// External global variable declarations
extern Player players[100];
extern User users[100];
extern int playerCount;
extern int userCount;
extern GamePlayer player;
extern Room rooms[MAX_ROOMS];
extern Enemy enemies[MAX_ENEMIES];
extern Item items[MAX_ITEMS];
extern SpecialElement specialElements[MAX_SPECIAL_ELEMENTS];
extern Bullet bullets[MAX_BULLETS];
extern Gold golds[MAX_GOLD];
extern Floor floors[MAX_FLOORS];
extern SecretDoor secretDoors[MAX_SECRET_DOORS];
extern CodeButton codeButtons[MAX_CODE_BUTTONS];
extern PasswordDoor passwordDoors[MAX_PASSWORD_DOORS];
extern char map[MAP_HEIGHT][MAP_WIDTH];
extern int seen[MAP_HEIGHT][MAP_WIDTH];
extern int playerGold;
extern int currentFloor;
extern int current_track;
extern pid_t music_pid;
extern int game_difficulty;
extern wchar_t hero_avatar;

// Function prototypes
void init();
void displayPage(Player players[], int start, int end, int count);
int ValidPassword(const char* password);
int ValidEmail(const char* email);
void saveUser(const User* user);
void generateRandomPassword(char* password, int length);
void recoverPassword(const char* username);
void showPreGameMenu(int option);
void CreateUser();
void showScoreboard(Player players[], int count);
void createNewFloor();
void placeUpStair();
void placeDownStair();
void renderMap();
void renderStair();
void renderItems();
void renderSpecialElements();
void renderGold();
void renderPlayer();
void renderStats();
void handlePlayerInput(int ch);
void handleCodeButtonInteraction();
void handleSpecialElementInteraction();
void collectGold();
void checkEnemyCollision();
void checkItemCollection();
void handleStairInteraction();
void moveEnemies();
void gameOverScreen();
void placeSpecialElements();
void placeGold();
void initializeGold();
void addSpecialElementsToMap();
void placeItemsInRooms();
void initializeItems();
void shootBulletsFromEnemies();
void renderEnemies();
void initializePlayer();
void generateRooms();
Room createRoom();
int checkOverlap(int x1, int y1, int x2, int y2);
int doesOverlap();
void addBullet();
void initializeBullets();
void initializeEnemies();
void handlePasswordDoorInteraction();
void updateCodeButtonVisibility();
void placeRandomPasswordDoor();
void placeRandomPasswordButton();
void initializePasswordDoors();
void initializeCodeButtons();
void placeRoom();
void discoverSecretDoors();
void placeSecretDoors();
void setupFloorTransition();
void connectRooms();
void updateVisibility();
void initializeMap();
void initializeSpecialElements();

// Music handling functions
extern const char *music_files[];
void play_music(const char *track);
void stop_music();
void handle_music_settings();
void handle_difficulty_settings();
void handle_avatar_settings();
void display_menu(WINDOW *menu_win, int highlight, char **choices, int n_choices);

#endif // COMMON_H
