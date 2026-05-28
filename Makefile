build:
	gcc main.c minesweeper.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o minesweeper