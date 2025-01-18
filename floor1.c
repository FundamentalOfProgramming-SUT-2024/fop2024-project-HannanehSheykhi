#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
typedef struct {
    int x,y;
    int width, height;
}Room;
char map[40][40];
int seen[40][40];
int playerX = 20;  
int playerY = 20;
Room createRoom() {
    Room room;
    room.width=rand()%(3)+4;
    room.height=rand()%(3)+4;
    room.x=rand()%(40-room.width-2)+1;
    room.y=rand()%(40-room.height-2)+1;
    return room;
}
void initializeMap(){
    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(i==0 || i==39 || j==0 || j==39){
                map[i][j]='*';
            }
            else{
                map[i][j]=' ';
            }
            seen[i][j]=0;
        }
    }
}
void drawMap(){
    for(int i=0; i<40; i++){
        for(int j=0; j<40; j++){
            if(abs(playerX-j)<6 && abs(playerY-i)<6 || seen[i][j] || map[i][j]=='*'){
                mvaddch(i,j,map[i][j]);
            }
            else{
                mvaddch(i,j,' ');
            }
        }
    }
}

int is_valid_move(int x, int y) {
    if(map[y][x]== '|' || map[y][x]== '=' || map[y][x]== '_'|| map[y][x]== 'o'){
        return 0;
    }
    return 1;
}
void initializeRoom(Room room){
    for(int i=room.y+1; i<room.y+room.height-1; i++){
        for(int j=room.x+1; j<room.x+ room.width-1; j++){
           map[i][j]='.';
        }
    }
    for(int i=room.y; i<room.y+room.height; i++){
        map[i][room.x]='|';
        map[i][room.x+room.width-1]='|';
    }
    for(int i=room.x; i<room.x+room.width; i++){
        map[room.y][i]='_';
        map[room.y+ room.height-1][i]='-';

    }
}
/*int window(Room room){
   int windowposition=rand()%4;
   int windowx=0, windowy =0;
   switch(windowposition){
     case 0:
     windowx= room.x+ rand()%(room.width-2)+1;
     windowy=room.y;
     break;
     case 1:
     windowx= room.x+ rand()%(room.width-2)+1;
     windowy=room.y+room.height-1;
     break;
     case 2:
     windowx= room.x;
     windowy=room.y+ rand()%(room.height-2)+1;
     break;
     case 3:
     windowx= room.x+ room.width-1;
     windowy=room.y+ rand()%(room.height-2)+1;
     break;
   } 
   map[windowy][windowx]= '=';  
   int pos[]={windowy,windowx};
   return pos; 
}
void pillar(Room room){
           int x=rand()%(room.height-room.x-2)+room.x+1;
          int  y=rand()%(room.width-room.y-2)+room.y+1; 
           map[x][y]='O';
}
int door(Room room){
   int doorposition=rand()%4;
   int doorx=0, doory =0;
   switch(doorposition){
     case 0:
     doorx= room.x+ rand()%(room.width-2)+1;
     doory=room.y;
     break;
     case 1:
    doorx= room.x+ rand()%(room.width-2)+1;
     doory=room.y+room.height-1;
     break;
     case 2:
     doorx= room.x;
     doory=room.y+ rand()%(room.height-2)+1;
     break;
     case 3:
     doorx= room.x+ room.width-1;
     doory=room.y+ rand()%(room.height-2)+1;
     break;
   }   
   map[doory][doorx]= '+';  
   int pos[]={doory,doorx};
   return pos;
}
void drawCorridor(Room room1, Room room2) {
    initializeRoom(room1);
    initializeRoom(room2);
    int pos1[]= door(room1);
    int pos2[]= door(room2);

    int x1 = pos1[0];
    int y1 = pos1[1];
    int x2 = pos2[0];
    int y2 = pos2[1];

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        map[y1][x1] = '#'; 

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}*/
int* door(Room room) {
    int doorposition = rand() % 4;
    int doorx = 0, doory = 0;
    static int pos[2];

    switch (doorposition) {
        case 0:
            doorx = room.x + rand() % (room.width - 2) + 1;
            doory = room.y;
            break;
        case 1:
            doorx = room.x + rand() % (room.width - 2) + 1;
            doory = room.y + room.height - 1;
            break;
        case 2:
            doorx = room.x;
            doory = room.y + rand() % (room.height - 2) + 1;
            break;
        case 3:
            doorx = room.x + room.width - 1;
            doory = room.y + rand() % (room.height - 2) + 1;
            break;
    }

    map[doory][doorx] = '+';
    pos[0] = doory;
    pos[1] = doorx;
    return pos;
}

void drawCorridor(Room room1, Room room2) {
    initializeRoom(room1);
    initializeRoom(room2);

    int* pos1 = door(room1);
    int* pos2 = door(room2);

    int x1 = pos1[0];
    int y1 = pos1[1];
    int x2 = pos2[0];
    int y2 = pos2[1];

    if (x1 == x2) {
        for (int y = y1; y <= y2; y++) {
            map[y][x1] = '#';
        }
    }

    else if (y1 == y2) {
        for (int x = x1; x <= x2; x++) {
            map[y1][x] = '#';
        }
    }

    else {
        for (int x = x1; x != x2; x += (x2 > x1 ? 1 : -1)) {
            map[y1][x] = '#';
        }
        for (int y = y1; y != y2; y += (y2 > y1 ? 1 : -1)) {
            map[y][x2] = '#';
        }
    }
}
 void updateSeen() {
    for (int y = playerY - 6; y <= playerY + 6; y++) {
        for (int x = playerX - 6; x <= playerX + 6; x++) {
            if (x >= 0 && x < 40 && y >= 0 && y < 40) {
                seen[y][x] = 1; 
            }
        }
    }
}


void move_cursor(int *x, int *y, int max_x, int max_y, int ch) {
    int new_x = *x;
    int new_y = *y;

    switch (ch) {
        case KEY_UP:
            if (*y > 0) new_y--;
            break;
        case KEY_DOWN:
            if (*y < max_y - 1) new_y++;
            break;
        case KEY_LEFT:
            if (*x > 0) new_x--;
            break;
        case KEY_RIGHT:
            if (*x < max_x - 1) new_x++;
            break;
    }

    if (is_valid_move(new_x, new_y)) {
        *x = new_x;
        *y = new_y;
    }
}

void draw_player(int x, int y) {
    mvaddch(y, x, '@');
}

int main() {
    int ch;

    initscr();           
    cbreak();            
    noecho();            
    curs_set(0);         
    srand(time(NULL));   
     initializeMap();     

    int numRooms = rand() % (10 - 4) + 4;
    if (numRooms < 6) numRooms = 6;
    Room rooms[numRooms];
    for (int i = 0; i < numRooms; i++) {
        Room room = createRoom();
        initializeRoom(room);
        rooms[i] = room;

        if (i > 0) {
            drawCorridor(rooms[i - 1], room);
        }
    }


    keypad(stdscr, TRUE);

    draw_player(playerX, playerY);

    refresh();

    while ((ch = getch()) != 'q') {
        mvaddch(playerY, playerX, '.'); 
        move_cursor(&playerX, &playerY, 40, 40, ch); 
        mvaddch(playerY, playerX, '@'); 
        //draw_player(playerX, playerY);
         updateSeen();
         drawMap();   
        refresh(); 
    }

    endwin();

    return 0;
}