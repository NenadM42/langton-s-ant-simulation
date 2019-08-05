#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"

short color_table1[] =
{
	COLOR_RED,COLOR_RED,COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
	COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

char *Start[7] =
{
	"####  #####    #      ####  #####",
	"#       #     # #     #  #    #  ",
	"####    #    #####    ####    #  ",
	"   #    #   #     #   # #     #  ",
	"####    #  #       #  #  #    #  ",
	""
};

char *Options[7] =
{
	"####  ####  #####  #  ####  #   #  ####",
	"#  #  #  #    #    #  #  #  ##  #  #   ",
	"#  #  ####    #    #  #  #  # # #  ####",
	"#  #  #       #    #  #  #  #  ##     #",
	"####  #       #    #  ####  #   #  ####",
	""
};

char *Exit[7] =
{
	"####  #   #  #  #####",
	"#      # #   #    #  ",
	"####    #    #    #  ",
	"#      # #   #    #  ",
	"####  #   #  #    #  ",
	""
};

char *Load[7] =
{
	"#	  ####	   #      #### ",
	"#	  #  #	  # #      #  # ",
	"#	  #  #	 #####     #  # ",
	"#	  #  #  #     #    #  # ",
	"####    #### #       #  #### ",
	""
};

char *Speed[7] =
{
	"#### #### #### #### #### ",
	"#    #  # #    #     #  # ",
	"#### #### #### ####  #  # ",
	"   # #    #    #     #  # ",
	"#### #    #### #### #### ",
	""
};

void menu()
{
	WINDOW *win;
	//nodelay(win, TRUE);
	initscr();
	keypad(stdscr, TRUE);
	noecho();

	win = newwin(0, 0, 0, 0);
	wrefresh(win);

	if (has_colors())
		start_color();
	int i;
	for (i = 2; i < NUMBER_OF_COLORS + 1; i++)
		init_pair(i, color_table1[i], COLOR_BLACK);

	wattrset(win, A_NORMAL);

	int x = 0, y = 0;
	int flag = 0;

	chtype bold = A_BOLD;


	wrefresh(win);
	wattrset(win, COLOR_PAIR(3) | bold);

	nodelay(win, TRUE);

	int state = 0;
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	int l = 0;
	while (1)
	{
		int ch;
		ch = wgetch(win);
		if (ch)
		{
			l = ch;

			if (ch == 's')
				state++;
			else if (ch == 'w')
				state--;

			if (ch == 10 && state == 0)
				startSim();
			else if (ch == 10 && state == 1)
			{
				loadSim();
				initColors();
				wattrset(win, A_NORMAL);
			}
			else if (ch == 10 && state == 2) {
				werase(win);
				wrefresh(win);
				options();
			}
			else if (ch == 10 && state == 3)
			{
				return 0;
			}

			if (state < 0)
				state = 0;
			if (state > 3)
				state = 3;

			if (state == 0)
			{
				wattrset(win, COLOR_PAIR(3) | bold);
			}
			else
			{
				wattrset(win, A_NORMAL);
			}
			werase(win);
			i = 0;
			while (*Start[i])
			{
				mvwaddstr(win, i + 2, COLS / 2 - 18, Start[i]);
				++i;
			}

			if (state == 1)
			{
				wattrset(win, COLOR_PAIR(3) | bold);
			}
			else
			{
				wattrset(win, A_NORMAL);
			}
			i = 0;
			while (*Load[i])
			{
				mvwaddstr(win, i + 9, COLS / 2 - 18, Load[i]);
				++i;
			}


			if (state == 2)
			{
				wattrset(win, COLOR_PAIR(3) | bold);
			}
			else
			{
				wattrset(win, A_NORMAL);
			}

			i = 0;
			while (*Options[i])
			{
				mvwaddstr(win, i + 16, COLS / 2 - 18, Options[i]);
				++i;
			}

			if (state == 3)
			{
				wattrset(win, COLOR_PAIR(3) | bold);
			}
			else
			{
				wattrset(win, A_NORMAL);
			}
			i = 0;
			while (*Exit[i])
			{
				mvwaddstr(win, i + 23, COLS / 2 - 18, Exit[i]);
				++i;
			}
			wattrset(win, A_NORMAL);
			wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
			mvwaddstr(win, 1, 1, "SPACE key to pause simulation");
			mvwaddstr(win, 2, 1, "ESC key to exit simulation");

			wrefresh(win);
		}
	}
}

void initColors()
{
	int i;
	for (i = 2; i < NUMBER_OF_COLORS + 1; i++)
		init_pair(i, color_table1[i], COLOR_BLACK);
}