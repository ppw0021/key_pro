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
  {16, 3, 3}
};

//Power pins
int colToPinArray[] = {9, 8, 7, 6};

//Reading pins
int rowToPinArray[] = {2, 3, 4, 5};

void setup() {
  Serial.begin(115200);
  int colSize = sizeof(colToPinArray);
  for (int i = 0; i < colSize; i++)
  {
    pinMode(colToPinArray[i], OUTPUT);
    digitalWrite(colToPinArray[i], HIGH);
  }

  int rowSize = sizeof(rowToPinArray);
  for (int i = 0; i < rowSize; i++)
  {
    pinMode(rowToPinArray[i], INPUT_PULLUP);
  }

  delay(100);
}

int matrixSize = sizeof(keyMatrixMap) / sizeof(keyMatrixMap[0]);

void loop() {
  for (int i = 0; i < matrixSize; i++) {
    int col = keyMatrixMap[i][1];
    int row = keyMatrixMap[i][2];

    bool state = checkPin(colToPinArray[col], rowToPinArray[row]);
    // Serial.print(colToPinArray[col]);
    // Serial.print(" ");
    // Serial.print(rowToPinArray[row]);
    // Serial.println();
    if (!state)
    {
      Serial.println(keyMatrixMap[i][0]);
    }
  }
  // Serial.println(checkPin(colToPinArray[0], rowToPinArray[0]));
    delay(50);
}

bool checkPin(int col, int row)
{
  digitalWrite(col, LOW);
  bool state = digitalRead(row);
  digitalWrite(col, HIGH);
  return state;
}
