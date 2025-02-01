#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 40
#define MAP_HEIGHT 40
#define MAX_ENEMIES 5
#define MAX_ITEMS 5
#define MAX_ROOMS 6

typedef struct {
    int x, y;  
    int health;  
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
    for (int i = player.y - 6; i <= player.y + 6; i++) {
        for (int j = player.x - 6; j <= player.x + 6; j++) {
            if (i >= 0 && i < MAP_HEIGHT && j >= 0 && j < MAP_WIDTH) {
                seen[i][j] = 1;
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

void connectRooms(Room room1, Room room2) {
    int x1 = room1.x + room1.width / 2;
    int y1 = room1.y + room1.height / 2;
    int x2 = room2.x + room2.width / 2;
    int y2 = room2.y + room2.height / 2;

    while (x1 != x2) {
        map[y1][x1] = '+';
        seen[y1][x1] = 1;  
        x1 += (x2 > x1) ? 1 : -1;
    }

    while (y1 != y2) {
        map[y1][x1] = '+';
        seen[y1][x1] = 1;  
        y1 += (y2 > y1) ? 1 : -1;
    }
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
}

void renderPlayer() {
    mvaddch(player.y, player.x, '@');
}

void renderHealth() {
    mvprintw(0, 0, "Health: %d", player.health);  // نمایش جان بازیکن در بالای نقشه
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
    if (map[newY][newX] == '.' || map[newY][newX] == '+' /*|| map[newY][newX] == ' '*/) {
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
            enemies[i].isAlive = 0;  
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

    while (1) {
        clear();
        updateVisibility();
        renderMap();
        renderEnemies();
        renderPlayer();
        renderHealth();  
        refresh();

        int ch = getch();
        if (ch == 'q') break;
        handlePlayerInput(ch);
        checkEnemyCollision();  
        moveEnemies();  
    }

    endwin();
    return 0;
}