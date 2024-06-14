#include <Adafruit_CircuitPlayground.h>

#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 5
#define SWITCH_PIN 7

int level = 1; // set level 1
bool gameOn = true; // game starts as on
bool ballHitTheTarget = false; // flag for hitting the target
unsigned long previousTime = 0; // time tracking for LED movement
long interval = 500; // initial interval for LED movement
int blueLEDPin = 0; // starting position of the blue LED
bool movingRight = true; // direction of LED movement
bool buttonAPressed = false; // flag for button A press
bool buttonBPressed = false; // flag for button B press

uint32_t ledColors[10]; // array to store LED colors

void setup() {
  Serial.begin(9600); // start serial communication
  CircuitPlayground.begin(); // initialize CPX
  pinMode(BUTTON_A_PIN, INPUT); // set button A as input
  pinMode(BUTTON_B_PIN, INPUT); // set button B as input
  pinMode(SWITCH_PIN, INPUT_PULLUP); // set switch as input with pullup

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), resetGame, CHANGE); // attach interrupt to switch

  gameOn = true; // game is on
  level = 1; // start at level 1

  CircuitPlayground.clearPixels(); // clear all LEDs

  Serial.println("Game Start"); // print game start message

  startLevel(); // start the first level
}

void loop() {
  if (!gameOn) return; // if game is off, exit loop

  unsigned long currentTime = millis(); // get current time

  // Check if button A is pressed
  if (digitalRead(BUTTON_A_PIN) == HIGH && !buttonAPressed) {
    buttonAPressed = true;
    nextLevel(); // move to next level
  } else if (digitalRead(BUTTON_A_PIN) == LOW) {
    buttonAPressed = false;
  }

  // Check if button B is pressed
  if (digitalRead(BUTTON_B_PIN) == HIGH && !buttonBPressed) {
    buttonBPressed = true;
    checkSuccess(); // check if the player succeeded
  } else if (digitalRead(BUTTON_B_PIN) == LOW) {
    buttonBPressed = false;
  }

  // Move the blue LED at the set interval
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;
    moveBlueLED(); // move the blue LED
  }
}

void resetGame() {
  static bool switchState = HIGH; // store previous switch state
  bool currentSwitchState = digitalRead(SWITCH_PIN); // read current switch state

  if (currentSwitchState == LOW && switchState == HIGH) {
    gameOn = false; // turn game off
    CircuitPlayground.clearPixels(); // clear all LEDs
    Serial.println("Game Off"); // print game off message
  } else if (currentSwitchState == HIGH && switchState == LOW) {
    gameOn = true; // turn game on
    level = 1; // reset to level 1
    startLevel(); // start level 1
    Serial.println("Game Reset"); // print game reset message
  }
  switchState = currentSwitchState; // update switch state
}

void nextLevel() {
  if (level < 10) {
    level++; // increment level
    startLevel(); // start the new level
  } else if (level == 10) {
    youWon(); // player won the game
  }
}

void startLevel() {
  Serial.print("Starting Level ");
  Serial.println(level); // print starting level

  showRainbow(); // show rainbow effect
  delay(1000); // wait for 1 second
  Serial.print("Level ");
  Serial.print(level);
  Serial.println(", Start!"); // print level start

  setGreenTargetZone(level); // set the green target zone
  blueLEDPin = 0; // reset blue LED position
  movingRight = true; // start moving right
  interval = 500 - (level - 1) * 40; // adjust interval based on level
  ballHitTheTarget = false; // reset target hit flag
}

void moveBlueLED() {
  CircuitPlayground.clearPixels(); // clear all LEDs
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, ledColors[i]); // set LED colors
  }
  CircuitPlayground.setPixelColor(blueLEDPin, 0, 0, 255); // set blue LED

  // Update LED movement direction
  if (blueLEDPin == 9) {
    movingRight = false;
  } else if (blueLEDPin == 0) {
    movingRight = true;
  }

  // Move the blue LED
  if (movingRight) {
    blueLEDPin++;
  } else {
    blueLEDPin--;
  }
}

void setGreenTargetZone(int level) {
  CircuitPlayground.clearPixels(); // clear all LEDs
  int greenLEDCount = (level == 10) ? 1 : 5 - (level / 2); // set green LED count
  int startLED = (level == 10) ? random(0, 10) : random(0, 10 - greenLEDCount); // random start position
  for (int i = 0; i < 10; i++) {
    ledColors[i] = 0; // clear all LED colors
  }
  for (int i = 0; i < greenLEDCount; i++) {
    ledColors[startLED + i] = 0x00FF00; // set green LEDs
  }
}

void checkSuccess() {
  Serial.println("Checking Success...");
  if (ledColors[blueLEDPin] == 0) {
    gameOver(); // game over if blue LED not on green
  } else if (ledColors[blueLEDPin] == 0x00FF00) {
    Serial.println("Success!"); // print success message
    Serial.println("Play Happy Tune"); // indicate happy tune
    if (level == 10) {
      youWon(); // player won the game
    } else {
      nextLevel(); // move to next level
    }
  }
}

void youWon() {
  Serial.println("You won!"); // print win message
  Serial.println("Play Happy Tune"); // indicate happy tune
  showRainbow(); // show rainbow effect
  Serial.println("Returning to Level 1"); // reset to level 1
  level = 1;
  startLevel();
}

void gameOver() {
  Serial.println("Game Over"); // print game over message
  Serial.println("Play Sad Tune"); // indicate sad tune
  flashRed(); // flash red LEDs
  level = 1; // reset to level 1
  startLevel(); // start level 1
}

void showRainbow() {
  Serial.println("Rainbow called"); // print rainbow called
  uint32_t colors[10] = {
    0xFF0000, 0xFF7F00, 0xFFFF00, 0x00FF00, 0x0000FF,
    0x4B0082, 0x9400D3, 0xFF00FF, 0x00FFFF, 0xFFFFFF
  }; // set rainbow colors
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, colors[i]); // set LEDs to rainbow colors
  }
  delay(500); // wait for 0.5 second
  CircuitPlayground.clearPixels(); // clear all LEDs
}

void flashRed() {
  for (int i = 0; i < 5; i++) {
    CircuitPlayground.clearPixels(); // clear all LEDs
    delay(250); // wait for 0.25 second
    setAllPixels(255, 0, 0); // set all LEDs to red
    delay(250); // wait for 0.25 second
  }
}

void setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b); // set all LEDs to specified color
  }
}
