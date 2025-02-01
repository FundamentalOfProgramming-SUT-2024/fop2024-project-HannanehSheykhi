#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h> 
#include <signal.h>
#include <wchar.h>
#include "common.h"

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

/*master key --> \u25B2
 gold --> \u26c0
  black gold --> \u26c2
   Health enchant --> \u2695
    Speed enchant --> \u26f7
    Damage enchant --> \u2620
     dagger --> \u2020wand
      -->  \u269A
       arrow --> \u27B3
       sword --> \u2694
        food --> \u2299
        floor --> \u2022*/

/*typedef struct {
    int x, y;
    int health;
    int itemsCollected;
} Player;


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
} PasswordDoor;*/

CodeButton codeButtons[MAX_CODE_BUTTONS];
PasswordDoor passwordDoors[MAX_PASSWORD_DOORS];
int lastValidCode = -1;
SecretDoor secretDoors[MAX_SECRET_DOORS];
char map[MAP_HEIGHT][MAP_WIDTH];
int seen[MAP_HEIGHT][MAP_WIDTH];
Player player;
Enemy enemies[MAX_ENEMIES];
Item items[MAX_ITEMS];
Room rooms[MAX_ROOMS];
SpecialElement specialElements[MAX_SPECIAL_ELEMENTS];
Bullet bullets[MAX_BULLETS];
Gold golds[MAX_GOLD];
int playerGold = 0; 
Floor floors[MAX_FLOORS];
int currentFloor = 0;
wchar_t playerChar[];



void initializeSpecialElements() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        specialElements[i].isActive = 0; 
    }
}

void initializeMap() {
    for (int i = 1; i < MAP_HEIGHT; i++) {
        for (int j = 1; j < MAP_WIDTH; j++) {
            if (i == 1 || i == MAP_HEIGHT - 1 || j == 1 || j == MAP_WIDTH - 1) {
                map[i][j] = '*';
            } else {
                map[i][j] = ' ';
            }
            seen[i][j] = 0;

        }
    }
}

void renderMap() {
    for (int i = 1; i < MAP_HEIGHT; i++) {
        for (int j = 1; j < MAP_WIDTH; j++) {
            if (map[i][j] == '*') {
                mvaddch(i, j, map[i][j]);
            } else if (seen[i][j]) {
                mvaddch(i, j, map[i][j]);
            } else {
                mvaddch(i, j, ' ');
            }
        }
    }
}

void updateVisibility() {
    for (int i = player.y - 5; i <= player.y + 5; i++) {
        for (int j = player.x - 5; j <= player.x + 5; j++) {
            if(map[player.y][player.x]=='.' && map[i][j]=='#'){}
            else if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
                seen[i][j] = 1;
            }
        }
    }
}
void connectRooms(Room room1, Room room2) {
    int x1, y1, x2, y2;

   do{if (rand() % 2 == 0) {
        x1 = room1.x + rand() % room1.width;  
        y1 = (rand() % 2 == 0) ? room1.y : room1.y + room1.height - 1;
    } else {
        y1 = room1.y + rand() % room1.height;  
        x1 = (rand() % 2 == 0) ? room1.x : room1.x + room1.width - 1;
    }

    if (rand() % 2 == 0) {
        x2 = room2.x + rand() % room2.width;
        y2 = (rand() % 2 == 0) ? room2.y : room2.y + room2.height - 1;
    } else {
        y2 = room2.y + rand() % room2.height;
        x2 = (rand() % 2 == 0) ? room2.x : room2.x + room2.width - 1;
    }}
    while(checkOverlap(x1,y1,x2,y2));
    
    map[y1][x1] = '+';
    map[y2][x2] = '+';
    
    while (x1 != x2) {
        x1 += (x2 > x1) ? 1 : -1;
        map[y1][x1] = '#';  
    }

    while (y1 != y2) {
        map[y1][x1] = '#'; 
        y1 += (y2 > y1) ? 1 : -1;

    }
}

