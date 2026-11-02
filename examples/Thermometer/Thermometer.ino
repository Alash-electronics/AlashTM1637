// [ru] AlashTM1637: термометр. Температура с аналогового датчика LM35 (10 мВ на градус) на пине A0 выводится с десятыми долями.
//      Если у вас другой датчик (DS18B20, AHT20), замените строку расчёта на чтение температуры из его библиотеки.
// [kk] AlashTM1637: термометр. A0 пініндегі LM35 аналогтық датчигінің температурасы (градусына 10 мВ) ондықтарымен шығады.
//      Басқа датчигіңіз болса (DS18B20, AHT20), есептеу жолын оның кітапханасынан температураны оқумен ауыстырыңыз.
// [en] AlashTM1637: a thermometer. The temperature from an analog LM35 sensor (10 mV per degree) on pin A0 is shown with tenths.
//      If you have another sensor (DS18B20, AHT20), replace the calculation line with reading the temperature from its library.
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
