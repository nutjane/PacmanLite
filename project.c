#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <windows.h> //for gotoxy
#include <STDLIB.H> //for random
#include <string.h>
#include "color.h"

#define posx 25
#define posy 3
#define maxstage 2

int bg[20][29];
int point[20][29];
int sumpoint,score=0,catch=0,highscore,stage=1;
char highscore_name[21],highscore_nametemp[21],mapname[5];

struct hero
{
    int pox,poy; //keep itself's position
    int w,a,s,d; //keep if it can move in that position
    float dw,da,ds,dd; //keep direction between itself and pacman
};

struct hero monster;
struct hero pacman;

void game_start();
void newgame();
void nextgame();
void welcome_screen();

void gotoxy(int x, int y)
{
    COORD c = {x,y};
    SetConsoleCursorPosition(  GetStdHandle(STD_OUTPUT_HANDLE) , c);
}

void gotoxyandprint(int x,int y,char s[])
{
    gotoxy(x,y);
    printf(s);
}

void print_delay(int a, int b)
{
    printf("%c",a);
    Sleep(b);
}

void gotoxyandprint_delay(int x, int y, int a, int b)
{
    gotoxy(x,y);
    print_delay(a,b);
}

void file_getmap()
{
    int i=0,j=0;
    switch(stage)
    {
        case 1: strcpy(mapname,"map1.txt"); break;
        case 2: strcpy(mapname,"map2.txt"); break;
    }
    FILE *fone;
    fone=fopen(mapname,"r");
    if(ferror(fone)!=0)
    {
        printf("Sorry! could not open your map file.");
    }
    else
    {
        while (!feof(fone))
        {
            fscanf(fone,"%d",&bg[i][j]);
            j++;
            if(j==29) j==0;
        }
        fclose(fone);
    }
}

void file_gethighscore()
{
    FILE *fone;
    fone=fopen("rankingpm.txt","r");
    if(ferror(fone)!=0)
    {
        printf("Sorry! could not open your score file.");
    }
    else
    {
        fscanf(fone,"%d",&highscore);
        fscanf(fone,"%s",&highscore_name);
        fclose(fone);
    }
}

void file_newhighscore()
{
    FILE *fone;
    fone=fopen("rankingpm.txt","w");
    if(ferror(fone)!=0)
    {
        printf("Sorry! could not save your score.");
    }
    else
    {
        fprintf(fone,"%d %s",score,highscore_nametemp);
        fclose(fone);
    }
}

void header()
{
    int i,j;
    color(darkred,darkred);
    for(i=0;i<112;i++) printf("%c",219);
    color(white,darkred);
    printf("%c P A C M A N %c",254,254);
    color(darkred,darkred);
    for(i=0;i<113;i++) printf("%c",219);
    reset();
}

void dialog_center()
{
    int i,j;
    gotoxy(23,9);
    for(i=0;i<33;i++) print_delay(178,30);
    for(j=0;j<10;j++) gotoxyandprint_delay(55,10+j,178,30);
    for(i=0;i<33;i++) gotoxyandprint_delay(55-i,19,178,30);
    for(j=0;j<10;j++) gotoxyandprint_delay(23,19-j,178,30);
    for(j=0;j<9;j++)
    {
        for(i=0;i<31;i++) gotoxyandprint_delay(24+i,10+j,0,1);
    }
}

void dialog_center_clear()
{
    int i,j;
    for(j=0;j<9;j++)
    {
        for(i=0;i<31;i++)
        {
            gotoxy(24+i,10+j);
            printf("%c",0);
        }
    }
}

int highscore_check(int a)
{
    int check;
    if(a>highscore) check=1;
    else check=0;
    return check;
}

void highscore_congrat()
{
    dialog_center_clear();
    gotoxyandprint(32,11,"CONGRATULATIONS!");
    gotoxy(28,13);
    printf("NEW HIGHSCORE (%d) SAVED",score);
    gotoxy(38-(strlen(highscore_nametemp)/2),15);
    printf("%s",highscore_nametemp);
    gotoxyandprint(27,17,"Press enter to continue...");
    while(getch()!=13) {}
    welcome_screen();

}

