void monitor_setup() {
  
}

#define READING_COUNT 20

void monitor_loop() {
  byte chipIndex = 0;
  byte pinIndex = 0;
  byte sensorIndex;
  byte readingIndex;
  int adcValue;
  int microseconds;
  float value;  
  unsigned int lowerIndex;
  unsigned int higherIndex;

  for (chipIndex = 0; chipIndex < chipCount; chipIndex++) {
    chipSelect(chipIndex);
    for (pinIndex = 0; pinIndex < chipChannels[chipIndex]; pinIndex++) {
      sensorIndex = getSensorIndex(chipIndex, pinIndex);
      value = 0;
      for (readingIndex = 0; readingIndex < READING_COUNT; readingIndex++) {
        value = value + adc->readADC(pinIndex);
      }
      value = (value / READING_COUNT);
      value = max(calibration_min[sensorIndex], value);
      value = min(calibration_max[sensorIndex], value);
      value -= calibration_min[sensorIndex];
      value /= calibration_max[sensorIndex] - calibration_min[sensorIndex];
      readings[sensorIndex] = value;

      //Serial.print(value);
      //Serial.print("\t");
    }
  }
  //Serial.println();

  higherIndex = 0;

  for (sensorIndex = 1; sensorIndex < 20; sensorIndex++) {
    if (readings[sensorIndex] > readings[higherIndex]) {
      higherIndex = sensorIndex;
    }
  }

  lowerIndex = (higherIndex == 0) ? 1 : 0;
  for (sensorIndex = 0; sensorIndex < 20; sensorIndex++) {
    if (sensorIndex != higherIndex && readings[sensorIndex] > readings[lowerIndex]) {
      lowerIndex = sensorIndex;
    }
  }
  /*
  Serial.print("IndexH: ");
  Serial.print(higherIndex);
  Serial.print("\t");
  Serial.print("IndexL: ");
  Serial.print(lowerIndex);
  Serial.print("\t");
  
  Serial.print("ReadingH: ");
  Serial.print(readings[higherIndex], 5);
  Serial.print("\t");
  Serial.print("ReadingL: ");
  Serial.print(readings[lowerIndex], 5);
  Serial.print("\t");
*/
  value = readings[higherIndex] / readings[lowerIndex];
  /*
  Serial.print("H/L: ");
  Serial.print(value, 5);
  Serial.print("\t");
*/
  value -= 1;
  /*
  Serial.print("H/L - 1: ");
  Serial.print(value, 5);
  Serial.print("\t");
*/
  value *= multipliers[higherIndex - 1];
  /*
  Serial.print("Multiplied: ");
  Serial.print(value, 5);
  Serial.print("\t");
  */
  value = 1.0 - value;
  /*
  Serial.print("Inverted: ");
  Serial.print(value, 5);
  Serial.print("\t");*/

  unsigned int leftIndex = (higherIndex < lowerIndex) ? higherIndex : lowerIndex;
  //Serial.print(higherIndex);
  //Serial.print("\t");
  //Serial.print("Mul: ");
  //Serial.print(multipliers[higherIndex - 1], 5);
  //Serial.print("\t");
  
  
  value = max(0.0, value);
  value = min(1.0, value);
  /*Serial.print("Clamped: ");
  Serial.print(value, 5);
  Serial.print("\t");*/

  value *= 0.5;
 /* Serial.print("Ratio: ");
  Serial.print(value, 5);
  Serial.print("\t");*/ 

  if (lowerIndex > higherIndex) {
    //Serial.print("+\t");
    value = (float)higherIndex + value;
  } else {
    //Serial.print("-\t");
    value = (float)higherIndex - value;
  }

  Serial.print(value * 20, 5);
 //Serial.print("\t");

  
  Serial.println();
}
