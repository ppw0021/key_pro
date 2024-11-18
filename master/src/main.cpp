#include <Arduino.h>
#include <Wire.h>
#include <Keyboard.h>

void setup()
{
  Wire.begin(); // Join I2C bus as master
  Keyboard.begin();
  Serial.begin(115200);
}

int keyMap[60] = {
    ' ',             // Placeholder for empty
    KEY_TAB,         // Placeholder for empty
    KEY_LEFT_ALT,    // Placeholder for empty
    0,               // Placeholder for empty
    KEY_LEFT_GUI,    // Placeholder for empty
    KEY_LEFT_CTRL,   // Placeholder for empty
    'b',             // ASCII for 'b'
    'v',             // ASCII for 'v'
    'c',             // ASCII for 'c'
    'x',             // ASCII for 'x'
    'z',             // ASCII for 'z'
    KEY_LEFT_SHIFT,  // Placeholder for empty
    'g',             // ASCII for 'g'
    'f',             // ASCII for 'f'
    'd',             // ASCII for 'd'
    's',             // ASCII for 's'
    'a',             // ASCII for 'a'
    KEY_DOWN_ARROW,  // Placeholder for empty
    't',             // ASCII for 't'
    'r',             // ASCII for 'r'
    'e',             // ASCII for 'e'
    'w',             // ASCII for 'w'
    'q',             // ASCII for 'q'
    KEY_UP_ARROW,    // Placeholder for empty
    KEY_F5,          // Placeholder for empty
    KEY_F4,          // Placeholder for empty
    KEY_F3,          // Placeholder for empty
    KEY_F2,          // Placeholder for empty
    KEY_F1,          // Placeholder for empty
    KEY_ESC,         // Placeholder for empty
    KEY_F12,         // RIGHT KEYBOARD STARTS HERE
    KEY_RIGHT_CTRL,  // Placeholder for empty
    KEY_MENU,        // Placeholder for empty
    0,               // Placeholder for empty
    KEY_RIGHT_ALT,   // Placeholder for empty
    ' ',             // Placeholder for empty
    KEY_RIGHT_SHIFT, // ASCII for 'b'
    KEY_RIGHT_ARROW, // ASCII for 'v'
    ']',             // ASCII for 'c'
    '[',             // ASCII for 'x'
    'm',             // ASCII for 'z'
    'n',             // Placeholder for empty
    KEY_RETURN,      // ASCII for 'g'
    KEY_LEFT_ARROW,  // ASCII for 'f'
    'l',             // ASCII for 'd'
    'k',             // ASCII for 's'
    'j',             // ASCII for 'a'
    'h',             // Placeholder for empty
    KEY_BACKSPACE,   // ASCII for 't'
    'p',             // ASCII for 'r'
    'o',             // ASCII for 'e'
    'i',             // ASCII for 'w'
    'u',             // ASCII for 'q'
    'y',             // Placeholder for empty
    KEY_F11,         // Placeholder for empty
    KEY_F10,         // Placeholder for empty
    KEY_F9,          // Placeholder for empty
    KEY_F8,          // Placeholder for empty
    KEY_F7,          // Placeholder for empty
    KEY_F6           // Placeholder for empty
};

bool keyState[60] = {false};

void loop()
{
  // byte count must match size of keyMatrixMap
  Wire.requestFrom(5, 30); // Request 30 bytes from slave device #4
  while (Wire.available())
  { // If slave has sent data

    int keyCode = Wire.read(); // Read each byte as a character
    if (keyCode == 255)
    {
      // No changes
    }
    else
    {
      // Press range
      if (keyState[keyCode])
      {
        Keyboard.release(keyMap[keyCode]);
        keyState[keyCode] = false;
      }
      else
      {
        // Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press(keyMap[keyCode]);
        keyState[keyCode] = true;
      }
    }
  }
  delay(5);

  // Right
  Wire.requestFrom(6, 30); // Request 30 bytes from slave device #4
  while (Wire.available())
  { // If slave has sent data

    int keyCode = Wire.read(); // Read each byte as a character
    if (keyCode == 255)
    {
      // No changes
    }
    else
    {
      // Press range
      if (keyState[keyCode])
      {
        Keyboard.release(keyMap[keyCode]);
        keyState[keyCode] = false;
      }
      else
      {
        Keyboard.press(keyMap[keyCode]);
        keyState[keyCode] = true;
      }
    }
  }
  delay(5); // Delay between requests
}