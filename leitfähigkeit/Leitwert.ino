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
const byte pwmPin =  3;
const byte input  = A6;
const byte modePin = 2;

void setup() {
  Serial.begin(9600);
  Serial.println(__FILE__);
  // erzeuge ein Rechtecksignal an pwmPin:
  analogWrite(pwmPin, 128);
}

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
  if (digitalRead(modePin) == LOW) {
    // DEBUG-Mode:
    v2 = maxi;
    v3 = mini;
  }
  else {
    int diff = maxi - mini;
    mean = mean - *p + diff; // sub alt, add neu
    *p = diff;
    if (++p > q) p = ring;   // puffer ende?
    v1 = mean / N1;
    v2 = v1;
    v3 = v1;
  }
  Serial.print(v1); Serial.print(" ");
  Serial.print(v2); Serial.print(" ");
  Serial.print(v3); Serial.print(" ");
  // verhindere Autoscaling:
  Serial.println(" 0 1023");
  delay(100);
}
