
// /*
// UNOR3
// VCC-3.3V
// GND - GND
// CSN-8
// CE-7
// SCK - 13 mặc định SPI
// MÓI - 11 SPi
// Miso - 12 SPI
// */
#define VrX A0
#define VrY A1
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>

RF24 radio(9,10); //CE, CSN
// const byte diachi[6] = "10001";
// const uint64_t diachi = 0xE8E8F0F0E1LL;
uint8_t diachi[][6] = { "1Node", "2Node" };
byte msg[30];

const int A = 2;
const int B = 3;
const int C = 4;
const int D = 5;
const int E = 6;
const int F = 7;
const int K = 8;
const int LED = 13;

int Pin[7] = {2,3,4,5,6,7,8};


void setup(){
  Serial.begin(115200);

  if(!radio.begin())
  {
    Serial.println("Can not open!!!!");
    while(1) {}
  }
  radio.setAutoAck(false);
  radio.openWritingPipe(diachi); // mo mac dinh la duong truyen 0
  radio.setPALevel(RF24_PA_MIN); // cai bo khuech dai cong suat Min or MAX
  // radio.setChannel(80);
  radio.setDataRate(RF24_1MBPS);
  //Toc do truyen du lieu 250KBPS, 1Mbps, 2Mbps (chiem 2 kenh)
  radio.stopListening();
  if(!radio.available())
  {
    Serial.println("Waitting for connect.......");
  }
  else {
    Serial.println("OK BRO");
  }
  

//SetUp Button
  for(int i = 0; i < 7; i++)
    pinMode(Pin[i], INPUT_PULLUP);
  pinMode(VrX, INPUT);
  pinMode(VrY, INPUT);
}

int Joystick(int val){
  return map(val, 0, 1023, 0, 100);
}

void Button(){
  int j = 2;
  for (int i = 0; i < 7; i++){
    msg[j] = digitalRead(Pin[i]);
    j++;
  }
}

void loop(){
      int Xvalue = analogRead(VrX);
      int Yvalue = analogRead(VrY);
      msg[0] = Joystick(Xvalue);
      msg[1] = Joystick(Yvalue);
      Button();
      radio.write(&msg, sizeof(msg));
      for (int i = 0; i < 9; i++){
        Serial.print(msg[i]);
        Serial.print("--");
      }
      Serial.println();
      delay(100);
}

