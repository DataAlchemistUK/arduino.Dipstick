#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MCP3008.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define WIRE Wire

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);

#define CLK 13
#define MOSI 11
#define MISO 12

#define DEBUG

Adafruit_MCP3008 *adc;

byte chipPins[] =     {10, 9, 8};
byte chipChannels[] = {8, 8, 4};
// Calibration -----------------------------------------------------------------------------------------------------------------  //
//                           [                   8                  ][                   9                  ][        10        ]
int calibration_readings[] = {540, 526, 533, 526, 530, 543, 541, 529, 523, 534, 536, 543, 525, 525, 531, 540, 525, 535, 528, 556};
// -----------------------------------------------------------------------------------------------------------------------------  //
float corrections[] = {0,2,4,6,8,10,10.8,11.6,12.4,13.2,14,14.8,15.6,16.4,17.2,18,18.5,19,19.5,20,20.5,21,21.5,22,22.5,23,23.2,23.4,23.6,23.8,24,24.2,24.4,24.6,24.8,25,25,25,25,25,25,25,25,25,25,25,24.95,24.9,24.85,24.8,24.75,24.7,24.65,24.6,24.55,24.5,24.35,24.2,24.05,23.9,23.75,23.6,23.45,23.3,23.15,23,22.75,22.5,22.25,22,21.75,21.5,21.25,21,20.75,20.5,20.15,19.8,19.45,19.1,18.75,18.4,18.05,17.7,17.35,17,16.65,16.3,15.95,15.6,15.25,14.9,14.55,14.2,13.85,13.5,13.15,12.8,12.45,12.1,11.75,11.4,11.05,10.7,10.35,10,9.3,8.6,7.9,7.2,6.5,5.8,5.1,4.4,3.7,3,2.4,1.8,1.2,0.6,0};


float readings[20];
byte chipCount = sizeof(chipPins);
byte sensorCount;

bool calibrating = false;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  //circle_fromThreePoints(1, 165, 6, 82, 11, 45);
  
  display.clearDisplay();
  display.display();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);  

  sensorCount = 0;
  for (byte i = 0; i < chipCount; i++) {
    sensorCount += chipChannels[i];
  }
  
  monitor_setup();

  adc = new Adafruit_MCP3008();
  adc->begin(CLK, MOSI, MISO, chipPins[0]);
}

void chipSelect(int chipIndex) {
  delete adc;
  adc = NULL;
  
  int i = 0;
  for(i = 0; i < chipCount; i++) {
    digitalWrite(chipPins[i], i != chipIndex);
  }
  adc = new Adafruit_MCP3008();
  adc->begin(CLK, MOSI, MISO, chipPins[chipIndex]);
}

byte getSensorIndex(byte chipIndex, byte pinIndex) {
  byte sensorIndex = 0;
  byte i = 0;

  for(i = 0; i < chipIndex; i++) {
    sensorIndex += chipChannels[i];
  }
  sensorIndex += pinIndex;
  return sensorIndex;
  //Serial.print(sensorIndex);
  //Serial.println();
}

void loop() {
  monitor_loop();
  #ifndef DEBUG
    delay(1);
  #else
    delay(1);
  #endif
}
