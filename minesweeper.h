#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "stdbool.h"

#define ROWS 20
#define COLS 40
#define MINES 150
#define SHOWMINES false

extern const int di[8];
extern const int dj[8];

extern unsigned int seed;
extern bool fail;
extern bool win;
extern bool firstClick;

typedef struct {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int mineCount;
} Cell;

Cell** InitGrid(int, int);
void CountAndPlaceNumbers(Cell***);
void FreeSpace(Cell***, int, int);
void FreeSpaceNum(Cell***, int, int);
void PlaceMines(Cell***, int, int);
void CheckWin(Cell***);
void FreeGrid(Cell**, int);

#endif