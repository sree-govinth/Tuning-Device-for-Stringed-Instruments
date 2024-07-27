# Tuning-Device-for-Stringed-Instruments
A frequency analysis/comparison project that uses MAX4466 microphone module, Arduino Nano, and an LCD Display (16x2). 

## How the device works
- For simplicity purpose, this device is made for a guitar.
- The MAX4466 module is connected to Arduino Nano, which in turn is connected to an LCD display.
- When the user plucks a string, the microcontroller takes in analog input from the microphone (MAX4466) and processes it to get the frequency.
- Once the frequency is computed, It is compared to the fixed frequencies of the strings. Depending on the difference, the LCD screen will display an advice to tune up or tune down the string.

## How is frequency computed from the analog signal?
-  The code uses autocorrelation and peak detection. Autocorrelation is the process of finding patterns in a signal with a delayed version of itself.
-  This autocorrelation data of the signal will be used for detecting peaks. This is done by making a note of when the signal rises and falls. Once the signal rises and falls, it means that a peak was reached, so the period of the signal will be noted down.
-  The sampling frequency will be divided by the period to get the frequency of the signal.
