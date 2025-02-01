#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 24

void init_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    // Initialize the entire map with floors
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = '.';
        }
    }
}

void draw_wall(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int x2, int y2) {
    // Draw vertical wall
    if (x1 == x2) {
        for (int y = y1; y <= y2; y++) {
            map[y][x1] = '|';
        }
    }
    // Draw horizontal wall
    else if (y1 == y2) {
        for (int x = x1; x <= x2; x++) {
            map[y1][x] = '_';
        }
    }
}

void add_room(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int width, int height) {
    // Draw walls of the room
    draw_wall(map, x1, y1, x1 + width - 1, y1); // Top wall
    draw_wall(map, x1, y1, x1, y1 + height - 1); // Left wall
    draw_wall(map, x1, y1 + height - 1, x1 + width - 1, y1 + height - 1); // Bottom wall
    draw_wall(map, x1 + width - 1, y1, x1 + width - 1, y1 + height - 1); // Right wall
}

void print_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mvaddch(y, x, map[y][x]);
        }
    }
}

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    // Adding rooms to the map
    add_room(map, 10, 5, 20, 10); // Example room
    add_room(map, 40, 8, 15, 5); // Example room

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    endwin();

    return 0;
}





void add_door(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    map[y][x] = '+';
}

void add_corridor(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int x2, int y2) {
    // Draw horizontal corridor
    for (int x = x1; x <= x2; x++) {
        map[y1][x] = '#';
    }
    // Draw vertical corridor
    for (int y = y1; y <= y2; y++) {
        map[y][x1] = '#';
    }
}

void add_pillar(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    map[y][x] = 'O';
}

void add_window(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    map[y][x] = '=';
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    // Adding rooms to the map
    add_room(map, 10, 5, 20, 10); // Example room
    add_door(map, 15, 5); // Door on top wall
    add_corridor(map, 15, 5, 15, 15); // Vertical corridor

    add_room(map, 40, 8, 15, 5); // Example room
    add_pillar(map, 42, 10); // Adding a pillar inside room
    add_window(map, 44, 8); // Window on top wall

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    endwin();

    return 0;
}



void add_trap(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    map[y][x] = '^';
}

void add_staircase(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y, char direction) {
    map[y][x] = direction; // '<' for going up, '>' for going down
}

void add_secret_door(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
    map[y][x] = '?';
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    // Adding rooms to the map
    add_room(map, 10, 5, 20, 10); // Example room
    add_door(map, 15, 5); // Door on top wall
    add_corridor(map, 15, 5, 15, 15); // Vertical corridor

    add_room(map, 40, 8, 15, 5); // Example room
    add_pillar(map, 42, 10); // Adding a pillar inside room
    add_window(map, 44, 8); // Window on top wall

    // Adding traps, staircases, and secret doors
    add_trap(map, 12, 7); // Trap inside room
    add_staircase(map, 18, 10, '>'); // Staircase down
    add_secret_door(map, 43, 9); // Secret door

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    endwin();

    return 0;
}



int check_overlap(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int width, int height) {
    for (int y = y1; y < y1 + height; y++) {
        for (int x = x1; x < x1 + width; x++) {
            if (map[y][x] != '.') {
                return 1; // Overlap detected
            }
        }
    }
    return 0; // No overlap
}

