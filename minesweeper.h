#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "stdbool.h"

#define ROWS 20
#define COLS 40
#define MINES 150
#define DEBUG_SHOW_MINES false

extern const int di[8];
extern const int dj[8];

extern unsigned int seed;
extern bool fail;
extern bool win;
extern bool firstClick;

typedef struct {
    unsigned int seed;
    bool win;
    bool fail;
    bool firstClick;
} GameState;

typedef struct {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int mineCount;
} Cell;

Cell** InitGrid(int rows, int cols);
void CountAndPlaceNumbers(Cell***);

void FreeSpace(Cell***, int x, int y);
void FreeSpaceNum(Cell***, int x, int y);
void PlaceMines(Cell***, int x, int y);
void CheckWin(Cell***);

void FreeGrid(Cell**, int rows);

#endif