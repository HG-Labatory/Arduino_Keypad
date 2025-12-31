#include <Keyboard.h>
#include <Mouse.h>

/* -------- Pins -------- */
const int pinX = A1;
const int pinY = A0;
const int buttonA = 3;
const int buttonB = 2;

/* -------- Einstellungen -------- */
const int CENTER = 512;
const int DEADZONE = 120;
const unsigned long LONG_PRESS = 600;

/* -------- Button-State -------- */
bool lastA = false;
bool lastB = false;
unsigned long pressTimeA = 0;
unsigned long pressTimeB = 0;

const int SCROLL_STEP = 1;              // ein Tick pro Aktion
const unsigned long SCROLL_INTERVAL = 80;
unsigned long lastScrollTime = 0;

/* -------- Setup -------- */
void setup() {
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);

  Keyboard.begin();
  Mouse.begin();
}

/* -------- Aktionen -------- */
void sendCombo(uint8_t mod, char key) {
  Keyboard.press(mod);
  Keyboard.press(key);
  delay(10);
  Keyboard.releaseAll();
}

void altTab() {
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_TAB);
  delay(60);
  Keyboard.releaseAll();
}

void copy()  { sendCombo(KEY_LEFT_CTRL, 'c'); }
void cut()   { sendCombo(KEY_LEFT_CTRL, 'x'); }
void paste() { sendCombo(KEY_LEFT_CTRL, 'v'); }

/* -------- Button-Handler -------- */
void handleButton(
  int pin,
  bool &lastState,
  unsigned long &pressTime,
  void (*shortAction)(),
  void (*longAction)()
) {
  bool pressed = !digitalRead(pin);

  if (pressed && !lastState) {
    pressTime = millis();
  }

  if (!pressed && lastState) {
    unsigned long duration = millis() - pressTime;
    if (duration < LONG_PRESS) shortAction();
    else longAction();
  }

  lastState = pressed;
}

/* -------- Loop -------- */
void loop() {
int x = analogRead(pinX);
int y = analogRead(pinY);

int moveX = 0;
int scrollY = 0;

/* X = Maus bewegen */
if (abs(x - CENTER) > DEADZONE) {
  moveX = (x > CENTER) ? 2 : -2;
}

/* Y = Scrollen (RICHTIG!) */
if (abs(y - CENTER) > DEADZONE) {
  scrollY = (y > CENTER) ? -SCROLL_STEP : SCROLL_STEP;
}

/* Rate-Limit fürs Scrollen */
unsigned long now = millis();

if (now - lastScrollTime >= SCROLL_INTERVAL) {
  if (moveX != 0 || scrollY != 0) {
    Mouse.move(moveX, 0, scrollY);
    lastScrollTime = now;
  }
} else if (moveX != 0) {
  Mouse.move(moveX, 0, 0);
}}
