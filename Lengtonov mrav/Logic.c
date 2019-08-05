#include <stdio.h>
#include<windows.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "Ant.h"

struct Node *getNode(long r, long c) {
	struct Node *p = (struct Node *)malloc(sizeof(struct Node));
	if (!p) {
		printf("FAIL!!!");
	}
	p->down = p->left = p->right = p->up = NULL;
	p->row = r;
	p->col = c;
	p->st = NULL;
	return p;
}

void insert(struct Node *row[], struct Node *col[], struct State *st, long r, long c) {
	struct Node *p = getNode(r, c);
	if (!row[r]) {
		row[r] = p;
	}
	else {
		struct Node *q = row[r];
		if (row[r]->col > c) {
			p->right = row[r];
			row[r]->left = p;
			row[r] = p;
		}
		else {
			while (q->right && q->right->col < c) {
				q = q->right;
			}
			if (q->right) {
				q->right->left = p;
			}
			p->right = q->right;
			p->left = q;
			q->right = p;
		}
	}
	if (!col[c]) {
		col[c] = p;
	}
	else {
		struct Node *s = col[c];
		if (col[c]->row > r) {
			p->down = s;
			s->up = p;
			col[c] = p;
		}
		else {
			while (s->down && s->down->row < r) {
				s = s->down;
			}
			if (s->down) {
				s->down->up = p;
			}
			p->down = s->down;
			p->up = s;
			s->down = p;
		}
	}
	p->st = st;
}

struct Node *nextStep(struct Ant *a) {
	struct Node *newNode;
	switch (a->face) {
	case 0:
		newNode = a->pos->up;
		break;
	case 1:
		newNode = a->pos->right;
		break;
	case 2:
		newNode = a->pos->down;
		break;
	case 3:
		newNode = a->pos->left;
		break;
	default:
		newNode = a->pos->left;
	}
	return newNode;
}

void buildStep(struct Node *row[], struct Node *col[], struct Ant *a, struct State *p, int limit_r, int limit_c) {
	long r = a->pos->row;
	long c = a->pos->col;
	struct Node *q;
	switch (a->face) {
	case 0:
		if (a->pos->row == 0 && !(a->pos->up)) {
			q = a->pos;
			while (q->down) {
				q = q->down;
			}
			if (!(q->row == limit_r - 1)) {
				insert(row, col, p, limit_r - 1, c);
				q = q->down;
			}
			q->down = a->pos;
			a->pos->up = q;
		}
		else if (!(a->pos->up) || !((a->pos->up->row == r - 1 || a->pos->up->row == limit_r - 1) && a->pos->up->col == c)) {
			insert(row, col, p, r - 1, c);
		}
		break;
	case 1:
		if (a->pos->col == limit_c - 1 && !(a->pos->right)) {
			q = row[r];
			if (!(q->col == 0)) {
				insert(row, col, p, r, 0);
			}
			a->pos->right = row[r];
			row[r]->left = a->pos;
		}
		else if (!(a->pos->right) || !((a->pos->right->col == c + 1 || a->pos->right->col == 0) && a->pos->right->row == r)) {
			insert(row, col, p, r, c + 1);
		}
		break;
	case 2:
		if (a->pos->row == limit_r - 1 && !(a->pos->down)) {
			q = col[c];
			if (!(q->row == 0)) {
				insert(row, col, p, 0, c);
			}
			a->pos->down = col[c];
			col[c]->up = a->pos;
		}
		else if (!(a->pos->down) || !((a->pos->down->row == r + 1 || a->pos->down->row == 0) && a->pos->down->col == c)) {
			insert(row, col, p, r + 1, c);
		}
		break;
	case 3:
		if (a->pos->col == 0 && !(a->pos->left)) {
			q = a->pos;
			while (q->right) {
				q = q->right;
			}
			if (!(q->col == limit_c - 1)) {
				insert(row, col, p, r, limit_c - 1);
				q = q->right;
			}
			q->right = row[r];
			row[r]->left = q;
		}
		else if (!(a->pos->left) || !((a->pos->left->col == c - 1 || a->pos->left->col == limit_c - 1) && a->pos->left->row == r)) {
			insert(row, col, p, r, c - 1);
		}
		break;
	}
}


void print(struct Node *row[], int limit_r, int limit_c) {
	int i, j;
	for (i = 0; i < limit_r; i++) {
		if (!row[i]) {
			for (j = 0; j < limit_c; j++) {
				printf("_");
			}
		}
		else {
			struct Node *p = row[i];
			struct Node *r = p->right;
			for (j = 0; j < p->col; j++) {
				printf("_");
			}
			//printf("%c", p->st->color);
			while (r && r != row[i]) {
				for (j = 0; j < r->col - p->col - 1; j++) {
					printf("_");
				}
				printf("%c", r->st->color);
				r = r->right;
				p = p->right;
			}
			for (j = 0; j < limit_c - p->col - 1; j++) {
				printf("_");
			}
		}
		printf("\n");
	}
	printf("\n\n\n\n\n\n\n");
}

void deleteWorld(struct Node *row[], int limit) {
	int i;
	struct Node *p, *old;
	for (i = 0; i < limit; i++) {
		if (row[i]) {
			p = row[i];
			while (p) {
				old = p;
				p = p->right;
				free(old);
			}
		}
	}
}