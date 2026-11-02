// [ru] AlashTM1637: счётчик нажатий. Каждое нажатие кнопки на пине 4 (второй контакт на GND) добавляет единицу к числу на дисплее.
//      Для надёжных нажатий без дребезга используйте библиотеку AlashButton.
// [kk] AlashTM1637: басу санауышы. 4 пиндегі батырманы әр басқан сайын дисплейдегі санға бір қосылады.
//      Діріліссіз сенімді басу үшін AlashButton кітапханасын қолданыңыз.
// [en] AlashTM1637: a press counter. Every press of the button on pin 4 (the other contact to GND) adds one to the number on the display.
//      For reliable presses without bounce use the AlashButton library.
#include <AlashTM1637.h>

AlashTM1637 display(2, 3);
int count = 0;
bool wasDown = false;

void setup() {
  pinMode(4, INPUT_PULLUP);
  display.begin(4);
  display.showNumber(count, true);        // true: с ведущими нулями 0000
}

void loop() {
  bool down = digitalRead(4) == LOW;
  if (down && !wasDown) {
    delay(20);                            // простая защита от дребезга
    count = (count + 1) % 10000;
    display.showNumber(count, true);
  }
  wasDown = down;
}