void highscore_check_do(int check)
{
    if(!check)
    {
        gotoxyandprint(33,15,"NOT HIGHSCORE");
        gotoxyandprint(27,17,"Press enter to continue...");
        while(getch()!=13) {}
        welcome_screen();
    }
    else
    {
        gotoxy(33,14);
        printf("HIGH score : %d",score);
        gotoxyandprint(25,16,"Enter your name (max 20 char)");
        gotoxyandprint(25,17,"> ");
        scanf("%s",highscore_nametemp);
        file_newhighscore();
        highscore_congrat();
    }
}

void win()
{
    int check;
    gotoxyandprint(10,23,"                       Game is ended.                          ");
    dialog_center();
    gotoxyandprint(30,12,"! ! Y O U   W I N ! !");
    gotoxy(33,14);
    printf("Your score : %d",score);
    if(stage<maxstage) nextgame(); //if user can play next stage, not neccessary to check highscore
    else
    {
        check=highscore_check(score);
        highscore_check_do(check);
    }

}

void lose()
{
    int check;
    gotoxyandprint(10,23,"                       Game is ended.                          ");
    dialog_center();
    gotoxyandprint(30,12,"Y O U   L O S E ! !");
    gotoxy(33,14);
    printf("Your score : %d",score);
    check=highscore_check(score);
    highscore_check_do(check);
}

void exitgame_ask()
{
    char a;
    gotoxyandprint(59,15,"E X I T ??");
    gotoxyandprint(60,16,"Press e to exit");
    gotoxyandprint(60,17,"any key to return.");
    a=getche();
    a=getche();
    if(a=='e') welcome_screen();
    else
    {
    gotoxyandprint(60,15,"                                          ");
    gotoxyandprint(60,16,"                                          ");
    gotoxyandprint(60,17,"                                          ");
    game_start();
    }
}


void pause()
{
    char a;
    gotoxyandprint(60,15,"P  A  U  S  E");
    gotoxyandprint(60,16,"Press any key ");
    gotoxyandprint(60,17,"to return.");
    a=getch();
    a=getch();
    gotoxyandprint(60,15,"                                          ");
    gotoxyandprint(60,16,"                                          ");
    gotoxyandprint(60,17,"                                          ");
    game_start();
}

void stage_print()
{
    int i,j;
    for(i=0;i<25;i++) printf(" ");
    for(j=0;j<20;j++)
    {
        for(i=0;i<29;i++)
        {
            printf("%c",bg[j][i]);
        }
        printf("\n");for(i=0;i<25;i++) printf(" ");
    }

}

void setpoint() //set point in array
{
    int i,j;
    sumpoint=0; //clear value
    for(j=0;j<20;j++)
    {
         for(i=0;i<29;i++)
        {
            if(bg[j][i]==0)
            {
                point[j][i]=1;
                sumpoint=sumpoint+1;
            }
        }
    }
}

void setpoint_print() //print point on screen
{
    int i,j;
    color(yellow,black);
    gotoxy(posx,posy);
    for(j=0;j<20;j++)
    {
         for(i=0;i<29;i++)
        {
            if(point[j][i]==1)
            {
                printf(".");
            }
            else
            {
                gotoxy(posx+i+1,posy+j); //skip 1 position
            }
        }
        printf("\n");
        for(i=0;i<25;i++) printf(" "); //print free space
    }
    reset();

}


void scoreboard(int a)
{
    int i,j;
    score=score+a;
    gotoxyandprint(58,4,"+------------------+");
    gotoxyandprint(58,5,"|  HIGH SCORE :    |");
    gotoxyandprint(58,6,"|  YOUR SCORE :    |");
    gotoxyandprint(58,7,"+------------------+");
    gotoxy(74,6);
    printf("%d",score);
    if(score>highscore) //show new hight score to scoreboard
    {
    gotoxy(58,5);
    printf("|  NEW HIGH SCORE! |");
    }
    else
    {
        gotoxy(74,5);
        printf("%d",highscore);
    }
}

