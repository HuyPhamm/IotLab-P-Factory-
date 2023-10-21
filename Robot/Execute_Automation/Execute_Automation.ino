#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Servo.h>

#define CSN 10
#define CE 9

#define Button 8

int step = 1;
bool mode = 1;
// RF24 radio(CE, CSN); //CE, CSN
// Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;
// Servo servo5;
// Servo servo6;
// uint8_t diachi[][6] = { "1Node", "2Node" };
byte msg[30];
//Angle home
int goc1 = 90;
int goc2 = 0;
int goc3 = 0;
int goc4 = 85;
int goc5 = 105;
int goc6 = 123;

int gochome[7] = {0, 90, 0, 0, 85, 105, 123};

//Angle realtime
int goc[7] = {0, 90, 0, 0, 85, 105, 123};


int angle_step1[7] = {0, 180, 68, 180, 147, 102, 123};
int angle_step2[7] = {0, 180, 85, 180, 147, 102, 145};
int angle_step3_Red1[7] = {0, 104, 85, 180, 135, 102, 145};
int angle_step3_Red2[7] = {0, 104, 73, 180, 135, 102, 145};
int angle_step3_Green1[7] = {0, 77, 97, 180, 130, 102, 145};
int angle_step3_Green2[7] = {0, 77, 45, 180, 130, 102, 145};
int angle_step3_Blue1[7] = {0, 53, 92, 180, 114, 63, 145};
int angle_step3_Blue2[7] = {0, 53, 42, 180, 114, 63, 145};
int angle_grab = 145;
int angle_throw = 123;

unsigned long oldtime = 0;

void setup(){
  Serial.begin(9600);

    pinMode(Button, INPUT_PULLUP);
  ////Starting setup NRF
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
  ////END Setup


  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);

  servo1.write(gochome[1]);
  servo2.write(gochome[2]);
  servo3.write(gochome[3]);
  servo4.write(gochome[4]);
  servo5.write(gochome[5]);
  servo6.write(gochome[6]);
  home();
}

// void ser1(){
//   goc[1] = servo1.read();
//   if(msg[0] < 25 && goc[1] > 0){
//     goc[1] -= step;
//     servo1.write(goc[1]);
//   }
//   if(msg[0] > 60 && goc[1] < 180){
//     goc[1] += step;
//     servo1.write(goc[1]);
//   }
// }

// void ser2(){
//   goc[2] = servo2.read();
//   if(msg[1] < 25 && goc[2] > 0){
//     goc[2] -= step;
//     servo2.write(goc[2]);
//   }
//   if(msg[1] > 60 && goc[2] < 180){
//     goc[2] += step;
//     servo2.write(goc[2]);
//   }
// }

// void ser3(){
//   goc[3] = servo3.read();
//   if(msg[2] == 0 && goc[3] > 0){
//     goc[3] -= step;
//     servo3.write(goc[3]);
//   }
//   if(msg[4] == 0 && goc[3] < 180){
//     goc[3] += step;
//     servo3.write(goc[3]);
//   }
// }

// void ser4(){
//   goc[4] = servo4.read();
//   if(msg[5] == 0 && goc[4] > 85){
//     goc[4] -= step;
//     servo4.write(goc[4]);
//   }
//   if(msg[3] == 0 && goc[4] < 180){
//     goc[4] += step;
//     servo4.write(goc[4]);
//   }
// }

// void ser5(){
//   goc[5] = servo5.read();
//   if(msg[7] == 0 && goc[5] > 0){
//     goc[5] -= step;
//     servo5.write(goc[5]);
//   }
//   if(msg[6] == 0 && goc[5] < 180){
//     goc[5] += step;
//     servo5.write(goc[5]);
//   }
// }

// int count = 0;
// int last = 1;

// void ser6(){
//   goc[6] = servo6.read();
//   if(msg[8] != last){
//     if(msg[8] == 0){
//       count++;
//     }
//   }
//   last = msg[8];
//   if(count % 2 == 0 && msg[8] == 0 && goc[6] < 180){
//     goc[6] += step;
//     servo6.write(goc[6]);
//   } 
//   if(count % 2 != 0 && msg[8] == 0 && goc[6] > 123){
//     goc[6] -= step;
//     servo6.write(goc[6]);
//   } 
  
