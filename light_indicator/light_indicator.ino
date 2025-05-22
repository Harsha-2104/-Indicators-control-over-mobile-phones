const int LEFT_BUTTON = 4;
const int RIGHT_BUTTON = 3;
const int LEFT_LED = 11;
const int RIGHT_LED = 10;

bool leftIndicatorOn = false;
bool rightIndicatorOn = false;
bool hazardMode = false;
bool toggleState = false;

unsigned long lastButtonCheck = 0;
unsigned long lastToggle = 0;
unsigned long leftPressStart = 0;
unsigned long rightPressStart = 0;

const unsigned long BUTTON_CHECK_INTERVAL = 50;
const unsigned long TOGGLE_INTERVAL = 300;
const unsigned long LONG_PRESS_TIME = 1000;

void setup() {
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("System Initialized");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastButtonCheck >= BUTTON_CHECK_INTERVAL) {
    lastButtonCheck = currentMillis;
    handleButtons(currentMillis);
  }

  if (currentMillis - lastToggle >= TOGGLE_INTERVAL) {
    lastToggle = currentMillis;
    toggleState = !toggleState;
    updateLEDs();
  }
}

void handleButtons(unsigned long now) {
  bool leftPressed = digitalRead(LEFT_BUTTON);
  bool rightPressed = digitalRead(RIGHT_BUTTON);

  static bool leftHandled = false;
  static bool rightHandled = false;

  //left press
  if (leftPressed) {
    if (leftPressStart == 0) leftPressStart = now;
    if (!leftHandled && now - leftPressStart >= LONG_PRESS_TIME) {
      if (hazardMode) {
        hazardMode = false;
        Serial.println("Hazard Mode Deactivated");
      } else {
        leftIndicatorOn = !leftIndicatorOn;
        rightIndicatorOn = false;
        Serial.println(leftIndicatorOn ? "Left Indicator ON" : "Left Indicator OFF");
      }
      leftHandled = true;
    }
  } else {
    leftPressStart = 0;
    leftHandled = false;
  }

  //right press
  if (rightPressed) {
    if (rightPressStart == 0) rightPressStart = now;
    if (!rightHandled && now - rightPressStart >= LONG_PRESS_TIME) {
      if (hazardMode) {
        hazardMode = false;
        Serial.println("Hazard Mode Deactivated");
      } else {
        rightIndicatorOn = !rightIndicatorOn;
        leftIndicatorOn = false;
        Serial.println(rightIndicatorOn ? "Right Indicator ON" : "Right Indicator OFF");
      }
      rightHandled = true;
    }
  } else {
    rightPressStart = 0;
    rightHandled = false;
  }

  //Hazard mode
  if (leftPressed && rightPressed &&
      (now - leftPressStart >= LONG_PRESS_TIME) &&
      (now - rightPressStart >= LONG_PRESS_TIME)) {
    if (!hazardMode) {
      hazardMode = true;
      leftIndicatorOn = false;
      rightIndicatorOn = false;
      Serial.println("Hazard Mode Activated");
    }
  }
}

void updateLEDs() {
  if (hazardMode) {
    digitalWrite(LEFT_LED, toggleState);
    digitalWrite(RIGHT_LED, toggleState);
  } else {
    digitalWrite(LEFT_LED, leftIndicatorOn ? toggleState : LOW);
    digitalWrite(RIGHT_LED, rightIndicatorOn ? toggleState : LOW);
  }
}
