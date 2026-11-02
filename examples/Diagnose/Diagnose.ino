// [ru] AlashTM1637: дисплей не светится? Скетч зажжёт все сегменты 8.8.8.8. на максимальной яркости
//      и подскажет, что проверить (провода CLK / DIO, питание).
// [kk] AlashTM1637: дисплей жанбай ма? Скетч барлық сегменттерді 8.8.8.8. максималды жарықтықта жағады
//      және нені тексеру керегін айтады (CLK / DIO сымдары, қуат).
// [en] AlashTM1637: the display does not light? The sketch lights all segments 8.8.8.8. at maximum brightness
//      and tells what to check (the CLK / DIO wires, power).
#include <AlashTM1637.h>

AlashTM1637 display(2, 3);

void setup() {
  Serial.begin(115200);
  delay(500);
  display.setLanguage(AlashTM1637::LANG_RU);   // LANG_KK: қазақша, LANG_EN: English
  display.begin();
  display.diagnose(Serial);
}

void loop() {}
