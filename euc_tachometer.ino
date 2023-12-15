#include <Adafruit_SSD1306.h>
#include <Wire.h>
Adafruit_SSD1306 display(128, 64);
int SENS = 1;
int counter = 0;
int TIMES = 0;
boolean LASTTEST = 0;
long hSEKUNDj = 0;
long COUNT = 0;
int CSPEED = 0;
long DISTANCE = 0;

/*long Checkspeed = 0;
long Displayspeed = 0;*/

void setup() {
pinMode(3, INPUT_PULLUP);
pinMode(LED_BUILTIN, OUTPUT);
Wire.begin();
delay(20);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  display.setTextColor(SSD1306_WHITE);
delay(500);
display.clearDisplay();
display.setCursor(0, 0);
display.setTextSize(6);
display.println("START");
display.display();
delay(1000);
}

void loop() {

//Checkspeed = (-(millis()));

  TIMES = 0;
  for (counter = 0; counter < 500; ++counter) {
  //TIMES += random(0, 2);
  if (digitalRead( 3) == LOW && SENS == 0) {
  if (LASTTEST = 1) {
  TIMES += 1;
  SENS = 1;
  }
  LASTTEST = 1;
  digitalWrite(LED_BUILTIN, HIGH);
  } 
  else {
      LASTTEST = 0;

  }
  if (digitalRead(3) == HIGH && SENS == 1) {
  SENS = 0;
  digitalWrite(LED_BUILTIN, LOW);
    }
  delay(1);
}

/*Checkspeed += millis();
Displayspeed = (-(millis()));*/

  display.clearDisplay();
  COUNT += 1;
  if (TIMES > 7) {         //WHEN MOVING ONLY
  DISTANCE += TIMES;
  hSEKUNDj += 1;
  } 
  else {
  TIMES = 0;
  }
  display.setCursor(5,0);
  display.setTextSize(5);
  display.println((TIMES * 0.33) , 0); // ... * CM / 100 000 x 6936.416184971098 / SENSORS = (0.3294797687861272)

  display.setCursor(80, 0);
  display.setTextSize(4);
  display.println(CSPEED);

  display.setTextSize(4);
  display.setCursor(5,37);
  if (DISTANCE > 19285) {
  display.println(DISTANCE * 0.0000519 , 2);
  } else {
  display.print(DISTANCE * 0.0519 , 0);
  }
          if (COUNT > 3 && hSEKUNDj > 1) {
          CSPEED = (DISTANCE / hSEKUNDj * 0.33);
          COUNT = 0;
          }
//display.fillRect(66, 0, 1, 32, WHITE);
display.display();
delay(1);

/*Displayspeed += millis();
display.clearDisplay();
  display.setCursor(80, 0);
display.println(Displayspeed);
  display.setCursor(0, 0);
  display.println(Checkspeed);
display.display();*/
}