/*void initializeQueue(Queue* queue) {
    queue->front = 0;
    queue->rear = 0;
}

void enqueue(Queue* queue, Point p) {
    queue->points[queue->rear++] = p;
}

Point dequeue(Queue* queue) {
    return queue->points[queue->front++];
}

bool isQueueEmpty(Queue* queue) {
    return queue->front == queue->rear;
}

bool isValidPoint(int x, int y) {
    return (x >= 0 && x < MAX_MAP_WIDTH && y >= 0 && y < MAX_MAP_HEIGHT);
}

void connectRooms(Room room1, Room room2) {
    bool visited[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {false};
    Point parent[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
    Queue queue;
    initializeQueue(&queue);

    Point start = {room1.x + room1.width / 2, room1.y + room1.height / 2};
    Point end = {room2.x + room2.width / 2, room2.y + room2.height / 2};

    enqueue(&queue, start);
    visited[start.y][start.x] = true;
    parent[start.y][start.x] = start;

    bool found = false;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (!isQueueEmpty(&queue) && !found) {
        Point current = dequeue(&queue);

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (isValidPoint(nx, ny) && !visited[ny][nx] && map[ny][nx] != '#') {
                visited[ny][nx] = true;
                parent[ny][nx] = current;
                enqueue(&queue, (Point){nx, ny});

                if (nx == end.x && ny == end.y) {
                    found = true;
                    break;
                }
            }
        }
    }
}
    if (found) {
        Point p = end;
        while (!(p.x == start.x && p.y == start.y)) {
            map[p.y][p.x] = '#';
            p = parent[p.y][p.x];
        }
    }*/



void placeUpStair(int floorNumber, int linkedFloor) {
    int placed = 0;

    while (!placed) {
        Room room = floors[floorNumber].rooms[rand() % MAX_ROOMS];
        int x = room.x + 1 + rand() % (room.width - 2);
        int y = room.y + 1 + rand() % (room.height - 2);

        if (floors[floorNumber].map[y][x] == '.') {
            floors[floorNumber].upStair.x = x;
            floors[floorNumber].upStair.y = y;
            floors[floorNumber].upStair.isActive = 1;
            floors[floorNumber].upStair.linkedFloor = linkedFloor;
            floors[floorNumber].map[y][x] = '>';
            seen[y][x]=0;
            placed = 1;
        }
    }
}

void placeDownStair(int floorNumber, int linkedFloor) {
    int placed = 0;

    while (!placed) {
        Room room = floors[floorNumber].rooms[rand() % MAX_ROOMS];
        int x = room.x + 1 + rand() % (room.width - 2);
        int y = room.y + 1 + rand() % (room.height - 2);

        if (floors[floorNumber].map[y][x] == '.') {
            floors[floorNumber].downStair.x = x;
            floors[floorNumber].downStair.y = y;
            floors[floorNumber].downStair.isActive = 1;
            floors[floorNumber].downStair.linkedFloor = linkedFloor;
            floors[floorNumber].map[y][x] = '<';
            seen[y][x]=0;
            placed = 1;
        }
    }
}

void createNewFloor(int newFloor) {
    initializeMap();
    generateRooms();
    initializePlayer();
    initializeEnemies();
    initializeItems();
    initializePasswordDoors();
    initializeCodeButtons();
    placeItemsInRooms();
    placeSecretDoors(rooms[0]);
    placeRandomPasswordDoor();
    initializeGold();
    placeGold();
    initializeSpecialElements();
    placeSpecialElements();
    addSpecialElementsToMap();
     
    for (int i = 0; i < MAX_ROOMS; i++) {
        floors[newFloor].rooms[i] = rooms[i];
    }
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            floors[newFloor].map[i][j] = map[i][j];
        }
    }
}
void setupFloorTransition() {
    int nextFloor = currentFloor + 1;

    if (nextFloor < MAX_FLOORS && floors[nextFloor].rooms[0].width == 0) {
        createNewFloor(nextFloor);
        placeUpStair(currentFloor, nextFloor);
        placeDownStair(nextFloor, currentFloor);
    }
}


void handleStairInteraction() {
    Stair *upStair = &floors[currentFloor].upStair;
    Stair *downStair = &floors[currentFloor].downStair;

    if (player.x == upStair->x && player.y == upStair->y && upStair->isActive) {
        int targetFloor = upStair->linkedFloor;

        if (targetFloor < MAX_FLOORS && floors[targetFloor].rooms[0].width == 0) {
            setupFloorTransition();   
        }

        currentFloor = targetFloor;
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                map[i][j] = floors[currentFloor].map[i][j];
            }
        }

        player.x = floors[currentFloor].downStair.x;
        player.y = floors[currentFloor].downStair.y;

        mvprintw(MAP_HEIGHT+2, 2, "Moved to Floor %d", currentFloor);
        refresh();
    }

    if (player.x == downStair->x && player.y == downStair->y && downStair->isActive) {
        int targetFloor = downStair->linkedFloor;

        currentFloor = targetFloor;
        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                map[i][j] = floors[currentFloor].map[i][j];
            }
        }

        player.x = floors[currentFloor].upStair.x;
        player.y = floors[currentFloor].upStair.y;

        mvprintw(MAP_HEIGHT+2, 2, "Returned to Floor %d", currentFloor);
        refresh();
    }
}

