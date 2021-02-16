/*
 * gol.c
 *
 *  Created on: May 30, 2020
 *      Author: takis
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include "gol_config.h"
#include "cells.h"
#include "display.h"

/*
 * global variables
 */
cell_t env[config_NE][config_ME];
cell_t update_env[config_NE][config_ME];
bool reproduction_flag = false; // is high when it's mating season

int STARTX = 0;
int STARTY = 0;
int ENDX = config_ME;
int ENDY = config_NE;
WINDOW *win;
/*
 * main code
 */
int main(void)
{
	pthread_t threadptrs[config_K * config_L]; // our thread handles
	threadID_t threadID[config_K * config_L]; // thread ID

	// initialize workspace
	initEnvironment();

	// create the threads
	printf("\ncreating threads...\n");
	size_t index;//to store index of thread being created
	
	//iterating through the community  rows
	for (size_t i = 0; i != config_K; ++i)
	{
		// iterating through communities beside each other(columns)
		for (size_t j = 0; j != config_L; ++j)
		{
			index = i * config_L + j; // map (i,j) to an 1-d index
			threadID[index].row = i;//setting row index of thread
			threadID[index].col = j;//setting column index of thread
			// the following if condition returns 0 on the successful creation of each thread:
			if (pthread_create(&threadptrs[index], NULL, &updateCommFunc,
					&threadID[index]) != 0)
			{
				//displayed if the pthread_create returns error status
				printf("failed to create the thread %d\n", (int) index);
				return 1;
			}
		}
	}

	// initialize display with ncurses
	initDisplay();

	// to store the number of generations passed between plots
	unsigned short int ctr = 0;
	while (1)
	{
		// indicating threads to start reproduction for next generation
		reproduction_flag = true;
		usleep(config_TL / 2); // allow new generation to check in
		//indicating threads not to start reproduction
		reproduction_flag = false;
		usleep(config_TL / 2); // put a hold on reproduction to update display
		if (++ctr == config_TDISP)
		{
			//reseting the number of generations passed between plots to 0
			ctr = 0;
			//plotting the current environment
			updateDisplay();
		}
		//update the current environment with that of next genenration using update_env
		copyEnvironment(); 
		// write changes to the environment, env, from update_env
		
	}

	//never reaches here
	return 1;
}
