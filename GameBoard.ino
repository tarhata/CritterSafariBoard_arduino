/**
 * 
 * Author: Tarhata Guiamelon
 * Critter Safari Game Board Code
 * MHCID Capstone 2014
 *
 **/

#include <Shifter.h>
#include "analogmuxdemux.h"

boolean charPresent = false;

const int buttonPin = 5;     // the number of the pushbutton pin
const int statusLED =  13;  // the number of the LED pin

const int charPin = A4; // character pin
const int muxPin = A5; // analog multiplexer input pin

// Number of pins connected to STEPS and FUNCTION rows of game board
const int stepsPins = 8;
const int functionPins = 4;

// FUNCTION row pins for reading
#define FN_PIN1 A0
#define FN_PIN2 A1
#define FN_PIN3 A2
#define FN_PIN4 A3

// FUNCTION ROW LED pins
#define FN_LED1 6
#define FN_LED2 7
#define FN_LED3 8
#define FN_LED4 9

// Shift register pins
#define SER_Pin 10 //SER_IN
#define RCLK_Pin 11 //L_CLOCK
#define SRCLK_Pin 12 //CLOCK

#define NUM_REGISTERS 1 // how many registers are in the chain

//initaize shifter using the Shifter library
Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS); 

// set up the Muxer ready to be used. Watch the order of S0, S1 and S2. 
AnalogMux amux(4,3,2, muxPin);

int stepsValues[] = {
  0, 0, 0, 0, 0, 0, 0, 0};  

int functionValues[] = {
  0, 0, 0, 0};    

int charVal = 1023;  
int range = 5;
String type = "";
int buttonState = 0;     // variable for reading the pushbutton status

int numSteps = 0;
int numFunction = 0;

boolean functionFound = false;
String functionType = "function";

void setup() {  

  // initialize led pins to output

  pinMode(statusLED, OUTPUT);  
  for (int ledPin = 4; ledPin < 10; ledPin++)
  {
    pinMode(ledPin, OUTPUT);
  }

  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT); 
  digitalWrite(buttonPin, HIGH); 

  Serial.begin(9600);
  shifter.clear(); //set all pins on the shift register chain to LOW  
  delay(1000);  

  Serial.println("Starting...");
}

void loop() {

  int reading = analogRead(charPin); 
  //    Serial.println(reading);

  int minReading = charVal - range;
  int maxReading = charVal + range;

  if (reading > maxReading || reading < minReading)
  {
    if (reading < 1000) 
    {
      Serial.print("show character ");   
      type = checkCharType(reading, type);        
      Serial.println(type);     
    } 
    else {
      Serial.print("hide character ");   
      type = checkCharType(charVal, type);     
      Serial.println(type);   
    }
    charVal = reading; 
    delay(50);
  }

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);    

  // Button pressed.
  if (buttonState == LOW && charVal < 1000) 
  {     
    // turn status LED on    
    digitalWrite(statusLED, HIGH);  

    // debounce button input for 50ms
    int x;
    while(x < 50)
    {
      if (digitalRead(buttonState) == HIGH)
        x++;
      else
        x = 0;
      delay(1);
    } 

    //set all steps pins on the shift register to LOW
    shifter.clear();   
    shifter.write();   

    // count how many sequential step blocks are present; ignore any  after an empty space
    for (int pin = 0; pin < stepsPins; pin++) {
      int value = stepsValues[pin];
      if (value > 1000) {
        break;
      } 
      else {
        numSteps++;
      }
    }   

    Serial.print("start-");
    // count how many sequential step blocks are present; ignore any  after an empty space   
    for (int pin = 0; pin < functionPins; pin++) {
      int value = functionValues[pin];
      if (value > 1000) {
        break;
      } 
      else {
        numFunction++;
      }
    }         

    // print step values
    Serial.print(numSteps);
    for (int pin=0; pin < numSteps; pin++)
    {
      int value = stepsValues[pin];
      type = checkType(value, type);    
      if (type.equals(functionType))
      {
        functionFound = true;
      }        
      Serial.print("-");   
     Serial.print(type); 
    }

    // print function values

    if (functionFound == true) 
    {
      Serial.print("-");
      Serial.print(numFunction);
      for (int pin=0; pin < numFunction; pin++)
      {
        int value = functionValues[pin];
        type = checkType(value, type);
        Serial.print("-");
        Serial.print(type);   
      }
    } 
    else {
      Serial.print("-0");
    }
    
    Serial.println();
    
    //turn off function LEDs
    for (int ledPin=6; ledPin<10; ledPin++)
    {
      digitalWrite(ledPin, LOW);
    }

    // reset all values to 0 after data sent
    numSteps = 0;
    numFunction = 0;    
    functionFound = false;
    delay(1000);      
  }

  // Button not pressed. Read game block values from STEPS and FUNCTION rows
  else 
  {
    // turn LED off:
    digitalWrite(statusLED, LOW);    

    // read the values on each pin and save it to their arrays
    for (int pin=0; pin < stepsPins; pin++)
    {
      amux.SelectPin(pin); // choose the pin you want to send signal to off the DeMux
      uint16_t reading = amux.AnalogRead();    
      stepsValues[pin] = reading; 

      //   print steps readings
      //        Serial.println("steps");      
      //        Serial.println(reading);

      if (reading < 1000) // check if block present
      {
        shifter.setPin(pin, HIGH);  // turn respective led on if block is present
      }
      else
      {
        shifter.setPin(pin, LOW);  // turn respective led off if block not present
      }
      shifter.write();     

      // turn on respective function led if block present
      for (int pin=0; pin < functionPins; pin++)
      {
        int reading = analogRead(pin);
        int ledPin = pin+6;
        functionValues[pin] = reading;       

        // print function readings
        //        Serial.println("Function: ");
        //        Serial.println(reading);

        if (reading < 1000)    
        {  
          digitalWrite(ledPin, HIGH);       
        }
        else
        {
          digitalWrite(ledPin,LOW);
        }  
      } 
    }    
  }

}

String checkType(int value, String type) {
  if (value > 0 && value < 6) {
    type = "up";  
  } 
  else if (value > 6 && value < 12) {
    type = "down";  
  } 
  else if (value > 15 && value < 25) {
    type = "left";
  } 
  else if(value > 25 && value < 35) { 
    type = "right"; 
  } 
  else if (value > 48 && value < 60) {
    type = "dance";
  } 
  else if (value > 85 && value < 98) {
    type = "magic";
  } 
  else if (value > 175 && value < 185) {
    type = "tantrum";  
  } 
  else if (value > 245 && value < 260) {
    type = "dress-up";
  }  
  else if (value > 320 && value < 330) {
    type = "sleep";
  } 
  else if (value > 505 && value < 515) {
    type = "function";
  }  
  else if (value > 675 && value < 690) {
    type = "love";
  }
  else if (value > 1000)  { 
    type = "";
  }
  return type;
}

String checkCharType(int value, String type) {
  if (value > 0 && value < 10) {
    type = "pink";  
  } 
  else if(value > 25 && value < 35) { 
    type = "blue"; 
  } 
  else if (value > 505 && value < 515) {
    type = "green";
  }
  return type;
}
