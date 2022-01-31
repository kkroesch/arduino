/*
   Leitwertmessung mit Arduino:
   Die Hardware:
   Behaelter mit Fluessigkeit
   2 Elektroden
   2 Widerstaende ca. 220 Ohm

   PWM-PIN--+   +-------+------------------ A6
            |   |       |
        |   |   |   |   |   +---------+
        |   |   |   |   +---| 220 Ohm |---- +5V
        |           |   |   +---------+
        +-----------+   |
          Behaelter     |   +---------+
                        +---| 220 Ohm |---- GND
                            +---------+
*/

#include <Wire.h> // Include the Arduino SPI library
// Here we'll define the I2C address of our S7S. By default it
//  should be 0x71. This can be changed, though.
const byte s7sAddress = 0x71;
char tempString[10];  // Will be used with sprintf to create strings

const byte pwmPin =  5;
const byte input  = A6;
const byte modePin = 2;


// Datenstruktur fuer gleitenden Mittelwert
const byte N1 = 10;
int ring[N1];
int *p = ring;        // puffer anfang
int *q = &ring[N1 - 1]; // puffer ende
long mean = 0;

// Werte, um max und min zu erkennen:
const float ex = 0.99;
const float xe = 1 - ex;
const int MAX = 1023;
const int MIN = MAX - MAX * ex;
float maxi = 0;
float mini = 1023;


void setup() {
  Serial.begin(9600);
  Serial.println(__FILE__);
  // erzeuge ein Rechtecksignal an pwmPin:
  analogWrite(pwmPin, 128);

  Wire.begin();  // Initialize hardware I2C pins

  // Clear the display, and then turn on all segments and decimals
  clearDisplayI2C();  // Clears display, resets cursor

  // Custom function to send four bytes via I2C
  //  The I2C.write function only allows sending of a single
  //  byte at a time.
  s7sSendStringI2C("On--");
  setBrightnessI2C(240);  // High brightness
  delay(2000);
}


void loop() {
  int v1, v2, v3;
  v1 = analogRead(input);
  /*
    der Analogwert wird durch das PWM-Signal
    staendig hin und her geschaltet
    Deswegen ist eine aufwendige
    Nachbereitung noetig.
  */
  maxi = maxi * ex;
  mini = mini * ex + MIN;
  if (v1 > maxi) maxi = v1;
  if (v1 < mini) mini = v1;

  int diff = maxi - mini;
  mean = mean - *p + diff; // sub alt, add neu
  *p = diff;
  if (++p > q) p = ring;   // puffer ende?
  v1 = mean / N1;

  // USB output
  Serial.print(v1); Serial.print("\n");

  // Display output
  sprintf(tempString, "%4d", v1);

  // This will output the tempString to the S7S
  s7sSendStringI2C(tempString);
  
  delay(500);
}

// This custom function works somewhat like a serial.print.
//  You can send it an array of chars (string) and it'll print
//  the first 4 characters in the array.
void s7sSendStringI2C(String toSend)
{
  Wire.beginTransmission(s7sAddress);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplayI2C()
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightnessI2C(byte value)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}