void monster_set_position()
{
    srand(time(NULL));
    monster.pox = (rand() % 29); // random position of monster
    monster.poy = (rand() % 20);
    while(bg[monster.poy][monster.pox]!=0||pacman.pox==monster.pox||pacman.poy==monster.poy) // check if that position is same as stage and pacman position
    {
        monster.pox = (rand() % 29);
        monster.poy = (rand() % 20);
    }
    gotoxy(monster.pox+posx,monster.poy+posy); // go to monster position and print
    color(red,black);
    printf("%c",225);
    reset();
}


void monster_move_up()
{
    gotoxy(monster.pox+posx,monster.poy+posy);
    color(yellow,black);
    if(point[monster.poy][monster.pox]==1) printf("."); //repalce : if that postion pacman never goes before ,print the point symbol
    else printf("%c",0); //if pacman went here, print blank
    monster.poy=monster.poy-1; //change position of monster
    gotoxy(monster.pox+posx,monster.poy+posy); //print monster at new position
    color(red,black);
    printf("%c",225);
    reset();
}

void monster_move_down()
{
    gotoxy(monster.pox+posx,monster.poy+posy);
    color(yellow,black);
    if(point[monster.poy][monster.pox]==1) printf(".");  //repalce : if that postion pacman never goes before ,print the point symbol
    else printf("%c",0); //if pacman went here, print blank
    monster.poy=monster.poy+1; //change position of monster
    gotoxy(monster.pox+posx,monster.poy+posy); //print monster at new position
    color(red,black);
    printf("%c",225);
    reset();
}

void monster_move_left()
{
    gotoxy(monster.pox+posx,monster.poy+posy);
    color(yellow,black);
    if(point[monster.poy][monster.pox]==1) printf(".");  //repalce : if that postion pacman never goes before ,print the point symbol
    else printf("%c",0); //if pacman went here, print blank
    monster.pox=monster.pox-1; //change position of monster
    gotoxy(monster.pox+posx,monster.poy+posy);  //print monster at new position
    color(red,black);
    printf("%c",225);
    reset();
}

void monster_move_right()
{
    gotoxy(monster.pox+posx,monster.poy+posy);
    color(yellow,black);
    if(point[monster.poy][monster.pox]==1) printf("."); //repalce : if that postion pacman never goes before ,print the point symbol
    else printf("%c",0); //if pacman went here, print blank
    monster.pox=monster.pox+1; //change position of monster
    gotoxy(monster.pox+posx,monster.poy+posy); //print monster at new position
    color(red,black);
    printf("%c",225);
    reset();
}

void monster_compute() //compute length between monster and pacman
{
    monster.w=0; monster.s=0; monster.a=0; monster.d=0;
    if(bg[monster.poy-1][monster.pox]==0) monster.w=1; //if monster can move up (not a border)
    if(bg[monster.poy+1][monster.pox]==0) monster.s=1; //if monster can move down (not a border)
    if(bg[monster.poy][monster.pox-1]==0) monster.a=1; //if monster can move left (not a border)
    if(bg[monster.poy][monster.pox+1]==0) monster.d=1; //if monster can move right (not a border)
    if(monster.w==1) monster.dw=sqrt(pow(pacman.pox-(monster.pox),2)+pow(pacman.poy-(monster.poy-1),2)); //compute distance between monster (in up position )and pacman
    else monster.dw=9999; //if can't move (border), set it to maximum value for comparison (no length can greater than this length)
    if(monster.s==1) monster.ds=sqrt(pow(pacman.pox-(monster.pox),2)+pow(pacman.poy-(monster.poy+1),2)); //compute distance between monster (in down position) and pacman
    else monster.ds=9999;  //if can't move (border), set it to maximum value for comparison (no length can greater than this length)
    if(monster.a==1)  monster.da=sqrt(pow(pacman.pox-(monster.pox-1),2)+pow(pacman.poy-(monster.poy),2)); //compute distance between monster (in left position) and pacman
    else monster.da=9999;  //if can't move (border), set it to maximum value for comparison (no length can greater than this length)
    if(monster.d==1) monster.dd=sqrt(pow(pacman.pox-(monster.pox+1),2)+pow(pacman.poy-(monster.poy),2)); //compute distance between monster (in right position) and pacman
    else monster.dd=9999;  //if can't move (border), set it to maximum value for comparison (no length can greater than this length)
}

