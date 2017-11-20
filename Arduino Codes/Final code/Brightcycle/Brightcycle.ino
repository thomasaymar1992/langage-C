//Define libraries

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
#include <SoftwareSerial.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

/*
   Connections
   ===================================================
   Light sensor 
   ==============
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   Lights 
   ==============
   Connect Front light to PIN 3
   Connect Back light(Red wire) to PIN 6
   Connect Left blinker(Green wire) to PIN 7
   Connect Right blinker(Yellow wire) to PIN 5
   Connect GROUND to common ground

   Bluetooth  
   ==============
   Connect TXD to PIN 4
   Connect RXD to PIN 2
   Connect VDD to 5V DC
   Connect GROUND to common ground
   ===================================================
   
*/
   
//Definition of the actuator's pins 
int FrontLightPin = 3;
int BackLightPin = 6;
int LeftBlinkerPin = 7;
int RightBlinkerPin = 5;

//Definition of bluetooth
SoftwareSerial BT1(4,2);    // RX(TXD), TX(RXD)
int state;                  // State of the bluetooth

//Defining variables and constans
int light;                  // Number of lux 
int quantity;               // Number from 0 to 255
const int mlight = 17000;   //Maximum light the light sensor can measure
boolean automode = HIGH;
String inString = "";    // string to hold input
boolean rightBlinker = LOW;
boolean leftBlinker = LOW;


//Define blinking constants
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 500;           // interval at which to blink (milliseconds)

void setup()
{
       //Configure bluetooth 
       Serial.begin(115200);
       Serial.println("Commands:");
       BT1.begin(115200);

       //Define outputs
       pinMode(FrontLightPin, OUTPUT);
       pinMode(BackLightPin, OUTPUT);
       pinMode(LeftBlinkerPin, OUTPUT);
       pinMode(RightBlinkerPin, OUTPUT);   

       //Configure light sensor
       void configureSensor();

       
}

void loop()
{
       //state = BT1.read(); //Always check the state of the bluetooth 
       
       if (BT1.available()){
            state = BT1.read();
            
            Serial.println(BT1.read());
            Serial.println("\n");
            Serial.println(state);
       }

       if(state=='l'){                                           //Work         
            leftBlinker = HIGH; 
            rightBlinker = LOW;            
       }

       if(state=='r'){                                            //Works          
            rightBlinker = HIGH; 
            leftBlinker = LOW; 
       }   

        if(state=='b'){                                            //Works          
            leftBlinker = LOW;
            rightBlinker = LOW;   
       }
       
       if(state=='a'){
        automode = HIGH;
       }
       
       /*if(state=='m'){
        automode = LOW;
       } */
       
       if(automode){                                               //Works
               /* Get a new sensor event */ 
               sensors_event_t event;
               tsl.getEvent(&event);
                    if (event.light > 0){
                          light = event.light;                    //Quantity of lux measured
                          conversion(); 
                          analogWrite(FrontLightPin, quantity);
                      }        
        }else{
                
                 int inChar = Serial.read();
    
                   if (isDigit(inChar)) {
                                                           // convert the incoming byte to a char
                                                           // and add it to the string:
                   inString += (char)inChar;
                    }
   
                                                         // if you get a newline, print the string,
                                                          // then the string's value:
                  if (state == '\n') {
      
                          int value = inString.toInt();
                          Serial.println(value);

                         if((value<= 100) && (value >= 0)){
                         manualLight(value);
                      } 
                 // clear the string for new input:
                  inString = "";
                  }   
        }

       if(state=='s')
       {
              TurnOn(BackLightPin);
              delay(3000);
       }

       if(state=='o')
       {
              TurnOff(BackLightPin);
       }


       if(rightBlinker == HIGH){
              blinking(RightBlinkerPin);
              TurnOff(LeftBlinkerPin);
       }
       if(leftBlinker == HIGH){
              blinking(LeftBlinkerPin);
              TurnOff(RightBlinkerPin);
       }
       if(leftBlinker == LOW){
              TurnOff(LeftBlinkerPin);
       }
       if(rightBlinker == LOW){
              TurnOff(RightBlinkerPin);
       }
}

void configureSensor(void)
{
  //Default settings
  tsl.enableAutoRange(true);                                 /* Auto-gain ... switches automatically between 1x and 16x */ 
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
}

void conversion()
{  
    if (light <= 400) //Considering that at 400lux it starts to be necessary to use lights
    {
      quantity = light/2;
      quantity = 200 - quantity;
      //Serial.println(light);Serial.println(" lux");
      analogWrite(BackLightPin, quantity);
    } else  {
      analogWrite(BackLightPin, 0);
      }
}



   
