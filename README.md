# AlashTM1637

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для 4-разрядного семисегментного дисплея на **TM1637** (модули для часов, счётчиков, термометров): числа, дробные с точкой, время с двоеточием, буквы и бегущая строка. Работает на любых двух пинах. Сама определяет, что дисплей не подключён. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

```cpp
#include <AlashTM1637.h>
AlashTM1637 display(2, 3);       // CLK, DIO

void setup() { display.begin(); }
void loop() {
  display.showNumber(1234);      // число
  delay(1000);
  display.showFloat(23.5, 1);    // 23.5
  delay(1000);
}
```

### Что умеет
- **Числа:** `showNumber(-999...9999)` выравнивает вправо, с ведущими нулями и минусом; слишком большое показывает `----` и `ERR_OVERFLOW`.
- **Дробные:** `showFloat(23.57, 1)` ставит точку у нужного разряда, сама уменьшает знаки после запятой, если не помещается; `NAN` показывает `----`.
- **Время:** `showTime(12, 34, colon)` с двоеточием, которое можно мигать.
- **Текст:** `showText("HELP")`, `scrollText("HELLO 2026")`; таблица знаков для цифр и части букв.
- **Яркость** 0-7, выключение без стирания; `showRaw()` для своих сегментов.
- **Знает, подключён ли дисплей:** по подтверждению каждого байта; `errorText()` и `diagnose()` зажигают 8.8.8.8. и подсказывают, что проверить.
- Работает на **Arduino AVR, ESP32, ESP8266** на любых двух цифровых пинах.
- **4 и 6 разрядов:** `AlashTM1637 display(CLK, DIO, 6)`; странный порядок разрядов у 6-разрядных модулей учтён, при необходимости свой порядок задаёт `setDigitMap()`. `showTimeHMS(12, 34, 56)` показывает часы с секундами.

### Подключение
| Модуль TM1637 | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

Пины CLK и DIO любые цифровые. У 6-разрядных модулей разряды подключены не по порядку: создавайте `AlashTM1637 display(CLK, DIO, 6)`, порядок учтён. Если цифры всё равно идут вразнобой, запустите SixDigits и задайте `setDigitMap()`.

### Чем отличается от других библиотек
Мы посмотрели TM1637 (avishorp), TM1637Display, GyverTM1637 и TM1637 (Rob Tillaart):
- В них нет проверки подключения дисплея: при неправильных проводах ничего не происходит, и непонятно почему. У нас `begin()` возвращает false с объяснением.
- Дробные числа и минус с выравниванием приходится собирать самому через массив сегментов; у нас `showFloat()`.
- Время и бегущая строка разбросаны по разным примерам; у нас готовые методы.

### Примеры
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

### Тесты
Протокол проверяется на уровне фронтов CLK и DIO имитацией дисплея (старт, стоп, байты, подтверждения): команды 0x40, 0xC0, яркость; числа с выравниванием, минус, ведущие нули, дробные с точкой и округлением 9.99, время, текст, бегущая строка, отсутствие дисплея проверены без железа: `extras/test`.

Подробное руководство: [docs/WIKI.md](docs/WIKI.md).

---

## Қазақша

**TM1637** 4-разрядты жеті сегментті дисплейге (сағат, санауыш, термометр модульдері) арналған қарапайым кітапхана: сандар, нүктелі бөлшектер, қос нүктелі уақыт, әріптер және жүгіретін жол. Кез келген екі пинде жұмыс істейді. Тәуелділіксіз.

```cpp
#include <AlashTM1637.h>
AlashTM1637 display(2, 3);       // CLK, DIO

void setup() { display.begin(); }
void loop() {
  display.showNumber(1234);      // число
  delay(1000);
  display.showFloat(23.5, 1);    // 23.5
  delay(1000);
}
```

### Мүмкіндіктері
- **Сандар:** `showNumber(-999...9999)` оңға тегістейді; тым үлкен болса `----`.
- **Бөлшектер:** `showFloat(23.57, 1)` нүктені дұрыс разрядқа қояды.
- **Уақыт:** `showTime(12, 34, colon)` жыпылықтайтын қос нүктемен.
- **Мәтін:** `showText("HELP")`, `scrollText(...)`.
- **Жарықтық** 0-7, өшіру; `showRaw()`.
- **Дисплей қосылғанын біледі:** `errorText()` және `diagnose()`.
- **Arduino AVR, ESP32, ESP8266**, кез келген екі пин.
- **4 және 6 разряд:** `AlashTM1637 display(CLK, DIO, 6)`; 6 разрядты модульдердің ерекше реті ескерілген, қажет болса `setDigitMap()` өз ретіңізді береді. `showTimeHMS(12, 34, 56)` сағатты секундпен көрсетеді.

