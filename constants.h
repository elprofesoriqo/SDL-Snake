#pragma once

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define BOARD_SIZE_X 40
#define BOARD_SIZE_Y 30
#define CELL_SIZE 20
#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y ((SCREEN_HEIGHT - (BOARD_SIZE_Y * CELL_SIZE)) / 2)
#define INITIAL_SNAKE_LENGTH 3
#define INFO_PANEL_X (BOARD_OFFSET_X + BOARD_SIZE_X * CELL_SIZE + 20)
#define INFO_PANEL_WIDTH 200
#define MAX_SNAKE_LENGTH 100
