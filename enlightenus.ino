#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Countimer.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Countimer timer; 
Countimer breakTimer;
Countimer miniTimer;

int pomodoro = 1;
bool onBreak = false;
bool onMiniBreak = false;
bool displayedText = false;
bool haveWorkedOnMultiple = false;
int pomosUntilSuperBreak = 3;

//PIP'S CODE
int analogPin= 3;
int raw= 0;
int led = 7;
int thres = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  timer.setCounter(0, 0, 3, timer.COUNT_DOWN, onComplete);
  timer.setInterval(refreshClock, 1000);

      breakTimer.setCounter(0, 0, 2, breakTimer.COUNT_DOWN, onBreakComplete);
    breakTimer.setInterval(refreshBreakClock, 1000);

          miniTimer.setCounter(0, 0, 2, miniTimer.COUNT_DOWN, onMiniBreakComplete);
    miniTimer.setInterval(refreshBreakClock, 1000);
}

void refreshClock() {
   Serial.print("Current count time is: ");
   Serial.println(timer.getCurrentTime());
}

void refreshBreakClock() {
   Serial.print("Current count time is: ");
   Serial.println(breakTimer.getCurrentTime());
}

void onComplete() {
    if (pomodoro % pomosUntilSuperBreak == 0 && !haveWorkedOnMultiple) {
    haveWorkedOnMultiple=true;
    Serial.println("Set haveWorkedOnMultiple to True");
    return;
  }
  Serial.print("Pomodoro ");
  Serial.print(pomodoro);
  Serial.println(" Complete!");
  pomodoro++;
  if (!timeForBreak()) {
    Serial.println("not time for a super break, just a regular break");
      onMiniBreak=true;
  }
}

void onBreakComplete(){
  Serial.println("Long ass break over!");
  displayedText=false;
  onBreak=false;
  breakTimer.restart();
  delay(100);
  haveWorkedOnMultiple = false;
  pomodoro++;
}

void onMiniBreakComplete() {
  Serial.println("Break over!");
  displayedText=false;
  onMiniBreak=false;
  miniTimer.restart();
  delay(100);
  pomodoro++;
}

bool timeForBreak() {
  return ((pomodoro % pomosUntilSuperBreak == 0) && pomodoro > 0 && haveWorkedOnMultiple);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  // is it time for the long ass break?
  if (timeForBreak() && !onBreak && !onMiniBreak) {
    onBreak=true;
    Serial.println("Started super break :-)");
    displayedText=false;
    delay(100);
  }
  if (!onBreak && !onMiniBreak) {
      if (displayedText==false) {
    display.clearDisplay();
    display.print(F("Pomodoro: "));
    display.println(pomodoro);
    display.display();
    Serial.print("Start Pomodoro ");
    Serial.println(pomodoro);
    delay(2000);
    display.clearDisplay();
    displayedText=true;
  }

  //P'S CODE 
    float raw = 0;
    raw = analogRead(analogPin);
    if (raw > thres) {
      timer.run();
      Serial.println("Weight - light should be on");
      digitalWrite(led,HIGH);
    }
    else {
      Serial.println("No phone - Light should be off");
      digitalWrite(led, LOW);
    }

      if(!timer.isCounterCompleted()) {
      timer.start();

    }
  display.clearDisplay();
  display.println(timer.getCurrentTime());
  display.display();      // Show initial text
  delay(100);
  if (timer.isCounterCompleted()) {
    displayedText=false;
    delay(500);
    timer.restart();
  }
 }

    if (onBreak) {
      if (displayedText==false) {
        display.clearDisplay();
        display.print(F("SUPER BREAK!"));
        display.display();
        delay(2000);
        display.clearDisplay();
        displayedText=true;
      }

    float raw = 0;
    raw = analogRead(analogPin);
        if (raw < thres) {
          digitalWrite(led, LOW);
          breakTimer.run();
          Serial.println("No phone; starting break timer");
        } else {
          digitalWrite(led,HIGH);
          Serial.println("Phone is on - stopping break timer");
        }
   

      if(!breakTimer.isCounterCompleted()) {
      breakTimer.start();
    }
      display.clearDisplay();
  display.println(breakTimer.getCurrentTime());
  display.display();      // Show initial text
  delay(100);
  } 
  else if (onMiniBreak) {
          if (displayedText==false) {
        display.clearDisplay();
        display.print(F("Mini Break"));
        display.display();
        delay(2000);
        display.clearDisplay();
        displayedText=true;
      }

    float raw = 0;
    raw = analogRead(analogPin);
        if (raw < thres) {
          digitalWrite(led, LOW);
          miniTimer.run();
          Serial.println("No phone; starting break timer");
        } else {
          digitalWrite(led,HIGH);
          Serial.println("Phone is on - stopping break timer");
        }
   

      if(!miniTimer.isCounterCompleted()) {
      miniTimer.start();
    }
      display.clearDisplay();
  display.println(miniTimer.getCurrentTime());
  display.display();      // Show initial text
  delay(100);
  }
}
