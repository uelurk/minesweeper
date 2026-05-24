#include <raylib.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#define ROWS 20
#define COLS 40
#define MINES 100
#define SHOWMINES false

typedef struct {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int mineCount;
} Cell;

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
    // possible directions around cell
    int di[] = {-1,-1,-1,0,0,1,1,1};
    int dj[] = {1,0,-1,-1,1,-1,0,1};

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
    int di[] = {-1, -1, -1,  0, 0,  1, 1, 1};
    int dj[] = { 1,  0, -1, -1, 1, -1, 0, 1};

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

void PlaceMines(Cell*** Grid) {
    srand(time(0));
    for(int i = 0; i < MINES; i++) {
        int randomRow = (rand() % ROWS);
        int randomCol = (rand() % COLS);
        if(!(*Grid)[randomRow][randomCol].isMine) {
            (*Grid)[randomRow][randomCol].isMine = true;
        } else {
            i--;
        }
    }
} 

int main(void) {
    Cell** Grid = InitGrid(ROWS, COLS);
    PlaceMines(&Grid);
    CountAndPlaceNumbers(&Grid);

    bool fail = false;
    
    int CellSize = 30;
    InitWindow(COLS*CellSize, ROWS*CellSize, "Minesweeper");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        if(!fail) {
            if(IsKeyPressed(KEY_A)) {
                Vector2 clickPos = GetMousePosition();
                int clickedCol = (int)clickPos.x / CellSize;
                int clickedRow = (int)clickPos.y / CellSize;
                if(Grid[clickedRow][clickedCol].isMine && !Grid[clickedRow][clickedCol].isFlagged) {
                    fail = true;
                }
                if(Grid[clickedRow][clickedCol].isFlagged) {
                    Grid[clickedRow][clickedCol].isFlagged = false;
                } else {
                    Grid[clickedRow][clickedCol].isRevealed = true; // testing
                    FreeSpace(&Grid, clickedRow, clickedCol);
                }
                TraceLog(LOG_INFO, "%d %d", clickedCol, clickedRow);
            }
            if(IsKeyPressed(KEY_D)) {
                Vector2 clickPos = GetMousePosition();
                int clickedCol = (int)clickPos.x / CellSize;
                int clickedRow = (int)clickPos.y / CellSize;
                if(Grid[clickedRow][clickedCol].isFlagged){
                    Grid[clickedRow][clickedCol].isFlagged = false;
                    
                } else if (!Grid[clickedRow][clickedCol].isRevealed) {
                    Grid[clickedRow][clickedCol].isFlagged = true; // testing
                }
                TraceLog(LOG_INFO, "%d %d", clickedCol, clickedRow);
            }
        }
        if(IsKeyPressed(KEY_R)) {
            fail = false;
            free(Grid);
            Grid = InitGrid(ROWS, COLS);
            PlaceMines(&Grid);
            CountAndPlaceNumbers(&Grid);
        }
        BeginDrawing();
            for(int i = 0; i < ROWS; i++) {
                for(int j = 0; j < COLS; j++) {
                    Color col;
                    if(i % 2 == 0 ){
                        if(j % 2 == 0) {
                            col = DARKGRAY;
                        } else {
                            col = GRAY;
                        }
                    } else {
                        if(j % 2 != 0) {
                            col = DARKGRAY;
                        } else {
                            col = GRAY;
                        }
                    }
                    DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize, col);
                    if(fail) {
                        DrawText("DEAD - PRESS R TO RESTART", 50, 50, 50, BLACK);
                        
                    }   
                    // testing
                    if (Grid[i][j].isMine && SHOWMINES) {
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize, BLUE);
                    }
                    if (Grid[i][j].isRevealed) {
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize, WHITE);
                        if(Grid[i][j].mineCount > 0) {
                            char num[4];
                            snprintf(num, sizeof(num), "%d", Grid[i][j].mineCount);
                            DrawText(num, j*CellSize, i*CellSize, 30, BLACK);
                        }
                    }
                    if (Grid[i][j].isFlagged) {
                        DrawRectangle(j*CellSize, i*CellSize, CellSize/2, CellSize, RED);
                    }
                    // #testing
                }
            }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}