void renderStair() {
    Stair *upStair = &floors[currentFloor].upStair;
    Stair *downStair = &floors[currentFloor].downStair;

    if (upStair->isActive) {
        mvaddch(upStair->y, upStair->x, '>');
    }
    if (downStair->isActive) {
        mvaddch(downStair->y, downStair->x, '<');
    }
}
void placeSecretDoors(Room room) {
    for (int i = 0; i < MAX_SECRET_DOORS; i++) {
        int x = room.x + 1 + rand() % (room.width - 2);
        int y = room.y + 1 + rand() % (room.height - 2);

        if (map[y][x] == '.') { 
            secretDoors[i].x = x;
            secretDoors[i].y = y;
            secretDoors[i].isDiscovered = 0; 

            map[y][x] = '.'; 
        }
    }
}

void discoverSecretDoors() {
    for (int i = 0; i < MAX_SECRET_DOORS; i++) {
        if (player.x == secretDoors[i].x && player.y == secretDoors[i].y && !secretDoors[i].isDiscovered) {
            secretDoors[i].isDiscovered = 1;
            map[secretDoors[i].y][secretDoors[i].x] = '?'; 
            mvprintw(MAP_HEIGHT + 2, 2, "You discovered a secret door!");
        }
    }
}
void placeRoom(Room room) {
    int doorPlaced = 0;
    int windowPlaced = 0;
    for (int i = room.y; i < room.y + room.height; i++) {
        for (int j = room.x; j < room.x + room.width; j++) {
            if (i == room.y || i == room.y + room.height - 1) {
                if (!windowPlaced && rand() % 5 == 0) {
                    map[i][j] = '=';
                    windowPlaced = 1;
                } else {
                    map[i][j] = '-';
                }
                if (!doorPlaced && rand() % 5 == 0) {
                    map[i][j] = '+';
                    doorPlaced = 1;
                }
            } else if (j == room.x || j == room.x + room.width - 1) {
                if (!windowPlaced && rand() % 10 == 0) {
                    map[i][j] = '=';
                    windowPlaced = 1;
                } else {
                    map[i][j] = '|';
                }
                if (!doorPlaced && rand() % 10 == 0) {
                    map[i][j] = '+';
                    doorPlaced = 1;
                }
            } else if (rand() % 20 == 0) {
                map[i][j] = 'o'; 
            } else {
                map[i][j] = '.';
            }
        }
    }
}



void initializeCodeButtons() {          
    for (int i = 0; i < MAX_CODE_BUTTONS; i++) {
        codeButtons[i].isActive = 0;
        codeButtons[i].isVisible = 0;
        codeButtons[i].generatedCode= 0;
        codeButtons[i].linkedDoorIndex = 0;
    }
}

void initializePasswordDoors() {
            
    for (int i = 0; i < MAX_PASSWORD_DOORS; i++) {
        passwordDoors[i].isActive = 0;
        passwordDoors[i].isUnlocked = 0;
        passwordDoors[i].isLocked = 0;
        passwordDoors[i].requiredCode = 0;
        passwordDoors[i].attemptsLeft= 0;
    }
}

void placeRandomPasswordButton(int doorIndex) {
    if (doorIndex < 0 || doorIndex >= MAX_PASSWORD_DOORS || !passwordDoors[doorIndex].isActive) return;

    Room associatedRoom;
    int foundRoom = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        Room room = rooms[i];
        if (passwordDoors[doorIndex].x >= room.x && passwordDoors[doorIndex].x < room.x + room.width &&
            passwordDoors[doorIndex].y >= room.y && passwordDoors[doorIndex].y < room.y + room.height) {
            associatedRoom = room;
            foundRoom = 1;
            break;
        }
    }

    if (!foundRoom) return;

    int corners[4][2] = {
        {associatedRoom.x + 1, associatedRoom.y + 1},                             
        {associatedRoom.x + associatedRoom.width - 2, associatedRoom.y + 1},     
        {associatedRoom.x + 1, associatedRoom.y + associatedRoom.height - 2},    
        {associatedRoom.x + associatedRoom.width - 2, associatedRoom.y + associatedRoom.height - 2}
    };

    int selectedCorner = rand() % 4;
    int x = corners[selectedCorner][0];
    int y = corners[selectedCorner][1];

    if (map[y][x] == '.') {
        codeButtons[doorIndex].x = x;
        codeButtons[doorIndex].y = y;
        codeButtons[doorIndex].isActive = 1;
        codeButtons[doorIndex].linkedDoorIndex = doorIndex;
        map[y][x] = '&';
    }
}

