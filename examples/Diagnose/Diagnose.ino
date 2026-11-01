// AlashTM1637: дисплей не светится? Скетч зажжёт все сегменты 8.8.8.8. на максимальной яркости
// и подскажет, что проверить (провода CLK / DIO, питание).
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
