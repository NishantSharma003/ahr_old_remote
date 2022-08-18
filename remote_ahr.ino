#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF24 radio(7, 8); // CE, CSN
////const byte addresses[][6] = {"00001", "00002"};
//
//const byte address[6] = "00001"; // Address


RF24 radio(7, 8);   // nRF24L01 (CE, CSN)
//const byte address[6] = "00001"; // Address
const byte addresses[][6] = {"00001", "00002"};

int rightup;
int rightside;  

int leftup;
int leftside;

String displaydata;

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

//  radio.begin();
           //  radio.openWritingPipe(addresses[1]); // 00002
           //  radio.openReadingPipe(1, addresses[0]); // 00001
//  radio.openWritingPipe(address);
//  radio.setPALevel(RF24_PA_MIN);

//  radio.begin();
//  radio.openWritingPipe(address);
//  radio.setAutoAck(false);
//  radio.setDataRate(RF24_250KBPS);
//  radio.setPALevel(RF24_PA_LOW);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  
  Serial3.begin(9600); // nextion display
  Serial.begin(9600);
  
  Serial3.print("page 1");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
  delay(1000);
  Serial3.print("page 2");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
  
}
void loop() {
  delay(5);
  radio.stopListening();
  
  data.rightpotupdown = map(analogRead(A0), 0, 1023, 0, 255);
  data.rightpotside = map(analogRead(A1), 0, 1023, 0, 255);
  data.leftpotupdown = map(analogRead(A2), 0, 1023, 0, 255);
  data.leftpotside = map(analogRead(A3), 0, 1023, 0, 255);
  
  if(Serial3.available()>0)
  {displaydata = Serial3.readString();
    if(displaydata[7] == 'B')
    {
    data.touchbutton = 5;
    }
    if(displaydata[7] == 'H')
    {
    data.touchbutton = 10;
    }
    if(displaydata[7] == '1')
    {
    data.touchbutton = 15;
    }
    if(displaydata[7] == '2')
    {
    data.touchbutton = 20;
    }
  }
  

  Serial.print("    leftpotupdown");
  Serial.print("      ");
  Serial.print(data.leftpotupdown); 
  Serial.print("    leftpotside");
  Serial.print("      ");
  Serial.print(data.leftpotside); 
  Serial.print("    rightpotupdown");
  Serial.print("      ");
  Serial.print(data.rightpotupdown); 
  Serial.print("    rightpotside");
  Serial.print("      ");
  Serial.print(data.rightpotside); 
  Serial.print("    touchbutton");
  Serial.print("      ");
  Serial.println(data.touchbutton);


  radio.write(&data, sizeof(Data_Package));
  delay(5);
  
}
