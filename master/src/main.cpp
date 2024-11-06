#include <Arduino.h>
#include <Wire.h>
#include <Keyboard.h>

void setup()
{
  Wire.begin();         // Join I2C bus as master
  //Keyboard.begin();
  Serial.begin(115200);
}

void loop()
{
  //byte count must match size of keyMatrixMap
  Wire.requestFrom(4, 16); // Request 6 bytes from slave device #4
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
      // if ((keyCode >= 65) && (keyCode <= 90))
      // {
      //   //Press range
      //   Keyboard.press(keyCode);
      //   Serial.println(keyCode);
      // }
      // if ((keyCode >= 97) && (keyCode <= 122))
      // {
      //   //Release range (32 offset because of lowercase to upper)
      //   Keyboard.release(keyCode - 32);
      // }
    }
  }

  delay(100); // Delay between requests
}