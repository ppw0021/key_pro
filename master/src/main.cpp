#include <Arduino.h>
#include <Wire.h>
#include <Keyboard.h>

void setup()
{
  Wire.begin(); // Join I2C bus as master
  // Keyboard.begin();
  Serial.begin(115200);
}

int keyMap[25] = {
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    0,   // Placeholder for empty
    'b', // ASCII for 'b'
    'v', // ASCII for 'v'
    'c', // ASCII for 'c'
    'x', // ASCII for 'x'
    'z', // ASCII for 'z'
    0,   // Placeholder for empty
    'g', // ASCII for 'g'
    'f', // ASCII for 'f'
    'd', // ASCII for 'd'
    's', // ASCII for 's'
    'a', // ASCII for 'a'
    0,   // Placeholder for empty
    't', // ASCII for 't'
    'r', // ASCII for 'r'
    'e', // ASCII for 'e'
    'w', // ASCII for 'w'
    'q', // ASCII for 'q'
    0    // Placeholder for empty
};

bool keyState[25] = {false};

void loop()
{
  // byte count must match size of keyMatrixMap
  Wire.requestFrom(5, 30); // Request 6 bytes from slave device #4
  while (Wire.available())
  { // If slave has sent data

    int keyCode = Wire.read(); // Read each byte as a character
    if (keyCode == 0)
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

  delay(10); // Delay between requests
}