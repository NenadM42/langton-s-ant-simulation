#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"

short color_table2[] =
{
	COLOR_RED,COLOR_RED,COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
	COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

struct State *getPointer(struct State *p, char c) {
	while (p->color != c) {
		p = p->next;
	}
	return p;
}

struct fNode *nodeTofNode(struct Node *n) {
	struct fNode *p = (struct fNode *)malloc(sizeof(struct fNode));
	p->col = n->col;
	p->row = n->row;
	p->st = n->st->color;
	return p;
}

struct fState *stateTofState(struct State *p) {
	struct fState *s = (struct fState *)malloc(sizeof(struct fState));
	s->color = p->color;
	s->turn = p->turn;
}

void loadSim()
{
	WINDOW *win;
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	win = newwin(0, 0, 0, 0);

	if (has_colors())
		start_color();
	int i;
	for (i = 2; i <= NUMBER_OF_COLORS + 2; i++)
		init_pair(i, color_table2[i], color_table2[i]);

	wattrset(win, A_NORMAL);

	struct State *p = getState(0, 0);
	p = setStates(p);
	//int zoom = 1;
	struct Node *row[1000], *col[1000];
	for (i = 0; i < 1000; i++) {
		row[i] = col[i] = NULL;
	}


	FILE *test = fopen("sim.bin", "rb");

	if (test == NULL)
		return;
	fclose(test);

	buildSimFormFile("sim.bin", row, col, p);
	printNodes(row, LINES, COLS, win);
	wrefresh(win);
	while (1)
	{
		if (wgetch(win) == ASCII_ESC)
			break;
	}

}

void createFile()
{
	FILE *fp = fopen("konf.txt", "w");
	fputs("2\n", fp);
	fputs("l\n", fp);
	fputs("l\n", fp);
	fputs("r\n", fp);
	fputs("l\n", fp);
	fputs("r\n", fp);
	fputs("l\n", fp);
	fputs("r\n", fp);
	fputs("123", fp);
	fclose(fp);
}

void saveSim(struct Node *row[], int limit_r, char *filename, struct State *p) {  //cuva sve da ne bi bilo problema oko trazenja starih pravila na novoj mrezi 
	FILE *output = fopen(filename, "wb");
	if (!output) {
		printf("Greska!!");
		exit(1);
	}
	int num = 1;
	struct State *r = p->next;
	while (r != p) {
		num++;
		r = r->next;
	}
	int i;
	fwrite(&num, sizeof(int), 1, output);
	r = p;
	struct fState *fs;
	for (i = 0; i < num; i++, r = r->next) {
		fs = stateTofState(r);
		fwrite(fs, sizeof(struct fState), 1, output);
		free(fs);
	}
	//cuvanje mreze
	struct fNode *q;
	for (i = 0; i < LINES; i++) {
		struct Node *p = row[i];
		if (p) {
			q = nodeTofNode(p);
			fwrite(q, sizeof(struct fNode), 1, output);
			p = p->right;
			while (p && p != row[i]) {
				q = nodeTofNode(p);
				fwrite(q, sizeof(struct fNode), 1, output);
				free(q);
				p = p->right;
			}
		}
	}
	fclose(output);
}

void buildSimFormFile(char *sim_filename, struct Node *row[], struct Node *col[], struct State **p) {
	FILE *input = fopen(sim_filename, "rb");
	if (!input) {
		exit(1);
	}
	int num, i = 0;
	struct fState s;
	fread(&num, sizeof(int), 1, input);
	fread(&s, sizeof(struct fState), 1, input);
	*p = getState(s.color, s.turn);
	struct State *m = *p, *n;
	for (i = 0; i < num - 1; i++) {
		fread(&s, sizeof(struct fState), 1, input);
		n = getState(s.color, s.turn);
		m->next = n;
		m = n;
	}
	m->next = *p;


	struct fNode f;
	while (fread(&f, sizeof(struct fNode), 1, input)) {
		insert(row, col, getPointer(*p, f.st), f.row, f.col);
	}
	fclose(input);
}