void monster_compare() //to compare which is the best direction to move
{
   float a[4],dmin;
   int i,choose;
   a[0]=monster.dw;
   a[1]=monster.ds;
   a[2]=monster.da;
   a[3]=monster.dd;
   dmin=9999;
   for(i=0;i<=3;i++) //compare
   {
       if(a[i]<dmin)
       {
           dmin=a[i];
           choose=i;
       }
   }
    Sleep(200);
    switch(choose)
    {
        case 0: monster_move_up(); break;
        case 1: monster_move_down(); break;
        case 2: monster_move_left(); break;
        case 3: monster_move_right(); break;
    }
}

void monster_move()
{
    monster_compute(); //to compute length of each direction between monster and pacman
    monster_compare(); //to compare which is the best direction to move
}

void monster_pre_move()
{
    while(kbhit()==0) //move weather user input anything or not
        {
            monster_move();
            if(pacman.poy==monster.poy&pacman.pox==monster.pox) break; //if pacman is captured
        }
}


void pacman_set_position()
{
    srand(time(NULL));
    pacman.pox = (rand() % 29); // random position of pacman
    pacman.poy = (rand() % 20);
    while(bg[pacman.poy][pacman.pox]!=0) // check if pacman position is same as stage position
    {
        pacman.pox = (rand() % 29); // random position of pacman again
        pacman.poy = (rand() % 20);
    }
    gotoxy(pacman.pox+posx,pacman.poy+posy); // go to pacman position and print
    color(cyan,black);
    printf("%c",2);
    reset();
    point[pacman.poy][pacman.pox]=2; //set point array in that position means 2
}

void pacman_move_up()
{
    if(bg[pacman.poy-1][pacman.pox]==0) //if pacman can move to that direction (not a border)
    {
    if(point[pacman.poy-1][pacman.pox]==1) scoreboard(1); //if that postion pacman never goes before ,so get point
    gotoxy(pacman.pox+posx,pacman.poy+posy); //repalce : print blank at lastest position
    printf("%c",0);
    pacman.poy=pacman.poy-1; //change position of pacman
    gotoxy(pacman.pox+posx,pacman.poy+posy); //print pacman at new position
    color(cyan,black);
    printf("%c",2);
    reset();
    point[pacman.poy][pacman.pox]=2; //keep position that pacman ever been (if come again ,get no point)

    }
}

void pacman_move_down()
{
    if(bg[pacman.poy+1][pacman.pox]==0) //if pacman can move to that direction (not a border)
    {
    if(point[pacman.poy+1][pacman.pox]==1) scoreboard(1); //if that postion pacman never goes before ,so get point
    gotoxy(pacman.pox+posx,pacman.poy+posy); //repalce : print blank at lastest position
    printf("%c",0);
    pacman.poy=pacman.poy+1; //change position of pacman
    gotoxy(pacman.pox+posx,pacman.poy+posy); //print pacman at new position
    color(cyan,black);
    printf("%c",2);
    reset();
    point[pacman.poy][pacman.pox]=2; //keep position that pacman ever been (if come again ,get no point)
    }
}

