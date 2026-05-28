#include <raylib.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "minesweeper.h"

extern unsigned int seed;
extern bool win;
extern bool fail;
extern bool firstClick;

int main(void) {
    Cell** Grid = InitGrid(ROWS, COLS);
    
    int CellSize = 32;
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
                        char* msg = "DEAD - PRESS R TO RESTART";
                        int textX = 50;
                        int textY = 50;
                        int textSize = 50;
                        DrawText(msg, textX+3, textY, textSize, BLACK);
                        DrawText(msg, textX-3, textY, textSize, BLACK);
                        DrawText(msg, textX, textY+3, textSize, BLACK);
                        DrawText(msg, textX, textY-3, textSize, BLACK);
                        DrawText(msg, textX, textY, textSize, WHITE);
                    }   
                    // testing
                    if (Grid[i][j].isMine && SHOWMINES) {
                        DrawCircle(j*CellSize + CellSize/2, i*CellSize + CellSize/2, CellSize/3, BLACK);
                        DrawCircle(j*CellSize + CellSize/2, i*CellSize + CellSize/2, CellSize/3-2, RED);
                    }
                    if (Grid[i][j].isRevealed) {
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize, WHITE);
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize/15, LIGHTGRAY);
                        DrawRectangle(j*CellSize, i*CellSize, CellSize/15, CellSize, LIGHTGRAY);

                        if(Grid[i][j].mineCount > 0) {
                            char num[4];
                            snprintf(num, sizeof(num), "%d", Grid[i][j].mineCount);
                            DrawText(num, j*CellSize + 5, i*CellSize+2, 30, BLUE);
                        }
                    }
                    if (Grid[i][j].isFlagged) {
                        DrawText("P", j*CellSize + 5, i*CellSize+2, 30, RED);
                        //DrawRectangle(j*CellSize+8, i*CellSize, CellSize/2, CellSize, RED);
                    }
                    if (Grid[i][j].isMine && fail) {
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize, WHITE);
                        DrawRectangle(j*CellSize, i*CellSize, CellSize, CellSize/15, LIGHTGRAY);
                        DrawRectangle(j*CellSize, i*CellSize, CellSize/15, CellSize, LIGHTGRAY);
                        DrawCircle(j*CellSize + CellSize/2, i*CellSize + CellSize/2, CellSize/3, BLACK);
                        DrawCircle(j*CellSize + CellSize/2, i*CellSize + CellSize/2, CellSize/3-2, RED);
                    }
                    if(win) {
                        char* msg = "WIN";
                        int textX = 100;
                        int textY = 100;
                        int textSize = 100;
                        DrawText(msg, textX+3, textY, textSize, BLACK);
                        DrawText(msg, textX-3, textY, textSize, BLACK);
                        DrawText(msg, textX, textY+3, textSize, BLACK);
                        DrawText(msg, textX, textY-3, textSize, BLACK);
                        DrawText(msg, textX, textY, textSize, iridescentColor);
                    }
                    // #testing
                }
            }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
