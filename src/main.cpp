#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>


const int IRTPin = 14;     // IR LED GPIO pin (KY-005 / HW489)
const int soundPin = 27; // Sound Sensor DO pin (KY-038 / HW485)
const int IRRPin = 34; // IR Receiver Pin (HW490)

IRsend irsend(IRTPin);
IRrecv irrecv(IRRPin);

decode_results results;

// Raw colour codes (source: https://github.com/danielweidman/flipper-pixmob-ir-codes)
uint16_t red[21] = {1400, 1400, 700, 700, 700, 1400, 700, 2800, 700, 2100, 1400, 700, 700, 700, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t green[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t light_green[26] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 1400, 700, 1400, 700, 700, 700, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t yellow_green[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 700, 700, 700, 2100, 1400, 2800, 700};
uint16_t blue[27] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 1400, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 2100, 700};
uint16_t light_blue[28] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 1400, 700, 700, 700, 700, 700, 700, 700, 700, 1400, 1400, 700, 2100, 700};
uint16_t magenta[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 1400, 2100, 700, 700, 700, 700, 1400, 2100, 700, 700, 700, 2100, 700};
uint16_t yellow[21] = {1400, 1400, 700, 700, 700, 700, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 1400, 1400, 2800, 1400, 2800, 700};
uint16_t pink[23] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 700, 700, 2100, 700};
uint16_t orange[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 2100, 700, 700, 700, 1400, 700, 700, 700, 1400, 700, 2100, 1400, 2800, 700};
uint16_t whitish[24] = {700, 700, 700, 2100, 1400, 700, 700, 2800, 700, 1400, 700, 700, 700, 2800, 1400, 1400, 700, 2100, 700, 1400, 700, 1400, 700};
uint16_t turquoise[24] = {700, 700, 700, 700, 1400, 1400, 1400, 2800, 700, 1400, 700, 700, 1400, 1400, 700, 1400, 1400, 2800, 700, 700, 700, 2100, 700};

// State management for continuous colour sending
bool continuousMode = false;
int currentColour = -1; 
unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 500; // Send every 500ms

// Having issues getting the IR to stop sending - its not perfect but spamming the off button will work for now, will fix.
int powerPressCount = 0;
unsigned long firstPowerPressTime = 0;
const int POWER_PRESS_LIMIT = 5;
const unsigned long POWER_TIME_WINDOW = 2000;

// Colour data structure for easier management
struct ColourData {
  uint16_t* data;
  int length;
  const char* name;
};

// Colour definitions
ColourData colours[10] = {
  {red, 21, "RED"},
  {green, 21, "GREEN"},
  {light_green, 26, "LIGHT GREEN"},
  {yellow_green, 24, "YELLOW GREEN"},
  {blue, 27, "BLUE"},
  {light_blue, 28, "LIGHT BLUE"},
  {magenta, 24, "MAGENTA"},
  {yellow, 21, "YELLOW"},
  {pink, 23, "PINK"},
  {orange, 24, "ORANGE"}
}; 

// Restart by spamming off button (hacky fix)
void handlePowerPress() {
  unsigned long currentTime = millis();
  
  // If this is the first press or more than 2 seconds have passed, reset counter
  if (powerPressCount == 0 || (currentTime - firstPowerPressTime) > POWER_TIME_WINDOW) {
    powerPressCount = 1;
    firstPowerPressTime = currentTime;
    Serial.print("POWER press count: ");
    Serial.println(powerPressCount);
  } else {
    powerPressCount++;
    Serial.print("POWER press count: ");
    Serial.println(powerPressCount);
    
    // Restart ESP32
    if (powerPressCount >= POWER_PRESS_LIMIT) {
      Serial.println("POWER pressed 5 times in 2 seconds - Restarting ESP32...");
      delay(100);
      ESP.restart();
    }
  }
}

