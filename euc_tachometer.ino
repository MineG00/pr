
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// Variables
float speed, current, distancekm, distance;
int battery, bat_lvl, temperature, total_distance, prev_total_distance;
byte count, i, data[100], alert, beeper;
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

void setup() {
  delay(1000);
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;
  }
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  delay(100);
  display.clearDisplay();
  //DATA PARSING
  if (Serial.available()) {
    count = Serial.available();
    i = 0;
    while (Serial.available()) {
      data[i] = Serial.read();  // Writing data as array 'data[]'
      i++;
    }
  }

  if (count == 48 && data[18] == (byte)0x00) {
    int spd = data[4] << 8 | data[5];
    speed = abs(spd * 3.6 / 100);
    word vbat = data[2] << 8 | data[3];
    calc_battery(vbat);
    int temp = data[12] << 8 | data[13];
    temperature = temp / 340.0 + 36.53;
    unsigned long dist = (unsigned long)data[6] << 24 | (unsigned long)data[7] << 16 | (unsigned long)data[8] << 8 | (unsigned long)data[9];
    distance = dist;
    unsigned long total_dist = (unsigned long)data[26] << 24 | (unsigned long)data[27] << 16 | (unsigned long)data[28] << 8 | (unsigned long)data[29];
    total_distance = total_dist / 1000;
    int amp = data[10] << 8 | data[11];
    current = amp / 100;
    alert = data[36];
    beeper = data[38];
  }

  if (speed > 2) {
    display.setCursor(5, 0);
    display.setTextSize(5);
    display.println((speed), 0);
    display.setCursor(75, 0);
    display.setTextSize(4);
    display.println(battery);
    display.setTextSize(4);
    display.setCursor(5, 37);
    if (distance > 999) {
      display.print((distance / 1000), 2);
    } else {
      display.print((distance), 0);
      display.println("m");
    }

  } else {
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.print(total_distance);
    display.println("km");
    display.setCursor(0, 24);
    //display.println("km");
    display.setTextSize(2);
    if (distance > 999) {
      display.print((distance / 1000), 3);
      display.println("km");
    } else {
      display.print((distance), 0);
      display.println("m");
    }
    display.setTextSize(3);
    display.print(battery);
    display.print("% ");
    display.print(temperature);
    display.println("C");
  }
  display.display();
}

int calc_battery(int voltage) {
  // WheelLog's 'better percents' formula
  if (voltage > 5440) {
    battery = (voltage - 5380) / 13;
  } else if (voltage > 5290) {
    battery = (int)round((voltage - 5290) / 32.5);
  } else {
    battery = 0;
  }
  /*//WheelLog's normal formula
    if (voltage <= 5290) {
      battery = 0;
    } else if (voltage >= 6580) {
      battery = 100;
    } else {
      battery = (voltage - 5290) / 13;
    }*/
  return battery;
}