### Қосу
| TM1637 модулі | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

CLK және DIO кез келген цифрлық пиндер. 6 разрядты модульдерде разрядтар ретімен жалғанбаған: `AlashTM1637 display(CLK, DIO, 6)` жасаңыз, реті ескерілген. Сандар әлі де араласып жүрсе, SixDigits іске қосып, `setDigitMap()` беріңіз.

### Басқа кітапханалардан айырмашылығы
TM1637 (avishorp), TM1637Display, GyverTM1637 және TM1637 (Rob Tillaart) қарадық:
- Дисплей қосылғанын тексеру жоқ; бізде `begin()` түсіндірмемен false қайтарады.
- Бөлшек пен минусты өзің жинайсың; бізде `showFloat()`.
- Бізде дайын уақыт және жүгіретін жол.

### Мысалдар
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

### Тесттер
Хаттама CLK және DIO фронттары деңгейінде тексеріледі: командалар, сандар, минус, бөлшектер, уақыт, мәтін, дисплейдің болмауы: `extras/test`.

Толық нұсқаулық: [docs/WIKI.md](docs/WIKI.md).

---

## English

A simple library for the 4-digit seven-segment display on the **TM1637** (the modules used for clocks, counters, thermometers): numbers, decimals with a point, time with a colon, letters and scrolling text. Works on any two pins. Detects that no display is connected. No dependencies. Made for classes and Alash Electronics kits.

```cpp
#include <AlashTM1637.h>
AlashTM1637 display(2, 3);       // CLK, DIO

void setup() { display.begin(); }
void loop() {
  display.showNumber(1234);      // число
  delay(1000);
  display.showFloat(23.5, 1);    // 23.5
  delay(1000);
}
```

### Features
- **Numbers:** `showNumber(-999...9999)` right-aligns with leading zeros and a minus sign; too large shows `----` and `ERR_OVERFLOW`.
- **Decimals:** `showFloat(23.57, 1)` puts the point at the right digit and drops decimals itself when it does not fit; `NAN` shows `----`.
- **Time:** `showTime(12, 34, colon)` with a colon you can blink.
- **Text:** `showText("HELP")`, `scrollText("HELLO 2026")`; character table for digits and many letters.
- **Brightness** 0-7, switch off without clearing; `showRaw()` for custom segments.
- **Knows whether a display is connected:** by acknowledging each byte; `errorText()` and `diagnose()` light 8.8.8.8. and tell what to check.
- Works on **Arduino AVR, ESP32, ESP8266** with any two digital pins.
- **4 and 6 digits:** `AlashTM1637 display(CLK, DIO, 6)`; the odd digit order of 6-digit modules is handled, and `setDigitMap()` sets your own if needed. `showTimeHMS(12, 34, 56)` shows a clock with seconds.

### Wiring
| TM1637 module | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

CLK and DIO may be any digital pins. The digits of 6-digit modules are wired out of order: create `AlashTM1637 display(CLK, DIO, 6)` and the order is handled. If the digits still look scrambled, run SixDigits and give `setDigitMap()`.

### How it differs from other libraries
We looked at TM1637 (avishorp), TM1637Display, GyverTM1637 and TM1637 (Rob Tillaart):
- They do not check that a display is connected: with wrong wires nothing happens and it is unclear why. Our `begin()` returns false with an explanation.
- Decimals and aligned minus signs must be assembled by hand from a segment array; we have `showFloat()`.
- Time and scrolling text are scattered across examples; we have ready methods.

### Examples
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

### Tests
The protocol is checked at the level of CLK and DIO edges by a display simulator (start, stop, bytes, acknowledgements): commands 0x40, 0xC0, brightness; aligned numbers, minus, leading zeros, decimals with the point and 9.99 rounding, time, text, scrolling and a missing display are checked without hardware: `extras/test`.

Full guide: [docs/WIKI.md](docs/WIKI.md).

---

MIT License, Alash Electronics.