void setup() {
  Serial.begin(9600); // Setup serial for debugging on 9600
  
  // Init IR Transmitter
  irsend.begin();
  Serial.println("IR Sender ready");

  // Init IR Receiver
  irrecv.enableIRIn();
  Serial.println("IR Receiver ready");

  // Init Sound Sensor
  pinMode(soundPin, INPUT);
}

void loop() {

  int data = digitalRead(soundPin); // If sound sensor is picking up sound, soundPin will be set HIGH.

  // If soundPin is set HIGH and NOT in continuous mode, then get a random number 1-6 and set the colour accordingly. 
  if (data == 1 && !continuousMode) {
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
      Serial.println("send red code");
      irsend.sendRaw(red, 21, 38);
    }
    delay(200);
    Serial.println(randomNumber);
    Serial.println("-----");
  }


  if (irrecv.decode(&results)) {
    Serial.println(resultToHumanReadableBasic(&results));
    Serial.println(resultToSourceCode(&results));
    Serial.println();

    if (results.decode_type == decode_type_t::NEC) {
      switch (results.value) {
        case 0xFF6897:   // "0" - Red
          currentColour = 0;
          continuousMode = true;
          Serial.println("Starting continuous RED");
          break;
        case 0xFF30CF:   // "1" - Green
          currentColour = 1;
          continuousMode = true;
          Serial.println("Starting continuous GREEN");
          break;
        case 0xFF18E7:   // "2" - Light Green
          currentColour = 2;
          continuousMode = true;
          Serial.println("Starting continuous LIGHT GREEN");
          break;
        case 0xFF7A85:   // "3" - Yellow Green
          currentColour = 3;
          continuousMode = true;
          Serial.println("Starting continuous YELLOW GREEN");
          break;
        case 0xFF10EF:   // "4" - Blue
          currentColour = 4;
          continuousMode = true;
          Serial.println("Starting continuous BLUE");
          break;
        case 0xFF38C7:   // "5" - Light Blue
          currentColour = 5;
          continuousMode = true;
          Serial.println("Starting continuous LIGHT BLUE");
          break;
        case 0xFF5AA5:   // "6" - Magenta
          currentColour = 6;
          continuousMode = true;
          Serial.println("Starting continuous MAGENTA");
          break;
        case 0xFF42BD:   // "7" - Yellow
          currentColour = 7;
          continuousMode = true;
          Serial.println("Starting continuous YELLOW");
          break;
        case 0xFF4AB5:   // "8" - Pink
          currentColour = 8;
          continuousMode = true;
          Serial.println("Starting continuous PINK");
          break;
        case 0xFF52AD:   // "9" - Orange
          currentColour = 9;
          continuousMode = true;
          Serial.println("Starting continuous ORANGE");
          break;
        case 0xFFA25D:   // "STOP"
          continuousMode = false;
          currentColour = -1;
          lastSend = 0;  // Reset the send timer
          Serial.println("STOP - Continuous mode disabled");
          Serial.println("All IR sending stopped");
          break;
        case 0xFF629D:   // "POWER" - Whitish (single send) + restart counter
          Serial.println("Sending WHITISH (single)");
          irsend.sendRaw(whitish, 24, 38);
          handlePowerPress(); // Track for restart functionality
          break;
        case 0xFFE21D:   // "MENU" - Turquoise (single send)
          Serial.println("Sending TURQUOISE (single)");
          irsend.sendRaw(turquoise, 24, 38);
          break;
        default:
          Serial.println("NEC code not handled");
          break;
      }
    }

    irrecv.resume();
    delay(50); // Small delay to prevent rapid button presses
  }

  // Continuous colour sending mode
  if (continuousMode && currentColour >= 0 && currentColour < 10) {
    unsigned long now = millis();
    if (now - lastSend > SEND_INTERVAL) {
      irsend.sendRaw(colours[currentColour].data, colours[currentColour].length, 38);
      Serial.print("Sending continuous ");
      Serial.println(colours[currentColour].name);
      lastSend = now;
    }
  }



}

