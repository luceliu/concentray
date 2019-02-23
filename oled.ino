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

int pomodoro = 1;
bool onBreak = false;
bool displayedText = false;
int pomosUntilBreak = 4;
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
  timer.setCounter(0, 0, 5, timer.COUNT_DOWN, onComplete);
  timer.setInterval(refreshClock, 1000);

      breakTimer.setCounter(0, 0, 3, breakTimer.COUNT_DOWN, onBreakComplete);
    breakTimer.setInterval(refreshBreakClock, 1000);
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
  Serial.print("Pomodoro ");
  Serial.print(pomodoro);
  Serial.println(" Complete!");
  pomodoro++;
}

void onBreakComplete(){
  Serial.println("Break over!");
  displayedText=false;
  onBreak=false;
  breakTimer.restart();
  delay(100);
  pomodoro++;
}

void loop() {

  // put your main code here, to run repeatedly:
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);


  if (!onBreak) {
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
    timer.run();
//    timer.restart();
      if(!timer.isCounterCompleted()) {
      timer.start();
//      pomodoro++;
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

  if ((pomodoro % pomosUntilBreak == 0) && pomodoro > 0) {
    onBreak=true;
    Serial.println(pomodoro);
    Serial.println(pomosUntilBreak);
    Serial.println(pomodoro % pomosUntilBreak);
    Serial.println("Started break :-)");
    displayedText=false;
    delay(100);
  }
 }

    if (onBreak) {
      if (displayedText==false) {
        display.clearDisplay();
        display.print(F("BREAK!"));
        display.display();
        delay(2000);
        display.clearDisplay();
        displayedText=true;
      }
   
    breakTimer.run();
      if(!breakTimer.isCounterCompleted()) {
      breakTimer.start();
    }
      display.clearDisplay();
  display.println(breakTimer.getCurrentTime());
  display.display();      // Show initial text
  delay(100);
  }
}
