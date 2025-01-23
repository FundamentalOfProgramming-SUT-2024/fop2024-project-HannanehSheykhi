#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define MAX_ENEMIES 5
#define MAX_ITEMS 5
#define MAX_ROOMS 6
#define MAX_BULLETS 10


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
    int isAlive;
    int canShoot;
    Room* room;
} Enemy;

typedef struct {
    int x, y;
    int isCollected;  
} Item;

typedef struct {
    int x, y;
    int dx,dy;
    int isActive;
    //int roomIndex;
    Room *room;
} Bullet;

char map[MAP_HEIGHT][MAP_WIDTH];
int seen[MAP_HEIGHT][MAP_WIDTH];
Player player;
Enemy enemies[MAX_ENEMIES];
Item items[MAX_ITEMS];
Room rooms[MAX_ROOMS];
Bullet bullets[MAX_BULLETS];


void initializeMap() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
                map[i][j] = '*';  
            } else {
                map[i][j] = ' ';  
            }
            seen[i][j] = 0;
        }
    }
}

void renderMap() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
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
void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            int newX = bullets[i].x + bullets[i].dx;
            int newY = bullets[i].y + bullets[i].dy;

            if (newX >= bullets[i].room->x && newX < bullets[i].room->x + bullets[i].room->width &&
                newY >= bullets[i].room->y && newY < bullets[i].room->y + bullets[i].room->height) {
                bullets[i].x = newX;
                bullets[i].y = newY;

                if (bullets[i].x == player.x && bullets[i].y == player.y) {
                    player.health--; 
                    bullets[i].isActive = 0; 
                    if (player.health <= 0) {
                        mvprintw(0, 10, "Game Over!");
                        refresh();
                        getch();
                        endwin();
                        exit(0);
                    }
                }
            } else {
                bullets[i].isActive = 0;
            }
        }
    }
}
void renderBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].isActive) {
            int bx = bullets[i].x;
            int by = bullets[i].y;

            if (seen[by][bx]) {
                mvaddch(by, bx, '*'); 
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

    while (x1 != x2) {
        map[y1][x1] = '#';  
        x1 += (x2 > x1) ? 1 : -1;
    }

    while (y1 != y2) {
        map[y1][x1] = '#';  
        y1 += (y2 > y1) ? 1 : -1;
    }

    map[y1][x1] = '+';
}

void updateVisibility() {
    for (int i = player.y - 6; i <= player.y + 6; i++) {
        for (int j = player.x - 6; j <= player.x + 6; j++) {
            if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
                seen[i][j] = 1;
            }
        }
    }
}
void placeRoom(Room room) {
    for (int i = room.y; i < room.y + room.height; i++) {
        for (int j = room.x; j < room.x + room.width; j++) {
            if (i == room.y || i == room.y + room.height - 1) {
                map[i][j] = '-';  
            } else if (j == room.x || j == room.x + room.width - 1) {
                map[i][j] = '|';  
            } else {
                map[i][j] = '.';  
            }
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
    room.width = rand() % 5 + 4;
    room.height = rand() % 5 + 4;
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
    mvprintw(0, 0, "Health: %d  Items: %d/%d", player.health, player.itemsCollected, MAX_ITEMS);  
}

void handlePlayerInput(int ch) {
    int newX = player.x, newY = player.y;
    switch (ch) {
        case KEY_UP:    newY--; break;
        case KEY_DOWN:  newY++; break;
        case KEY_LEFT:  newX--; break;
        case KEY_RIGHT: newX++; break;
    }
    if (map[newY][newX] == '.' || map[newY][newX] == '#' || map[newY][newX] == '+') {
        player.x = newX;
        player.y = newY;
    }
}

void initializeEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {

        int roomIndex = rand() % MAX_ROOMS;

        Room room = rooms[roomIndex];

        enemies[i].x = room.x + 1 + rand() % (room.width - 2);
        enemies[i].y = room.y + 1 + rand() % (room.height - 2);

        enemies[i].isAlive = 1;                 
        enemies[i].canShoot = rand() % 2;       
        enemies[i].room = &rooms[roomIndex];    
    }
}
void moveEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive) {
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;
            int newX = enemies[i].x + dx;
            int newY = enemies[i].y + dy;

            if (map[newY][newX] == '.') {
                enemies[i].x = newX;
                enemies[i].y = newY;
            }
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

                for (int j = 0; j < MAX_ROOMS; j++) {
                    if (enemies[i].x >= rooms[j].x && enemies[i].x < rooms[j].x + rooms[j].width &&
                        enemies[i].y >= rooms[j].y && enemies[i].y < rooms[j].y + rooms[j].height) {
                        addBullet(enemies[i].x, enemies[i].y, dx, dy, &rooms[j]);
                        break;
                    }
                }
            }
        }
    }
}
void renderEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isAlive && seen[enemies[i].y][enemies[i].x]) {
            mvaddch(enemies[i].y, enemies[i].x, 'E');
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
            mvaddch(items[i].y, items[i].x, 'I');
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

int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    initializeMap();
    generateRooms();
    initializePlayer();
    initializeEnemies();
    initializeItems();
    placeItemsInRooms();

    while (1) {
        clear();
        updateVisibility();
        renderMap();
        renderEnemies();
        renderItems();
        renderPlayer();
        renderStats();
        renderBullets();
        refresh();

        int ch = getch();
        if (ch == 'q') break;
        handlePlayerInput(ch);
        checkEnemyCollision();
        checkItemCollection();
        moveEnemies();
        shootBulletsFromEnemies();
        updateBullets();
    }

    endwin();
    return 0;
}