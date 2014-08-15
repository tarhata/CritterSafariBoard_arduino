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

int buttonState = 0;     // variable for reading the pushbutton status
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
}

void loop() {

  if (charPresent == true)  // check if character game piece on board
  {
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);    

    // Button pressed.
    if (buttonState == LOW) 
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

      shifter.clear(); //set all pins on the shift register to LOW   
      shifter.write();   

      Serial.print("Steps values: ");
      for (int pin=0; pin < stepsPins; pin++)
      {
        Serial.print(stepsValues[pin]);      
        Serial.print(" ");      
      }
      Serial.println();

      Serial.print("Function values: ");
      for (int pin=0; pin < functionPins; pin++)
      {
        Serial.print(functionValues[pin]);      
        Serial.print(" ");      
      }
      Serial.println();    

      //turn off function LEDs
      for (int ledPin=6; ledPin<10; ledPin++)
      {
        digitalWrite(ledPin, LOW);
      }

      // Blink status led
      for (int i=0; i<5; i++)
      {
        digitalWrite(statusLED, HIGH);
        delay(50);
        digitalWrite(statusLED, LOW);
        delay(50);
      }      
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

        //      Serial.print(reading);      
        //      Serial.println(" ");

        if (reading < 1000) // check if block present
        {
          shifter.setPin(pin, HIGH);  // turn respective led on if block is present
        }
        else
        {
          shifter.setPin(pin, LOW);  // turn respective led off if block not present
        }
        shifter.write();     
      }

      for (int pin=0; pin < functionPins; pin++)
      {
        int reading = analogRead(pin);
        int ledPin = pin+6;
        functionValues[pin] = reading;       

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
  else // character not on board; keeps checking for reading
  {
    int charValue = analogRead(charPin);
    if (charValue < 550)
    {
      Serial.print("Character: ");
      Serial.println(charValue);
      charPresent = true;  
    }
  }
}




