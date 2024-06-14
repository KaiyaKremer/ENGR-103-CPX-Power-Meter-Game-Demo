#include <Adafruit_CircuitPlayground.h>

#define BUTTON_A_PIN 4
#define BUTTON_B_PIN 5
#define SWITCH_PIN 7

int level = 1;
bool gameOn = true;
bool ballHitTheTarget = false;
unsigned long previousTime = 0;
long interval = 500;
int blueLEDPin = 0;
bool movingRight = true;
bool buttonAPressed = false;
bool buttonBPressed = false;

uint32_t ledColors[10];

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  pinMode(BUTTON_A_PIN, INPUT);
  pinMode(BUTTON_B_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), resetGame, CHANGE);

  gameOn = true;
  level = 1;

  CircuitPlayground.clearPixels();

  Serial.println("Game Start");

  startLevel();
}

void loop() {
  if (!gameOn) return;

  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_A_PIN) == HIGH && !buttonAPressed) {
    buttonAPressed = true;
    nextLevel();
  } else if (digitalRead(BUTTON_A_PIN) == LOW) {
    buttonAPressed = false;
  }

  if (digitalRead(BUTTON_B_PIN) == HIGH && !buttonBPressed) {
    buttonBPressed = true;
    checkSuccess();
  } else if (digitalRead(BUTTON_B_PIN) == LOW) {
    buttonBPressed = false;
  }

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;
    moveBlueLED();
  }
}

void resetGame() {
  static bool switchState = HIGH;
  bool currentSwitchState = digitalRead(SWITCH_PIN);

  if (currentSwitchState == LOW && switchState == HIGH) {
    gameOn = false;
    CircuitPlayground.clearPixels();
    Serial.println("Game Off");
  } else if (currentSwitchState == HIGH && switchState == LOW) {
    gameOn = true;
    level = 1;
    startLevel();
    Serial.println("Game Reset");
  }
  switchState = currentSwitchState;
}

void nextLevel() {
  if (level < 10) {
    level++;
    startLevel();
  } else if (level == 10) {
    youWon();
  }
}

void startLevel() {
  Serial.print("Starting Level ");
  Serial.println(level);
  showRainbow();
  delay(1000);
  Serial.print("Level ");
  Serial.print(level);
  Serial.println(", Start!");

  setGreenTargetZone(level);
  blueLEDPin = 0;
  movingRight = true;
  interval = 500 - (level - 1) * 40;
  ballHitTheTarget = false;
}

void moveBlueLED() {
  CircuitPlayground.clearPixels();
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, ledColors[i]);
  }
  CircuitPlayground.setPixelColor(blueLEDPin, 0, 0, 255);

  if (blueLEDPin == 9) {
    movingRight = false;
  } else if (blueLEDPin == 0) {
    movingRight = true;
  }

  if (movingRight) {
    blueLEDPin++;
  } else {
    blueLEDPin--;
  }
}

void setGreenTargetZone(int level) {
  CircuitPlayground.clearPixels();
  int greenLEDCount = (level == 10) ? 1 : 5 - (level / 2);
  int startLED = (level == 10) ? random(0, 10) : random(0, 10 - greenLEDCount);
  for (int i = 0; i < 10; i++) {
    ledColors[i] = 0;
  }
  for (int i = 0; i < greenLEDCount; i++) {
    ledColors[startLED + i] = 0x00FF00;
  }
}

void checkSuccess() {
  Serial.println("Checking Success...");
  if (ledColors[blueLEDPin] == 0) {
    gameOver();
  } else if (ledColors[blueLEDPin] == 0x00FF00) {
    Serial.println("Success!");
    Serial.println("Play Happy Tune");
    if (level == 10) {
      youWon();
    } else {
      nextLevel();
    }
  }
}

void youWon() {
  Serial.println("You won!");
  Serial.println("Play Happy Tune");
  showRainbow();
  Serial.println("Returning to Level 1");
  level = 1;
  startLevel();
}

void gameOver() {
  Serial.println("Game Over");
  Serial.println("Play Sad Tune");
  flashRed();
  level = 1;
  startLevel();
}

void showRainbow() {
  Serial.println("Rainbow called");
  uint32_t colors[10] = {
    0xFF0000, 0xFF7F00, 0xFFFF00, 0x00FF00, 0x0000FF,
    0x4B0082, 0x9400D3, 0xFF00FF, 0x00FFFF, 0xFFFFFF
  };
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, colors[i]);
  }
  delay(500);
  CircuitPlayground.clearPixels();
}

void flashRed() {
  for (int i = 0; i < 5; i++) {
    CircuitPlayground.clearPixels();
    delay(250);
    setAllPixels(255, 0, 0);
    delay(250);
  }
}

void setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}