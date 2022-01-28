#include <Wire.h>
#include <Adafruit_BMP085.h>  // Adafruit Bibliothek einbinden

Adafruit_BMP085 bmp;

void setup(){

 // Seriellen Monitor aktivieren
 Serial.begin(9600);

 // Sensor initialisieren
 if (!bmp.begin()) {
  Serial.println("Keinen gültigen BMP085 Sensor gefunden!");
  while (true) {}
 }
}

void loop(){

 Serial.print("Temperatur: ");
 // readTemperature() Temperatur °C auslesen und anzeigen
 Serial.print(bmp.readTemperature());
 Serial.print(" ");  // Hier müssen wir ein wenig tricksen
 Serial.write(176);  // um das °-Zeichen korrekt darzustellen
 Serial.println("C");

 Serial.print("Luftdruck: ");
 // readPressure() Luftdruck in Pa auslesen und anzeigen
 Serial.print(bmp.readPressure());
 Serial.println(" Pa");

 Serial.print("Hoehe: ");
 // readAltitude() Höhe in meter auslesen und anzeigen
 Serial.print(bmp.readAltitude());
 Serial.println(" meter");

 Serial.println(); // Leerzeile anzeigen
 delay(5000);      // 5 Sekunden warten
}#include <Wire.h>
#include <Adafruit_BMP085.h>  // Adafruit Bibliothek einbinden

Adafruit_BMP085 bmp;

void setup(){

 // Seriellen Monitor aktivieren
 Serial.begin(9600);

 // Sensor initialisieren
 if (!bmp.begin()) {
  Serial.println("Keinen gültigen BMP085 Sensor gefunden!");
  while (true) {}
 }
}

void loop(){

 Serial.print("Temperatur: ");
 // readTemperature() Temperatur °C auslesen und anzeigen
 Serial.print(bmp.readTemperature());
 Serial.print(" ");  // Hier müssen wir ein wenig tricksen
 Serial.write(176);  // um das °-Zeichen korrekt darzustellen
 Serial.println("C");

 Serial.print("Luftdruck: ");
 // readPressure() Luftdruck in Pa auslesen und anzeigen
 Serial.print(bmp.readPressure());
 Serial.println(" Pa");

 Serial.print("Hoehe: ");
 // readAltitude() Höhe in meter auslesen und anzeigen
 Serial.print(bmp.readAltitude());
 Serial.println(" meter");

 Serial.println(); // Leerzeile anzeigen
 delay(5000);      // 5 Sekunden warten
}
