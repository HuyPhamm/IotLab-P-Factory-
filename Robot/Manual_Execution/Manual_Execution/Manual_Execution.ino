
// /*
// UNOR3
// VCC-3.3V
// GND - GND
// CSN- 10
// CE- 9
// SCK - 13 mặc định SPI
// MÓI - 11 SPi
// Miso - 12 SPI
// */
/** 
Servo info   
servo 1: 0 - 180 (home 90) - 2 - jX
servo 2: 0 - 180 (home 0) - 3 - jY
servo 3: 0 - 180 (home 0) - 4 - A C
servo 4: 180- 85 (home 180) - 5 - D B
servo 5: 0 - 180 (home 0) - 6 - F E
servo6: 180 - 123 (home 180) -7 - K
**/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Servo.h>

#define CSN 10
#define CE 9

#define Button 8
int step = 1;
bool ButtonState;
RF24 radio(CE, CSN); //CE, CSN
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
// const byte diachi[6] = "10001";
// const uint64_t diachi = 0xE8E8F0F0E1LL;
uint8_t diachi[][6] = { "1Node", "2Node" };
byte msg[30];
//Angle home
int goc1 = 90;
int goc2 = 0;
int goc3 = 0;
int goc4 = 85;
int goc5 = 0;
int goc6 = 180;
// void home();
int gochome[7] = {0, 90, 0, 0, 85, 90, 180};

//Angle realtime
int goc[7] = {0, 90, 0, 0, 85, 180, 180};

//Setup home
unsigned long oldtime = 0;
int delaytime = 100;

void setup(){
  Serial.begin(115200);
  //Setup button
  pinMode(Button, INPUT_PULLUP);

  //Setup NRF
  if(!radio.begin())
  {
    Serial.println("Can not open!!!!");
    while(1) {}
  }
  radio.setAutoAck(false); //Nếu radio.available == False thì thêm dòng này
  radio.openReadingPipe(1,diachi); // khong dung chung kenh 0 de tranh chong lan du lieu
  radio.setPALevel(RF24_PA_MIN); // cai bo khuech dai cong suat Min or MAX
  // radio.setChannel(80);
  radio.setDataRate(RF24_1MBPS);
  //Toc do truyen du lieu 250KBPS, 1Mbps, 2Mbps (chiem 2 kenh)
  radio.startListening();
  if(!radio.available())
  {
    Serial.println("Waitting for connect.......");
  }

  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);

  home1();
}
//Setup vị trí an toàn
void home1(){
  servo1.write(gochome[1]);
  servo2.write(gochome[2]);
  servo3.write(gochome[3]);
  servo4.write(gochome[4]);
  servo5.write(gochome[5]);
  servo6.write(gochome[6]);
}

void home(){
  goc[1] = servo1.read();
  goc[2] = servo2.read();
  goc[3] = servo3.read();
  goc[4] = servo4.read();
  goc[5] = servo5.read();
  goc[6] = servo6.read();
  for(int i = 1; i < 7; i++)
  {
    oldtime = millis();
    while(goc[i] != gochome[i]){

      if((unsigned long) (millis() - oldtime) > delaytime) 
      {
        if(goc[i] > gochome[i]){
          goc[i]--;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          servo6.write(goc[6]);
        }
        else if(goc[i] < gochome[i]){
          goc[i]++;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          servo6.write(goc[6]);
        }
        oldtime = millis();
      }
    }
  }
  Serial.println("Hello.......");
}

void ser1(){
  goc[1] = servo1.read();
  if(msg[0] < 25 && goc[1] > 0){
    goc[1] -= step;
    servo1.write(goc[1]);
  }
  if(msg[0] > 60 && goc[1] < 180){
    goc[1] += step;
    servo1.write(goc[1]);
  }
}

void ser2(){
  goc[2] = servo2.read();
  if(msg[1] < 25 && goc[2] > 0){
    goc[2] -= step;
    servo2.write(goc[2]);
  }
  if(msg[1] > 60 && goc[2] < 180){
    goc[2] += step;
    servo2.write(goc[2]);
  }
}

void ser3(){
  goc[3] = servo3.read();
  if(msg[2] == 0 && goc[3] > 0){
    goc[3] -= step;
    servo3.write(goc[3]);
  }
  if(msg[4] == 0 && goc[3] < 180){
    goc[3] += step;
    servo3.write(goc[3]);
  }
}

void ser4(){
  goc[4] = servo4.read();
  if(msg[5] == 0 && goc[4] > 0){
    goc[4] -= step;
    servo4.write(goc[4]);
  }
  if(msg[3] == 0 && goc[4] < 180){
    goc[4] += step;
    servo4.write(goc[4]);
  }
}

void ser5(){
  goc[5] = servo5.read();
  if(msg[7] == 0 && goc[5] > 0){
    goc[5] -= step;
    servo5.write(goc[5]);
  }
  if(msg[6] == 0 && goc[5] < 180){
    goc[5] += step;
    servo5.write(goc[5]);
  }
}
int count = 0;
int last = 1;

void ser6(){
  goc[6] = servo6.read();
  if(msg[8] != last){
    if(msg[8] == 0){
      count++;
    }
  }
  last = msg[8];
  if(count % 2 == 0 && msg[8] == 0 && goc[6] < 180){
    goc[6] += step;
    servo6.write(goc[6]);
  } 
  if(count % 2 != 0 && msg[8] == 0 && goc[6] > 123){
    goc[6] -= step;
    servo6.write(goc[6]);
  } 
  
}
void loop(){

  if(radio.available())
  {
    int Stt = digitalRead(Button);
    if(Stt == 0) home();
    radio.read(&msg, sizeof(msg));
    ser1();
    ser2();
    ser3();
    ser4();
    ser5();
    ser6();

    for(int i = 1; i < 7; i++){
      Serial.print(goc[i]);
      Serial.print("--");
    }
    // for(int i = 0; i < 9; i++){
    //   Serial.print(msg[i]);
    //   Serial.print("--");
    // }
    Serial.print(Stt);
    Serial.println();
    delay(100);

  }

}