void placeRandomPasswordDoor() {
    int placed = 0;
    while (!placed) {
        Room room = rooms[rand() % MAX_ROOMS]; 
        int x = room.x + 1 + rand() % (room.width - 2);
        int y = room.y + 1 + rand() % (room.height - 2);

        if (map[y][x] == '.') { 
            for (int i = 0; i < MAX_PASSWORD_DOORS; i++) {
                if (!passwordDoors[i].isActive) {
                    passwordDoors[i].x = x;
                    passwordDoors[i].y = y;
                    passwordDoors[i].requiredCode = 0;
                    passwordDoors[i].isActive = 1;
                    passwordDoors[i].isUnlocked = 0;
                    passwordDoors[i].attemptsLeft = 3; 
                    passwordDoors[i].isLocked = 0;    
                    map[y][x] = '@'; 
                    placeRandomPasswordButton(i);
                    placed = 1;
                    break;
                }
            }
        }
    }
}
/*void handleCodeButtonInteraction() {
    for (int i = 0; i < MAX_CODE_BUTTONS; i++) {
        if (codeButtons[i].isActive && player.x == codeButtons[i].x && player.y == codeButtons[i].y) {
            codeButtons[i].generatedCode = rand() % 9000 + 1000;
            codeButtons[i].generationTime = time(NULL);        
            codeButtons[i].isVisible = 1;                      
            lastValidCode = codeButtons[i].generatedCode;      
            passwordDoors[i].requiredCode = lastValidCode;
            mvprintw(MAP_HEIGHT-2, 2, "Generated Code: %d (Valid for 30 seconds)", codeButtons[i].generatedCode);
            updateCodeButtonVisibility();
            refresh();
        }
    }
}

void updateCodeButtonVisibility() {
    for (int i = 0; i < MAX_CODE_BUTTONS; i++) {
        if (codeButtons[i].isVisible) {
            time_t currentTime = time(NULL);
            int timeElapsed = (int) difftime(currentTime, codeButtons[i].generationTime);

            if (timeElapsed > 30) {
                codeButtons[i].isVisible = 0; 
                mvprintw(MAP_HEIGHT, 0, "\t\t\t\t");  // Clear the line for code visibility
                refresh();
            } else {
                mvprintw(MAP_HEIGHT-2, 2, "Generated Code: %d (Valid for %d seconds)", 
                         codeButtons[i].generatedCode, 30 - timeElapsed);
                refresh();
            }
        }
    }
}*/

void handleCodeButtonInteraction() {
    for (int i = 0; i < MAX_CODE_BUTTONS; i++) {
        if (codeButtons[i].isActive && player.x == codeButtons[i].x && player.y == codeButtons[i].y) {
            codeButtons[i].generatedCode = rand() % 9000 + 1000;
            codeButtons[i].generationTime = time(NULL);        
            codeButtons[i].isVisible = 1;                      
            lastValidCode = codeButtons[i].generatedCode;      
            passwordDoors[i].requiredCode = lastValidCode;
            mvprintw(MAP_HEIGHT-2, 2, "Generated Code: %d (Valid for 30 seconds)", codeButtons[i].generatedCode);
            refresh();

            // نمایش کد برای ۳۰ ثانیه
            for (int j = 0; j < 30; j++) {
                updateCodeButtonVisibility();
                refresh();
                usleep(1000000); 
            }
            codeButtons[i].isVisible = 0; 
            mvprintw(MAP_HEIGHT-2, 2, "\t\t\t\t");  // پاک کردن خط مربوط به کد
            refresh();
        }
    }
}

