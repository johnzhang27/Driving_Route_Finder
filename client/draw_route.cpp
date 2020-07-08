/* --------------------------------------------------
*  Name: YongQuan Zhang
*  ID: 1515873
*  CMPUT 275, Winter 2020
*  Assignment 2 Part 2: Directions
*---------------------------------------------------*/
#include "draw_route.h"
#include "map_drawing.h"
#include "lcd_image.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

extern shared_vars shared;

void draw_route(){
    /* This function is for frawing lines on the Edmonton map.

    Args: None. We used some global variables in const_and_types.

    Return: None.
    */
	int16_t start_x,start_y, end_x,end_y;
	for(int i = 1;i <= shared.num_waypoints-1;i++){

		start_x = (longitude_to_x(shared.map_number,
			shared.waypoints[i-1].lon)-shared.map_coords.x);

		start_y = (latitude_to_y(shared.map_number,
			shared.waypoints[i-1].lat)-shared.map_coords.y);

		end_x = (longitude_to_x(shared.map_number,
			shared.waypoints[i].lon)-shared.map_coords.x);

		end_y = (latitude_to_y(shared.map_number,
			shared.waypoints[i].lat)-shared.map_coords.y);

		if(start_y > displayconsts::display_height){
			continue;
		}
		else{
			shared.tft->drawLine(start_x,start_y,end_x,end_y,TFT_RED);
		}
	}
}
