#include <Arduino.h>

#include <Wire.h>
int command = 0;

// Ascii upper, colID, rowID
int keyMatrixMap[][3] = {
    {1, 0, 0},
    {2, 1, 0},
    {3, 2, 0},
    {4, 3, 0},
    {5, 0, 1},
    {6, 1, 1},
    {7, 2, 1},
    {8, 3, 1},
    {9, 0, 2},
    {10, 1, 2},
    {11, 2, 2},
    {12, 3, 2},
    {13, 0, 3},
    {14, 1, 3},
    {15, 2, 3},
    {16, 3, 3}};

// State and change
int ketStates[][2] = {
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0},
    {0, 0}};

// Power pins (colID)
int colToPinArray[] = {9, 8, 7, 6};

// Reading pins (rowID)
int rowToPinArray[] = {2, 3, 4, 5};

// Total key count
int keyCount = (sizeof(keyMatrixMap) / sizeof(keyMatrixMap[0]));

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int bytes)
{
  if (Wire.available())
  {
    command = Wire.read(); // Read the command sent by the master
  }
}

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
    int pressedAsciiCode = keyMatrixMap[i][0];
    int releasedAsciiCode = keyMatrixMap[i][0] + 32;
    int currentState = ketStates[i][0];
    int change = ketStates[i][1];

    if ((currentState == 1) && (change == 1))
    {
      // Pressed
      Serial.println();
      Serial.print(pressedAsciiCode);
      Serial.print(" pressed");
      Serial.println();
      Wire.write(pressedAsciiCode);
      ketStates[i][1] = 0;
    }
    if ((currentState == 0) && (change == 1))
    {
      // Released
      Serial.println();
      Serial.print(releasedAsciiCode);
      Serial.print(" released");
      Serial.println();
      Wire.write(releasedAsciiCode);
      ketStates[i][1] = 0;
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

  // Set pinmodes for outputs
  int colSize = sizeof(colToPinArray) / sizeof(colToPinArray[0]);
  for (int i = 0; i < colSize; i++)
  {
    pinMode(colToPinArray[i], OUTPUT);
    digitalWrite(colToPinArray[i], HIGH);
  }

  // Set pinmodes for inputs
  int rowSize = sizeof(rowToPinArray) / sizeof(rowToPinArray[0]);
  for (int i = 0; i < rowSize; i++)
  {
    pinMode(rowToPinArray[i], INPUT_PULLUP);
  }
}

// Check a pin at given colID and rowID
bool checkPin(int colID, int rowID)
{
  int col = colToPinArray[colID];
  int row = rowToPinArray[rowID];
  digitalWrite(col, LOW);
  bool state = digitalRead(row);
  digitalWrite(col, HIGH);
  return !state;
}

void loop()
{
  for (int i = 0; i < keyCount; i++)
  {
    int currentState = ketStates[i][0];
    int change = ketStates[i][1];
    int col = (keyMatrixMap[i][1]);
    int row = (keyMatrixMap[i][2]);
    if (((checkPin(col, row)) && (currentState == 0)) && (change == 0))
    {
      // Pressed
      digitalWrite(10, HIGH);
      ketStates[i][0] = 1;
      ketStates[i][1] = 1;
    }
    if (((!checkPin(col, row)) && (currentState == 1)) && (change == 0))
    {
      // Released
      digitalWrite(10, LOW);
      ketStates[i][0] = 0;
      ketStates[i][1] = 1;
    }
  }
  delay(10);
}