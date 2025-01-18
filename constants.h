#pragma once

//plansza
#define SCREEN_WIDTH 1060
#define SCREEN_HEIGHT 600
#define BOARD_SIZE_X 40
#define BOARD_SIZE_Y 30
#define CELL_SIZE 20
#define BOARD_OFFSET_X 20
#define BOARD_OFFSET_Y ((SCREEN_HEIGHT - (BOARD_SIZE_Y * CELL_SIZE)) / 2)
#define INFO_PANEL_X (BOARD_OFFSET_X + BOARD_SIZE_X * CELL_SIZE + 20)
#define INFO_PANEL_WIDTH 200
#define GAME_OVER_DURATION 5.0        //czas wyśiwtlania podsumowania gry

//font
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

//snake
#define INITIAL_SNAKE_LENGTH 3 //startowa długość
#define MAX_SNAKE_LENGTH 100
#define BASE_MOVE_DELAY 0.15
#define SPEED_INCREASE_INTERVAL 10.0  //co ile czasu gry snake przyśpiesza
#define SPEED_INCREASE_AMOUNT 0.2     //o ile się zwiększa

//startowa pozycja/ środek
#define SNAKE_START_X (BOARD_SIZE_X / 2)
#define SNAKE_START_Y (BOARD_SIZE_Y / 2)
//punkty
#define BLUE_FOOD_SCORE 10
#define RED_FOOD_SCORE 20

//red dot bonus
#define SNAKE_SHRINK_AMOUNT 2   
#define SPEED_DECREASE_FACTOR 0.9 
#define RED_FOOD_DURATION 5.0         //czas red food
#define RED_FOOD_COOLDOWN 20.0        //co ile czasu się pojawia


