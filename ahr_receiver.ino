#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
const byte addresses[][6] = {"00001", "00002"};

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;


// Hip motor ###########
int motorla1 = 2;
int motorld1 = 3;
int motorld2 = 4;

int motorra1 = 9;
int motorrd1 = 10;
int motorrd2 = 11;


// Base motor ###########
int motorbaseleft1 =5;
int motorbaseleft2 =6;

int motorbaseright1 =12;
int motorbaseright2 =13;


// limit switch safety ##########
int interruptPinla1 = 20;
int interruptPinla2 = 19;
int interruptPinra1 = 18;
int interruptPinra2 = 21;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte leftpotupdown;
  byte leftpotside;
  byte rightpotupdown;
  byte rightpotside;
  byte touchbutton;
};
Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);

  pinMode(motorla1,OUTPUT);
  pinMode(motorld1,OUTPUT);
  pinMode(motorld2,OUTPUT);

  pinMode(motorra1,OUTPUT);
  pinMode(motorrd1,OUTPUT);
  pinMode(motorrd2,OUTPUT);

  pinMode(motorbaseleft1,OUTPUT);
  pinMode(motorbaseleft2,OUTPUT);
  pinMode(motorbaseright1,OUTPUT);
  pinMode(motorbaseright2,OUTPUT);
  
  pinMode(interruptPinla1,INPUT_PULLUP);
  pinMode(interruptPinla2,INPUT_PULLUP);
  pinMode(interruptPinra1,INPUT_PULLUP);
  pinMode(interruptPinra2,INPUT_PULLUP);

  analogWrite(motorla1,250);
  analogWrite(motorra1,250);
  resetData();


}
void loop() {
  radio.startListening();
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
    
   if(data.touchbutton == 10)
   {   Serial.println("hip mode on >>>>>>>>>>>");
       if(data.leftpotupdown > 140 )
       { if (!digitalRead(interruptPinla1)) {}
        else {
        digitalWrite(motorld1,HIGH);
        digitalWrite(motorld2,LOW);
        Serial.println("UP");
        delay(100);
        }
       }
       if(data.leftpotupdown < 100 )
       { if (!digitalRead(interruptPinla2)) {}
        else {
        digitalWrite(motorld2,HIGH);
        digitalWrite(motorld1,LOW);
        Serial.println("DOWN");
        delay(100);
        }
       }

       if(data.rightpotupdown > 140 )
       { if (!digitalRead(interruptPinra1)) {}
        else {
        digitalWrite(motorrd1,HIGH);
        digitalWrite(motorrd2,LOW);
        Serial.println("UP");
        delay(100);
        }
       }
       if(data.rightpotupdown < 100 )
       { if (!digitalRead(interruptPinra2)) {}
        else {
        digitalWrite(motorrd2,HIGH);
        digitalWrite(motorrd1,LOW);
        Serial.println("DOWN");
        delay(100);
        }
       }
   }
   // Base motion ############################################################################
   
   if(data.touchbutton == 5)
   {   Serial.println("base mode on >>>>>>>>>>>");
       while(data.rightpotupdown < 100 )
       {
        analogWrite(motorbaseleft1,0);
        analogWrite(motorbaseleft2,150);

        analogWrite(motorbaseright1,150);
        analogWrite(motorbaseright2,0);
        Serial.println("jpystick back");
        Serial.println("working correct");
        radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
        //delay(100);       
       }
       while(data.leftpotupdown < 100 )
       { 

        analogWrite(motorbaseleft1,150);
        analogWrite(motorbaseleft2,0);

        analogWrite(motorbaseright1,0);
        analogWrite(motorbaseright2,150);
        Serial.println("joystick front");
        radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
        //delay(100);       
       }
       
       // side joystick tuning ######################
       while(data.rightpotside < 100 )
       {
        analogWrite(motorbaseleft1,150);
        analogWrite(motorbaseleft2,0);

        analogWrite(motorbaseright1,150);
        analogWrite(motorbaseright2,0);
        Serial.println("joystick left");
        Serial.println("working correct");
        radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
        //delay(100);
        
       }
       while(data.leftpotside < 100 )
       { 
        analogWrite(motorbaseleft1,0);
        analogWrite(motorbaseleft2,150);

        analogWrite(motorbaseright1,0);
        analogWrite(motorbaseright2,150);
        Serial.println("joystick right");
        radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure
        //delay(100);
       }
   }


       
   
      analogWrite(motorbaseleft1,0);
      analogWrite(motorbaseleft2,0);
      analogWrite(motorbaseright1,0);
      analogWrite(motorbaseright2,0);
        
      digitalWrite(motorld2,LOW);
      digitalWrite(motorld1,LOW);
      digitalWrite(motorrd2,LOW);
      digitalWrite(motorrd1,LOW);
    
    
    
    
    
    
    
    lastReceiveTime = millis(); // At this moment we have received the data
  }
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 1000 ) { // If current time is more then 1 second since we have recived the last data, that means we have lost connection
    resetData(); // If connection is lost, reset the data. It prevents unwanted behavior, for example if a drone has a throttle up and we lose connection, it can keep flying unless we reset the values
  }
  // Print the data in the Serial Monitor
  
//  Serial.print("leftup: ");
//  Serial.print(data.leftpotupdown);
//  Serial.print("; leftside: ");
//  Serial.print(data.leftpotside);
//  Serial.print("; rightup: ");
//  Serial.print(data.rightpotupdown);
//  Serial.print("; rightside: ");
//  Serial.print(data.rightpotside);
//  Serial.print("; button: ");
//  Serial.println(data.touchbutton); 

  digitalWrite(motorld2,LOW);
  digitalWrite(motorld1,LOW);
  digitalWrite(motorrd2,LOW);
  digitalWrite(motorrd1,LOW);

  analogWrite(motorbaseleft1,0);
  analogWrite(motorbaseleft2,0);
  analogWrite(motorbaseright1,0);
  analogWrite(motorbaseright2,0);
  
}
void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.leftpotupdown = 127;
  data.leftpotside = 127;
  data.rightpotupdown = 127;
  data.rightpotside = 127;
  data.touchbutton = 10;
}
