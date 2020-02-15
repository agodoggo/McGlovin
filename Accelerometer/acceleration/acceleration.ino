/**************************************************************************/
/*!
    @file     Adafruit_MMA8451.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is an example for the Adafruit MMA8451 Accel breakout board
    ----> https://www.adafruit.com/products/2019

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
int pushButton = 2;
float initialx = -123;
float initialy = -123;
float currentx; float currenty;

float velocityx0 = 0; float velocityx1; float velocityy0 = 0; float velocityy1;
float positionx0 = 0; float positionx1; float positiony0 = 0; float positiony1;
void setup(void) {
  Serial.begin(9600);
  
  Serial.println("Adafruit MMA8451 test!");
  pinMode(pushButton, INPUT);

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  
  
}

void loop() {
  while (digitalRead(pushButton) == HIGH)
  {
    read();
  }
  
  

}

void read()
{
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");

  if (initialx == -123)
  {
    initialx = event.acceleration.x;
  }

 //first x integration
  currentx = event.acceleration.x;
    currenty = event.acceleration.y;
    
  velocityx1= velocityx0 + initialx+ ((currentx - initialx)/2); 
  
  // second x integration
  positionx1= positionx0 + velocityx0 + ((velocityx1 - velocityx0)/2);

  //first y integration
  velocityy1= velocityy0 + initialy+ ((currenty - initialy)/2); 
  
  // second y integration
  positiony1= positiony0 + velocityy0 + ((velocityy1 - velocityy0)/2);

   Serial.print("X position: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y position: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.println("");
  
  
  
 
}