// }

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

      if((unsigned long) (millis() - oldtime) > 50) 
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
}

void step_moving(int a[7], int delaytime){
  goc[1] = servo1.read();
  goc[2] = servo2.read();
  goc[3] = servo3.read();
  goc[4] = servo4.read();
  goc[5] = servo5.read();
  //goc[6] = servo6.read();
  for(int i = 1; i < 6; i++)
  {
    oldtime = millis();
    while(goc[i] != a[i]){
      if((unsigned long) (millis() - oldtime) > delaytime) 
      {
        if(goc[i] > a[i]){
          goc[i]--;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          //servo6.write(goc[6]);
        }
        else if(goc[i] < a[i]){
          goc[i]++;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          //servo6.write(goc[6]);
        }
        oldtime = millis();
      }
    }
  }
}

void step_moving1(int a[7], int delaytime){
  goc[1] = servo1.read();
  goc[2] = servo2.read();
  goc[3] = servo3.read();
  goc[4] = servo4.read();
  goc[5] = servo5.read();
  //goc[6] = servo6.read();
  for(int i = 3; i < 6; i++)
  {
    oldtime = millis();
    while(goc[i] != a[i]){

      if((unsigned long) (millis() - oldtime) > delaytime) 
      {
        if(goc[i] > a[i]){
          goc[i]--;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          //servo6.write(goc[6]);
        }
        else if(goc[i] < a[i]){
          goc[i]++;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          //servo6.write(goc[6]);
        }
        oldtime = millis();
      }
    }
  }

  for(int i = 2; i > 0; i--)
  {
    oldtime = millis();
    while(goc[i] != a[i]){

      if((unsigned long) (millis() - oldtime) > delaytime) 
      {
        if(goc[i] > a[i]){
          goc[i]--;
          servo1.write(goc[1]);
          servo2.write(goc[2]);
          servo3.write(goc[3]);
          servo4.write(goc[4]);
          servo5.write(goc[5]);
          servo6.write(goc[6]);
        }
        else if(goc[i] < a[i]){
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

}

void grab(int a, int delaytime){
  servo6.read();
  oldtime = millis();
  while(goc[6] != a){
    if((unsigned long) (millis() - oldtime) > delaytime) 
      {
        if(goc[6] > a){
          goc[6]--;
          servo6.write(goc[6]);
        }
        else if(goc[6] < a){
          goc[6]++;
          servo6.write(goc[6]);
        }
        oldtime = millis();
      }
  }
}

void drop(int a, int delaytime){
  servo6.read();
  while(goc[6] != a){
    if(goc[6] > a){
      goc[6]--;
      servo6.write(goc[6]);
    }
    else if(goc[6] < a){
      goc[6]++;
      servo6.write(goc[6]);
    }
  }
}

void loop(){
  byte check = Serial.read();
  if(check == 0){
    step_moving(angle_step1, 5);
    delay(2300);
    grab(angle_grab, 2);
    step_moving(angle_step2, 2);
    delay(200);
    step_moving(angle_step3_Red1, 5);
    delay(500);
    step_moving(angle_step3_Red2, 5);
    drop(angle_throw, 2);
    step_moving1(gochome, 5);
  }
  else if(check == 1){
    step_moving(angle_step1, 5);
    delay(2300);
    grab(angle_grab, 2);
    step_moving(angle_step2, 2);
    delay(200);
    step_moving(angle_step3_Green1, 5);
    delay(500);
    step_moving(angle_step3_Green2, 5);
    drop(angle_throw, 2);
    step_moving1(gochome, 5);
  }
  else if(check == 2){
    step_moving(angle_step1, 5);
    delay(2300);
    grab(angle_grab, 2);
    step_moving(angle_step2, 2);
    delay(200);
    step_moving(angle_step3_Blue1, 5);
    delay(500);
    step_moving(angle_step3_Blue2, 5);
    drop(angle_throw, 2);
    step_moving1(gochome, 5);
  }
}