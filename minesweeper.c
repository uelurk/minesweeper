#include "minesweeper.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const int di[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int dj[] = { 1,  0, -1, -1, 1, -1, 0, 1};

unsigned int seed = 0;
bool fail = false;
bool win = false;
bool firstClick = true;

Cell** InitGrid(int rows, int cols) {
    Cell** array = malloc(rows * sizeof(Cell*));
    for(int i = 0; i < rows; i++) {
        array[i] = malloc(cols * sizeof(Cell));
    }
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            Cell cell;
            cell.isMine = false;
            cell.isRevealed = false;
            cell.isFlagged = false;
            cell.mineCount = 0;
            array[i][j] = cell;
        }
    }
    return array;
}
void CountAndPlaceNumbers(Cell*** Grid) {
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if((*Grid)[i][j].isMine) {
                continue;
            }
            for (int k = 0; k < 8; k++) {
                // n - neighbor
                int nI = i + di[k];
                int nJ = j + dj[k];
                if(nI >= 0 && nI < ROWS && nJ >= 0 && nJ < COLS) {
                    if ((*Grid)[nI][nJ].isMine) {
                        (*Grid)[i][j].mineCount++;
                    }
                }
            }
        }
    }
}
void FreeSpace(Cell*** Grid, int x, int y) {
    if(!(*Grid)[x][y].isRevealed || (*Grid)[x][y].isMine || (*Grid)[x][y].isFlagged) {
        return;
    }
    if((*Grid)[x][y].mineCount > 0) {
        return;
    }
    for (int k = 0; k < 8; k++) {
        int nX = x + di[k];
        int nY = y + dj[k];
        if(nX >= 0 && nX < ROWS && nY >= 0 && nY < COLS) {
            if(
                !(*Grid)[nX][nY].isMine &&
                !(*Grid)[nX][nY].isFlagged &&
                !(*Grid)[nX][nY].isRevealed   
            ) {
                (*Grid)[nX][nY].isRevealed = true;
                if((*Grid)[nX][nY].mineCount == 0) {
                    FreeSpace(Grid, nX, nY);
                }
            }
        }
    }   
}
void FreeSpaceNum(Cell*** Grid, int x, int y) {
    int flags = 0;
    if((*Grid)[x][y].mineCount == 0) {
        return;
    }
    for (int k = 0; k < 8; k++) {
        int nX = x + di[k];
        int nY = y + dj[k];
        if(nX >= 0 && nX < ROWS && nY >= 0 && nY < COLS) {
            if((*Grid)[nX][nY].isFlagged) {
                flags++;
            }
        }
    }
    if((*Grid)[x][y].mineCount == flags) {
        for (int k = 0; k < 8; k++) {
            int nX = x + di[k];
            int nY = y + dj[k];
            if(nX >= 0 && nX < ROWS && nY >= 0 && nY < COLS) {
                if(
                    !(*Grid)[nX][nY].isFlagged &&
                    !(*Grid)[nX][nY].isRevealed   
                ) {
                    if((*Grid)[nX][nY].isMine) {
                        fail = true;
                    }
                    (*Grid)[nX][nY].isRevealed = true;
                    FreeSpace(Grid, nX, nY);
                }
            }
        }   
    }
}

void PlaceMines(Cell*** Grid, int x, int y) {
    srand(seed + time(0));
    for(int i = 0; i < MINES; i++) {
        int randomRow = (rand() % ROWS);
        int randomCol = (rand() % COLS);

        bool inSaveZone = false;
        if (randomRow >= x - 1 && randomRow <= x + 1 &&
            randomCol >= y - 1 && randomCol <= y + 1) {
            inSaveZone = true;
        }
        if(!(*Grid)[randomRow][randomCol].isMine && !inSaveZone) {
            (*Grid)[randomRow][randomCol].isMine = true;
        } else {
            i--;
        }
    }
}

void CheckWin(Cell*** Grid) {
    int unrevealedCells = 0;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if(!(*Grid)[i][j].isRevealed) {
                unrevealedCells++;
            }
        }
    }
    if(unrevealedCells == MINES) {
        win = true;
    }
}

void FreeGrid(Cell** array, int rows) {
    for(int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}