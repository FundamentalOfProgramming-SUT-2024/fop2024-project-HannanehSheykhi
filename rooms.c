#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define SCREEN_OFFSET 2
#define MIN_CORRIDOR_LENGTH 3
#define MAX_ROOM_WIDTH 10
#define MAX_ROOM_HEIGHT 5
#define NUM_ROOMS 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position position;
    Position block;
    int width, height;
    bool doors[4]; // Four doors: left, top, right, bottom
} Room;

Room rooms[NUM_ROOMS]; // Array to hold rooms
int num_rooms = NUM_ROOMS;

void draw_room(Room *room);
void draw_rooms();
void draw_corridors();
void generate_map();
bool handle_game();

void draw_room(Room *room) {
    int y, x, height = room->height + 2, width = room->width + 2;
    x = SCREEN_OFFSET + room->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + room->position.x;
    y = SCREEN_OFFSET + room->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + room->position.y;

    for (int i = 0; i < height; i++) {
        move(i + y, x);
        for (int j = 0; j < width; j++) {
            // Randomly decide if a '+' should be placed
            bool is_door = (rand() % 10) < 2; // 20% chance to place a '+'
            
            if ((i == 0 || i == height - 1 || j == 0 || j == width - 1) && is_door) {
                printw("+");
                // Store door information
                if (i == 0) room->doors[1] = true;     // Top
                if (i == height - 1) room->doors[3] = true; // Bottom
                if (j == 0) room->doors[0] = true;     // Left
                if (j == width - 1) room->doors[2] = true; // Right
            } else if (0 < i && i < height - 1 && 0 < j && j < width - 1) {
                printw(".");
            } else if (i == 0 && j == 0) {
                printw("┌");
            } else if (i == 0 && j == width - 1) {
                printw("┐");
            } else if (i == height - 1 && j == width - 1) {
                printw("┘");
            } else if (i == height - 1 && j == 0) {
                printw("└");
            } else if (i == 0 || i == height - 1) {
                printw("─");
            } else if (j == 0 || j == width - 1) {
                printw("│");
            }
        }
    }
}

void draw_rooms() {
    for (int i = 0; i < num_rooms; i++) {
        draw_room(&rooms[i]);
    }
}

void draw_corridors() {
    for (int i = 0; i < num_rooms; i++) {
        for (int j = i + 1; j < num_rooms; j++) {
            Room *a = &rooms[i];
            Room *b = &rooms[j];

            Position door_a, door_b;

            // Draw horizontal corridors
            if (a->doors[2] && b->doors[0]) {
                door_a.x = SCREEN_OFFSET + a->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + a->position.x + a->width + 1;
                door_a.y = SCREEN_OFFSET + a->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + a->position.y + a->height / 2;
                door_b.x = SCREEN_OFFSET + b->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + b->position.x - 1;
                door_b.y = SCREEN_OFFSET + b->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + b->position.y + b->height / 2;

                for (int k = door_a.x; k <= door_b.x; k++) {
                    mvprintw(door_a.y, k, "░");
                }
            }

            // Draw vertical corridors
            if (a->doors[3] && b->doors[1]) {
                door_a.x = SCREEN_OFFSET + a->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + a->position.x + a->width / 2;
                door_a.y = SCREEN_OFFSET + a->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + a->position.y + a->height + 1;
                door_b.x = SCREEN_OFFSET + b->block.x * (MIN_CORRIDOR_LENGTH + MAX_ROOM_WIDTH + 2) + b->position.x + b->width / 2;
                door_b.y = SCREEN_OFFSET + b->block.y * (MIN_CORRIDOR_LENGTH + MAX_ROOM_HEIGHT + 2) + b->position.y - 1;

                for (int k = door_a.y; k <= door_b.y; k++) {
                    mvprintw(k, door_a.x, "░");
                }
            }
        }
    }
}

void generate_map() {
    for (int i = 0; i < num_rooms; i++) {
        rooms[i].width = rand() % MAX_ROOM_WIDTH + 3; // Random width between 3 and MAX_ROOM_WIDTH
        rooms[i].height = rand() % MAX_ROOM_HEIGHT + 3; // Random height between 3 and MAX_ROOM_HEIGHT
        rooms[i].position.x = rand() % (COLS - rooms[i].width - SCREEN_OFFSET);
        rooms[i].position.y = rand() % (LINES - rooms[i].height - SCREEN_OFFSET);
        rooms[i].block.x = rand() % 5; // Random block position (adjust as needed)
        rooms[i].block.y = rand() % 5; // Random block position (adjust as needed)
        
        // Initialize doors as closed
        for (int j = 0; j < 4; j++) {
            rooms[i].doors[j] = false;
        }
    }
}

bool handle_game() {
    int ch;
    erase();
    generate_map();
    draw_rooms();
    draw_corridors();
    while (1) {
        ch = getch();
        if (ch == KEY_F(1))
            return false;
        else if (ch == 'r') {
            generate_map();
            erase();
            draw_rooms();
            draw_corridors();
        }
    }
    return false;
}

int main() {
    srand(time(NULL)); // Initialize random seed
    setlocale(LC_ALL, ""); // Ensure proper locale for Unicode characters
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    if (!handle_game()) {
        endwin();
        return 0;
    }

    endwin();
    return 0;
}

