#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 180
#define MAP_HEIGHT 50
#define MAX_ENEMIES 5
#define MAX_ITEMS 5
#define MAX_ROOMS 6

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
} Enemy;

typedef struct {
    int x, y;
    int isCollected;
} Item;

char map[MAP_HEIGHT][MAP_WIDTH];
int seen[MAP_HEIGHT][MAP_WIDTH];
Player player;
Enemy enemies[MAX_ENEMIES];
Item items[MAX_ITEMS];
Room rooms[MAX_ROOMS];

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
void updateVisibility() {
    for (int i = player.y - 5; i <= player.y + 5; i++) {
        for (int j = player.x - 5; j <= player.x + 5; j++) {
            if(map[player.y][player.x]=='.' && map[i][j]=='#'){}
            else if(map[i][j]=='='){
              for (int a = j - 5; a <= j + 5; a++) {
        for (int b = i - 5; b <= i + 5; b++) {  
            if (a >= 0 && a < MAP_HEIGHT && b >= 0 && b < MAP_WIDTH) {
                seen[a][b] = 1;
            }
            }
              }
            }
            else if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
                seen[i][j] = 1;
            }
        }
    }
}

void connectRooms(Room room1, Room room2) {
    int x1, y1, x2, y2;

    x1 = room1.x + room1.width / 2;
    y1 = room1.y + room1.height / 2;
    x2 = room2.x + room2.width / 2;
    y2 = room2.y + room2.height / 2;

    while (x1 != x2) {
        map[y1][x1] = '#';
        x1 += (x2 > x1) ? 1 : -1;
    }

    while (y1 != y2) {
        map[y1][x1] = '#';
        y1 += (y2 > y1) ? 1 : -1;
    }

    map[y2][x2] = '#';
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
                map[i][j] = 'o'; // Add obstacles
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
    room.width = rand() % 8 + 4;
    room.height = rand() % 8 + 4;
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

void initializeEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % (MAP_WIDTH - 2) + 1;
        enemies[i].y = rand() % (MAP_HEIGHT - 2) + 1;
        enemies[i].isAlive = 1;
        while (map[enemies[i].y][enemies[i].x] != '.') {
            enemies[i].x = rand() % (MAP_WIDTH - 2) + 1;
            enemies[i].y = rand() % (MAP_HEIGHT - 2) + 1;
        }
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
        refresh();

        int ch = getch();
        if (ch == 'q') break;
        handlePlayerInput(ch);
        checkEnemyCollision();
        checkItemCollection();
        moveEnemies();
    }

    endwin();
    return 0;
}

