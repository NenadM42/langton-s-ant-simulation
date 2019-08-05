#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"


short color_table3[] =
{
	COLOR_RED,COLOR_RED,COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
	COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

void setSimulationSpeed(FILE *fp, WINDOW *win)
{
	nodelay(stdscr, TRUE);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	mvwaddstr(win, 2, 10, "Enter speed of simulation:");
	mvwaddstr(win, 4, 10, "1 - slow | 2 - medium | 3 - fast");
	int ch;
	int repeat = 1;

	while (repeat--)
	{

		wattrset(win, A_NORMAL);
		ch = getch();
		if (ch == 49)
		{
			fputs("1\n", fp);
		}
		else if (ch == 50)
		{
			fputs("2\n", fp);
		}
		else if (ch == 51)
		{
			fputs("3\n", fp);
		}
		else if (ch == ERR)
		{
			repeat = 1;
			nodelay(stdscr, FALSE);
		}
		else
		{
			mvwaddstr(win, 3, 8, "You entered invalid value! Try again.");
			repeat = 1;
		}
		wrefresh(win);

	}
	werase(win);
}

int isValidValueMovement(int a)
{
	if (a == 'l' || a == 'L' || a == 'r' || a == 'R')
		return 1;
	else
		return 0;
}

void setColors(FILE *fp, WINDOW *win)
{
	initscr(win);
	nodelay(win, TRUE);

	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

	for (int i = 2; i < NUMBER_OF_COLORS + 2; i++)
		init_pair(i, color_table3[i], color_table3[i]);


	for (int i = 2; i < NUMBER_OF_COLORS + 2; i++)
	{
		wattrset(win, NORMAL_PRINT);

		mvwaddstr(win, 2, 6, "Enter ant's movement for a given color:");
		mvwaddstr(win, 4, 13, "R - right or L - left.");

		wattrset(win, COLOR_PAIR(i));
		mvwaddstr(win, 6, 20, "       ");
		mvwaddstr(win, 7, 20, "       ");
		mvwaddstr(win, 8, 20, "       ");
		mvwaddstr(win, 9, 20, "       ");

		wrefresh(win);

		int ch;
		int repeat = 1;

		while (repeat--)
		{
			ch = getch();
			if (isValidValueMovement(ch))
			{
				if (ch == 'r' || ch == 'R')
					fputs("r\n", fp);
				else if (ch == 'n' || ch == 'N')
					fputs("n\n", fp);
				else
					fputs("l\n", fp);
			}
			else
			{
				//init_pair(i, color_table[i], COLOR_BLACK);
				wattrset(win, A_NORMAL);
				mvwaddstr(win, 3, 8, "You entered invalid value! Try again.");
				repeat = 1;
			}
			wrefresh(win);
		}
		init_pair(i, color_table3[i], COLOR_BLACK);
	}
	werase(win);
}

int isValidNumber(char c, char connect[10])
{
	if (c == '1' || c == '2' || c == '3' || c == '4' ||
		c == '5' || c == '6' || c == '7')
	{
		//prodjemo kroz connect da vidimo da nije vec bio ovaj broj
		for (int i = 0; i < 8; i++)
		{
			if (connect[i] == c)
				return 0;
		}
		return 1;
	}
	return 0;
}

void printColorsChoosing(WINDOW *win)
{
	wattrset(win, A_NORMAL);
	mvwaddstr(win, 1, 2, "Enter number of colors that you want");
	mvwaddstr(win, 2, 2, "to connect. Example(1 3 4) for");
	mvwaddstr(win, 3, 2, "Red->Green->Cyan");
	mvwaddstr(win, 4, 2, "Press ENTER to confirm | BACKSPACE to undo");


	for (int i = 2; i < NUMBER_OF_COLORS + 2; i++)
	{
		init_pair(i, color_table3[i], color_table3[i]);
	}

	for (int i = 2; i < NUMBER_OF_COLORS + 2; i++)
	{
		wattrset(win, COLOR_PAIR(i));
		mvwaddstr(win, 7, (i - 2) * 3 + 1 + 10, "   ");
	}

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wattrset(win, COLOR_PAIR(1));

	char c;

	wattrset(win, A_NORMAL);
	for (int i = 0; i < NUMBER_OF_COLORS; i++)
	{
		c = (i + 1) + '0';
		mvwaddch(win, 8, i * 3 + 2 + 10, c);
	}

	wrefresh(win);
}

void connectColors(FILE *fp, WINDOW *win)
{
	wattrset(win, A_NORMAL);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	char connect[8];
	for (int i = 0; i < 8; i++)
	{
		connect[i] = '0';
	}
	int i = 0;
	int j = 0;
	char connected[16];
	connected[0] = '\0';
	int cnt = 0;
	while (1)
	{
		wrefresh(win);

		//printf("%d\n",i);

		printColorsChoosing(win);

		char ch = getch();

		if ((ch == ASCII_ENTER || i == 7) && cnt > 0)
			break;
		else if (ch == 8 && j > 0)
		{
			werase(win);
			i--;
			connect[i] = '0';
			j -= 2;
			connected[j] = '\0';
			wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
			mvwaddstr(win, 9, 9, "List:");
			mvwaddstr(win, 10, 9, connected);
			continue;
		}
		if (isValidNumber(ch, connect))
		{
			cnt++;
			connect[i] = ch;
			connected[j] = ch;
			connected[j + 1] = '-';
			j += 2;
			connected[j] = '\0';
		}
		else
		{
			mvwaddstr(win, 13, 9, "Wrong value! Try again!");
			i--;
		}
		i++;
		mvwaddstr(win, 9, 9, "List:");
		mvwaddstr(win, 10, 9, connected);
	}
	connect[i] = '\0';

	fprintf(fp, "%s", connect);
}

void options()
{
	int i;
	for (i = 0; i < 8; i++)
		init_pair(i, color_table3[i], COLOR_BLACK);


	if (has_colors())
		start_color();

	WINDOW *win;
	win = newwin(15, 50, 5, COLS / 2 - 25);
	wattrset(win, COLOR_PAIR(1));
	initscr(win);
	keypad(win, TRUE);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

	wrefresh(win);
	nodelay(win, TRUE);

	chtype bold = A_BOLD;

	FILE *fp = fopen("konf.txt", "w");

	wrefresh(win);

	setSimulationSpeed(fp, win);
	setColors(fp, win);
	connectColors(fp, win);
	initColors();

	fclose(fp);
}

struct State *getState(int color, int turn)
{
	struct State *p = (struct State*)malloc(sizeof(struct State));
	p->color = color;
	p->turn = turn;
	return p;
}

struct State *setStates(struct State *p)
{
	struct State *pom = p;
	FILE *fp = fopen("konf.txt", "r");
	int cnt = 0;

	struct State *states[9];

	for (int i = 0; i < 8; i++)
	{
		char c[2];
		fscanf(fp, "%s", c);

		if (c[0] == 'l')
		{
			pom->color = i;
			pom->turn = LEFT;
			pom->next = getState(0, 0);
			states[i] = pom;
			pom = pom->next;
		}
		else if (c[0] == 'r')
		{
			pom->color = i;
			pom->turn = RIGHT;
			pom->next = getState(0, 0);
			states[i] = pom;
			pom = pom->next;
		}
	}

	char connected[8];
	fscanf(fp, "%s", connected);
	int L = strlen(connected);
	char pomocni = connected[0];

	p = states[connected[0] - '0'];
	pom = p;

	for (int i = 1; i < L; i++)
	{
		pom->next = states[connected[i] - '0'];
		pom = pom->next;
	}
	pom->next = p;

	fclose(fp);
	return p;
}


