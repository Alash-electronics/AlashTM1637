// AlashTM1637: самый простой пример. Четырёхразрядный дисплей считает от 0 до 9999.
//
// Подключение модуля TM1637: VCC к 5 В (или 3.3 В), GND к GND, CLK и DIO на любые два цифровых пина.
#include <AlashTM1637.h>

const uint8_t CLK_PIN = 2;
const uint8_t DIO_PIN = 3;
AlashTM1637 display(CLK_PIN, DIO_PIN);

void setup() {
  Serial.begin(115200);
  if (!display.begin(4)) {                // 4 = яркость 0..7
    Serial.println(display.errorText());  // объяснит, что проверить
    while (true) delay(1000);
  }
}

void loop() {
  for (int i = 0; i <= 9999; i += 7) {
    display.showNumber(i);                // число выравнивается вправо
    delay(30);
  }
}
