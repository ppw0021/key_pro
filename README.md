# DIY Split Keyboard

## Overview

This project is a highly ergonomic and efficient **DIY split keyboard**, designed to enhance comfort and productivity. The keyboard is built using **three Arduinos**, with an **Arduino Leonardo** acting as the master controller, while the **left and right keypads** are each controlled by an **Arduino Pro Micro**. This setup ensures seamless communication between the components, allowing for a fully functional and customizable typing experience.

## Hardware & Architecture

### **Microcontrollers**

- **Arduino Leonardo** – Acts as the **master controller** and utilizes the **HID (Human Interface Device) library** to function as a keyboard.
- **Arduino Pro Micro (x2)** – One for each **left and right keypad**.

### **Communication Protocol**

- The **Leonardo** communicates with the **left and right keypads** using the **I2C protocol**.
- The **master** continuously polls the **slave devices** for input data, ensuring responsive keypress detection.

### **Key Matrix Design**

One of the challenges of this project was handling **more switches than available input pins**. This issue was solved by implementing a **key matrix**, which allows multiple keys to be scanned efficiently with fewer pins.

- **Rows and columns** were created by:
  - Connecting **Pin 1** of each key switch in a row together.
  - Connecting **Pin 2** of each key switch in a column together.
- This matrix design allows the detection of **30 switches** using only **11 input pins**.
- By activating one column at a time and scanning the rows, keypresses are efficiently detected.

## Software & Programming

- The **Arduino firmware** is written in **C++** using **PlatformIO** within **Visual Studio Code**.
- The **I2C protocol** is used for seamless communication between the microcontrollers.
- The **Arduino Leonardo** uses the **HID library** to emulate a keyboard and send keystrokes to a connected computer.
- Custom firmware ensures accurate key mapping and dynamic switching of inputs.

## Special Features

- **Alternative Shift Key** – Allows access to an **additional set of inputs** (e.g., numbers, symbols) by dynamically switching keys between different characters.
- **Customizable Key Mapping** – The firmware can be easily modified to accommodate different layouts or user preferences.
- **Compact & Ergonomic** – The split keyboard design reduces strain and improves typing comfort over long periods.

## Future Improvements

- Adding **RGB backlighting** for visual feedback and customization.
- Implementing **wireless communication** between the keypads and the master controller.
- Creating a **graphical user interface (GUI)** to allow users to configure key mappings easily.

---

This DIY split keyboard project is a powerful and customizable alternative to commercial ergonomic keyboards, offering great flexibility and an open-source approach to keyboard design!

## Code

### Master Arduino Leonardo Code
This code is the master controller, it processes information from each slave arduino device, and emulates keypresses. It is also where you can assign which each key does.
```cpp
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
```

### Left Slave Arduino Pro Micro
```cpp
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
```

### Right Slave Arduino Pro Micro
```cpp
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
```
