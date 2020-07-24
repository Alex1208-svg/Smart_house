#include <TroykaLight.h>
#include <Strela.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <LiquidCrystal_I2C.h>
#include <TroykaThermometer.h>
TroykaThermometer b(P7);
TroykaThermometer c(P11);
TroykaLight sensorLight(P12);
LiquidCrystal_I2C lcd(LC_ADDR, LCEN, LCRW, LCRS, LC4, LC5, LC6, LC7);
int s;
bool automatic = true;
bool a = true;
int u = 0;
bool lgh = 1;
int bt1 = 0;
int bt2 = 0;
unsigned  last_time;
#define PN532_IRQ   P1
Adafruit_PN532 nfc(PN532_IRQ, 100);
uint8_t uidFirstCard[] = {0x34, 0xB6, 0x3D, 0x41, 0x1C, 0x69, 0xA6};
boolean comparisonOfUid(uint8_t uidRead[8], uint8_t uidComp[8], uint8_t uidLen) {
  for (uint8_t i = 0; i < uidLen; i++) {
    if (uidRead[i] != uidComp[i]) {
      return false;
    }
    if (i == (uidLen)-0x01) {
      return true;
    }
  }
}
void setup() {
  Serial.begin(9600);
   nfc.begin();
  int versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    while (1) {
      Serial.print("Didn't find RFID/NFC reader");
      delay(1000);
    }
  }
  Serial.println("Found RFID/NFC reader");
  nfc.SAMConfig();
  Serial.println("Waiting for a card ...");
}
void loop(){
  uint8_t success;
  uint8_t uid[8];
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (comparisonOfUid(uid, uidFirstCard, uidLength)) {
      drive(0,-150);
      delay(500);
      drive(0,0);
      delay(3000);
      drive(0,150);
      delay(500);
      drive(0,0);
   while(uDigitalRead(S2)==0){
    sensorLight.read();
    lcd.begin(16, 2);
    lcd.home();
    int t1 = b.getTemperatureC();
    int t2 = c.getTemperatureC();
    b.read();
    c.read();
    lcd.print(t1);
    lcd.print("C");
    lcd.print(" ");
    lcd.print(t2);
    lcd.print("C");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,1);    
    s=analogRead(P5);
    s = map(s, 0, 980, 0, 100);
    lcd.print(s);
    lcd.print("%");
    delay(1000);
    lcd.clear();
  Serial.println(sensorLight.getLightLux());
  Serial.println(" Lx\t");
//  if (uDigitalRead(P9) == 1) {
//    automatic = not automatic;
//    if (lgh = 1) {
//      drive(-250,0);
//      delay(6000);
//      drive(0,0);
//      lgh = 0;
//    } else {
//      drive(250,0);
//      delay(6000);
//      drive(0,0);
//      lgh = 1;
//    }  
    if (sensorLight.getLightLux() > 500 and lgh == 1){
    drive(100,0);
    delay(4500);
    drive(0,0);
    lgh = 0;
  }
  if (sensorLight.getLightLux() < 5 and lgh == 0){
    Serial.println("Hello");
    drive(-100,0);
    delay(4000);
    drive(0,0);
    lgh = 1;
  }
  }
      drive(0,-150);
      delay(500);
      drive(0,0);
      delay(3000);
      drive(0,150);  
      delay(500);
      drive(0,0); 
   if (uDigitalRead(P4) == 1 and uDigitalRead(P6) == 1) { 
     a = true;
   }
   else{
    a = false;
   }
   bt1 = 0;
   bt2 = 0;
   while (a) {
    delay(1000);
    uDigitalWrite(L3, LOW);
    uDigitalWrite(L2, LOW);
    if (uDigitalRead(P3) == 1) {
     tone(BUZZER, 1000, 1000);
    }
    if (uDigitalRead(P4) == 0) {
      bt1 ++;
      Serial.println(bt1);
      uDigitalWrite(L3, HIGH);
      }
    if (uDigitalRead(P6) == 0) {
      bt2 ++;
      uDigitalWrite(L2, HIGH);
      Serial.println(bt2);
    }
    if(bt1 > 15 or bt2 > 15){
      bt1 = 0;
      bt2 = 0;
      
    }
    if (bt2 == 8 and bt1 == 10) {
      a = false;
    }
   
 //     u ++;
 //   Serial.println(u);
      
//    if (u >= 3) {
//      tone(BUZZER, 1000, 1000);
//      if (bt1 == 10 and bt2 == 8) {
//        u = 0;
//        a = false;
//      }
    
 //   }
    
    }
}
}
