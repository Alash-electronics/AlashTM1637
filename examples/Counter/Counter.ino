// AlashTM1637: счётчик нажатий. Каждое нажатие кнопки на пине 4 (второй контакт на GND) добавляет единицу к числу на дисплее.
// Для надёжных нажатий без дребезга используйте библиотеку AlashButton.
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
