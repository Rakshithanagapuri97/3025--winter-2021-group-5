/*
 * display.c
 *
 *  Created on: May 30, 2020
 *      author: takis
 *      note: a lot of this code adapted from the TDLP tutorial on ncurses,
 *      by Pradeep Padala
 */

#include "gol_config.h"
#include <unistd.h>
#include <ncurses.h>
#include "display.h"

/*
 * important variables, defined elsewhere
 */
extern cell_t env[config_NE][config_ME];
//stores the current environment (defined in gol.h)

extern int STARTX; //0
extern int STARTY; //0

extern int ENDX;
//stores the total columns to be displayed (defined in gol.c)

extern int ENDY;
//stores the total rows to be displayed (defined in gol.c)

extern WINDOW *win;
//stores the window reference

/*
 * PRIVATE FUNCTIONS
 */


void create_newwin(int height, int width)
{
	//creates a new window of given height and width
	win = newwin(height, width, STARTY, STARTX);
	box(win, 0, 0); /* 0, 0 gives default characters
	 * for the vertical and horizontal
	 * lines */
	wrefresh(win); /* shows a box */

	return;
}

/*
 * PUBLIC FUNCTIONS
 */

//initializes a window
void initDisplay(void)
{
	printf("\ninitializing display...\n");
	usleep(2 * config_TL);
	//determines terminal type and clears all previous content from display
	initscr();
	//configures the terminal in cbreak mode
	cbreak();
	//set timeout
	timeout(TIME_OUT);
	//enables keypad of users terminal
	keypad(stdscr, TRUE);
	//creates a new window with number of columns as width and number of rows as height
	create_newwin(config_NE, config_ME);
}

void updateDisplay(void)
{
	int i, j;
	//clears the previous content from window
	wclear(win);
	//iterating through the columns to be displayed
	for (i = STARTX; i != config_ME; ++i)
		//iterating through the rows to be displayed
		for (j = STARTY; j != config_NE; ++j)
			if (env[j][i] == live)
				//displays cell characted if it is live
				mvwaddch(win, j, i, CELL_CHAR);
	//updates the window with new changes made in buffer
	wrefresh(win);
}

/*
 ******************************************************************************************** reference
 */