void updateCodeButtonVisibility() {
    for (int i = 0; i < MAX_CODE_BUTTONS; i++) {
        if (codeButtons[i].isVisible) {
            time_t currentTime = time(NULL);
            int timeElapsed = (int) difftime(currentTime, codeButtons[i].generationTime);

            if (timeElapsed > 30) {
                codeButtons[i].isVisible = 0; 
                mvprintw(MAP_HEIGHT-2, 2, "\t\t\t\t");  // پاک کردن خط مربوط به کد
                refresh();
            } else {
                mvprintw(MAP_HEIGHT-2, 2, "Generated Code: %d (Valid for %d seconds)", 
                         codeButtons[i].generatedCode, 30 - timeElapsed);
                refresh();
            }
        }
    }
}


void handlePasswordDoorInteraction() {
    for (int i = 0; i < MAX_PASSWORD_DOORS; i++) {
        if (passwordDoors[i].isActive && player.x == passwordDoors[i].x && player.y == passwordDoors[i].y) {
            mvprintw(MAP_HEIGHT + 2, 2, "Enter the code: ");
            refresh();

            char input[5];
            echo();
            curs_set(1);
            getstr(input);
            noecho();
            curs_set(0);
            int enteredCode = atoi(input);
            if (enteredCode == passwordDoors[i].requiredCode) {
                passwordDoors[i].isUnlocked = 1;
                mvprintw(MAP_HEIGHT +2, 2, "Door unlocked!");
                map[passwordDoors[i].y][passwordDoors[i].x] = '.'; 
            } else {
                passwordDoors[i].attemptsLeft--;
                if (passwordDoors[i].attemptsLeft == 2) {
                    attron(COLOR_PAIR(1)); 
                    mvprintw(MAP_HEIGHT+2 , 2, "Incorrect code! 2 attempts left.");
                    attroff(COLOR_PAIR(1));
                } else if (passwordDoors[i].attemptsLeft == 1) {
                    attron(COLOR_PAIR(2)); 
                    mvprintw(MAP_HEIGHT +2, 2, "Incorrect code! 1 attempt left.");
                    attroff(COLOR_PAIR(2));
                } else if (passwordDoors[i].attemptsLeft <= 0) {
                    passwordDoors[i].isLocked = 1;
                    attron(COLOR_PAIR(3)); 
                    mvprintw(MAP_HEIGHT+2 , 2, "Security mode activated! Door is locked.");
                    attroff(COLOR_PAIR(3));
                }
            }

            refresh();
        }
    }
}


void initializeEnemies() {
    /*
    switch(game_difficulty){
    case 1: 
    enemies[i].type = rand() % 15 + 1; 
    break;
    case 2:
    enemies[i].type = rand() % 10 + 1; 
    break;
    case 3:
    enemies[i].type = rand() % 5 + 1; 
    break;
    }*/
    for (int i = 0; i < MAX_ENEMIES; i++) {
        int roomIndex = rand() % MAX_ROOMS; 
        Room room = rooms[roomIndex];      

        enemies[i].x = room.x + 1 + rand() % (room.width - 2); 
        enemies[i].y = room.y + 1 + rand() % (room.height - 2);

        enemies[i].type = rand() % 5 + 1;  
        enemies[i].isAlive = 1;            
        enemies[i].room = &rooms[roomIndex]; 

        switch (enemies[i].type) {
            case 1:
                enemies[i].health = 5;
                enemies[i].damage = 2;
                enemies[i].canChase = 0;
                enemies[i].canShoot = 0;
                enemies[i].chaseSteps = 0;
                break;
            case 2: 
                enemies[i].health = 10;
                enemies[i].damage = 5;
                enemies[i].canChase = 0;
                enemies[i].canShoot = 1; 
                enemies[i].chaseSteps = 0;
                break;
            case 3: 
                enemies[i].health = 15;
                enemies[i].damage = 10;
                enemies[i].canChase = 1; 
                enemies[i].canShoot = 0;
                enemies[i].chaseSteps = 5;
                break;
            case 4: 
                enemies[i].health = 20;
                enemies[i].damage = 7;
                enemies[i].canChase = 1; 
                enemies[i].canShoot = 0;
                enemies[i].chaseSteps = -1; 
                break;
            case 5: 
                enemies[i].health = 30;
                enemies[i].damage = 15;
                enemies[i].canChase = 1;
                enemies[i].canShoot = 0;
                enemies[i].chaseSteps = 5; 
                break;
        }
    }
}
void initializeBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].isActive = 0; 
    }
}
void addBullet(int x, int y, int dx, int dy, Room *room) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].isActive) { 
            bullets[i].x = x;       
            bullets[i].y = y;
            bullets[i].dx = dx;     
            bullets[i].dy = dy;
            bullets[i].isActive = 1; 
            bullets[i].room = room;  
            break; 
        }
    }
}

