#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

void draw_title() {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Calculate the center position for the title
    int title_row = row / 2 - 5;
    int title_col = col / 2 - 10;

    // Draw the title
    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(title_row, title_col, "Welcome to Rogue Dungeons!");
    attroff(A_BOLD | COLOR_PAIR(1));
}

void draw_instructions() {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Calculate the center position for the instructions
    int instr_row = row / 2 - 2;
    int instr_col = col / 2 - 20;

    // Draw the instructions
    attron(COLOR_PAIR(1));
    mvprintw(instr_row, instr_col, "Instructions:");
    mvprintw(instr_row + 1, instr_col, "Use movement controls to move around.");
    mvprintw(instr_row + 2, instr_col, "Fight monsters and find treasures!");
    mvprintw(instr_row + 3, instr_col, "Press any key to start the game...");
    attroff(COLOR_PAIR(1));
}

void draw_border() {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Draw a border around the screen using Unicode characters
    attron(COLOR_PAIR(1));
    for (int i = 0; i < col; i++) {
        mvprintw(0, i, "═");  // Top border
        mvprintw(row - 1, i, "═");  // Bottom border
    }

    for (int i = 0; i < row; i++) {
        mvprintw(i, 0, "║");  // Left border
        mvprintw(i, col - 1, "║");  // Right border
    }

    // Draw corners
    mvprintw(0, 0, "╔");  // Top-left corner
    mvprintw(0, col - 1, "╗");  // Top-right corner
    mvprintw(row - 1, 0, "╚");  // Bottom-left corner
    mvprintw(row - 1, col - 1, "╝");  // Bottom-right corner
    attroff(COLOR_PAIR(1));
}

void draw_producer_credit() {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Calculate the position for the producer credit
    int credit_row = row - 2;  // Second-to-last row
    int credit_col = col / 2 - 10;

    // Draw the producer credit
    attron(COLOR_PAIR(1));
    mvprintw(credit_row, credit_col, "Produced by H.SH");
    attroff(COLOR_PAIR(1));
}

void draw_movement_controls() {
    int row, col;
    getmaxyx(stdscr, row, col);

    // Calculate the position for the movement controls textbox
    int controls_row = row / 2 - 4;
    int controls_col = 4;  // Left-aligned with some padding

    // Draw the movement controls
    attron(COLOR_PAIR(1));
    mvprintw(controls_row, controls_col, "Movement Controls:");
    mvprintw(controls_row + 1, controls_col, " Y   J   U");
    mvprintw(controls_row + 2, controls_col, "  ↖  ↑  ↗");
    mvprintw(controls_row + 3, controls_col,"H ←  *  → L");
    mvprintw(controls_row + 4, controls_col, "  ↙  ↓  ↘");
    mvprintw(controls_row + 5, controls_col, " B   K   N");
    attroff(COLOR_PAIR(1));
}

int main() {
    // Initialize ncurses
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Start color mode
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  // White text on black background

    // Clear the screen
    clear();

    // Draw the entrance page
    draw_title();
    draw_instructions();
    draw_border();
    draw_producer_credit();
    draw_movement_controls();

    // Refresh the screen to show the changes
    refresh();

    // Wait for a key press to start the game
    getch();

    // End ncurses
    endwin();

    // Start the game (you can replace this with your game logic)
    printf("Starting the game...\n");

    return 0;
}