#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

typedef struct {
   char username[50];
   int score;
   int gold;
   int gameplayed;
   char title [20];
   int experience;
} Player;

Player players[100];
int playerCount;

void init() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
}

// Function prototype declaration for displayPage
void displayPage(Player players[], int start, int end, int count);

void showPreGameMenu() {
    clear();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 10, "---Welcome to the game!---\n");
    attroff(COLOR_PAIR(1) | A_BOLD);
    attron(COLOR_PAIR(4));
    mvprintw(4, 10, "1. Start New Game\n");
    mvprintw(5, 10, "2. Continue Last Game\n");
    mvprintw(6, 10, "3. View Scoreboard\n");
    mvprintw(7, 10, "4. Settings\n");
    mvprintw(8, 10, "5. Profile\n");
    mvprintw(9, 10, "6. Back to Main Menu\n");
    mvprintw(10, 10, "Select an option: ");
    attron(COLOR_PAIR(4));
    refresh();
}

void sortPlayersByScore(Player players[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].score < players[j + 1].score || 
                (players[j].score == players[j + 1].score && strcmp(players[j].username, players[j + 1].username) > 0)) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    if (count > 0) strcpy(players[0].title, "GOAT");
    if (count > 1) strcpy(players[1].title, "Legend");
    if (count > 2) strcpy(players[2].title, "Champion");
}

void addPlayer(Player players[], int *count, const char *username) {
    snprintf(players[*count].username, sizeof(players[*count].username), "%s", username);
    players[*count].score = 0;
    players[*count].gold = 0;
    players[*count].gameplayed = 0;
    players[*count].experience = 0;
    strcpy(players[*count].title, "Player");
    (*count)++;
}

void updateScore(Player players[], int count, const char *username, int newScore) {
    for (int i = 0; i < count; i++) {
        if (strcmp(players[i].username, username) == 0) {
            players[i].score += newScore;
            players[i].gameplayed++;
            break;
        }
    }
}

void showScoreboard(Player players[], int count) {
    int page = 0;
    int totalPages = (count + 10 - 1) / 10;
    while (1) {
        int start = page * 10;
        int end = start + 10;
        if (end > count) {
            end = count;
        }
        displayPage(players, start, end, count);
        int ch = getch();
        if (ch == KEY_RIGHT && page < totalPages - 1) {
            page++;
        } else if (ch == KEY_LEFT && page > 0) {
            page--;
        } else if (ch == 'q') {
            break;
        }
    }
}

void displayPage(Player players[], int start, int end, int count) {
    clear();
    mvprintw(2, 10, "Scoreboard - Page %d", start / 10 + 1);
    mvprintw(4, 2, "Rank\tUsername\tScore\tGold\tGames Played\tExperience\tTitle");

    for (int i = start; i < end; i++) {
        if (i < 3) {
            attron(COLOR_PAIR(1));
            attron(A_BOLD);       
        } else {
            attron(COLOR_PAIR(3));
        }
        mvprintw(6 + i - start, 2, "%d\t%s\t\t%d\t%d\t%d\t\t%d\t%s", 
                 i + 1, players[i].username, players[i].score, 
                 players[i].gold, players[i].gameplayed, 
                 players[i].experience, players[i].title);

        if (i < 3) {
            attroff(A_BOLD);
            attroff(COLOR_PAIR(1));
        } else {
            attroff(COLOR_PAIR(3));
        }
    }

    attron(COLOR_PAIR(4));
    mvprintw(6, 70, "🏆");
    mvprintw(7, 70, "🥈");
    mvprintw(8, 70, "🥉");
    attroff(COLOR_PAIR(4));

    mvprintw(18, 10, "Press LEFT/RIGHT to scroll, 'q' to return to menu...");
    refresh();
}

int main() {
    init();
    int choice;
    showPreGameMenu();
    choice = getch() - '0';
    switch (choice) {
    case 1:
        clear();
        mvprintw(10, 10, "Starting New Game...");
        break;
    case 2:
        clear();
        mvprintw(10, 10, "Continuing Previous Game...");
        break;
    case 3:
        showScoreboard(players, playerCount);
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        endwin();
        return 0;
    default: 
        mvprintw(10, 10, "Invalid option! Please try again ");
        getch();
    }
    refresh();
    return 0;
}
