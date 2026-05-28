build:
	gcc main.c minesweeper.c draw.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o minesweeper