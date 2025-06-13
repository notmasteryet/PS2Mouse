/*
USB Pinout (Left to Right, USB symbol up)
4: GND
3: Clk
2: Data
1: Vcc
*/

#include "PS2Mouse.h"

PS2Mouse mouse(6,5);

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.print("Setup...");

  mouse.begin(true);
  Serial.println("complete!");
}

void loop(){
  uint8_t stat;
  int x,y,z;
  //mouse.getPosition(stat,x,y);
   mouse.getAbsPosition(stat,x,y,z);
   
  Serial.print(stat, BIN);
  Serial.print("\tx=");
  Serial.print(x, DEC);
  Serial.print("\ty=");
  Serial.print(y, DEC);
  Serial.print("\tz=");
  Serial.println(z, DEC);
  
  delay(200);  
}

