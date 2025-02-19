//자동정렬 shift + alt + f

#include <Arduino.h>
#include "bluetooth.h"
#include "gesture.h"
#include "neopixel.h"
#include "dustsensor.h"
#include "lcd.h"

//전역변수
int turn = 0;
int VolumeState = 0;

//미세먼지 전역변수
int sharpLEDPin = 8;
int sharpVoPin = A1;
unsigned long VoRawTotal = 0;
int VoRawCount = 0;
float Voc = 0.6;
float K = 0.5;
int VoRaw;
float Vo;
float CurrentDust = 0;
int AvgDustNum = 30;

//블루투스 관련 선언
String data = "";
bool connectflag = false;
void getbtstring();
void bluetoothonoff();
void btserialFlush();

//제스쳐관련 선언
void handleGesture();

//네오픽셀관련 선언
void ChangeColor();
void TurnOnLight();
void TurnOffLight();

void setup()
{
  btSerial.begin(9600);
  Serial.begin(9600);

  // lcd.begin(); // LCD 초기화
  // lcd.backlight();
  // lcd.setCursor(0, 0); // 1번째, 1라인
  // lcd.print("PROOF THAT STARK");
  // lcd.setCursor(0, 1); // 1번째, 2라인
  // lcd.print("  HAS A HEART");
  // delay(2500);

  // lcd.begin(); // LCD 초기화
  // lcd.backlight();
  // lcd.setCursor(0, 0); // 1번째, 1라인
  // lcd.print("Turning On");
  // lcd.setCursor(0, 1); // 1번째, 2라인
  // lcd.print("Gesture Sensor..");
  // delay(1000);

  // lcd.begin(); // LCD 초기화
  // lcd.backlight();
  // lcd.setCursor(0, 0); // 1번째, 1라인
  // lcd.print("Turning On");
  // lcd.setCursor(0, 1); // 1번째, 2라인
  // lcd.print("Dust Sensor..");
  // delay(1000);

  // randomSeed(analogRead(0)); //랜덤시드 for gestrue light changing system

  strip.setBrightness(brightness);
  strip.begin();
  strip.show();

  pinMode(APDS9960_INT, INPUT);
  pinMode(sharpLEDPin, OUTPUT);

  attachInterrupt(0, interruptRoutine, FALLING);
  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true))
  {
    Serial.println(F("Gesture sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  lcd.begin();
}

void loop()
{  
  //불이 꺼져있을 경우 연결 요청을 하는 사인을 내보냄
  if (LightState == false)
  {
    //whiteOverRainbow(100, 2);
    // if (turn % 2 == 0)
    // {
    //   printGestureRunning();
    //   if (turn % 2 == 0)
    //     lcd.print(".");
    //   else
    //     lcd.print(" ");
    // }
    // else
    // {
    //   printBluetoothOkay();
    //   if (turn % 2 == 0)
    //     lcd.print(".");
    //   else
    //     lcd.print(" ");
    // }
    // lcd.setCursor(0, 1);
    // lcd.print(" Turn On Light.");
    //pulseWhite(4);
  }

  //레지스터의 값에 따라 LCD가 종료되었다 켜지면, 다시 초기화를 시켜줌
  UpdateLCDbyRegister();

  //켜져있는 경우, 미세먼지를 계속 계산해줌
  if (LightState == true)
  {
    printGestureRunning();
    //7번주기로, 미세먼지 계산해줌
    if (turn % 7 == 6)
    {
      CurrentDust = CalculatDust();
      lcd.setCursor(0, 1);
      lcd.print("dust");
      lcd.setCursor(5, 1);
      lcd.print(':');
      lcd.setCursor(8, 1);
      lcd.print(CurrentDust);
      lcd.setCursor(14, 1);
      lcd.print("ym");
      char buffer[10];
      String strdust = (String)dtostrf(CurrentDust, 1, 3, buffer);
      strcat(buffer, "\r\n");
      Serial.println(buffer);
      btSerial.write(buffer);
    }
  }
  //라이트 모드가 DUSTMODE일 경우
  //라이트가 미세먼지에 따라 변하고, 블루투스로 켜고 끌수 있음
  if (LightMode == DUSTMODE)
  {
    if (0 <= CurrentDust && CurrentDust < 50)
    {
      colorWipe(strip.Color(0, 255, 0, 0), 30);
    }
    else if (50 <= CurrentDust && CurrentDust < 100)
    {
      colorWipe(strip.Color(255, 255, 0, 0), 30);
    }
    else
    {
      colorWipe(strip.Color(255, 0, 0, 0), 30);
    }
    if (isr_flag == 1)
    {
      detachInterrupt(0);
      handleGesture();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
    }
    getbtstring();
    bluetoothonoff();
  }
  //라이트모드가 랜덤인 경우 
  if(LightMode == RANDOMMODE)
  {
    ChangeColor();
    if (isr_flag == 1)
    {
      detachInterrupt(0);
      handleGesture();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
    }
    getbtstring();
    bluetoothonoff();
  }


  //라이트모드가 GENERALMODE일 경우
  //제스쳐 인터럽트와 블루투스 인터럽트 발생
  if(LightMode == GENERALMODE)
  {
    //제스쳐 인터럽트
    if (isr_flag == 1)
    {
      detachInterrupt(0);
      handleGesture();
      isr_flag = 0;
      attachInterrupt(0, interruptRoutine, FALLING);
    }
    getbtstring();
    bluetoothonoff();

    //모드 기능, 이스터에그, 4번이상 up할경우 특수 디자인 나옴
    if (VolumeState == 4)
    {
      //whiteOverRainbow(102, 2);
      rainbowFade2White(3, 3, 0);
    }
  }
  //매 회마다 turn을 늘려줌으로써, 계속 실행될 필요 없는 함수들 간격두고 실행
  turn++;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//네오픽셀 함수

void ChangeColor()
{
  if (LightState == true)
  {
    long randNumber1 = random(255);
    long randNumber2 = random(255);
    long randNumber3 = random(255);
    //long randNumber4 = random(255);

    colorWipe(strip.Color(randNumber1, randNumber2, randNumber3, 0), 30);
  }
}

void TurnOnLight()
{
  if (LightState == false) //꺼져있을때만 점등
  {
    strip.begin();
    //이전 lcd 초기화해줌
    lcd.clear();
    for (int i = 0; i < 24; i++)
    {
      strip.setPixelColor(i, 0, 0, 0, 255); //점등
      strip.show();
      delay(100);
    }
    LightState = true; //불 켜져 있음 표시
    Serial.println("불이 켜져있습니다.");
  }
  else
  {
    if (brightness < 127.5)
    {
      brightness += 127.5;
      strip.setBrightness(brightness);
      strip.show();
    }
  }
}

void TurnOffLight()
{
  if (brightness >= 127.5 && LightState == true)
  {
    brightness -= 127.5;
    strip.setBrightness(brightness);
    strip.show();
  }
  else
  {
    for (int i = 0; i < 24; i++)
    {
      strip.setPixelColor(i, 0, 0, 0, 0); // 소등
      strip.show();
      delay(50);
    }
    LightState = false;
    Serial.println("불이 꺼졌습니다.");
    lcd.clear();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//제스쳐 함수

void handleGesture()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      if (VolumeState == 4)
      {
      }
      else
      {
        Serial.println("UP");
        lcd.setCursor(15, 0); // 1번째, 2라인
        lcd.print('+');
        TurnOnLight();
        VolumeState++;
      }
      break;
    case DIR_DOWN:
      if (VolumeState == 0)
      {
      }
      lcd.setCursor(15, 0); // 1번째, 2라인
      lcd.print('-');
      TurnOffLight();
      if (VolumeState == 4)
      {
        VolumeState -= 4;
        TurnOffLight();
      }
      VolumeState--;
      break;
    case DIR_LEFT:
      Serial.println("LEFT");
      lcd.setCursor(15, 0); // 1번째, 2라인
      lcd.print('<');
      ChangeColor();
      break;
    case DIR_RIGHT:
      Serial.println("RIGHT");
      lcd.setCursor(15, 0); // 1번째, 2라인
      lcd.print('>');
      ChangeColor();
      break;
    // case DIR_NEAR:
    //   Serial.println("NEAR");
    //   break;
    // case DIR_FAR:
    //   Serial.println("FAR");
    //   break;
    default:
      Serial.println("NONE");
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//블루투스 함수

void btserialFlush()
{
  while (btSerial.available() > 0)
  {
    char t = btSerial.read();
  }
}

void getbtstring()
{
  while (btSerial.available())
  {
    char datachar = (char)btSerial.read();
    if (data.length() == 6)
    {
      btserialFlush();
      Serial.println("6개라서 끊어준다");
      break;
    }
    if (datachar == '\r') // /n 과 라인체인지값 빼준다.
    {
      continue;
    }
    if (datachar == '\n')
    {
      continue;
    }
    data += datachar;
    delay(10);
  }
  Serial.println(data); //끝나고 읽은 값 출력해줌
  delay(300);
}

void bluetoothonoff()
{
  if (!data.equals("")) //myString 값이 있다면
  {
    //6글자 까지만 읽으므로 6글자만 함, 이보다 작을경우 랜덤값이 추가되는 오류 해결
    if(data == "connet"){
      pulseWhite(0.5);
      pulseWhite(0.5);
      //켜져있는경우 랜덤으로 바꿔줌
      if(LightState == true){
        ChangeColor();
      }
    }
    else if (data == "on")
    {
      TurnOnLight();
    }
    else if (data == "off")
    {
      TurnOffLight();
    }
    else if (data == "gen")
    {
      LightMode = GENERALMODE;
    }
    else if (data == "dus")
    {
      LightMode = DUSTMODE;
    }
    else if (data == "ran")
    {
      LightMode = RANDOMMODE;
    }
    else //색 정보 보내줬을 경우
    {
      if (LightState == true && LightMode == GENERALMODE)
      {
        String red = data.substring(0, 2);
        String green = data.substring(2, 4);
        String blue = data.substring(4, 6);

        char r[3];
        red.toCharArray(r, 3);
        char g[3];
        green.toCharArray(g, 3);
        char b[3];
        blue.toCharArray(b, 3);


        int rhex = strtol(r, NULL, 16);
        int ghex = strtol(g, NULL, 16);
        int bhex = strtol(b, NULL, 16);

        colorWipe(strip.Color(rhex, ghex, bhex, 0), 10);
      }
    }
    data = ""; //myString 변수값 초기화
  }
}


