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
float calibration_min[] = {539.35, 524.75, 532.70, 524.60, 528.15, 541.95, 539.10, 527.70, 521.60, 533.25, 535.35, 541.25, 523.80, 524.25, 530.00, 538.75, 524.55, 534.55, 527.60, 553.90};
float calibration_max[] = {794.35, 791.70, 793.90, 793.50, 793.00, 793.70, 794.35, 792.40, 792.40, 792.25, 794.05, 793.25, 792.30, 792.55, 791.30, 792.40, 792.05, 791.15, 792.05, 792.45};
float multipliers[]     = {0.169, 0.153, 0.167, 0.157, 0.168, 0.164, 0.159, 0.158, 0.165, 0.16, 0.163, 0.162, 0.164, 0.158, 0.159, 0.159, 0.151, 0.156};

// -----------------------------------------------------------------------------------------------------------------------------  //

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
