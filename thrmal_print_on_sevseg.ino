#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> //Click here to get the library: http://librarymanager/All#Qwiic_IR_Thermometer by SparkFun
#include "SevSeg.h"

SevSeg sevseg; 
IRTherm therm; // Create an IRTherm object to interact with throughout
unsigned long currentMillis, previousMillis;

#define BLINK_INTERVAL 1000
int buzzerPin = 16;
const int BUTTON_PIN = 14; // the number of the pushbutton pin
int was_pressed=0;

void setup() 
{
  Serial.begin(9600); // Initialize Serial to log output
  Wire.begin(); //Joing I2C bus
  
  if (therm.begin() == false){ // Initialize thermal IR sensor
    Serial.println("Qwiic IR thermometer did not acknowledge! Freezing!");
    while(1);
  }
  Serial.println("Qwiic IR Thermometer did acknowledge.");
  
  therm.setUnit(TEMP_C); // Set the library's units to Farenheit
  // Alternatively, TEMP_F can be replaced with TEMP_C for Celsius or
  // TEMP_K for Kelvin.
  
  //pinMode(LED_BUILTIN, OUTPUT); // LED pin as output



  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};

  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(100);

  currentMillis = millis();
  previousMillis = currentMillis;

  pinMode(buzzerPin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() 
{
 // digitalWrite(LED_BUILTIN, HIGH);
    currentMillis = millis();
    if (currentMillis - previousMillis >= BLINK_INTERVAL) {  
      //sevseg.setNumber(numbers[curIdx]);
      previousMillis = currentMillis;
        
      // Call therm.read() to read object and ambient temperatures from the sensor.
      if (therm.read()) // On success, read() will return 1, on fail 0.
      {
        // Use the object() and ambient() functions to grab the object and ambient
      // temperatures.
      // They'll be floats, calculated out to the unit you set with setUnit().
        Serial.print("Object: " + String(therm.object(), 2));
        Serial.println("C");
        Serial.print("Ambient: " + String(therm.ambient(), 2));
        Serial.println("C");
        Serial.println();
      }

      sevseg.setNumberF(therm.object(),1);

      if(therm.object() >25 && was_pressed==0)
         digitalWrite(buzzerPin, HIGH);
      else
         digitalWrite(buzzerPin, LOW);    
    }
 // digitalWrite(LED_BUILTIN, LOW);
  sevseg.refreshDisplay(); 
    // read the state of the switch/button:
  int currentState = digitalRead(BUTTON_PIN);
   if(currentState == LOW){
     Serial.println("pressed!");
     was_pressed =1;
   }
}