void pacman_move_left()
{
    if(bg[pacman.poy][pacman.pox-1]==0) //if pacman can move to that direction (not a border)
    {
    if(point[pacman.poy][pacman.pox-1]==1) scoreboard(1); //if that postion pacman never goes before ,so get point
    gotoxy(pacman.pox+posx,pacman.poy+posy); //repalce : print blank at lastest position
    printf("%c",0);
    pacman.pox=pacman.pox-1; //change position of pacman
    gotoxy(pacman.pox+posx,pacman.poy+posy); //print pacman at new position
    color(cyan,black);
    printf("%c",2);
    reset();
    point[pacman.poy][pacman.pox]=2; //keep position that pacman ever been (if come again ,get no point)

    }
}

void pacman_move_right()
{
    if(bg[pacman.poy][pacman.pox+1]==0) //if pacman can move to that direction (not a border)
    {
    if(point[pacman.poy][pacman.pox+1]==1) scoreboard(1); //if that postion pacman never goes before ,so get point
    gotoxy(pacman.pox+posx,pacman.poy+posy); //repalce : print blank at lastest position
    printf("%c",0);
    pacman.pox=pacman.pox+1; //change position of pacman
    gotoxy(pacman.pox+posx,pacman.poy+posy); //print pacman at new position
    color(cyan,black);
    printf("%c",2);
    reset();
    point[pacman.poy][pacman.pox]=2; //keep position that pacman ever been (if come again ,get no point)
    }
}

void clearallhistory()
{
    int i,j;
    catch=0;
    score=0;
    for(i=0;i<20;i++)
    {
        for(j=0;j<29;j++)
        {
            point[i][j]=0;
        }
    }
}

void clearstage() //clear point array before playing next stage
{
    int i,j;
    for(i=0;i<20;i++)
    {
        for(j=0;j<29;j++)
        {
            point[i][j]=0;
        }
    }
}

void game_start()
{
    char a,i;
    while(a!='p'&&a!='e'&&score!=(sumpoint-1)&&catch==0)
    {
        gotoxy(21,23);
        printf("Press any key to start [arrow : %c %c %c %c]",24,25,27,26);
        if(kbhit()) //do it whether type any key or not
            {
            gotoxy(10,23);
            printf("Please use arrow %c %c %c %c to move your pacman [p=pause e=exit]",24,25,27,26);
            a=getch();
            switch(a)
            {
            case 72: pacman_move_up(); monster_pre_move(); break;
            case 80: pacman_move_down(); monster_pre_move(); break;
            case 75: pacman_move_left(); monster_pre_move(); break;
            case 77: pacman_move_right(); monster_pre_move(); break;
            default:  monster_pre_move(); break;
            }
            if(pacman.poy==monster.poy&&pacman.pox==monster.pox) catch=1; //check if pacman is captured by monster
        }
    }
    //lose();
    //pause();
    //exitgame_ask();
    if(catch) lose();
    else if(score==(sumpoint-1)) win();
    else
    {
      switch(a)
        {
        case 'p': pause(); break;
        case 'e': exitgame_ask(); break;
        }
    }

}

void nextgame()
{
    gotoxyandprint(27,17,"Press enter to continue...");
    while(getch()!=13) {}
    stage++; // add value of stage so program can get correct map
    system("cls");
    clearstage();
    header();
    file_getmap();
    stage_print();
    setpoint();
    setpoint_print();
    scoreboard(0);
    pacman_set_position();
    monster_set_position();
    game_start();
}

void newgame()
{
    clearallhistory();
    system("cls");
    header();
    file_getmap();
    stage_print();
    setpoint();
    setpoint_print();
    scoreboard(0);
    pacman_set_position();
    monster_set_position();

    game_start();
}

