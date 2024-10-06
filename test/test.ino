const int NUM_SLIDERS = 5;
const int analogInputs[NUM_SLIDERS] = {A3, A4, A5, A6, A7};
#include "GyverEncoder.h"
int analogSliderValues[NUM_SLIDERS];
unsigned long timer;
unsigned long timer_smooth_running_rainbow;
unsigned long timer_smooth_rainbow;
unsigned long timer_Slider;

#include <EEPROM.h>
#include <FastLED.h>
#define NUM_LEDS 25
#define DATA_PIN 8
CRGB leds[NUM_LEDS];

int mode = 1;
int hell = 40;
int pos = 0;
int direction = 1;
bool keyMode = 0;

Encoder enc0(2, 3, 4);
Encoder enc1(5, 6, 7);
Encoder enc2(9, 10, 11);
Encoder enc3(A0, A1, A2);

String Slider0;
String Slider1;
String Slider2;
String Slider3;
String Slider4;
int ReadSlider0;
int ReadSlider1;
int ReadSlider2;
int ReadSlider3;
int ReadSlider4;

int MaxSlider0;
int MaxSlider1;
int MaxSlider2;
int MaxSlider3;
int MaxSlider4;
String FulEnc;
String FulEnclast;

int val0 = 100;
int val1 = 100;
int val2 = 100;
int val3 = 0;

int val[4] = {};

String fixVal0;
String fixVal1;
String fixVal2;
String fixVal3;

String fixVal[4] = {};

// Режимы подсветки

void from_red_to_green(){ //режим свечения от красного к зелёному +
  for (int i = 0; i < 5; i++) {
    // Вычисляем цвет для каждого светодиода
    if (mode != 0){clearLed(); break;}
    int red = map(i, 0, 4, 0, 255);
    int green = 255 - red;
    leds[i] = CRGB(red, green, 0); 
    leds[19-i] = CRGB(red, green, 0);
    leds[i+10] = CRGB(red, green, 0);
    leds[9-i] = CRGB(red, green, 0);
    leds[i+20] = CRGB(red, green, 0);
    FastLED.setBrightness(hell);
    readEncod();
    readSlider();
    sendValues();
  }
  FastLED.show();
}

void smooth_running_rainbow(int speed){  //плавная бегущая радуга +
  if (millis() - timer_smooth_running_rainbow > speed){
    timer_smooth_running_rainbow = millis();
    for (int i = NUM_LEDS - 1; i > 19; i--) {
      leds[i] = leds[i - 1];
      if (mode != 1 & mode != 2){clearLed(); break;}
    }
    for (int i = 1; i < 5; i++) {
      leds[19 - i] = leds[20+i];
      if (mode != 1 & mode != 2){clearLed(); break;}
    }
    for (int i = 14; i > 9; i--) {
      leds[i] = leds[i + 10];
      if (mode != 1 & mode != 2){clearLed(); break;}
    }
    for (int i = 1; i < 5; i++) {
      leds[9 - i] = leds[10+i];
      if (mode != 1 & mode != 2){clearLed(); break;}
    }
    for (int i = 4; i > 0; i--) {
      leds[i] = leds[i + 10];
      if (mode != 1 & mode != 2){clearLed(); break;}
    }
    int hue = millis() / 20 % 255;
    leds[0] = CHSV(hue, 255, 255);
    leds[9] = CHSV(hue, 255, 255);
    leds[10] = CHSV(hue, 255, 255);
    leds[19] = CHSV(hue, 255, 255);
    leds[20] = CHSV(hue, 255, 255);
    FastLED.setBrightness(hell);
    FastLED.show();
    readEncod();
    readSlider();
    sendValues();
  }
}

void smooth_rainbow(){  //плавная радуга 
  if (millis() - timer_smooth_rainbow > 20){
    timer_smooth_rainbow = millis();
    for (int i = 0; i < 256; i++) {
      if (mode != 3){clearLed(); break;}
      for (int j = 0; j < NUM_LEDS; j++) {
        leds[j] = CHSV(i, 255, 255);
        readEncod();
        readSlider();
        sendValues();
      }
      FastLED.setBrightness(hell);
      FastLED.show();
    }
  }
}

void paint_the_tape_in(){  // покарсить всё в один цвет
  for (int i = 0; i < NUM_LEDS; i++) {
    if (mode != 4){clearLed(); break;}
    leds[i] = CRGB(255, 0, 250); 
    FastLED.setBrightness(hell);
    readEncod();
    readSlider();
    sendValues();
  }
  FastLED.show();
}

