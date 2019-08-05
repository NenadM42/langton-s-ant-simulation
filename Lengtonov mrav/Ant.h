#pragma once


#define RIGHT 1
#define LEFT 3
#define ASCII_SPACE 32
#define ASCII_ENTER 10
#define ASCII_ESC 27

#define SPEED_SLOW 1
#define SPEED_MEDIUM 2
#define SPEED_FAST 3

#define NUMBER_OF_COLORS 7


struct State {
	char color;
	int turn;
	struct State *next;
};

struct Node {
	struct State *st;
	struct Node *left, *right, *up, *down;
	long row, col;
};

struct Ant {
	struct Node *pos;
	int face;
};

struct fState {
	char color;
	int turn;
};

struct fNode {
	char st;
	long row, col;
};

struct Node *getNode(long r, long c);

void insert(struct Node *row[], struct Node *col[], struct State *st, long r, long c);

struct Node *nextStep(struct Ant *a);

void buildStep(struct Node *row[], struct Node *col[], struct Ant *a, struct State *p, int limit_r, int limit_c);

void print(struct Node *row[], int limit_r, int limit_c);

void deleteWorld(struct Node *row[], int limit);


void initColors();

void drawFg(WINDOW *win);

void setSimulationSpeed(FILE *fp, WINDOW *win);

int isValidValueMovement(int a);

void setColors(FILE *fp, WINDOW *win);

int isValidNumber(char c, char connect[10]);

void printColorsChoosing(WINDOW *win);

void connectColors(FILE *fp, WINDOW *win);

void options();

struct State *getState(int color, int turn);

struct State *setStates(struct State *p);

void printList(struct State *h);


struct State *getPointer(struct State *p, char c);

struct fNode *nodeTofNode(struct Node *n);

struct fState *stateTofState(struct State *p);



void printNodes(struct Node *row[], int limit_r, int limit_c, WINDOW *win);

void saveSim(struct Node *row[], int limit_r, char *filename, struct State *p);

void buildSimFormFile(char *sim_filename, struct Node *row[], struct Node *col[], struct State **p);

void printPause(WINDOW *win);

void pause(int *speed, struct Node *row[], struct State *p);

void startSim();

void loadSim();

void menu();
