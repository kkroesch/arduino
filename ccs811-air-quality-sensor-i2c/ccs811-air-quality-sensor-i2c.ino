/*!
 * @file readData.ino
 * @brief Read the concentration of carbon dioxide and TVOC
 * @n Experiment phenomenon: read data every 0.5s, and print it out on serial port. 
 *
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (https://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2019-07-19
 * @https://github.com/DFRobot/DFRobot_CCS811
 */
#include "DFRobot_CCS811.h"
#include <arduino-timer.h>

auto timer = timer_create_default();

/*
 * IIC address default 0x5A, the address becomes 0x5B if the ADDR_SEL is soldered.
 */
//DFRobot_CCS811 sensor(&Wire, /*IIC_ADDRESS=*/0x5A);
DFRobot_CCS811 sensor;

const int ledPin = 12;
int ledState = LOW;
bool alarm_set = false;
uint16_t co2_value = 0;
int red_light_pin= 11;
int green_light_pin = 10;
int blue_light_pin = 9;

void setup(void)
{
    Serial.begin(115200);
    /*Wait for the chip to be initialized completely, and then exit*/
    
    /**
     * @brief Set measurement cycle
     * @param cycle:in typedef enum{
     *                  eClosed,      //Idle (Measurements are disabled in this mode)
     *                  eCycle_1s,    //Constant power mode, IAQ measurement every second
     *                  eCycle_10s,   //Pulse heating mode IAQ measurement every 10 seconds
     *                  eCycle_60s,   //Low power pulse heating mode IAQ measurement every 60 seconds
     *                  eCycle_250ms  //Constant power mode, sensor measurement every 250ms
     *                  }eCycle_t;
     */
    if (sensor.begin() != 0){
        Serial.println("failed to init chip, please check if the chip connection is fine");
    }
    sensor.setMeasCycle(sensor.eCycle_250ms);

    pinMode(red_light_pin, OUTPUT);
    pinMode(green_light_pin, OUTPUT);
    pinMode(blue_light_pin, OUTPUT);

    timer.every(5000, read_sensor);
    timer.every(500, toggle_led);
    timer.every(1000, set_status_led);
}

void loop() {
    timer.tick();
}

bool toggle_led(void) {
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    if (alarm_set)
      digitalWrite(ledPin, ledState);
    else
      digitalWrite(ledPin, LOW);
    return true;
}

bool read_sensor(void) {
  
    if(sensor.checkDataReady() == true){
        Serial.print("CO2: ");
        co2_value = sensor.getCO2PPM();
        alarm_set = (co2_value > 3000);
        Serial.print(co2_value);
        Serial.print("ppm, TVOC: ");
        Serial.print(sensor.getTVOCPPB());
        Serial.println("ppb");

    } else {
        Serial.println("Data is not ready!");
    }
    /*!
     * @brief Set baseline
     * @param get from getBaseline.ino
     */
    sensor.writeBaseLine(0x447B);
    //delay cannot be less than measurement cycle
    return true;
}

bool set_status_led(void) {
   if (co2_value < 600)
      set_rgb_color(0, 255, 0);  // green
   if (co2_value >= 1500 && co2_value < 2000)
      set_rgb_color(255, 255, 0);  // yellow
   if (co2_value >= 2000)
      set_rgb_color(255, 0, 0);  // red
       
}

void set_rgb_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
