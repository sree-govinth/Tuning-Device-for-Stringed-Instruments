#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LENGTH 512
byte rawData[LENGTH];

int count = 0;
const float sample_freq = 8919; // Sample Frequency in Hz
int len = LENGTH;
int i, k;
long sum, sum_old;
int thresh = 0;
float freq_per = 0;
byte pd_state = 0;

float freqarray[] = {82.41, 108.00, 143.83, 193.00, 241.94, 318.63}; // Guitar string frequencies in Hz
String stringNames[] = {"E2", "A2", "D3", "G3", "B3", "E4"};

// Initialize the LCD, set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.print("RVU GUITAR TUNER"); // Initial message
  delay(2000); // Display initial message for 2 seconds
  lcd.clear();
  lcd.print("PLAY A STRING");
}

void loop() {
  if (count < LENGTH) {
    rawData[count] = analogRead(A2) >> 2;
    count++;
  } else {
    sum = 0;
    pd_state = 0;
    int period = 0;

    for (i = 0; i < len; i++) {
      // Autocorrelation
      sum_old = sum;
      sum = 0;
      for (k = 0; k < len - i; k++) sum += (rawData[k] - 128) * (rawData[k + i] - 128) / 256;

      // Peak Detect State Machine
      if (pd_state == 2 && (sum - sum_old) <= 0) {
        period = i;
        pd_state = 3;
      }
      if (pd_state == 1 && (sum > thresh) && (sum - sum_old) > 0) pd_state = 2;
      if (!i) {
        thresh = sum * 0.5;
        pd_state = 1;
      }
    }

    // Frequency identified in Hz
    if (thresh > 500 && period > 0) { // Ensure period is greater than 0
      freq_per = sample_freq / period;
      /*if (freq_per < 340.00){
        lcd.clear(); // Clear the display before printing new data
        lcd.setCursor(0, 0);
        lcd.print("Freq: ");
        lcd.print(freq_per);
      }*/

      // Find the closest string frequency
      float min_diff = 99;
      int closest_string = -1;
      for (int s = 0; s < 6; s++) {
        float diff = abs(freq_per - freqarray[s]);
        if (diff < min_diff) {
          min_diff = diff;
          closest_string = s;
        }
      }

      // Check if the detected frequency is close to the string frequency
      if (min_diff < 20 && freq_per < 340.00) { // Adjust the threshold as needed
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(stringNames[closest_string]);
        lcd.setCursor(0,1);
        if (freq_per < freqarray[closest_string] - 1.0) { // Adjust the range as needed
          lcd.print("Tune UP");
        } else if (freq_per > freqarray[closest_string] + 1.0) { // Adjust the range as needed
          lcd.print("Tune DOWN");
        } else {
          lcd.print("Perfect!");
        }
      } else {
        //lcd.print("No string detected");
        //let it be empty and retain the previous result.
      }
    }
    count = 0;
  }
}



