#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"

int zoom = 1;

short color_table[] =
{
	COLOR_RED,COLOR_RED,COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
	COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};


void printPause(WINDOW *win)
{
	mvwaddstr(win, 1, 9, "Press S key to save this map.");
	mvwaddstr(win, 2, 6, "Change simulation speed on 1,2 & 3:");
	mvwaddstr(win, 4, 8, "1 - slow | 2 - medium | 3 - fast");
	wrefresh(win);
}

void pause(int *speed, struct Node *row[], struct State *p)
{
	WINDOW *win;
	win = newwin(15, 50, 5, COLS / 2 - 25);


	initscr(win);
	keypad(win, TRUE);
	//noecho();
	wattrset(win, A_NORMAL);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(win);
	printPause(win);
	while (1)
	{
		char ch = getch();
		if (ch == ASCII_SPACE)
			break;
		else if (ch == 'S' || ch == 's')
		{
			mvwaddstr(win, 9, 1, "Simulation saved!");
			wrefresh(win);
			wattrset(win, A_NORMAL);
			saveSim(row, LINES, "sim.bin", p);
		}
		else if (ch == '1')
		{
			*speed = SPEED_SLOW;
			mvwaddstr(win, 10, 1, "Speed changed!");
			wrefresh(win);
		}
		else if (ch == '2')
		{
			*speed = SPEED_MEDIUM;
			mvwaddstr(win, 10, 1, "Speed changed!");
			wrefresh(win);
		}
		else if (ch == '3')
		{
			*speed = SPEED_FAST;
			mvwaddstr(win, 10, 1, "Speed changed!");
			wrefresh(win);
		}
	}
	werase(win);
	wrefresh(win);
}



void startSim() {
	int i, limit = 150;
	WINDOW *win;
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	win = newwin(0, 0, 0, 0);

	if (has_colors())
		start_color();

	for (i = 2; i <= NUMBER_OF_COLORS + 2; i++)
		init_pair(i, color_table[i], color_table[i]);

	wattrset(win, A_NORMAL);
	chtype bold = A_BOLD;

	FILE *fp = fopen("konf.txt", "r");

	int speed;
	fscanf(fp, "%d", &speed);

	struct State *p = getState(0, 0);
	p = setStates(p);
	zoom = 1;

	struct Node *row[1000], *col[1000];
	for (i = 0; i < 1000; i++) {
		row[i] = col[i] = NULL;
	}

	insert(row, col, p, LINES / 2, COLS / 2);
	struct Ant *a = (struct Ant *)malloc(sizeof(struct Ant));
	a->face = 0;
	a->pos = row[LINES / 2];
	i = 0;
	printNodes(row, LINES, COLS, win);

	nodelay(stdscr, TRUE);
	while (1) {
		if (speed == SPEED_SLOW)
		{
			napms(100);
		}
		else if (speed == SPEED_MEDIUM)
		{
			napms(30);
		}
		else if (speed == SPEED_FAST)
		{
			napms(1);
		}
		char ch = getch();
		if (ch == ASCII_SPACE)
		{
			//zoom = 0;
			pause(&speed, row, p);
			werase(win);
			printNodes(row, LINES, COLS, win,zoom);
		}
		else if (ch == ASCII_ESC)
		{
			initColors();
			return;
		}
		wattrset(win, COLOR_PAIR(a->pos->st->color + 1) | bold);

		int xx[4] = { 0,1,-1 };
		int yy[4] = { 0,1,-1 };
		int posRow = a->pos->row * 3 + 1;
		int posCol = a->pos->col * 3 + 1;

		if ((a->pos->row <= 10 || a->pos->row >= 19) && zoom == 1)
		{
			zoom = 0;
			werase(win);
			printNodes(row, LINES, COLS, win);

		}
		if (zoom == 1)
			for (int k = 0; k < 4; k++)
			{
				for (int k2 = 0; k2 < 4; k2++)
					mvwaddch(win, posRow + xx[k2] - LINES, posCol + yy[k] - COLS, ' ');
			}
		else
		{
			mvwaddch(win, a->pos->row, a->pos->col, ' ');
		}
		wrefresh(win);
		a->face = (a->face + a->pos->st->turn) % 4;
		a->pos->st = a->pos->st->next;
		buildStep(row, col, a, p, LINES, COLS);
		a->pos = nextStep(a);
	}
	deleteWorld(row, 50);
}