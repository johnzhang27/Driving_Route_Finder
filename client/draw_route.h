#ifndef __DRAW_ROUTE_H
#define __DRAW_ROUTE_H

#include "consts_and_types.h"
#include "draw_route.h"
#include "map_drawing.h"
#include "lcd_image.h"
#include <Arduino.h>

// core graphics library (written by Adafruit)
#include <Adafruit_GFX.h>

// Hardware-specific graphics library for MCU Friend 3.5" TFT LCD shield
#include <MCUFRIEND_kbv.h>

// LCD and SD card will communicate using the Serial Peripheral Interface (SPI)
// e.g., SPI is used to display images stored on the SD card
#include <SPI.h>

// needed for reading/writing to SD card
#include <SD.h>

extern shared_vars shared;
/*
   Your job!

   Draw the route to the tft display using the extern variable shared.
   If the number of waypoints is 0, then draw nothing.

   Guidelines:
    - It is OK if the cursor erases the route when it goes over it.
    - Don't worry about making the route "thick", the normal drawLine function
      of the tft display is good enough.
    - Make it a colour that can be easily distinguished from the rest.
*/
void draw_route();

#endif
