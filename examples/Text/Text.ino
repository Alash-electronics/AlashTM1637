// [ru] AlashTM1637: слова и бегущая строка. Семисегментный дисплей умеет цифры и часть букв: A b C d E F G H I J L n o P r S t U y.
// [kk] AlashTM1637: сөздер және жүгіретін жол. Жеті сегментті дисплей цифрларды және кейбір әріптерді көрсете алады: A b C d E F G H I J L n o P r S t U y.
// [en] AlashTM1637: words and a scrolling text. A seven-segment display can show digits and some letters: A b C d E F G H I J L n o P r S t U y.
#include <AlashTM1637.h>

AlashTM1637 display(2, 3);

void setup() {
  display.begin(5);
}

void loop() {
  display.showText("HELP");               // до четырёх символов
  delay(1500);
  display.showText("Err1");
  delay(1500);
  display.scrollText("HELLO 2026", 250);  // бегущая строка
  display.showText("done");
  delay(1500);
}
