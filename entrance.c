#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

void draw_title() {
    int row, col;
    getmaxyx(stdscr, row, col);

    int title_row = row / 2 - 5;
    int title_col = col / 2 - 10;

    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(title_row, title_col, "Welcome to Rogue Dungeons!");
    attroff(A_BOLD | COLOR_PAIR(1));
}

void draw_instructions() {
    int row, col;
    getmaxyx(stdscr, row, col);

    int instr_row = row / 2 - 2;
    int instr_col = col / 2 - 20;

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

    attron(COLOR_PAIR(1));
    for (int i = 0; i < col; i++) {
        mvprintw(0, i, "═");  
        mvprintw(row - 1, i, "═"); 
    }

    for (int i = 0; i < row; i++) {
        mvprintw(i, 0, "║");  
        mvprintw(i, col - 1, "║"); 
    }

    mvprintw(0, 0, "╔");  
    mvprintw(0, col - 1, "╗");  
    mvprintw(row - 1, 0, "╚");  
    mvprintw(row - 1, col - 1, "╝"); 
    attroff(COLOR_PAIR(1));
}

void draw_producer_credit() {
    int row, col;
    getmaxyx(stdscr, row, col);

    int credit_row = row - 2;  
    int credit_col = col / 2 - 10;

    attron(COLOR_PAIR(1));
    mvprintw(credit_row, credit_col, "Produced by H.SH");
    attroff(COLOR_PAIR(1));
}

void draw_movement_controls() {
    int row, col;
    getmaxyx(stdscr, row, col);

    int controls_row = row / 2 - 4;
    int controls_col = 4;  

    attron(COLOR_PAIR(1));
    mvprintw(controls_row, controls_col, "Movement Controls:");
    mvprintw(controls_row + 1, controls_col, " Y   J   U");
    mvprintw(controls_row + 2, controls_col, "  ↖  ↑  ↗");
    mvprintw(controls_row + 3, controls_col,"H ←  *  → L");
    mvprintw(controls_row + 4, controls_col, "  ↙  ↓  ↘");
    mvprintw(controls_row + 5, controls_col, " B   K   N");
    attroff(COLOR_PAIR(1));
}

void draw_rogue_art() {
    int row, col;
    getmaxyx(stdscr, row, col);

    int art_row = row / 2 - 14;  
    int art_col = col - 40;  
    attron(COLOR_PAIR(1));
    mvprintw(art_row, art_col, "  _____");
    mvprintw(art_row + 1, art_col, " |  __ \\");
    mvprintw(art_row + 2, art_col, " | |__) |");
    mvprintw(art_row + 3, art_col, " |  _  / ");
    mvprintw(art_row + 4, art_col, " | | \\ \\ ");
    mvprintw(art_row + 5, art_col, " |_|  \\_\\ ");
    mvprintw(art_row + 7, art_col, "  ____");
    mvprintw(art_row + 8, art_col, " / __ \\");
    mvprintw(art_row + 9, art_col, "| |  | |");
    mvprintw(art_row + 10, art_col, "| |  | |");
    mvprintw(art_row + 11, art_col, "| |__| |");
    mvprintw(art_row + 12, art_col, " \\____/");
    mvprintw(art_row + 14, art_col, "  ____");
    mvprintw(art_row + 15, art_col, " / ___|");
    mvprintw(art_row + 16, art_col, "| |  __");
    mvprintw(art_row + 17, art_col, "| | |_ |");
    mvprintw(art_row + 18, art_col, "| |__| |");
    mvprintw(art_row + 19, art_col, " \\____|");
    mvprintw(art_row + 21, art_col, " _    _");
    mvprintw(art_row + 22, art_col, "| |  | |");
    mvprintw(art_row + 23, art_col, "| |  | |");
    mvprintw(art_row + 24, art_col, "| |  | |");
    mvprintw(art_row + 25, art_col, "| |__| |");
    mvprintw(art_row + 26, art_col, " \\____/");
    mvprintw(art_row + 28, art_col, " _____");
    mvprintw(art_row + 29, art_col, "|  ___|");
    mvprintw(art_row + 30, art_col, "| |__");
    mvprintw(art_row + 31, art_col, "|  __|");
    mvprintw(art_row + 32, art_col, "| |___");
    mvprintw(art_row + 33, art_col, "|_____|");
    attroff(COLOR_PAIR(1));
}

int render1() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  

    clear();

    draw_title();
    draw_instructions();
    draw_border();
    draw_producer_credit();
    draw_movement_controls();
    draw_rogue_art();  

    refresh();
    getch();

    endwin();

    printf("Starting the game...\n");

}

int main() {
    render1();
    render2();
    render3();
    render4();
    return 0;
}
