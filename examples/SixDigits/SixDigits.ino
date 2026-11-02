// [ru] 6-разрядный модуль TM1637: AlashTM1637 display(CLK, DIO, 6). Часы 12.34.56, число до 999999.
//      Цифры должны идти слева направо: 1 2 3 4 5 6. Если порядок другой, задайте свою раскладку через setDigitMap().
// [kk] 6 разрядты TM1637 модулі: AlashTM1637 display(CLK, DIO, 6). Сағат 12.34.56, 999999-ға дейінгі сан.
//      Сандар солдан оңға жүруі керек: 1 2 3 4 5 6. Реті басқа болса, setDigitMap() арқылы өз ретіңізді беріңіз.
// [en] 6-digit TM1637 module: AlashTM1637 display(CLK, DIO, 6). A 12.34.56 clock, numbers up to 999999.
//      The digits must run left to right: 1 2 3 4 5 6. If the order differs, set your own layout with setDigitMap().
#include <AlashTM1637.h>

const int PIN_CLK = 2, PIN_DIO = 3;
AlashTM1637 display(PIN_CLK, PIN_DIO, 6);

// Раскладка по умолчанию для 6-разрядных модулей: слева направо адреса микросхемы 2 1 0 5 4 3.
// Если у вашего модуля цифры перепутаны, подберите числа и раскомментируйте:
// const uint8_t MY_MAP[6] = {0, 1, 2, 3, 4, 5};

void setup() {
  Serial.begin(115200);
  if (!display.begin(5)) {
    Serial.println(display.errorText());
    while (true) delay(1000);
  }
  // display.setDigitMap(MY_MAP);
  display.showNumber(123456);          // должно быть видно: 1 2 3 4 5 6
  delay(2000);
}

void loop() {
  static uint32_t seconds = 12UL * 3600 + 34 * 60 + 50;
  display.showTimeHMS(seconds / 3600 % 24, seconds / 60 % 60, seconds % 60);
  seconds++;
  delay(1000);
}
