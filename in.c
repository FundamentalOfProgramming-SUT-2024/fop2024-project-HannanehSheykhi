#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 180
#define MAP_HEIGHT 50
#define MAX_ENEMIES 5
#define MAX_ITEMS 5
#define MAX_ROOMS 6
#define MAX_SPECIAL_ELEMENTS 20
#define MAX_BULLETS 10
#define MAX_GOLD 20 


typedef struct {
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

    if (rand() % 2 == 0) { 
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
    }

    map[y1][x1] = '+';
    map[y2][x2] = '+';

    int currentX = x1;
    while (currentX != x2) {
        currentX += (x2 > currentX) ? 1 : -1;
        if (map[y1][currentX] == '-' || map[y1][currentX] == '|') break; 
        map[y1][currentX] = '#';
    }

    int currentY = y1;
    while (currentY != y2) {
        currentY += (y2 > currentY) ? 1 : -1;
        if (map[currentY][x2] == '-' || map[currentY][x2] == '|') break; 
        map[currentY][x2] = '#';
    }
}
void validateCorridors() {
    for (int i = 0; i < MAX_ROOMS - 1; i++) {
        Room room1 = rooms[i];
        Room room2 = rooms[i + 1];

        connectRooms(room1, room2);

        if (map[room1.y][room1.x] != '+' || map[room2.y][room2.x] != '+') {
            connectRooms(room1, room2);
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
                if (!doorPlaced && rand() % 10 == 0) {
                    map[i][j] = '+';
                    doorPlaced = 1;
                }
            } else if (j == room.x || j == room.x + room.width - 1) {
                if (!windowPlaced && rand() % 5 == 0) {
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

void initializeEnemies() {
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

Room createRoom() {
    Room room;
    room.width = rand() % 8 + 6;
    room.height = rand() % 8 + 6;
    room.x = rand() % (MAP_WIDTH - room.width - 2) + 1;
    room.y = rand() % (MAP_HEIGHT - room.height - 2) + 1;
    return room;
}

void generateRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        Room room;
        do {
            room = createRoom();
        } while (doesOverlap(room));

        rooms[i] = room;
        placeRoom(room);

        if (i > 0) {
            connectRooms(rooms[i - 1], room);
        }
    }
}

void initializePlayer() {
    player.x = rooms[0].x + rooms[0].width / 2;
    player.y = rooms[0].y + rooms[0].height / 2;
    player.health = 3;
    player.itemsCollected = 0;
}

void renderPlayer() {
    mvaddch(player.y, player.x, '@');
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
    if (map[newY][newX] == '.' || map[newY][newX] == '#' || map[newY][newX] == '+') {
        player.x = newX;
        player.y = newY;
    }
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
            if (player.health <= 0) {
                mvprintw(0, 10, "Game Over!");
                refresh();
                getch();
                endwin();
                exit(0);
            }
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
            mvaddch(items[i].y, items[i].x, 'F');
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

void placeSpecialElements() {
    int elementIndex = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        Room room = rooms[i];

        if (elementIndex < MAX_SPECIAL_ELEMENTS) {
            specialElements[elementIndex].x = room.x + 1 + rand() % (room.width - 2);
            specialElements[elementIndex].y = room.y + 1 + rand() % (room.height - 2);
            specialElements[elementIndex].type = 'T';
            specialElements[elementIndex].isActive = 1;
            elementIndex++;
        }

        if (elementIndex < MAX_SPECIAL_ELEMENTS) {
            specialElements[elementIndex].x = room.x + room.width - 1;
            specialElements[elementIndex].y = room.y + rand() % room.height;
            specialElements[elementIndex].type = '?';
            specialElements[elementIndex].isActive = 0; 
            elementIndex++;
        }

        if ((i == 0 || i == MAX_ROOMS - 1) && elementIndex < MAX_SPECIAL_ELEMENTS) {
            specialElements[elementIndex].x = room.x + room.width / 2;
            specialElements[elementIndex].y = room.y + room.height / 2;
            specialElements[elementIndex].type = '<';
            specialElements[elementIndex].isActive = 1;
            elementIndex++;
        }
    }
}

void renderSpecialElements() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        if (specialElements[i].isActive && seen[specialElements[i].y][specialElements[i].x]) {
            mvaddch(specialElements[i].y, specialElements[i].x, specialElements[i].type);
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

void handleSpecialElementInteraction() {
    for (int i = 0; i < MAX_SPECIAL_ELEMENTS; i++) {
        if (specialElements[i].isActive && player.x == specialElements[i].x && player.y == specialElements[i].y) {
            switch (specialElements[i].type) {
                case 'T': 
                    mvprintw(0, 0, "You activated a trap!");
                    player.health--; 
                    break;

                case '<':
                    mvprintw(0, 0, "You moved to the next level!");
                    break;

                case '?': 
                    mvprintw(0, 0, "You found a secret door!");
                    specialElements[i].isActive = 0; 
                    break;

                case '@': 
                    mvprintw(0, 0, "Enter the code to open the door!");
                    break;
            }
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
        Room room = rooms[rand() % MAX_ROOMS]; 

        do {
            golds[i].x = room.x + 1 + rand() % (room.width - 2);
            golds[i].y = room.y + 1 + rand() % (room.height - 2);
        } while (map[golds[i].y][golds[i].x] != '.'); 

        golds[i].value = (rand() % 2 == 0) ? 10 : 50;  
        golds[i].isCollected = 0; 
    }
}
void renderGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        if (!golds[i].isCollected && seen[golds[i].y][golds[i].x]) {
            char symbol = (golds[i].value == 10) ? '$' : '€'; 
            mvaddch(golds[i].y, golds[i].x, symbol);
        }
    }
}
void collectGold() {
    for (int i = 0; i < MAX_GOLD; i++) {
        if (!golds[i].isCollected && player.x == golds[i].x && player.y == golds[i].y) {
            playerGold += golds[i].value; 
            golds[i].isCollected = 1;    
            mvprintw(0, 0, "You collected %d gold!", golds[i].value);
        }
    }
}
int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    initializeMap();
    generateRooms();
    validateCorridors();
    initializePlayer();
    initializeEnemies();
    initializeItems();
    placeItemsInRooms();

    while (1) {
        clear();
        updateVisibility();
        renderMap();
        renderEnemies();
        renderGold();
        renderItems();
        renderSpecialElements();
        addSpecialElementsToMap();
        renderPlayer();
        renderStats();
        refresh();

        int ch = getch();
        if (ch == 'q') break;
        handlePlayerInput(ch);
        collectGold();
        handleSpecialElementInteraction();
        checkEnemyCollision();
        checkItemCollection();
        moveEnemies();
    }

    endwin();
    return 0;
}