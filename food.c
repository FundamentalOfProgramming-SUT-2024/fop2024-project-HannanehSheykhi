#include <ncurses.h>
#include <string.h>

// Function prototypes
void showMainMenu();
void showWeaponMenu();
void showFoodMenu();
void showCharmMenu();

void showMainMenu() {
    clear();
    mvprintw(0, 0, "Main Menu:");
    mvprintw(1, 0, "1. Weapons");
    mvprintw(2, 0, "2. Food");
    mvprintw(3, 0, "3. Charms");
    mvprintw(4, 0, "Enter your choice: ");
    refresh();

    int choice = getch();
    switch (choice) {
        case '1':
            showWeaponMenu();
            break;
        case '2':
            showFoodMenu();
            break;
        case '3':
            showCharmMenu();
            break;
        default:
            break;
    }
}

void showWeaponMenu() {
    clear();
    mvprintw(0, 0, "Weapon Menu:");
    mvprintw(1, 0, "1. Mace (5)");
    mvprintw(2, 0, "2. Dagger (12)");
    mvprintw(3, 0, "3. Magic Wand (15)");
    mvprintw(4, 0, "4. Normal Arrow (5)");
    mvprintw(5, 0, "5. Sword (10)");
    mvprintw(6, 0, "Enter any key to return to main menu...");
    refresh();
    getch();
    showMainMenu();
}

void showFoodMenu() {
    clear();
    mvprintw(0, 0, "Food Menu:");
    mvprintw(1, 0, "1. Usual Food (Increases health once)");
    mvprintw(2, 0, "2. Expensive Food (Increases health twice)");
    mvprintw(3, 0, "3. Magical Food (Increases health 3 times)");
    mvprintw(4, 0, "4. Spoiled Food (Decreases health once)");
    mvprintw(5, 0, "Enter any key to return to main menu...");
    refresh();
    getch();
    showMainMenu();
}

void showCharmMenu() {
    clear();
    mvprintw(0, 0, "Charm Menu:");
    mvprintw(1, 0, "1. Health Magic (Increases health twice)");
    mvprintw(2, 0, "2. Speed Magic");
    mvprintw(3, 0, "3. Damage Magic");
    mvprintw(4, 0, "Enter any key to return to main menu...");
    refresh();
    getch();
    showMainMenu();
}

int main() {
    initscr();
    noecho();
    cbreak();

    showMainMenu();

    endwin();
    return 0;
}