int doesOverlap(Room room) {
    for (int i = 0; i < MAX_ROOMS; i++) {
        Room r = rooms[i];
        if ((room.x < r.x + r.width && room.x + room.width > r.x) &&
            (room.y < r.y + r.height && room.y + room.height > r.y)) {
            return 1;
        }
    }
    return 0;
}
int checkOverlap( x1,y1,x2,y2){
        if(!((map[y1][x1]==  '+' || '=' || '.')|| (map[y2][x2]==  '+' || '=' || '.'))) return 1;
        else return 0;

}

Room createRoom() {
    Room room;
    room.width = rand() % 10 + 6;
    room.height = rand() % 8 + 6;
    room.x = rand() % (MAP_WIDTH - room.width - 2) +2;
    room.y = rand() % (MAP_HEIGHT - room.height - 2) + 2;
    return room;
}

void generateRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        Room room;
        do {
            room = createRoom();
        } while (doesOverlap(room));

        rooms[i] = room;
        floors[currentFloor].rooms[i]=room;
        placeRoom(room);

        if (i > 0) {
            connectRooms(rooms[i - 1], room);
        }
    }
} 

void initializePlayer() {
    player.x = rooms[0].x + rooms[0].width / 2;
    player.y = rooms[0].y + rooms[0].height / 2;
    player.health = 20;
    player.itemsCollected = 0;
}

void renderPlayer() {
    //wchar_t playerChar[] = {L'☺'}; // استفاده از کاراکتر یونیکد wchar_t hero_avatar = L'☺'; 
    mvaddwstr(player.y, player.x, playerChar); // استفاده از mvaddwstr برای چاپ رشته‌های وسیع
}
void renderStats() {
    mvprintw(0, 0, "Health: %d  Gold: %d Items: %d/%d", player.health, playerGold, player.itemsCollected, MAX_ITEMS);
}

void handlePlayerInput(int ch) {
    int newX = player.x, newY = player.y;
    switch (ch) {
        case 'j': newY--; break;
        case 'k': newY++; break;
        case 'h': newX--; break;
        case 'l': newX++; break;
        case 'y': newY--; newX--; break;
        case 'u': newX++; newY--; break;
        case 'b': newX--; newY++; break;
        case 'n': newX++; newY++; break;
    }
         attron(COLOR_PAIR(3)); 
    if (map[newY][newX] == '.' || map[newY][newX] == '#' || map[newY][newX] == '+' ||
        map[newY][newX] == 'g' || map[newY][newX] == 'b' || 
        map[newY][newX] == '^' || map[newY][newX] == '<' || map[newY][newX] == '?'||
        map[newY][newX] == 'T' ||map[newY][newX] == '>' 
        ||map[newY][newX] == '@' ||map[newY][newX] == '&') {
        player.x = newX;
        player.y = newY;
    }
     attroff(COLOR_PAIR(3)); 
}
void renderEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive && seen[enemies[i].y][enemies[i].x]) {
            char enemyChar;
            switch (enemies[i].type) {
                case 1: enemyChar = 'D'; break; 
                case 2: enemyChar = 'F'; break; 
                case 3: enemyChar = 'G'; break; 
                case 4: enemyChar = 'S'; break; 
                case 5: enemyChar = 'U'; break; 
                default: enemyChar = 'E'; break;
            }
            mvaddch(enemies[i].y, enemies[i].x, enemyChar);
        }
    }
}
void shootBulletsFromEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive && enemies[i].canShoot) {
            if (rand() % 10 == 0) {
                int dx = 0, dy = 0;

                if (player.x > enemies[i].x) dx = 1;
                else if (player.x < enemies[i].x) dx = -1;

                if (player.y > enemies[i].y) dy = 1;
                else if (player.y < enemies[i].y) dy = -1;

                addBullet(enemies[i].x, enemies[i].y, dx, dy, enemies[i].room);
            }
        }
    }
}

void moveEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].isAlive) continue; 

        if (enemies[i].canChase) {
            if (enemies[i].chaseSteps != 0) {
                int dx = 0, dy = 0;

                if (player.x > enemies[i].x) dx = 1;
                else if (player.x < enemies[i].x) dx = -1;

                if (player.y > enemies[i].y) dy = 1;
                else if (player.y < enemies[i].y) dy = -1;

                int newX = enemies[i].x + dx;
                int newY = enemies[i].y + dy;

                if (newX >= enemies[i].room->x && newX < enemies[i].room->x + enemies[i].room->width &&
                    newY >= enemies[i].room->y && newY < enemies[i].room->y + enemies[i].room->height &&
                    map[newY][newX] == '.') {
                    enemies[i].x = newX;
                    enemies[i].y = newY;
                }

                if (enemies[i].chaseSteps > 0) {
                    enemies[i].chaseSteps--;
                }
            }
        } else {
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;
            int newX = enemies[i].x + dx;
            int newY = enemies[i].y + dy;

            if (newX >= enemies[i].room->x && newX < enemies[i].room->x + enemies[i].room->width &&
                newY >= enemies[i].room->y && newY < enemies[i].room->y + enemies[i].room->height &&
                map[newY][newX] == '.') {
                enemies[i].x = newX;
                enemies[i].y = newY;
            }
        }
    }
}
void checkEnemyCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive && player.x == enemies[i].x && player.y == enemies[i].y) {
            player.health--;
           /* if (player.health <= 0) {
                mvprintw(0, 10, "Game Over!");
                refresh();
                getch();
                endwin();
                exit(0);
            }*/
        }
    }
}
void initializeItems() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].x = rand() % (MAP_WIDTH - 2) + 1;
        items[i].y = rand() % (MAP_HEIGHT - 2) + 1;
        items[i].isCollected = 0;
        while (map[items[i].y][items[i].x] != '.') {
            items[i].x = rand() % (MAP_WIDTH - 2) + 1;
            items[i].y = rand() % (MAP_HEIGHT - 2) + 1;
        }
    }
}

void renderItems() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].isCollected && seen[items[i].y][items[i].x]) {
            mvaddch(items[i].y, items[i].x, 'w');
        }
    }
}

void checkItemCollection() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].isCollected && player.x == items[i].x && player.y == items[i].y) {
            items[i].isCollected = 1;
            player.itemsCollected++;
        }
    }
}

void placeItemsInRooms() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        int placed = 0;
        while (!placed) {
            Room room = rooms[rand() % MAX_ROOMS];

            int x = room.x + 1 + rand() % (room.width - 2);
            int y = room.y + 1 + rand() % (room.height - 2);

            if (map[y][x] == '.') { 
                items[i].x = x;
                items[i].y = y;
                items[i].isCollected = 0;
                placed = 1;
            }
        }
    }
}
void addSpecialElementsToMap() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        if (specialElements[i].isActive) {
            map[specialElements[i].y][specialElements[i].x] = specialElements[i].type;
        }
    }
}
void initializeGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        golds[i].x = -1;         
        golds[i].y = -1;         
        golds[i].value = 0;      
        golds[i].isCollected = 0; 
    }
}
void placeGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        int placed = 0;
        while (!placed) {
            Room room = rooms[rand() % MAX_ROOMS]; 

            int x = room.x + 1 + rand() % (room.width - 2);
            int y = room.y + 1 + rand() % (room.height - 2);

            if (map[y][x] == '.') { 
                golds[i].x = x;
                golds[i].y = y;
                golds[i].value = (rand() % 2 == 0) ? 10 : 50; 
                golds[i].isCollected = 0;
                map[y][x] = (golds[i].value == 10) ? 'g' : 'b'; 
                placed = 1;
            }
        }
    }
}
void renderGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        if (!golds[i].isCollected && seen[golds[i].y][golds[i].x]) {
            char symbol = (golds[i].value == 10) ? 'g' : 'b'; 
            mvaddch(golds[i].y, golds[i].x, symbol);
        }
    }
}
void collectGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        if (!golds[i].isCollected && player.x == golds[i].x && player.y == golds[i].y) {
            playerGold += golds[i].value;  
            golds[i].isCollected = 1;     
            map[golds[i].y][golds[i].x] = '.'; 
            mvprintw(MAP_HEIGHT+2, 2, "You collected %d gold!", golds[i].value);
        }
    }
}
void placeSpecialElements() {
    int elementIndex = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        Room room = rooms[i];

        while (elementIndex < MAX_SPECIAL_ELEMENTS) {
            int x = room.x + 1 + rand() % (room.width - 2);
            int y = room.y + 1 + rand() % (room.height - 2);

            if (map[y][x] == '.') { 
                specialElements[elementIndex].x = x;
                specialElements[elementIndex].y = y;

                if (elementIndex % 3 == 0) {
                    specialElements[elementIndex].type = 'T'; 
                } 
                specialElements[elementIndex].isActive = 1; 
                map[y][x] = '.'; 
                elementIndex++;
            }
        }
    }
}

