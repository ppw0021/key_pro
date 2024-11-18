#include <Arduino.h>

#include <Wire.h>
int command = 0;

// Ascii upper, colID, rowID
int keyMatrixMap[][3] = {
    {30, 0, 0},  // F12
    {31, 1, 0},  // RIGHT CNTRL
    {32, 2, 0},  // OPTION
    {33, 3, 0},  // SWITCH
    {34, 4, 0},  // RIGHT ALT
    {35, 5, 0},  // SPACE
    {36, 0, 1},  // RIGHT SHIFT
    {37, 1, 1},  // RIGHT ARROW
    {38, 2, 1},  // ]
    {39, 3, 1},  // [
    {40, 4, 1},  // m
    {41, 5, 1},  // n
    {42, 0, 2},  // DEL
    {43, 1, 2},  // LEFT ARROW
    {44, 2, 2},  // l
    {45, 3, 2},  // k
    {46, 4, 2},  // j
    {47, 5, 2},  // h
    {48, 0, 3},  // BACKSPACE
    {49, 1, 3},  // p
    {50, 2, 3},  // o
    {51, 3, 3},  // i
    {52, 4, 3},  // u
    {53, 5, 3},  // y
    {54, 0, 4},  // F11
    {55, 1, 4},  // F10
    {56, 2, 4},  // F9
    {57, 3, 4},  // F8
    {58, 4, 4},  // F7
    {59, 5, 4}}; // F6

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
  Wire.begin(6);                // join i2c bus with address #4
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