void running_lights(){ // бегущие огни
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 256; j++) {
      if (mode != 5){clearLed(); break;}
      readEncod();
      readSlider();
      sendValues();
      leds[i] = CRGB(j, j, j);
      leds[9-i] = CRGB(j, j, j);
      leds[i+10] = CRGB(j, j, j);
      leds[19-i] = CRGB(j, j, j);
      leds[i+20] = CRGB(j, j, j);
      FastLED.setBrightness(hell);
      FastLED.show();
    }
    for (int j = 255; j >= 0; j--) {
      if (mode != 5){clearLed(); break;}
      readEncod();
      readSlider();
      sendValues();
      leds[i] = CRGB(j, j, j);
      leds[9-i] = CRGB(j, j, j);
      leds[i+10] = CRGB(j, j, j);
      leds[19-i] = CRGB(j, j, j);
      leds[i+20] = CRGB(j, j, j);
      FastLED.setBrightness(hell);
      FastLED.show();
    }
  }
}

void readHellMode() {
  EEPROM.get(0, hell); // Чтение первой переменной из EEPROM (начиная с адреса 0)
  EEPROM.get(sizeof(hell), mode); // Чтение второй переменной из EEPROM (начиная с адреса sizeof(variable1))
}

void writeHellMode() {
  EEPROM.put(0, hell); // Запись первой переменной в EEPROM (начиная с адреса 0)
  EEPROM.put(sizeof(hell), mode); // Запись второй переменной в EEPROM (начиная с адреса sizeof(variable1))
}

void readEncod(){ // прочитать значение с энкодеров

  enc0.tick();
  enc1.tick();
  enc2.tick();
  enc3.tick();

  
  if (enc0.isClick())  {
    val[3] = 111;
  }
  if (enc0.isTurn()){
    if (enc0.isFastR() && val0 < 100) val0 += 10; if (val0 > 100) val0 = 100;
    if (enc0.isFastL() && val0 > 0) val0 -= 10; if (val0 < 0) val0 = 0;
    if (enc0.isRight() && val0 < 100) val0++;
    if (enc0.isLeft() && val0 > 0) val0--;
  }


  if (enc1.isClick())  {
    val[3] = 222;
  }
  if (enc1.isTurn()){
    if (enc1.isFastR() && val1 < 100) val1 += 10; if (val1 > 100) val1 = 100;
    if (enc1.isFastL() && val1 > 0) val1 -= 10; if (val1 < 0) val1 = 0;
    if (enc1.isRight() && val1 < 100) val1++;
    if (enc1.isLeft() && val1 > 0) val1--;
  }

  if (enc2.isClick())  {
    val[3] = 333;
  }
  if (enc2.isTurn()){
    if (enc2.isFastR() && val2 < 100) val2 += 10; if (val2 > 100) val2 = 100;
    if (enc2.isFastL() && val2 > 0) val2 -= 10; if (val2 < 0) val2 = 0;
    if (enc2.isRight() && val2 < 100) val2++;
    if (enc2.isLeft() && val2 > 0) val2--;
  }

  if (enc3.isClick())  {
    if (keyMode == 0){
      keyMode = 1; 
    }else{
      keyMode = 0; 
    }
  }


  if (enc3.isTurn() && keyMode == 1){
    if (enc3.isRight() && mode < 5) mode++;
    if (enc3.isLeft() && mode > 0) mode--;
    writeHellMode();
  }
  if (enc3.isRightH() && hell < 100){
    hell++;
    keyMode = 0;
    writeHellMode();
  }
  if (enc3.isLeftH() && hell > 0){
    hell--;
    keyMode = 0;
    writeHellMode();
  }

  val[0] = val0;
  val[1] = val1;
  val[2] = val2;

  for (int i = 0; i < 4; i++){
    if (val[i] >= 100){
      fixVal[i] = String((int)val[i]);  
    }

    if (val[i] >= 10 && val[i] < 100){
      fixVal[i] = "0" + String((int)val[i]);  
    }

    if (val[i] < 10) {
      fixVal[i] = "00" + String((int)val[i]);
    }
  }

}

