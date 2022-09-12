 void monitor_setup() {
  
}

void monitor_loop() {
  byte chipIndex = 0;
  byte pinIndex = 0;
  byte sensorIndex;
  int adcValue;
  int adcCalibration;
  int microseconds;
  //sSerial.println("monitor_loop");

  // Get current readings
  //Serial.print(micros());
  //Serial.print("\t");
  //if (DEBUG) Serial.println("Readings:");
  for (chipIndex = 0; chipIndex < chipCount; chipIndex++) {
    chipSelect(chipIndex);
    for (pinIndex = 0; pinIndex < chipChannels[chipIndex]; pinIndex++) {
      sensorIndex = getSensorIndex(chipIndex, pinIndex);
      adcCalibration = calibration_readings[sensorIndex];
      adcValue = adc->readADC(pinIndex) - adcCalibration;
      readings[sensorIndex] = adcValue;
      #ifdef DEBUG
        //Serial.print(adcValue);
        //Serial.print("\t");
      #endif
    }
  }
  //if (DEBUG) Serial.println();
  byte highestReadingIndex;
  byte secondHighestReadingIndex;

  // Find highest reading
  adcValue = 0;
  for (sensorIndex = 0; sensorIndex < sensorCount; sensorIndex++) {
    if (readings[sensorIndex] > adcValue) {
      adcValue = readings[sensorIndex];
      highestReadingIndex = sensorIndex;
    }
  }

  //if (DEBUG) Serial.print("Highest Reading: ");
  //if (DEBUG) Serial.println(adcValue);

  // Find second highest reading
  adcValue = 0;
  for (sensorIndex = 0; sensorIndex < sensorCount; sensorIndex++) {
    if (sensorIndex == highestReadingIndex) continue;
    if (readings[sensorIndex] > adcValue) {
      adcValue = readings[sensorIndex];
      secondHighestReadingIndex = sensorIndex;
    }
  }

  // Clamp the second highest reading
  if (readings[secondHighestReadingIndex] < 45) 
    readings[secondHighestReadingIndex] = 45;
  else if (readings[secondHighestReadingIndex] > 165)
    readings[secondHighestReadingIndex] = 165;

  readings[secondHighestReadingIndex] += corrections[readings[secondHighestReadingIndex] - 45];

  // Calculate % position of second highest value between 40 and 165
  float ratio;
  float offset;

  //Serial.print(highestReadingIndex);
  //Serial.print("\t");
  //Serial.print(secondHighestReadingIndex);
  //Serial.print("\t");
        
  float pos;
  float straight;
  float x;

  float m;
  if (highestReadingIndex > secondHighestReadingIndex) {
    m = -12;
  } else {
    m = +12;
  }
  ratio = 1.0 - abs(((readings[secondHighestReadingIndex] - 165) / m) / 10.0);
  offset = ratio * 0.5;
  
  if (highestReadingIndex > secondHighestReadingIndex) {
    offset = -offset;
  }
  
  pos = highestReadingIndex + offset;
  //Serial.print(ratio);
  //Serial.print("\t");
    
  #ifdef DEBUG
    /*
    Serial.print(highestReadingIndex);
    Serial.print("\t");
    */
    /*
    Serial.print(readings[secondHighestReadingIndex]);
    Serial.print("\t");*/
    /*
    Serial.print(ratio);
    Serial.print("\t");
    
    Serial.print(offset);
    Serial.print("\t");
      
    Serial.print(pos);
    Serial.print("\t");
    */
  #endif

  float distance = round(pos * 40) / 2.0;
  distance -= 17.0;
  if (distance < 0) distance = 0;
  //if (DEBUG) Serial.print("Distance: ");
 
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(distance, 1);
  display.display(); 
  Serial.println(distance, 1);
}