void generate_random_rooms(char map[MAP_HEIGHT][MAP_WIDTH], int room_count) {
    int attempts = 0;
    while (room_count > 0 && attempts < 1000) {
        int width = rand() % 8 + 4; // Room width between 4 and 11
        int height = rand() % 8 + 4; // Room height between 4 and 11
        int x = rand() % (MAP_WIDTH - width);
        int y = rand() % (MAP_HEIGHT - height);

        if (!check_overlap(map, x, y, width, height)) {
            add_room(map, x, y, width, height);
            room_count--;
        }
        attempts++;
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    endwin();

    return 0;
}



void add_treasure_room(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int width, int height) {
    add_room(map, x1, y1, width, height);
    // Adding traps and treasure
    for (int i = 0; i < 5; i++) { // Example of adding traps
        int tx = x1 + rand() % (width - 2) + 1;
        int ty = y1 + rand() % (height - 2) + 1;
        add_trap(map, tx, ty);
    }
    int tx = x1 + rand() % (width - 2) + 1;
    int ty = y1 + rand() % (height - 2) + 1;
    map[ty][tx] = '*'; // Treasure symbol
}

void add_enchant_room(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int width, int height) {
    add_room(map, x1, y1, width, height);
    // Adding spells and potions
    for (int i = 0; i < 3; i++) {
        int sx = x1 + rand() % (width - 2) + 1;
        int sy = y1 + rand() % (height - 2) + 1;
        map[sy][sx] = 'P'; // Potion symbol
    }
}

void add_regular_room(char map[MAP_HEIGHT][MAP_WIDTH], int x1, int y1, int width, int height) {
    add_room(map, x1, y1, width, height);
    // Adding small amounts of various items
    for (int i = 0; i < 2; i++) {
        int gx = x1 + rand() % (width - 2) + 1;
        int gy = y1 + rand() % (height - 2) + 1;
        map[gy][gx] = 'G'; // Gold symbol
    }
    for (int i = 0; i < 1; i++) {
        int tx = x1 + rand() % (width - 2) + 1;
        int ty = y1 + rand() % (height - 2) + 1;
        add_trap(map, tx, ty);
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    add_treasure_room(map, 10, 5, 20, 10); // Example treasure room
    add_enchant_room(map, 40, 8, 15, 5); // Example enchant room
    add_regular_room(map, 25, 15, 10, 10); // Example regular room

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    endwin();

    return 0;
}





void save_game(char map[MAP_HEIGHT][MAP_WIDTH], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to save game");
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            fputc(map[y][x], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}

void load_game(char map[MAP_HEIGHT][MAP_WIDTH], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to load game");
        return;
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = fgetc(file);
        }
        fgetc(file); // Skip the newline character
    }

    fclose(file);
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    print_map(map);
    refresh();
    
    getch(); // Wait for user input
    
    save_game(map, "savefile.txt"); // Save the game state
    
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game(map, "savefile.txt");
    print_map(map);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}



typedef struct {
    int x;
    int y;
} Player;

void move_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, int dx, int dy) {
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    if (map[new_y][new_x] != '|' && map[new_y][new_x] != '_') {
        player->x = new_x;
        player->y = new_y;
    }
}

void print_map_with_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == player->x && y == player->y) {
                mvaddch(y, x, '@'); // Player symbol
            } else {
                mvaddch(y, x, map[y][x]);
            }
        }
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10}; // Initial player position

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        switch (ch) {
            case 'w':
                move_player(map, &player, 0, -1);
                break;
            case 's':
                move_player(map, &player, 0, 1);
                break;
            case 'a':
                move_player(map, &player, -1, 0);
                break;
            case 'd':
                move_player(map, &player, 1, 0);
                break;
        }
        print_map_with_player(map, &player);
        refresh();
    }
    
    save_game(map, "savefile.txt"); // Save the game state
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game(map, "savefile.txt");
    print_map_with_player(map, &player);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}



void move_player_advanced(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, int dx, int dy, int *running) {
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    if (map[new_y][new_x] != '|' && map[new_y][new_x] != '_') {
        player->x = new_x;
        player->y = new_y;
    } else {
        *running = 0; // Stop running when hitting a wall
    }
}

void handle_movement(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, int ch) {
    int running = 1;
    switch (ch) {
        case 'w':
        case 'W': move_player_advanced(map, player, 0, -1, &running); break; // Up
        case 's':
        case 'S': move_player_advanced(map, player, 0, 1, &running); break;  // Down
        case 'a':
        case 'A': move_player_advanced(map, player, -1, 0, &running); break; // Left
        case 'd':
        case 'D': move_player_advanced(map, player, 1, 0, &running); break;  // Right
        case 'q':
        case 'Q': move_player_advanced(map, player, -1, -1, &running); break; // Up-Left
        case 'e':
        case 'E': move_player_advanced(map, player, 1, -1, &running); break;  // Up-Right
        case 'z':
        case 'Z': move_player_advanced(map, player, -1, 1, &running); break; // Down-Left
        case 'c':
        case 'C': move_player_advanced(map, player, 1, 1, &running); break;  // Down-Right
    }
    
    while (running && (ch = getch()) == 'f') {
        move_player_advanced(map, player, dx, dy, &running);
    }
}

void pick_up_item(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == 'G') {
        // Add code to increase gold count
        map[y][x] = '.'; // Remove gold from the map
    } else if (map[y][x] == 'P') {
        // Add code to pick up a potion
        map[y][x] = '.'; // Remove potion from the map
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10}; // Initial player position

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        handle_movement(map, &player, ch);
        pick_up_item(map, &player);
        print_map_with_player(map, &player);
        refresh();
    }

    save_game(map, "savefile.txt"); // Save the game state
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game(map, "savefile.txt");
    print_map_with_player(map, &player);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}


