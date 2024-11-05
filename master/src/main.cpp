#include <Arduino.h>
#include <Wire.h>
#include <Keyboard.h>

void setup()
{
  Wire.begin();         // Join I2C bus as master
  Serial.begin(115200); // Initialize Serial Monitor for debugging
  Keyboard.begin();
}

void loop()
{
  Wire.requestFrom(4, 3); // Request 6 bytes from slave device #4
  while (Wire.available())
  { // If slave has sent data

    int keyCode = Wire.read(); // Read each byte as a character
    if (keyCode == 0)
    {
      // No changes
    }
    else
    {
      Serial.println(keyCode);
      if ((keyCode >= 65) && (keyCode <= 90))
      {
        //Press range
        Keyboard.press(keyCode);
      }
      if ((keyCode >= 97) && (keyCode <= 122))
      {
        //Release range
        Keyboard.release(keyCode);
      }
      
    }
  }

  delay(100); // Delay between requests
}