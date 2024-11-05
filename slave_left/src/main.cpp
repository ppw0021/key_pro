#include <Arduino.h>

#include <Wire.h>
int command = 0;


//Ascii upper, col, row
int keyMatrixMap[][3] = {
  {81, 0, 0},
  {87, 1, 0},

};

int colToPinArray[][2] = {
  {},

};

int rowToPinArray[][2] = {

};

// keys
// asciilower, asciiupper, currentState, buttonChange, pin

int keyMap[][5] = {
    {65, NULL, 0, 0, 9}, // A, a, false, false, ID
    {66, NULL, 0, 0, 8}, // B, b, false, false, pin
    {67, NULL, 0, 0, 7}, // C, c, false, false, pin
};

int keyCount = (sizeof(keyMap) / sizeof(keyMap[0]));

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int bytes)
{
  if (Wire.available())
  {
    command = Wire.read(); // Read the command sent by the master
  }
}
bool buttonCurrentState = false;
bool buttonChange = false;



void requestEvent()
{
  // if (command == 2)
  // {
  //   Wire.write(keyCount);
  //   command = 1;
  //   return;
  // }
  for (int i = 0; i < keyCount; i++)
  {
    int pressedAsciiCode = keyMap[i][0];
    int releasedAsciiCode = keyMap[i][1];
    int currentState = keyMap[i][2];
    int change = keyMap[i][3];

    if ((currentState == 1) && (change == 1))
    {
      // Pressed
      Wire.write(pressedAsciiCode);
      keyMap[i][3] = 0;
    }
    if ((currentState == 0) && (change == 1))
    {
      // Released
      Wire.write(releasedAsciiCode);
      keyMap[i][3] = 0;
    }
    Wire.write(0);
  }
}

void setup()
{

  pinMode(10, OUTPUT);
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // Register request handler
  Serial.begin(115200);         // start serial for output

  // Assign pins
  for (int i = 0; i < keyCount; i++)
  {
    int pin = keyMap[i][4];
    keyMap[i][1] = (keyMap[i][0] + 32);
    pinMode(pin, INPUT);
    Serial.println(pin);
  }
}

void loop()
{

  int keyCount = (sizeof(keyMap) / sizeof(keyMap[0]));
  for (int i = 0; i < keyCount; i++)
  {
    int currentState = keyMap[i][2];
    int change = keyMap[i][3];
    int pin = keyMap[i][4];
    if (((digitalRead(pin)) && (currentState == 0)) && (change == 0))
    {
      // Pressed
      digitalWrite(10, HIGH);
      keyMap[i][2] = 1;
      keyMap[i][3] = 1;
    }
    if (((!digitalRead(pin)) && (currentState == 1)) && (change == 0))
    {
      // Released
      digitalWrite(10, LOW);
      keyMap[i][2] = 0;
      keyMap[i][3] = 1;
    }
  }
  delay(1);
}

