/**
 * 
 * Author: Tarhata Guiamelon
 * Critter Safari Game Dock Code
 * MHCID Capstone 2014
 *
 **/

// Object and Setting Pieces
#define SETTING A0
#define OBJECT1 A1
#define OBJECT2 A2
#define OBJECT3 A3
#define OBJECT4 A4

void setup() {  
  Serial.begin(57600);
}

void loop() {
  for (int pin=0; pin < 6 ; pin++)
  {
    int reading = analogRead(pin);
    Serial.print(reading);
    Serial.print(" ");
    delay(500);  
  }
  Serial.println();
}





