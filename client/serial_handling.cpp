/* --------------------------------------------------
*  Name: YongQuan Zhang
*  ID: 1515873
*  CMPUT 275, Winter 2020
*  Assignment 2 Part 2: Directions
*---------------------------------------------------*/
#include "serial_handling.h"
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <SD.h>
#include "consts_and_types.h"
#include <assert.h>

using namespace std;

extern shared_vars shared;


void readline(String &readFromSerial){
  /* This function is similar with the readline function we 
  used for server.

  Args: string readFromSerial

  Return: None, we used return by reference.
  */
  while(true){
    if (Serial.available()) {
      // read the incoming byte:
      char in_char = Serial.read();

      // if end of line is received, waiting for line is done:
      if (in_char == '\n' || in_char == '\r') {
          // now we process the buffer
          return;
          }
      else {
          // add character to buffer, provided that we don't overflow.
          // drop any excess characters.
        readFromSerial += in_char;
      }
    }
  }
}

bool  wait_on_serial( uint8_t  nbytes , long  timeout ) {
  /* A function could be used for the timeout transition in 
    handshaking process.
  Args:
    nbytes: number of bytes that you want to receive.
    timeout: how many ms you want to wait after you receive some bytes.
  Returns:
    true or false: if greater than or equal to nbytes, return true,
     otherwise return false.
  */
  unsigned  long  deadline = millis () + timeout;
  while (Serial.available ()<nbytes  && (timeout <0 ||  millis ()<deadline)){
    delay (1);
  }
  return  Serial.available () >=nbytes;
}


uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  /* A function for the communication protocol. It will read
  information from the buffer and store in global variables.
  Args:
    lon_lat_32& start, the start vertex.
    lon_lat_32& end, the end vertex.
  Returns:
    1: communication is successful.
  */
  shared.num_waypoints = 0;
  String readFromSerial = "";
  char instruction;
  bool communicating = true;
  int indexOfSpace;
  // Print receving path on the green bar.
  status_message("Receiving path...");
  while(communicating){
    int count = 0;
    // Sending request.
    Serial.print("R");
    Serial.print(" ");
    Serial.print(start.lat);
    Serial.print(" ");
    Serial.print(start.lon);
    Serial.print(" ");
    Serial.print(end.lat);
    Serial.print(" ");
    Serial.print(end.lon);
    Serial.print("\n");
    Serial.flush();
    // Set a 10 second timeout for after sending request.
    // If nothing reply, then we go back the the start of 
    // the while loop and send the request again.
    if(wait_on_serial(1,10000)==false){
      continue;
    }
    // Set a 1 second timeout after sending acknowledgement.
    while(wait_on_serial(1,1000)!=false){
      readFromSerial = "";
      readline(readFromSerial);
      assert(readFromSerial!=NULL);
      // If we read total number of waypoints.
      if(readFromSerial.indexOf("N") != -1){
        indexOfSpace = readFromSerial.indexOf(" ");
        String substr = readFromSerial.substring(indexOfSpace+1);
        shared.num_waypoints = substr.toInt();
        // If number of waypoints are greater than 500, then 
        // we say there is no path.
        if(shared.num_waypoints > max_waypoints){
          communicating = false;
          break;
        }
        else if(shared.num_waypoints == 0){
          return 1;
        }
        Serial.print("A\n");
      }
      // If we received one waypoints, we read it and store in 
      // the gloabal variable structure waypoints[].
      else if(readFromSerial.indexOf("W") != -1){
        indexOfSpace = readFromSerial.indexOf(" ");
        String substr_lat_lon = readFromSerial.substring(indexOfSpace+1);
        indexOfSpace = substr_lat_lon.indexOf(" ");
        String substr_lat = substr_lat_lon.substring(0,indexOfSpace);
        shared.waypoints[count].lat = substr_lat.toInt();
        String substr_lon = substr_lat_lon.substring(indexOfSpace+1);
        shared.waypoints[count].lon = substr_lon.toInt();
        count++;
        Serial.print("A\n");
      }
      else if(readFromSerial.indexOf("E") != -1){
        communicating = false;
        break;
      }
    }
    Serial.flush();
  }
  Serial.flush();
  return 1;
}