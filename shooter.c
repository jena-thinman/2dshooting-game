/*-------------------------------BEGINNING------------------------------*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<ncurses.h>
#include<string.h>
#include<time.h>
#include<pthread.h>

#define name_row 26
#define name_col 12
#define score_row 26
#define score_col 85
/*----------------------GLOBAL DECLARATIONS--------------------------------*/
bool game_on;											//GAME STATUS VARIABLE

void render_screen(void);				//SCREEN RENDERING FUNCTION
void print_turret(void);				//GUN RENDRING FUNCTION

typedef enum direction{					//Gun angle definitions
				nw4 = 0,
				nw3 = 1,
				nw2 =2,
				nw1 =3,
				cent=4,
				ne1=5,
				ne2=6,
				ne3=7,
				ne4=8,
				fire=9
				}direction;
direction cursor_pos = cent;	//Variable instance
int score=0;
void firing_mechanism(char);		//User keypress and gun angle control function
void foxtrot(direction);				// Firing mechanism function
void* display_targets(void*);		//Target display THREAD declaration
void score_check(void);					//Score tracking function
/*-----------------------GLOBAL VARIABLES FOR BULLET----------------------*/
        int k,l,max_k,max_l;
        int cur_row=0,cur_col=0;
/*----------------------GlOBAL VARIABLE FOR TARGET-----------------------*/
				int end_i=0,end_j=0;
/*-----------------------------------------------------------------------*/


/*-------------------------------------MAIN FUNCTION-----------------------------------*/
int main(void){
	char ch_key;
	srand((unsigned) time(NULL));		//random function initialize
	initscr();											//Initialize NCURSES
	getmaxyx(stdscr,max_k,max_l);		//Get max height and width of the screen
	render_screen();								//initial first time screen rendering
	print_turret();									//initial dun printing
	refresh();											// refresh() dumps the characters on screen using NCURSES API

	game_on = TRUE;									// Switch GAME STATUS to TRUE

	pthread_t thread_0;							// Pthread id initialization
	pthread_create(&thread_0, NULL, &display_targets, NULL);	//create child thread

	usleep(10000);			//This sleep hlps prevent printing garbage values on screen berfore the creation of thread

	while(game_on){				//Exit game when user presses 'q'
	ch_key=getch();
	firing_mechanism(ch_key);				//Scan for 

	if(cursor_pos==fire){
				if(cur_col>(max_l/2))
					for(k=cur_row-1,l=cur_col;k>0;k--,l++){
						init_pair(7,COLOR_RED,COLOR_BLACK);
						attron(COLOR_PAIR(7));
						mvaddch(k,l,'#');
						attroff(COLOR_PAIR(7));												//BULLET TRAJECTORY AND PROPAGATION
						usleep(20000);
						refresh();
					}
				else
						for(k=cur_row-1,l=cur_col;k>0;k--,l--){
							attron(COLOR_PAIR(7));
							mvaddch(k,l,'#');												//BULLET TRAJECTORY AND PROPAGATION
							attroff(COLOR_PAIR(7));
							usleep(20000);
							refresh();
						}
		}
	score_check();														//CHECK SCORE
	}
	endwin();																//Esures a clean terminal after exit.
	return 0;
}
/*-------------------------END OF MAIN FUNCTION-----------------------------*/

void render_screen(){									//Screen render definition
	int row,col,i=0,j=0;


getmaxyx(stdscr,row,col);
noecho();

mvprintw(i,j,"/");
	for(i=0,j=1;j<col-1;j++)
	mvprintw(i,j,"-");

mvprintw(i,j,"\\");

	for(i=1,j=0;i<row-1;i++)
	mvprintw(i,j,"|");
mvprintw(i,j,"\\");
	for(i=1,j=col-1;i<row-1;i++)
	mvprintw(i,j,"|");
mvprintw(i,j,"/");
start_color();
init_pair(1,COLOR_CYAN,COLOR_BLACK);
init_pair(2, COLOR_RED, COLOR_BLACK);
init_pair(3,COLOR_YELLOW,COLOR_BLACK);
init_pair(4,COLOR_GREEN,COLOR_BLACK);

	for(i=row-1,j=1;j<col-1;j++)
	mvprintw(i,j,"-");
/*attron(COLOR_PAIR(1));
	for(i=3,j=1;j<col-1;j++)
	mvprintw(i,j,"_");
	for(i=6,j=1;j<col-1;j++)
	mvprintw(i,j,"_");
*/
attroff(COLOR_PAIR(1));
	for(i=24,j=1;j<col-1;j++)
	mvprintw(i,j,"_");
	mvprintw(26,4,"Player: ");
attron(COLOR_PAIR(2));
	mvprintw(26,77,"Score: ");
attroff(COLOR_PAIR(2));
	mvprintw(26,84,"%d",score);

}