void check_trap(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == '^') {
        // Add code to reduce player's health or trigger trap effects
        mvprintw(0, 0, "You stepped on a trap!"); // Example message
    }
}

void use_staircase(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, char direction) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == direction) {
        // Add code to move to the next level or previous level
        mvprintw(1, 0, "You used the staircase!"); // Example message
    }
}

void reveal_secret_door(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == '?') {
        // Add code to reveal the secret door
        mvprintw(2, 0, "You found a secret door!"); // Example message
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10}; // Initial player position

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        handle_movement(map, &player, ch);
        pick_up_item(map, &player);
        check_trap(map, &player);
        use_staircase(map, &player, '>');
        reveal_secret_door(map, &player);
        print_map_with_player(map, &player);
        refresh();
    }

    save_game(map, "savefile.txt"); // Save the game state
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game(map, "savefile.txt");
    print_map_with_player(map, &player);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}



void save_game_with_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to save game");
        return;
    }

    fprintf(file, "%d %d\n", player->x, player->y); // Save player's position

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            fputc(map[y][x], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}

void load_game_with_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to load game");
        return;
    }

    fscanf(file, "%d %d\n", &player->x, &player->y); // Load player's position

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = fgetc(file);
        }
        fgetc(file); // Skip the newline character
    }

    fclose(file);
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10}; // Initial player position

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        handle_movement(map, &player, ch);
        pick_up_item(map, &player);
        check_trap(map, &player);
        use_staircase(map, &player, '>');
        reveal_secret_door(map, &player);
        print_map_with_player(map, &player);
        refresh();
    }

    save_game_with_player(map, &player, "savefile.txt"); //


    void save_game_with_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to save game");
        return;
    }

    fprintf(file, "%d %d\n", player->x, player->y); // Save player's position

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            fputc(map[y][x], file);
        }
        fputc('\n', file);
    }

    fclose(file);
}

void load_game_with_player(char map[MAP_HEIGHT][MAP_WIDTH], Player *player, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to load game");
        return;
    }

    fscanf(file, "%d %d\n", &player->x, &player->y); // Load player's position

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = fgetc(file);
        }
        fgetc(file); // Skip the newline character
    }

    fclose(file);
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10}; // Initial player position

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        handle_movement(map, &player, ch);
        pick_up_item(map, &player);
        check_trap(map, &player);
        use_staircase(map, &player, '>');
        reveal_secret_door(map, &player);
        print_map_with_player(map, &player);
        refresh();
    }

    save_game_with_player(map, &player, "savefile.txt"); // Save the game state with player position
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game_with_player(map, &player, "savefile.txt");
    print_map_with_player(map, &player);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}



typedef struct {
    int x;
    int y;
    int health;
    int hunger;
    int gold;
} Player;

void display_status(Player *player) {
    mvprintw(0, 0, "Health: %d  Hunger: %d  Gold: %d", player->health, player->hunger, player->gold);
}

void handle_food(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == 'F') {
        player->health += 10; // Increase health by 10
        player->hunger -= 5; // Reduce hunger by 5
        map[y][x] = '.'; // Remove food from map
    }
}

void handle_gold(char map[MAP_HEIGHT][MAP_WIDTH], Player *player) {
    int x = player->x;
    int y = player->y;
    if (map[y][x] == '*') {
        player->gold += 50; // Increase gold count by 50
        map[y][x] = '.'; // Remove gold from map
    }
}

// Example usage in main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));

    char map[MAP_HEIGHT][MAP_WIDTH];
    init_map(map);

    generate_random_rooms(map, 10); // Generate 10 random rooms

    Player player = {10, 10, 100, 100, 0}; // Initial player position and stats

    print_map_with_player(map, &player);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        handle_movement(map, &player, ch);
        pick_up_item(map, &player);
        check_trap(map, &player);
        use_staircase(map, &player, '>');
        reveal_secret_door(map, &player);
        handle_food(map, &player);
        handle_gold(map, &player);
        display_status(&player);
        print_map_with_player(map, &player);
        refresh();
    }

    save_game_with_player(map, &player, "savefile.txt"); // Save the game state with player position
    endwin();

    // Load the game state
    initscr();
    noecho();
    curs_set(FALSE);

    load_game_with_player(map, &player, "savefile.txt");
    print_map_with_player(map, &player);
    refresh();

    getch(); // Wait for user input
    endwin();

    return 0;
}



