#include <LiquidCrystal_I2C.h>

#define Led_Red 11
#define Led_Green 12
#define Led_Blue 13
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

LiquidCrystal_I2C lcd(0x27,16,2); 

int frequency_red = 0;
int frequency_green = 0;
int frequency_blue = 0;
int Result[3] = {0, 0, 0};

unsigned long oldtime = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(sensorOut, INPUT);
  
  pinMode(Led_Red, OUTPUT);
  pinMode(Led_Green, OUTPUT);
  pinMode(Led_Blue, OUTPUT);

  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);
}

int Read_Red(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  return frequency_red = pulseIn(sensorOut, LOW);
}
int Read_Green(){
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  return frequency_green = pulseIn(sensorOut, LOW);
}
int Read_Blue(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  return frequency_blue = pulseIn(sensorOut, LOW);
}
void loop() {
  lcd.clear();
  int check;
  digitalWrite(Led_Red, LOW);
  digitalWrite(Led_Green, LOW);
  digitalWrite(Led_Blue, LOW);

  Result[0] = Read_Red();
  Result[1] = Read_Green();
  Result[2] = Read_Blue();

  if(Result[0] > 110 && Result[0] <140 && Result[1] > 185){
    digitalWrite(Led_Red, HIGH);
    check = 0;
    Serial.write(check);
    lcd.setCursor(0,0);
    lcd.print("RED");
    // Serial.println('0');
    delay(2000);
  } 
  if(Result[1] < 185 && Result[1] > 150 && Result[2] > 130){
    digitalWrite(Led_Green, HIGH);
    check = 1;
    Serial.write(check);
    lcd.setCursor(0,0);
    lcd.print("Green");
    // Serial.println('1');
    delay(2000);
  }
  if(Result[2] < 130 && Result[2] > 90 && Result[0] > 200 ) 
  {
    digitalWrite(Led_Blue, HIGH);
    check = 2;
    Serial.write(check);
    lcd.setCursor(0,0);
    lcd.print("BLUE");
    // Serial.println('2');
    delay(2000);
  }
  
  // Serial.println(check);
}