void deco_pacman()
{
    gotoxyandprint(1,2,"                   .....");
    gotoxyandprint(1,3,"             . .IMM~+++~MM.");
    gotoxyandprint(1,4,"            .MI++++++++++++?M?");
    gotoxyandprint(1,5,"          .M++++++++++++++++++?7");
    gotoxyandprint(1,6,"        ..M++++++++++$++++++++$:");
    gotoxyandprint(1,7,"        .N++++++++++$$$++++++M.");
    gotoxyandprint(1,8,"        M+++++++++++M$M++++M.");
    gotoxyandprint(1,9,"      .M+++++++++++++++++M.");
    gotoxyandprint(1,10,"      .M++++++++++++++++N");
    gotoxyandprint(1,11,"      .7++++++++++++++M..");
    gotoxyandprint(1,12,"       ?+++++++++++++~.");
    gotoxyandprint(1,13,"      .M+++++++++++++++M.");
    gotoxyandprint(1,14,"      .M?+++++++++++++++M .");
    gotoxyandprint(1,15,"      ..7+++++++++++++++?+M.");
    gotoxyandprint(1,16,"        .~+++++++++++++++++M.");
    gotoxyandprint(1,17,"         .=+++++++++++++++++~M..");
    gotoxyandprint(1,18,"          .M++++++++++++++++++M.");
    gotoxyandprint(1,19,"            .M+++++++++++++++M..");
    gotoxyandprint(1,20,"             ..M8++++++++=MM.");
    gotoxyandprint(1,21,"                  ++++++++");
}

void deco_pacman2()
{
    gotoxyandprint(1,2,"                   .....");
    gotoxyandprint(1,3,"             . .IMM~+++~MM.");
    gotoxyandprint(1,4,"            .MI+++++++++++++?M");
    gotoxyandprint(1,5,"          .M++++++++++++++++++?");
    gotoxyandprint(1,6,"        ..M++++++++++0++++++++$");
    gotoxyandprint(1,7,"        .N++++++++++000+++++++M..");
    gotoxyandprint(1,8,"        M+++++++++++000+++++++++M.");
    gotoxyandprint(1,9,"      .M++++++++++++++++++++++M.");
    gotoxyandprint(1,10,"      .M++++++++++++++++++++N");
    gotoxyandprint(1,11,"      .7++++++++++++++++++M");
    gotoxyandprint(1,12,"       ?+++++++++++++++~.");
    gotoxyandprint(1,13,"      .M++++++++++++++++++M.");
    gotoxyandprint(1,14,"      .?+++++++++++++++++++M .");
    gotoxyandprint(1,15,"       .7++++++++++++++++++++M. ");
    gotoxyandprint(1,16,"        .~+++++++++++++++++++++M.");
    gotoxyandprint(1,17,"         .=++++++++++++++++++++~O..");
    gotoxyandprint(1,18,"          .M+++++++++++++++++++M. ");
    gotoxyandprint(1,19,"            .M+++++++++++++++M..");
    gotoxyandprint(1,20,"               .M8++++++=MM..");
    gotoxyandprint(1,21,"                  ++++++++");
}


void deco_pacmanlogo(int x,int y)
{
    gotoxyandprint(x,y," ___  ___  ___  __ __  ___  _ _ ");
    gotoxyandprint(x,y+1,"| . \\| . ||  _>|  \\  \\| . || \\ |");
    gotoxyandprint(x,y+2,"|  _/|   || <__|     ||   ||   |");
    gotoxyandprint(x,y+3,"|_|  |_|_|\\___/|_|_|_||_|_||_\\_|");
    gotoxyandprint(x,y+5,"        2    0    1    3");
}

void deco_panel()
{
gotoxyandprint(5,9,"                                                               .---.");
gotoxyandprint(5,10,"                                                              /  .  \\ ");
gotoxyandprint(5,11,"                                                             |\\_/|   |");
gotoxyandprint(5,12,"                                                             |   |  /|");
gotoxyandprint(5,13,"  .----------------------------------------------------------------' |");
gotoxyandprint(5,14," /  .-.                                                              |");
gotoxyandprint(5,15,"|  /   \\                                                             |");
gotoxyandprint(5,16,"| |\\_.  |                                                            |");
gotoxyandprint(5,17,"|\\|  | /|                                                            |");
gotoxyandprint(5,18,"| `---' |                                                            |");
gotoxyandprint(5,19,"|       |                                                           /");
gotoxyandprint(5,20,"|       |----------------------------------------------------------'");
gotoxyandprint(5,21,"\\       |");
gotoxyandprint(5,22," \\     /");
gotoxyandprint(5,23,"  `---'");

}