void renderSpecialElements() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        if (specialElements[i].isActive && seen[specialElements[i].y][specialElements[i].x]) {
            char displayChar = '.'; 

            if (map[specialElements[i].y][specialElements[i].x] == '^' || map[specialElements[i].y][specialElements[i].x] == '?') {
                displayChar = map[specialElements[i].y][specialElements[i].x]; 
            }

            mvaddch(specialElements[i].y, specialElements[i].x, displayChar);
        }
    }
}

void handleSpecialElementInteraction() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        if (specialElements[i].isActive && player.x == specialElements[i].x && player.y == specialElements[i].y) {
            switch (specialElements[i].type) {
                case 'T': 
                    mvprintw(MAP_HEIGHT+2, 2, "You activated a trap! Health reduced.");
                    player.health--;  
                    map[specialElements[i].y][specialElements[i].x] = '^';  
                    specialElements[i].isActive = 0; 
                    break;

             
            }
        }
    }
}
void gameOverScreen() {
    clear(); 
    int choice = 0; 
    const char *options[] = {
        "Return to Main Menu",
        "Quit the Game"
    };
    int numOptions = 2;

    while (1) {
        clear();
        mvprintw(MAP_HEIGHT / 2 - 2, MAP_WIDTH / 2 - 10, "Game Over!"); 
        mvprintw(MAP_HEIGHT / 2, MAP_WIDTH / 2 - 15, "Your Score: %d (Gold: %d, Items: %d)", playerGold, playerGold, player.itemsCollected);
        for (int i = 0; i < numOptions; i++) {
            if (i == choice) {
                attron(A_REVERSE); 
                mvprintw(MAP_HEIGHT / 2 + i + 2, MAP_WIDTH / 2 - 10, options[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(MAP_HEIGHT / 2 + i + 2, MAP_WIDTH / 2 - 10, options[i]);
            }
        }

        refresh();

        int ch = getch();
        switch (ch) {
            case KEY_UP: 
                choice--;
                if (choice < 0) {
                    choice = numOptions - 1;
                }
                break;

            case KEY_DOWN: 
                choice++;
                if (choice >= numOptions) {
                    choice = 0;
                }
                break;

            case '\n': 
                if (choice == 0) { 
                    initializeMap(); 
                    generateRooms();
                    initializePlayer();
                    initializeEnemies();
                    initializeItems();
                    placeItemsInRooms();
                    initializeGold();
                    initializeSpecialElements();
                    addSpecialElementsToMap();

                    player.health = 3; 
                    playerGold = 0;    
                    return;
                } else if (choice == 1) { 
                    stop_music();
                    endwin();  
                    exit(0);  
                }
                break;
        }
    }
}
int render4() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(3, COLOR_RED, COLOR_BLACK); 
    init_pair(4, COLOR_GREEN, COLOR_BLACK); 
    play_music(music_files[current_track]);
    
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    createNewFloor(0);
    placeUpStair(0, 1);
    createNewFloor(1);
    placeDownStair(1, 0);
    placeUpStair(1,2);
    createNewFloor(2);
    placeDownStair(2,1);
    placeUpStair(2,3);
    createNewFloor(3);
    placeDownStair(3,2);



    while (1) {
        clear();
        updateVisibility();
        renderMap();
        renderEnemies();
        renderStair();
        renderItems();
        renderSpecialElements();
        renderGold();
        renderPlayer();
        renderStats();
        refresh();

        int ch = getch();
        if (ch == 'q') {
            stop_music();
        
        break;
        }
        handlePlayerInput(ch);
        handleCodeButtonInteraction();
        handlePasswordDoorInteraction();
        discoverSecretDoors();
        handleSpecialElementInteraction();
        collectGold();
        checkEnemyCollision();
        checkItemCollection();
        handleStairInteraction();
        moveEnemies();
        if (player.health <= 0) {
            gameOverScreen();
        }
    }
    refresh();
    getch();
    endwin();
}