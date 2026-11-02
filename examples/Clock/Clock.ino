// [ru] AlashTM1637: часы с мигающим двоеточием. Время считается от millis() (после включения часы начинают с 12:00).
//      Для точного времени подключите часы DS3231 и библиотеку AlashRTC: rtc.hour(), rtc.minute().
// [kk] AlashTM1637: жыпылықтайтын қос нүктесі бар сағат. Уақыт millis() бойынша есептеледі (қосқаннан кейін сағат 12:00-ден басталады).
//      Дәл уақыт үшін DS3231 сағатын және AlashRTC кітапханасын қосыңыз: rtc.hour(), rtc.minute().
// [en] AlashTM1637: a clock with a blinking colon. The time is counted from millis() (after power-on the clock starts at 12:00).
//      For accurate time connect a DS3231 clock and the AlashRTC library: rtc.hour(), rtc.minute().
#include <AlashTM1637.h>

AlashTM1637 display(2, 3);
uint32_t startSeconds = 12UL * 3600UL;    // с какого времени начинаем, секунд от полуночи

void setup() {
  display.begin(3);
}

void loop() {
  uint32_t total = startSeconds + millis() / 1000;
  uint8_t h = (total / 3600) % 24;
  uint8_t m = (total / 60) % 60;
  bool colon = (millis() / 500) % 2 == 0; // двоеточие мигает раз в секунду
  display.showTime(h, m, colon);
  delay(100);
}