void how_to_play()
{
    system("cls");
    deco_pacmanlogo(25,3);
    deco_panel();
    gotoxy(33,10);
    color(white,darkred);
    printf(" %c HOW TO PLAY %c \n",3,3);
    reset();
    gotoxyandprint(15,14,"The player controls Pac-Man through a maze, eating pac-dots");
    gotoxyandprint(15,15,"(called pellets). When all pac-dots are eaten, you're win.");
    gotoxyandprint(15,16,"Only one enemie roam the maze, trying to catch Pac-Man.");
    gotoxyandprint(15,17,"If enemy touches Pac-Man, a life is lost and the Pac-Man ");
    gotoxyandprint(15,18,"withers and dies, so the game ends. Yoy have 3 stages. ");
    gotoxyandprint(15,19,"The high score player will be shown and rewarded !. ");

    gotoxyandprint(28,22,"Press any key to return ...");
    getch();
    welcome_screen();
}

void highscore_screen()
{
    system("cls");
    deco_pacmanlogo(25,3);
    deco_panel();
    gotoxy(33,10);
    color(white,darkred);
    printf(" %c HIGH SCORE %c \n",3,3);
    color(white,black);
    gotoxyandprint(15,15,"Ladies and Gentlemen please give a round of applause . . .");
    gotoxy(25,18); printf("SCORE : ");
    gotoxy(38,18); printf("By");
    color(red,black);
    gotoxy(33,18); printf("%d",highscore);
    gotoxy(42,18); printf("%s",highscore_name);
    reset();
    gotoxyandprint(28,22,"Press any key to return ...");
    getch();
    welcome_screen();
}

void menu_command(int a)
{
    switch(a)
    {
        case 1: newgame(); break;
        case 2: how_to_play(); break;
        case 3: highscore_screen(); break;
        case 4: break;
    }
}

void menu_print()
{
    int i;
    gotoxy(45,11);
    printf("%c WELCOME TO PACMAN %c\n",3,3);
    gotoxy(45,13);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",218,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,191);
    gotoxy(45,18);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",192,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,196,217);
    for(i=0;i<4;i++)
    {
        gotoxy(45,14+i); printf("%c                   %c",179,179);
    }
    color(black,white);
    gotoxyandprint(46,14,"      NEWGAME      ");
    gotoxyandprint(46,15,"    HOW TO PLAY    ");
    gotoxyandprint(46,16,"     HIGH SCORE    ");
    gotoxyandprint(46,17,"        EXIT       ");
    reset();
}

int menu()
{
    int a=1,k=1; //set a = default menu , k = garbage value
    while(k!=13) // 13 = enter
    {
        if(k==72) // up arrow
        {
            if(a!=1) a=a-1;
        }
        else
        if(k==80) // down arrow
        {
            if(a!=4) a=a+1;
        }
        menu_print();
        color(white,darkred);
        switch(a)
            {
                case 1: gotoxyandprint(46,14,"      NEWGAME      "); break;
                case 2: gotoxyandprint(46,15,"    HOW TO PLAY    "); break;
                case 3: gotoxyandprint(46,16,"     HIGH SCORE    "); break;
                case 4: gotoxyandprint(46,17,"        EXIT       "); break;
            }
        color(black,black);
        gotoxy(0,0);
        k=getche();
        reset();
    }
    menu_command(a);
}

void welcome_screen()
{
    system("cls");
    file_gethighscore();
    deco_pacman();
    deco_pacmanlogo(40,4);
    menu();
}

void splash_screen()
{

    deco_pacmanlogo(40,9);
    deco_pacman();
    Sleep(700);
    deco_pacman2();
    Sleep(700);
    system("cls");
    deco_pacman();
    deco_pacmanlogo(40,9);
    Sleep(700);
    deco_pacman2();
    Sleep(700);
}

int main()
{
    splash_screen();
    welcome_screen();
    //newgame();
    return 0;
}
