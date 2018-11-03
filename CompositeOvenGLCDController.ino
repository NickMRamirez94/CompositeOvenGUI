#include <openGLCD.h>

byte opCode, x, y;
byte coord[2];
String message;

void serialEvent() {
  if (!digitalRead(2)) {
    opCode = Serial.read();
    switch (opCode) {
      case (1):
        message = Serial.readStringUntil(13);
        GLCD.print(message);
        break;
      case (2):
        Serial.readBytes(coord, 2);
        GLCD.CursorTo(coord[0], coord[1]);
        break;
      case (3):
        GLCD.ClearScreen();
    }
  }
}

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  GLCD.Init();
  GLCD.SelectFont(Iain5x7);
}

void loop() {
}
