// [ru] AlashTM1637: самый простой пример. Четырёхразрядный дисплей считает от 0 до 9999.
//
//      Подключение модуля TM1637: VCC к 5 В (или 3.3 В), GND к GND, CLK и DIO на любые два цифровых пина.
// [kk] AlashTM1637: ең қарапайым мысал. Төрт разрядты дисплей 0-ден 9999-ға дейін санайды.
//
//      TM1637 модулін қосу: VCC 5 В-қа (немесе 3.3 В), GND GND-ға, CLK және DIO кез келген екі сандық пинге.
// [en] AlashTM1637: the simplest example. A four-digit display counts from 0 to 9999.
//
//      TM1637 module wiring: VCC to 5 V (or 3.3 V), GND to GND, CLK and DIO to any two digital pins.
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
