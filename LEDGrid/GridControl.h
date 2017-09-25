#ifndef GRID_CONTROL_H_
#define GRID_CONTROL_H_

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
  * Custom library for controlling LED Grid based on led wiring 
  * --> = LED DIRECTION,  O< = LED,  X = LED on second LAYER 
  * EXAMPLE 6 ROWS x 8 COLS
  *                 _   _    _     ___     
  *                 / \ / \  / \   /   \             
  *              /-+-+--+-+--+-+--+--->+------\                  
  *              \-X-O<-X-O<-X-O<-X-O<-X-\     |
  *                | |  | |  | |  | |  ↑  |    |                    
  *              ,>O-X->O-X->O-X->O-X->O-'     |
  *              | ↓ ↑  ↓ ↑  ↓ ↑  ↓ |  |       |
  *              `-X-O<-X-O<-X-O<-X-O<-X-\     |                  
  *                | |  | |  | |  | |  ↑  |    |             
  *              ,>O-X->O-X->O-X->O-X->O-'     V
  *              | ↓ ↑  ↓ ↑  ↓ ↑  ↓ |  ↑       |
  *              `-X-O<-X-O<-X-O<-X-O<-X-\     |
  *                | |  | |  | |  | |  ↑  |    |
  * ARDUINO------->O-X->O-X->O-X->O-X->O-'     |
  *                  ↑  | ↑  |  ↑   |  \---<--/
  *                  `-'  `--'  `--'
  */            

#define ROWS        6
#define COLUMNS     9

// LED GRID INDICES
static const PROGMEM uint8_t LED_INDICES[ROWS][COLUMNS] = {{53, 26, 47, 25, 41, 24, 35, 23, 29},
                                                           {18, 48, 19, 42, 20, 36, 21, 30, 22},
                                                           {52, 17, 46, 16, 40, 15, 34, 14, 28},
                                                           { 9, 49, 10, 43, 11, 37, 12, 31, 13},
                                                           {51,  8, 45,  7, 39,  6, 33,  5, 27},
                                                           { 0, 50,  1, 44,  2, 38,  3, 32,  4}};

#define FONT_X        3
#define FONT_Y        4
#define CHAR_OFFSET  65

// A..Z CAPS ONLY BINARY REPRESENTATION
static const PROGMEM byte CHARACTERS[26][FONT_Y] = {{B010, B101, B111, B101}, {B110, B110, B101, B110}, {B011, B100, B100, B011},
                                                    {B110, B101, B101, B110}, {B111, B100, B110, B111}, {B111, B100, B110, B100},
                                                    {B011, B100, B101, B011}, {B101, B101, B111, B101}, {B111, B010, B010, B111},
                                                    {B001, B001, B101, B010}, {B101, B101, B110, B101}, {B100, B100, B100, B111},
                                                    {B111, B111, B111, B101}, {B101, B111, B111, B101}, {B111, B101, B101, B111},
                                                    {B110, B101, B110, B100}, {B010, B101, B111, B011}, {B110, B101, B110, B101},
                                                    {B011, B100, B001, B110}, {B111, B010, B010, B010}, {B101, B101, B101, B111},
                                                    {B101, B101, B101, B010}, {B101, B111, B111, B111}, {B101, B010, B010, B101},
                                                    {B101, B101, B010, B010}, {B111, B001, B100, B111}};

#define NUM_X        2
#define NUM_Y        4

// 0..9 and special char :
static const PROGMEM byte NUMBERS[10][NUM_Y] = {{B11, B11, B11, B11}, {B01, B11, B01, B01}, {B11, B01, B10, B11}, 
                                                {B11, B01, B11, B11}, {B01, B11, B11, B01}, {B11, B10, B01, B11},
                                                {B11, B10, B11, B11}, {B11, B01, B01, B01}, {B11, B11, B00, B11},
                                                {B11, B11, B01, B01}};

static uint8_t activeLeds[ROWS][COLUMNS];

// index(row, col) returns the corresponding index given
// a specified row and col returns -1 if invalid row col vals
uint8_t gridIndex(uint8_t row, uint8_t col);

// resetActiveLeds() sets all led indices to 0 (OFF)
void resetActiveLeds();

// setActiveLed(row, col) sets an led index to be on where row
// col are valid
void setActiveLed(uint8_t row, uint8_t col);

// setActiveLed(row, col) sets an led index to be a specific value where row
// col are valid
void setActiveLed(uint8_t row, uint8_t col, uint8_t val);

// removeActiveLed(row, col) sets an led index to be inactive
void removeActiveLed(uint8_t row, uint8_t col);

// getActiveLed(row, col) returns 1 if the led at row col is active, 0
// otherwise
uint8_t getActiveLed(uint8_t row, uint8_t col);

// fillActiveLed(row, col, num, vertical, left) set a column or row
// of leds to active given a specified number and orientation (vertical/left)
void fillActiveLed(uint8_t row, uint8_t col, uint8_t num, bool vertical, bool lft_down);

// fillChar(c) sets active leds at target location defined by row
// and col to display char c, row and col do not have to be valid
void fillChar(uint8_t row, uint8_t col, char c);

// fillNum(c) sets active leds at target location defined by row
// and col to display int i, row and col do not have to be valid
void fillNum(uint8_t row, uint8_t col, uint8_t i);
#endif
