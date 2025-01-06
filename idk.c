#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int is_valid_move(int x, int y) {
    char ch = mvinch(y, x) & A_CHARTEXT;
    return !(ch == '|' || ch == '=' || ch == '-');
}

void move_cursor(int *x, int *y, int max_x, int max_y, int ch) {
    int new_x = *x;
    int new_y = *y;

    switch (ch) {
        case KEY_UP:
            if (*y > 0) new_y--;
            break;
        case KEY_DOWN:
            if (*y < max_y) new_y++;
            break;
        case KEY_LEFT:
            if (*x > 0) new_x--;
            break;
        case KEY_RIGHT:
            if (*x < max_x) new_x++;
            break;
    }

    if (is_valid_move(new_x, new_y)) {
        *x = new_x;
        *y = new_y;
        move(*y, *x);
        addch('#'); // Leave a # where the cursor moves
    }
}

void create_paths() {
    // Create specific paths between doors
    mvprintw(17, 10, "#"); mvprintw(17, 11, "#"); mvprintw(17, 12, "#"); mvprintw(17, 13, "#"); mvprintw(17, 14, "#"); mvprintw(17, 15, "#");

    mvprintw(5, 11, "#"); mvprintw(5, 12, "#"); mvprintw(5, 13, "#"); mvprintw(5, 14, "#"); mvprintw(5, 15, "#"); mvprintw(5, 16, "#");
    
    mvprintw(6, 20, "#"); mvprintw(7, 20, "#"); mvprintw(8, 20, "#"); mvprintw(9, 20, "#"); mvprintw(10, 20, "#"); mvprintw(11, 20, "#");
    mvprintw(12, 20, "#"); mvprintw(13, 20, "#"); mvprintw(14, 20, "#"); mvprintw(15, 20, "#"); mvprintw(16, 20, "#"); mvprintw(17, 20, "#");

    mvprintw(13, 26, "#"); mvprintw(14, 26, "#"); mvprintw(15, 26, "#"); mvprintw(16, 26, "#"); mvprintw(17, 26, "#"); mvprintw(18, 26, "#");
    mvprintw(19, 26, "#"); mvprintw(20, 26, "#"); mvprintw(21, 26, "#"); mvprintw(22, 26, "#"); mvprintw(23, 26, "#");
}

int main() {
    int x = 0, y = 0;
    int ch;

    initscr();
    noecho();
    curs_set(FALSE); // Hide the cursor
    keypad(stdscr, TRUE);

    // Add fixed rooms and doors
    mvprintw(13, 10, "-+----");
    mvprintw(14, 10, "|.....|");
    mvprintw(15, 10, "|.o...|");
    mvprintw(16, 10, "|.....|");
    mvprintw(17, 10, "|.....+");
    mvprintw(18, 10, "-=-----");

    mvprintw(2, 20, "--=---");
    mvprintw(3, 20, "|.....|");
    mvprintw(4, 20, "|.....|");
    mvprintw(5, 20, "+.....|");
    mvprintw(6, 20, "|....o|");
    mvprintw(7, 20, "-------");

    mvprintw(8, 26, "---------");
    mvprintw(9, 26, "|o.......+");
    mvprintw(10, 26, "|........|");
    mvprintw(11, 26, "|........|");
    mvprintw(12, 26, "|........|");
    mvprintw(13, 26, "---+-----");

    mvprintw(18, 20, "----+----");
    mvprintw(19, 20, "|...o....|");
    mvprintw(20, 20, "|........|");
    mvprintw(21, 20, "|........|");
    mvprintw(22, 20, "|.....o..|");
    mvprintw(23, 20, "----+-----");

    mvprintw(2, 11, "------");
    mvprintw(3, 11, "|..o..|");
    mvprintw(4, 11, "|.....=");
    mvprintw(5, 11, "+.....|");
    mvprintw(6, 11, "-------");

    mvprintw(3, 40, "---=-----");
    mvprintw(4, 40, "+..o.....|");
    mvprintw(5, 40, "|........|");
    mvprintw(6, 40, "|..o.....|");
    mvprintw(7, 40, "|......o.|");
    mvprintw(8, 40, "-+--------");

    // Create paths between rooms
    create_paths();

    move(y, x);

    while ((ch = getch()) != 'q') {
        mvprintw(y, x, "#"); // Leave a # at the current cursor position
        move_cursor(&x, &y, COLS - 1, LINES - 1, ch);
        mvprintw(y, x, "@"); // Draw cursor at new position
    }

    endwin();
    return 0;
}
