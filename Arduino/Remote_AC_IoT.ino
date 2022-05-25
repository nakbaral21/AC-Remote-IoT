//Blynk
#define BLYNK_TEMPLATE_ID     "TMPLYFPZPVPj"
#define BLYNK_DEVICE_NAME     "AC Remote Control"
#define BLYNK_AUTH_TOKEN      "gYF-9CW-PvLrbTgO8YypY16locPQIidx"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

//Sinkronisasi Blynk ketika baru nyala
BLYNK_CONNECTED() {
  Blynk.syncAll();
}

//WiFi
char ssid[] = "ZTE-3xtruV";
char pass[] = "chepot123";

//IRRemote
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
int spNow;
int spPrev  = 0;

//Data Remote LG
uint16_t acAddress  = 0x88;
uint8_t  acRepeats  = 0;
uint16_t acOn       = 0x34;
uint16_t acOff      = 0xC005;
uint16_t ac18       = 0x834;
uint16_t ac19       = 0x844;
uint16_t ac20       = 0x854;
uint16_t ac21       = 0x864;
uint16_t ac22       = 0x874;
uint16_t ac23       = 0x884;
uint16_t ac24       = 0x894;
uint16_t ac25       = 0x8A4;
uint16_t ac26       = 0x8B4;
uint16_t ac27       = 0x8C4;
uint16_t ac28       = 0x8D4;
uint16_t ac29       = 0x8E4;
uint16_t ac30       = 0x8F4;

//Initial LM75A
#include <inttypes.h>
#include <Wire.h>
#include <lm75.h>
TempI2C_LM75 termo = TempI2C_LM75(0x48,TempI2C_LM75::nine_bits);
float TempC;


//Nextion Library
#include "Nextion.h"
NexNumber displaySetPoint = NexNumber(0, 3, "n0");
NexVariable displaySuhu = NexVariable(0, 4, "x0");
NexDSButton powerButton = NexDSButton(0, 5, "bt0");



//Fungsi Baca nilai suhu dari LM75A
void BacaTemperature()
{
  TempC = termo.getTemp();
  int tempInt = TempC*100;
  displaySuhu.setValue(tempInt);
  Blynk.virtualWrite(V0, TempC);  //Update nilai suhu ke V0
}

//Set Point Control
BLYNK_WRITE(V1)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);  //Update state
  spNow = value;
}

//Power Control
BLYNK_WRITE(V2)
{
  int value = param.asInt();
  Blynk.virtualWrite(V2, value);  //Update state
  spNow = value;
}

void setup() {
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);
  Blynk.virtualWrite(V1, spNow);

  IrSender.begin();
  
  timer.setInterval(1000L, BacaTemperature);
}

void mainProgram(){
  if (spNow!=spPrev){          //jika ada perubahan dari setpoint atau power
    if(spNow > 1){
      displaySetPoint.setValue(spNow);  
    }
    switch (spNow){
      case 1:
      IrSender.sendLG(acAddress, acOn, acRepeats);
      powerButton.setValue(1);
      break;
      case 0:
      IrSender.sendLG(acAddress, acOff, acRepeats);
      powerButton.setValue(0);
      break;
      case 18:
      IrSender.sendLG(acAddress, ac18, acRepeats);
      break;
      case 19:
      IrSender.sendLG(acAddress, ac19, acRepeats);
      break;
      case 20:
      IrSender.sendLG(acAddress, ac20, acRepeats);
      break;
      case 21:
      IrSender.sendLG(acAddress, ac21, acRepeats);
      break;
      case 22:
      IrSender.sendLG(acAddress, ac22, acRepeats);
      break;
      case 23:
      IrSender.sendLG(acAddress, ac23, acRepeats);
      break;
      case 24:
      IrSender.sendLG(acAddress, ac24, acRepeats);
      break;
      case 25:
      IrSender.sendLG(acAddress, ac25, acRepeats);
      break;
      case 26:
      IrSender.sendLG(acAddress, ac26, acRepeats);
      break;
      case 27:
      IrSender.sendLG(acAddress, ac27, acRepeats);
      break;
      case 28:
      IrSender.sendLG(acAddress, ac28, acRepeats);
      break;
      case 29:
      IrSender.sendLG(acAddress, ac29, acRepeats);
      break;
      case 30:
      IrSender.sendLG(acAddress, ac30, acRepeats);
      break;
    }
  }
  spPrev=spNow;
}

void loop() {
  Blynk.run();
  timer.run();
  if(Serial.available()){
    int incomingData = Serial.read();
    if(incomingData == 4 && spNow > 1){
      spNow += 1;
      displaySetPoint.setValue(spNow);
      Blynk.virtualWrite(V1, spNow);
      return;
    }
    if(incomingData == 3 && spNow > 1){
      spNow -= 1;
      displaySetPoint.setValue(spNow);
      Blynk.virtualWrite(V1, spNow);
      return;
    }
    if(incomingData == 0){
      spNow = 0;
      displaySetPoint.setValue(spNow);
      powerButton.setValue(0);
      Blynk.virtualWrite(V2, spNow);
      return;
    }
    if(incomingData == 1){
      spNow = 1;
      displaySetPoint.setValue(spNow);
      powerButton.setValue(1);
      Blynk.virtualWrite(V2, spNow);
      return;
    }
  }
  TempC = termo.getTemp();
  int tempInt = TempC*100;
  displaySuhu.setValue(tempInt);
  mainProgram();
}
