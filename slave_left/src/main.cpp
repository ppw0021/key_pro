#include <Arduino.h>

#include <Wire.h>
int command = 0;

// Ascii upper, colID, rowID
int keyMatrixMap[][3] = {
    {0, 0, 0},
    {1, 1, 0},
    {2, 2, 0},
    {3, 3, 0},
    {4, 4, 0},
    {5, 5, 0},
    {6, 0, 1},  // b
    {7, 1, 1},  // v
    {8, 2, 1},  // c
    {9, 3, 1}, // x
    {10, 4, 1}, // z
    {11, 5, 1}, // SHIFT
    {12, 0, 2}, // g
    {13, 1, 2}, // f
    {14, 2, 2}, // d
    {15, 3, 2}, // s
    {16, 4, 2}, // a
    {17, 5, 2},
    {18, 0, 3}, // t
    {19, 1, 3}, // r
    {20, 2, 3}, // e
    {21, 3, 3}, // w
    {22, 4, 3}, // q
    {23, 5, 3},
    {24, 0, 4},
    {25, 1, 4},
    {26, 2, 4},
    {27, 3, 4},
    {28, 4, 4},
    {29, 5, 4}}; //ESC

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
int colToPinArray[] = {4, 3, 2, 10, 11, 12};

// Reading pins (rowID)
int rowToPinArray[] = {9, 8, 7, 6, 5};

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
  for (int i = 0; i < keyCount; i++)
  {
    int pressedAsciiCode = keyMatrixMap[i][0];
    int releasedAsciiCode = keyMatrixMap[i][0];
    int currentState = ketStates[i][0];
    int change = ketStates[i][1];

    bool somethingSent = false;
    if ((currentState == 1) && (change == 1))
    {
      // Pressed
      Serial.println();
      Serial.print(pressedAsciiCode);
      Serial.print(" pressed");
      Serial.println();
      Wire.write(pressedAsciiCode);
      ketStates[i][1] = 0;
      somethingSent = true;
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
      somethingSent = true;
    }
    if (somethingSent != true)
    {
      Wire.write(255);
    }
  }
}

void setup()
{

  pinMode(13, OUTPUT);
  Wire.begin(5);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // Register request handler
  Serial.begin(115200);         // start serial for output

  // Set pinmodes for outputs
  int colSize = sizeof(colToPinArray) / sizeof(colToPinArray[0]);
  for (int i = 0; i < colSize; i++)
  {
    // CHANGED
    pinMode(colToPinArray[i], INPUT);
    // digitalWrite(colToPinArray[i], HIGH);
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
  pinMode(col, OUTPUT);
  digitalWrite(col, LOW);
  bool state = digitalRead(row);
  // digitalWrite(col, HIGH);
  pinMode(col, INPUT);
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
      digitalWrite(13, HIGH);
      ketStates[i][0] = 1;
      ketStates[i][1] = 1;
    }
    if (((!checkPin(col, row)) && (currentState == 1)) && (change == 0))
    {
      // Released
      digitalWrite(13, LOW);
      ketStates[i][0] = 0;
      ketStates[i][1] = 1;
    }
  }
  delay(10);
}