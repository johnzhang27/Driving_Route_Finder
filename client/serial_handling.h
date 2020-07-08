#ifndef __SERIAL_HANDLING_H
#define __SERIAL_HANDLING_H

#include "consts_and_types.h"
#include "map_drawing.h"
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>

using namespace std;

extern shared_vars shared;

void readline(String &readFromSerial);

bool  wait_on_serial( uint8_t  nbytes , long  timeout );

uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end);

#endif
