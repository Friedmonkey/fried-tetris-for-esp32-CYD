#pragma once

//use port CN1
//connect ground to ground and vcc to the v3.3
//connect gpio27 to out
#define INPUT_PIN 27

enum KEY
{
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_SELECT,
};


static bool IsKeyDown(KEY key)
{
  int val = analogRead(INPUT_PIN);

  switch (key)
  {
    case KEY_UP: return (val >= 550 && val <= 580);
    case KEY_DOWN: return (val >= 1250 && val <= 1300);
    case KEY_LEFT: return (val < 10);
    case KEY_RIGHT: return (val >= 2020 && val <= 2060);
    case KEY_SELECT: return (val >= 3180 && val <= 3210);
  }
}

static KEY GetKeyPressed()
{
  int val = analogRead(INPUT_PIN);

  if (val > 4000) {
    return KEY_NONE;
  } else if (val < 10) {
    return KEY_LEFT;
  } else if (val >= 550 && val <= 580) {
    return KEY_UP;
  } else if (val >= 1250 && val <= 1300) {
    return KEY_DOWN;
  } else if (val >= 2020 && val <= 2060) {
    return KEY_RIGHT;
  } else if (val >= 3180 && val <= 3210) {
    return KEY_SELECT;
  } else {
    Serial.print("Unknown value: ");
    Serial.println(val);
    return KEY_NONE;
  }
}