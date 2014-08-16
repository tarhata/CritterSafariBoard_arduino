/**
 * 
 * Author: Tarhata Guiamelon
 * Critter Safari Game Dock Code
 * MHCID Capstone 2014
 *
 **/


// Object and Background Pieces
#define OBJECT1 A0
#define OBJECT2 A1
#define OBJECT3 A2
#define OBJECT4 A3
#define BACKGROUND A4

int values[] = {
  1023, 1023, 1023, 1023, 1023};  // {object, object, object, object, background}

int prevValues [] = {
  1023, 1023, 1023, 1023, 1023}; // {object, object, object, object, background}

const int range = 5; // range that accounts for variance in analog readings

String type = "";

void setup() {  
  Serial.begin(57600);
  Serial.println("Starting...");
}

void loop() {
  for (int pin=0; pin < 5; pin++)
  {
    int reading = analogRead(pin); 
    //    Serial.println(reading);

    int value = values[pin];
    int prevValue = prevValues[pin];

    int minReading = value - range;
    int maxReading = value + range;

    if (reading > maxReading || reading < minReading)
    {
      if (reading < 1000 && pin < 4) 
      {
        Serial.print("add object ");
        type = checkType(reading, type);     
        Serial.println(type);       
      } 
      else if (reading > 1000 && pin < 4) 
      {
        Serial.print("remove object ");
        type = checkType(value, type);  
        Serial.println(type);    
      }
      else if (reading < 1000 && pin == 4)
      {
        Serial.print("set bg ");
        type = checkType(reading, type);
        Serial.println(type);       
      }         
    }
    prevValues[pin] = values[pin]; // save current values
    values[pin] = reading;        // update currently read value      
    delay(5);
  }
  
  //check current readings
//  Serial.print("Current readings = ");  
//  for (int pin = 0; pin<5; pin++) {
//    Serial.print(values[pin]);
//    Serial.print(" ");
//  }
//  Serial.println();
}

String checkType(int value, String type) {
  if (value > 15 && value < 25) {
    type = "fox";  
  } 
  else if(value > 25 && value < 35) { 
    type = "pig"; 
  } 
  else if (value > 85 && value < 98) {
    type = "monkey";
  } 
  else if (value > 505 && value < 515) {
    type = "alien";
  }
  if (value > 175 && value < 185) {
    type = "forest";  
  } 
  else if (value > 5 && value < 15) {
    type = "space";
  } 
  else if (value > 1000)
  { 
    type = "";
  }
  return type;
}




