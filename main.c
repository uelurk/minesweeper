#include <raylib.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "minesweeper.h"
#include "draw.h"

extern unsigned int seed;
extern bool win;
extern bool fail;
extern bool firstClick;

int main(void) {
    int CellSize = 32;
    Cell** Grid = InitGrid(ROWS, COLS);
    InitWindow(COLS*CellSize, ROWS*CellSize, "Minesweeper");
    SetTargetFPS(60);


    float hue = 0.0f;
    while(!WindowShouldClose()) {
        hue += 1.0f;
        if (hue > 360) hue = 0;
        Color iridescentColor = ColorFromHSV(hue, 0.7f, 0.9f); 
        CheckWin(&Grid);
        if(!fail && !win) {
            if(IsKeyPressed(KEY_A)) {
                Vector2 clickPos = GetMousePosition();
                int clickedCol = (int)clickPos.x / CellSize;
                int clickedRow = (int)clickPos.y / CellSize;

                if(firstClick) {
                    PlaceMines(&Grid, clickedRow, clickedCol);
                    CountAndPlaceNumbers(&Grid);
                    firstClick = false;
                }
                if(!Grid[clickedRow][clickedCol].isFlagged) {
                    if(Grid[clickedRow][clickedCol].isMine && !Grid[clickedRow][clickedCol].isFlagged) {
                        fail = true;
                        continue;
                    }
                    if (!Grid[clickedRow][clickedCol].isRevealed) {
                        Grid[clickedRow][clickedCol].isRevealed = true; // testing
                        FreeSpace(&Grid, clickedRow, clickedCol);
                    } else if (Grid[clickedRow][clickedCol].isRevealed && Grid[clickedRow][clickedCol].mineCount > 0) {
                        FreeSpaceNum(&Grid, clickedRow, clickedCol);
                    }
                }
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
            }
        }
        if(IsKeyPressed(KEY_R)) {
            fail = false;
            win = false;
            firstClick = true;
            seed++;
            FreeGrid(Grid, ROWS);
            Grid = InitGrid(ROWS, COLS);
        }
        BeginDrawing();
            DrawBoard(Grid, iridescentColor);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
