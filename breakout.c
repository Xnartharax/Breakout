#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#define HEIGHT 17
#define WIDTH 33
#define PADDLE_Y 15
enum{
    NONE=0,
    BRICK,
    WALL,
    PADDLE,
    DEATH_ZONE
};
enum{
    C_BRICK=1,
    C_WALL,
    C_BALL,
    C_PADDLE,
    C_DEATH
};
int bricks = 0;
int x_paddle = 17;
int x_ball = 17;
int y_ball = 14;
static int game [HEIGHT][WIDTH];
int inv_v_x = 3;
int inv_v_y = 3;
void init(){
    int i;
    for(i=0;i<HEIGHT;i++){
        game[i][WIDTH-1] = WALL;
        game[i][0] = WALL;
    }
    for(i=0;i<WIDTH;i++){
         game[0][i] = WALL;
         game[HEIGHT-1][i] = DEATH_ZONE;
    }
    int y;
    int x;
    for(y=2;y<10;y++){
        for(x=2; x<WIDTH-2;x++){
            game[y][x] = BRICK;
            bricks++;
        }
    }
}
void print_game(){
    clear();
    int x;
    int y;
    for(y=0;y<HEIGHT;y++){
        for(x=0;x<WIDTH;x++){
            if(game[y][x] == WALL){
                attron(COLOR_PAIR(C_WALL));
                printw("#");
                attroff(COLOR_PAIR(C_WALL));
            }
            else if(game[y][x] == BRICK){
                attron(COLOR_PAIR(C_BRICK));
                printw("#");
                attroff(COLOR_PAIR(C_BRICK));
            }
            else if(y==PADDLE_Y && (x==x_paddle|| x==x_paddle-1 || x==x_paddle+1)){
                attron(COLOR_PAIR(C_PADDLE));
                printw("^");
                attroff(COLOR_PAIR(C_PADDLE));
            }
            else if(y==y_ball && x==x_ball){
                attron(COLOR_PAIR(C_BALL));
                printw("+");
                attroff(COLOR_PAIR(C_BALL));
            }
            else if(game[y][x] == DEATH_ZONE){
                attron(COLOR_PAIR(C_DEATH));
                printw("#");
                attroff(COLOR_PAIR(C_DEATH));
            }
            else{
                printw(" ");
            }
            
        }
        printw("\n");
    }
    refresh();
}

void frame(int count){
    int new_x_ball = x_ball;
    int new_y_ball = y_ball;
    char *c;
    if((c = getch()) != ERR){
        if(c=='a'){
            if (x_paddle>3)
            x_paddle--;
        }
        if(c=='d'){
            if (x_paddle<WIDTH-3)
            x_paddle++;
        }
    }
    while(getch()!=ERR); //clearing all additional key presses
    if (count % inv_v_x == 0){
        if(inv_v_x>0)
            new_x_ball-=1;
        if (inv_v_x<0)
            new_x_ball+=1;
    }
    if (count % inv_v_y == 0){
        if(inv_v_y>0)
            new_y_ball-=1;
        if (inv_v_y<0)
            new_y_ball+=1;
    }
    if(new_x_ball>=x_paddle-1 && new_x_ball <= x_paddle+1 && new_y_ball == PADDLE_Y){
            inv_v_y = -inv_v_y;
            if(new_x_ball == x_paddle-1 || x_paddle+1){
                if(inv_v_x <0)
                    inv_v_x = -2;
                else
                    inv_v_x = 2;
            }
            else
            {
                inv_v_x = 2;
            }
            
            if(inv_v_x < 0) x_ball++;
            else if(inv_v_x > 0) x_ball--;
    }
    else if (game[new_y_ball][new_x_ball] == NONE){
        
        
            x_ball = new_x_ball;
            y_ball = new_y_ball;
        
    }
    else{
        if(game[new_y_ball][new_x_ball] == BRICK){
            game[new_y_ball][new_x_ball] = NONE;
            bricks--;
            inv_v_y = -inv_v_y;
            if(bricks == 0){
                while(getch()!=ERR); //clearing all additional key presses
                endwin();
                printw("YOU WON\n");
                exit(0);
            }
        }
        else if(game[new_y_ball][new_x_ball] == DEATH_ZONE){
            while(getch()!=ERR); //clearing all additional key presses
            endwin();
            printw("YOU LOST\n");
            exit(0);
        }
        else{
            if (new_x_ball == 0 || new_x_ball == WIDTH-1){
                inv_v_x = -inv_v_x;
            }
            if (new_y_ball==0){
                inv_v_y = -inv_v_y;
            }
        }

    }
    print_game();
    usleep(100*1000);
}
int main(){
    initscr();
    nodelay(stdscr, TRUE);
    init();
    print_game();
    int count = 0;
    start_color();
    init_pair(C_BRICK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(C_BALL, COLOR_CYAN, COLOR_BLACK);
    init_pair(C_PADDLE, COLOR_GREEN, COLOR_BLACK);
    init_pair(C_WALL, COLOR_WHITE, COLOR_BLACK);
    init_pair(C_DEATH, COLOR_RED, COLOR_RED);
    while(1){
        frame(count);
        count++;
    }
    endwin();
    return 0;
}