void readSlider(){ // прочитать значение со слайдеров
  

  ReadSlider0 = analogRead(A3);
  ReadSlider1 = analogRead(A4);
  ReadSlider2 = analogRead(A5);
  ReadSlider3 = analogRead(A6);
  ReadSlider4 = analogRead(A7);
  
  ReadSlider0 = map( ReadSlider0, 3, 1023, 0, 100);
  ReadSlider1 = map( ReadSlider1, 3, 1023, 0, 100);
  ReadSlider2 = map( ReadSlider2, 3, 1023, 0, 100);
  ReadSlider3 = map( ReadSlider3, 3, 1023, 0, 100);
  ReadSlider4 = map( ReadSlider4, 3, 1023, 0, 100);

  ReadSlider0 = constrain( ReadSlider0, 0, 100);
  ReadSlider1 = constrain( ReadSlider1, 0, 100);
  ReadSlider2 = constrain( ReadSlider2, 0, 100);
  ReadSlider3 = constrain( ReadSlider3, 0, 100);
  ReadSlider4 = constrain( ReadSlider4, 0, 100);

  if (MaxSlider0 <= ReadSlider0){ MaxSlider0 = ReadSlider0;}
  if (MaxSlider1 <= ReadSlider1){ MaxSlider1 = ReadSlider1;}
  if (MaxSlider2 <= ReadSlider2){ MaxSlider2 = ReadSlider2;}
  if (MaxSlider3 <= ReadSlider3){ MaxSlider3 = ReadSlider3;}
  if (MaxSlider4 <= ReadSlider4){ MaxSlider4 = ReadSlider4;}

  if (MaxSlider0 > ReadSlider0 && MaxSlider0 - ReadSlider0 > 1){ MaxSlider0 = ReadSlider0;}
  if (MaxSlider1 > ReadSlider1 && MaxSlider1 - ReadSlider1 > 1){ MaxSlider1 = ReadSlider1;}
  if (MaxSlider2 > ReadSlider2 && MaxSlider2 - ReadSlider2 > 1){ MaxSlider2 = ReadSlider2;}
  if (MaxSlider3 > ReadSlider3 && MaxSlider3 - ReadSlider3 > 1){ MaxSlider3 = ReadSlider3;}
  if (MaxSlider4 > ReadSlider4 && MaxSlider4 - ReadSlider4 > 1){ MaxSlider4 = ReadSlider4;}


  Slider0 = String(int(MaxSlider0)); 
  Slider1 = String(int(MaxSlider1)); 
  Slider2 = String(int(MaxSlider2)); 
  Slider3 = String(int(MaxSlider3)); 
  Slider4 = String(int(MaxSlider4)); 


  if (Slider0.length() == 2){Slider0 = "0" + Slider0;} if (Slider0.length() == 1){Slider0 = "00" + Slider0;}
  if (Slider1.length() == 2){Slider1 = "0" + Slider1;} if (Slider1.length() == 1){Slider1 = "00" + Slider1;}
  if (Slider2.length() == 2){Slider2 = "0" + Slider2;} if (Slider2.length() == 1){Slider2 = "00" + Slider2;}
  if (Slider3.length() == 2){Slider3 = "0" + Slider3;} if (Slider3.length() == 1){Slider3 = "00" + Slider3;}
  if (Slider4.length() == 2){Slider4 = "0" + Slider4;} if (Slider4.length() == 1){Slider4 = "00" + Slider4;}


}

void sendValues(){ // отправить значение в порте если они были изменены
  FulEnc = "|" + Slider0 + "|" + Slider1 + "|" + Slider2 + "|" + Slider3 + "|" + Slider4 + "|" + fixVal[0] + "|" + fixVal[1] + "|" + fixVal[2] + "|" + fixVal[3] + "|";
  if (millis() - timer > 100){ //таймер на 100мл
    timer = millis();
    if (FulEnc != FulEnclast) { //проверка на изменение
      Serial.println(FulEnc); //отправка данных
      FulEnclast = FulEnc;
      val[3] = 0;
      timer_Slider = millis();
      MaxSlider0 = 0;
      MaxSlider1 = 0;
      MaxSlider2 = 0;
      MaxSlider3 = 0;
      MaxSlider4 = 0;
      
    }
  }  
  
}

void clearLed(){
  FastLED.clear();
  FastLED.show();
}

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  enc0.setType(TYPE2);
  enc1.setType(TYPE2);
  enc2.setType(TYPE2);
  enc3.setType(TYPE2);
  Serial.begin(9600);

  readHellMode();
  // hell = 40;
  // mode = 0;
  // writeHellMode();
}

void loop() {

  readEncod();
  readSlider();
  sendValues();

  switch (mode) {
    case 0:
      from_red_to_green();
      break;
    case 1: 
      smooth_running_rainbow(100);
      break;
    case 2: 
      smooth_running_rainbow(10);
      break;
    case 3:
      smooth_rainbow();
      break;
    case 4:
      paint_the_tape_in();
      break;
    case 5:
      running_lights();
      break;
  }

  //MixerValue = Serial.readStringUntil('\n');

}
/*
сделать:
-один энкодер отдать на регулировку подсветки 
-одно нажатие это зайти в настройки режима и при последующем вращении менять его при повторном нажатии сохранить режим в память чтобюы при следующем запуске остался он
-врашение с нажатой кнопкой изменяет яркость
*/

