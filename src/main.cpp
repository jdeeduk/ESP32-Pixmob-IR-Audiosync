#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const int IRPin = 14;     // IR LED GPIO pin (KY-005 / HW489)
const int soundPin = 27; // Sound Sensor DO pin (KY-038 / HW485)

IRsend irsend(IRPin);

// Raw colour codes (source: https://github.com/danielweidman/flipper-pixmob-ir-codes)
uint16_t yellow[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t red[21]    = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t green[21]  = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t pink[23]   = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t blue[27]   = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};


void setup() {
  Serial.begin(9600); // Setup serial for debugging on 9600
  
  // Init IR
  irsend.begin();
  Serial.println("IR Sender ready");

  // Init Sound Sensor
  pinMode(soundPin, INPUT);
}

void loop() {

  int data = digitalRead(soundPin); // If sound sensor is picking up sound, soundPin will be set HIGH.

  // If soundPin is set HIGH then get a random number 1-6 and set the colour accordingly. 
  if (data == 1) {
    int randomNumber = random(1, 6);

    if (randomNumber == 1) {
      Serial.println("send yellow code");
      irsend.sendRaw(yellow, 21, 38);
    }
    if (randomNumber == 2) {
      Serial.println("send green code");
      irsend.sendRaw(green, 21, 38);
    }
    if (randomNumber == 3) {
      Serial.println("send pink code");
      irsend.sendRaw(pink, 23, 38);
    }
    if (randomNumber == 4){
      Serial.println("send blue code");
      irsend.sendRaw(blue, 27, 38);
    }
    if (randomNumber == 5){
      Serial.println("send blue code");
      irsend.sendRaw(red, 21, 38);
    }
    delay(200);
    Serial.println(randomNumber);
    Serial.println("-----");
  }



}

