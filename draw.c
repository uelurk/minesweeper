#include <raylib.h>
#include <stdio.h>
#include "minesweeper.h"

int CellSize = 32;

void DrawUnrevealedCell(Cell cell, int x, int y) {
    Color col;
    if(x % 2 == 0 ){
        if(y % 2 == 0) {
            col = DARKGRAY;
        } else {
            col = GRAY;
        }
    } else {
        if(y % 2 != 0) {
            col = DARKGRAY;
        } else {
            col = GRAY;
        }
    }
    DrawRectangle(y*CellSize, x*CellSize, CellSize, CellSize, col);
}
void DrawRevealedCell(Cell cell, int x, int y) {
    if (cell.isRevealed) {
        DrawRectangle(y*CellSize, x*CellSize, CellSize, CellSize, WHITE);
        DrawRectangle(y*CellSize, x*CellSize, CellSize, CellSize/15, LIGHTGRAY);
        DrawRectangle(y*CellSize, x*CellSize, CellSize/15, CellSize, LIGHTGRAY);
        
        if(cell.mineCount > 0) {
            char num[4];
            snprintf(num, sizeof(num), "%d", cell.mineCount);
            DrawText(num, y*CellSize + 5, x*CellSize+2, 30, BLUE);
        }
    }
}
void DrawMine(Cell cell, int x, int y) {
    if(cell.isMine) {
        DrawRectangle(y*CellSize, x*CellSize, CellSize, CellSize, WHITE);
        DrawRectangle(y*CellSize, x*CellSize, CellSize, CellSize/15, LIGHTGRAY);
        DrawRectangle(y*CellSize, x*CellSize, CellSize/15, CellSize, LIGHTGRAY);
        DrawCircle(y*CellSize + CellSize/2, x*CellSize + CellSize/2, CellSize/3, BLACK);
        DrawCircle(y*CellSize + CellSize/2, x*CellSize + CellSize/2, CellSize/3-2, RED);
    }
}
void DrawFlag(Cell cell, int x, int y) {
    if (cell.isFlagged) {
        DrawText("P", y*CellSize + 5, x*CellSize+2, 30, RED);
        //DrawRectangle(j*CellSize+8, i*CellSize, CellSize/2, CellSize, RED);
    }
}
void DrawWinText(Color iridescentColor) {
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

void DrawFailText() {
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
void DrawBoard(Cell** Grid, Color iridescentColor) {
    for(int x = 0; x < ROWS; x++) {
        for(int y = 0; y < COLS; y++) {
            Cell cell = Grid[x][y];
            DrawUnrevealedCell(cell, x, y);
            if(win) DrawWinText(iridescentColor);
            if (fail) {
                DrawFailText();
                DrawMine(cell, x, y);
            };
            if (DEBUG_SHOW_MINES) DrawMine(cell, x, y);
            DrawRevealedCell(cell, x, y);
            DrawFlag(cell, x, y);
        }
    }
}