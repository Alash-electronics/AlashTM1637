// AlashTM1637: термометр. Температура с аналогового датчика LM35 (10 мВ на градус) на пине A0 выводится с десятыми долями.
// Если у вас другой датчик (DS18B20, AHT20), замените строку расчёта на чтение температуры из его библиотеки.
#include <AlashTM1637.h>

AlashTM1637 display(2, 3);

void setup() {
  display.begin(5);
}

void loop() {
  float volts = analogRead(A0) * 5.0 / 1023.0;
  float celsius = volts * 100.0;          // LM35: 10 мВ на градус
  display.showFloat(celsius, 1);          // например 23.5
  delay(500);
}