void print_turret(){											//Gun display definition

	mvprintw(21,43,"*");	//Center C
	mvprintw(21,44,"*");	//NE-1
	mvprintw(21,42,"*");	//NW-1
	mvprintw(22,40,"*");	//NW-2
	mvprintw(23,38,"*");	//NW-3
	mvprintw(24,36,"*");	//NW-4
	mvprintw(22,46,"*");	//NE-2
	mvprintw(23,48,"*");	//NE-3
	mvprintw(24,50,"*");	//NE-4
}

void firing_mechanism(char key_get){ 			//User kepress control definition

	game_on = TRUE;
	switch(key_get){
		case('a'):
			{
			if(cursor_pos<=nw4){
			cursor_pos = 4;
			break;
			}
			else{
			--cursor_pos;
			break;
			}
			}

		case('d'):
			{
			if(cursor_pos>=ne4){
			cursor_pos = 4;
			break;
			}
			else{
			++cursor_pos;
			break;
			}
			}

		case('w'):
			{
			cursor_pos = fire;
			break;
			}

		case('q'):
			{
				game_on = FALSE;
				break;
			}

			default:
			{
				cursor_pos = cent;
				break;
			}
		}
cursor_pos = cursor_pos;

}

void foxtrot(direction position){		//Fire and gun angle control corresponding to user keypress

	start_color();
	noecho();
	keypad(stdscr,TRUE);

		switch(position){
		case(cent):
			{
				mvaddch(21,43,'*'|A_REVERSE|A_BLINK);
				getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}
		case(ne1):
			{
				mvaddch(21,44,'*'|A_REVERSE|A_BLINK);
				 getyx(stdscr,cur_row,cur_col);
				break;
			}
		case(ne2):
			{
				mvaddch(22,46,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}
		case(ne3):
			{
				mvaddch(23,48,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}
		case(ne4):
			{
				mvaddch(24,50,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}

		case(nw1):
			{
				mvaddch(21,42,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				break;
			}

		case(nw2):
			{
				mvaddch(22,40,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}

		case(nw3):
			{
				mvaddch(23,38,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}

		case(nw4):
			{
				mvaddch(24,36,'*'|A_REVERSE|A_BLINK);
			 	getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}

		default:
			{
				attron(A_REVERSE|A_BLINK);
				mvprintw(21,43,"*");
				attroff(A_REVERSE|A_BLINK);
				 getyx(stdscr,cur_row,cur_col);
				refresh();
				break;
			}
		}
	}

void* display_targets(void* unused){			//Target display thread definition
	int end_x,end_y;
	while(game_on == TRUE){
			start_color();
			init_pair(6,COLOR_YELLOW,COLOR_BLACK);
			getmaxyx(stdscr,end_y,end_x);
					for(end_i=(rand()%6),end_j=0;end_i>2,end_j<(end_x-1);end_j++){
						clear();
						render_screen();
						print_turret();
						attron(COLOR_PAIR(6));
						noecho();
						mvprintw(end_i,end_j,">--|-->");
						attroff(COLOR_PAIR(6));
						foxtrot(cursor_pos);
						score_check();
						refresh();
						usleep(25000);
					}
	}
return NULL;
}

void score_check(void){			//Score checker
	if(end_i == k)
		if((end_j <= l) && (l <= (end_j+6))){
			score++;
		}
}
/*----------------------------END-------------------------------------*/
