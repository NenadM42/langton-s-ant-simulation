#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"



void printList(struct State *h)
{
	struct State *p = h;
	int i = 0;
	while (i < 12)
	{
		printf("%d\n", p->color);
		p = p->next;
		i++;
	}
}

void printNodes(struct Node *row[], int limit_r, int limit_c, WINDOW *win, int zoom) {
	int i;
	for (i = 0; i < limit_r; i++) {
		if (row[i]) {
			struct Node *p = row[i];

			int c = p->st->color;
			while (p->st->next->color != c)
			{
				p->st = p->st->next;
			}
			wattrset(win, COLOR_PAIR(p->st->color + 1));

			int xx[4] = { 0,1,-1 };
			int yy[4] = { 0,1,-1 };
			int posRow = p->row * 3 + 1;
			int posCol = p->col * 3 + 1;
			if (zoom == 1)
				for (int k = 0; k < 4; k++)
				{
					for (int k2 = 0; k2 < 4; k2++)
						mvwaddch(win, posRow + xx[k2] - LINES, posCol + yy[k] - COLS, ' ');
				}
			else
			{
				mvwaddch(win, p->row, p->col, ' ');
			}



			//mvwaddch(win, p->row, p->col, ' ');
			p->st = p->st->next;

			//printf("\n%c %d %d", p->st->color, p->row, p->col);
			p = p->right;
			while (p && p != row[i]) {
				int c = p->st->color;
				while (p->st->next->color != c)
				{
					p->st = p->st->next;
				}
				wattrset(win, COLOR_PAIR(p->st->color + 1));

				int xx[4] = { 0,1,-1 };
				int yy[4] = { 0,1,-1 };
				int posRow = p->row * 3 + 1;
				int posCol = p->col * 3 + 1;
				if (zoom == 1)
					for (int k = 0; k < 4; k++)
					{
						for (int k2 = 0; k2 < 4; k2++)
							mvwaddch(win, posRow + xx[k2] - LINES, posCol + yy[k] - COLS, ' ');
					}
				else
				{
					mvwaddch(win, p->row, p->col, ' ');
				}

				//mvwaddch(win, p->row, p->col, ' ');
				p->st = p->st->next;

				//printf("\n%c %d %d", p->st->color, p->row, p->col);
				p = p->right;
			}
		}
	}
	//printf("\n\n");
}