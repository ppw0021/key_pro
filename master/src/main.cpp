#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Wire.begin();         // Join I2C bus as master
  Serial.begin(115200); // Initialize Serial Monitor for debugging
}

void loop()
{
  Wire.requestFrom(4, 3); // Request 6 bytes from slave device #4
    while (Wire.available())
    { // If slave has sent data

      int a = Wire.read(); // Read each byte as a character
      if (a == 0)
      {
        // No changes
      }
      else
      {
        Serial.println(a);
      }
  }
  delay(100); // Delay between requests
}