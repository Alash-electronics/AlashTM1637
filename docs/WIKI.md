# AlashTM1637: руководство / нұсқаулық / guide

[Русский](#русский) | [Қазақша](#қазақша) | [English](#english)

---

## Русский

Простая библиотека для 4-разрядного семисегментного дисплея на **TM1637** (модули для часов, счётчиков, термометров): числа, дробные с точкой, время с двоеточием, буквы и бегущая строка. Работает на любых двух пинах. Сама определяет, что дисплей не подключён. Без зависимостей. Сделана для уроков и наборов Alash Electronics.

### Установка
Arduino IDE: Скетч, Подключить библиотеку, Управлять библиотеками, найти **AlashTM1637** и нажать Установить. Или скачайте ZIP с GitHub: Скетч, Подключить библиотеку, Добавить .ZIP библиотеку.

### Платы
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 и другие, где работает Arduino-ядро.

### Подключение
| Модуль TM1637 | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

Пины CLK и DIO любые цифровые. У 6-разрядных модулей разряды подключены не по порядку: создавайте `AlashTM1637 display(CLK, DIO, 6)`, порядок учтён. Если цифры всё равно идут вразнобой, запустите SixDigits и задайте `setDigitMap()`.

### Быстрый старт
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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin(brightness)` | Настроить пины, включить дисплей (false, если не отвечает) |
| `showNumber(n, leadingZeros)` | Целое -999...9999 |
| `showFloat(x, decimals)` | Дробное с точкой |
| `showText(s) / scrollText(s, ms)` | Слово до 4 символов / бегущая строка |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Свои сегменты |
| `setBrightness(0..7) / display(on)` | Яркость / вкл-выкл |
| `clear()` | Погасить |
| `diagnose(Serial)` | Зажечь 8.8.8.8. и подсказать |
| `showTime(h, m, colon)` | Время 12:34 |
| `showTimeHMS(h, m, s)` | 6 разрядов: 12.34.56 |
| `AlashTM1637(clk, dio, digits)` | Число разрядов 4 или 6 |
| `setDigitMap(map)` | Своя раскладка разрядов |

### Если что-то не работает
**Дисплей не светится**

Проверьте VCC и GND, не перепутаны ли CLK и DIO. Запустите пример Diagnose: он зажжёт все сегменты. Если ответ есть, но темно, поднимите `setBrightness(7)`.

**Двоеточие не горит**

Двоеточие есть не на всех модулях. На модулях для часов оно подключено к точке второго разряда; на остальных его нет.

**Цифры идут в странном порядке (3 2 1 6 5 4)**

Это 6-разрядный модуль, а объект создан на 4 разряда. Напишите `AlashTM1637 display(CLK, DIO, 6)`. Если и так вразнобой, запустите SixDigits и задайте `setDigitMap()`.

**Число не помещается, горит ----**

4 разряда: -999...9999; 6 разрядов: -99999...999999. Для 6-разрядного модуля создайте объект с шестью разрядами.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

---

## Қазақша

**TM1637** 4-разрядты жеті сегментті дисплейге (сағат, санауыш, термометр модульдері) арналған қарапайым кітапхана: сандар, нүктелі бөлшектер, қос нүктелі уақыт, әріптер және жүгіретін жол. Кез келген екі пинде жұмыс істейді. Тәуелділіксіз.

### Орнату
Arduino IDE: Sketch, Include Library, Manage Libraries, **AlashTM1637** табып, Install басыңыз. Немесе GitHub-тан ZIP жүктеп, Sketch, Include Library, Add .ZIP Library.

### Тақталар
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 және Arduino ядросы бар басқалар.

### Қосу
| TM1637 модулі | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

CLK және DIO кез келген цифрлық пиндер. 6 разрядты модульдерде разрядтар ретімен жалғанбаған: `AlashTM1637 display(CLK, DIO, 6)` жасаңыз, реті ескерілген. Сандар әлі де араласып жүрсе, SixDigits іске қосып, `setDigitMap()` беріңіз.

### Жылдам бастау
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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin(brightness)` | Пиндерді баптау |
| `showNumber(n, leadingZeros)` | Бүтін сан |
| `showFloat(x, decimals)` | Нүктелі бөлшек |
| `showText(s) / scrollText(s, ms)` | Сөз / жүгіретін жол |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Өз сегменттері |
| `setBrightness(0..7) / display(on)` | Жарықтық / қосу-өшіру |
| `clear()` | Өшіру |
| `diagnose(Serial)` | 8.8.8.8. жағу |
| `showTime(h, m, colon)` | Уақыт |
| `showTimeHMS(h, m, s)` | 6 разряд: 12.34.56 |
| `AlashTM1637(clk, dio, digits)` | Разряд саны 4 не 6 |
| `setDigitMap(map)` | Өз разряд реті |

### Бірдеңе жұмыс істемесе
**Дисплей жанбайды**

VCC мен GND, CLK мен DIO шатыспағанын тексеріңіз. Diagnose мысалын іске қосыңыз.

**Қос нүкте жанбайды**

Қос нүкте барлық модульде жоқ. Сағат модульдерінде ғана бар.

**Сандар ерекше ретпен жүр (3 2 1 6 5 4)**

Бұл 6 разрядты модуль, ал нысан 4 разрядқа жасалған. `AlashTM1637 display(CLK, DIO, 6)` жазыңыз. Әлі араласса, SixDigits іске қосып, `setDigitMap()` беріңіз.

**Сан сыймайды, ---- жанады**

4 разряд: -999...9999; 6 разряд: -99999...999999. 6 разрядты модуль үшін нысанды алты разрядпен жасаңыз.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

---

## English

A simple library for the 4-digit seven-segment display on the **TM1637** (the modules used for clocks, counters, thermometers): numbers, decimals with a point, time with a colon, letters and scrolling text. Works on any two pins. Detects that no display is connected. No dependencies. Made for classes and Alash Electronics kits.

### Installation
Arduino IDE: Sketch, Include Library, Manage Libraries, search **AlashTM1637** and click Install. Or download the ZIP from GitHub: Sketch, Include Library, Add .ZIP Library.

### Boards
Arduino Uno / Nano / Mega (AVR), ESP32, ESP8266 and others with an Arduino core.

### Wiring
| TM1637 module | Uno / Nano | ESP32 |
|---|---|---|
| VCC | 5 В (или 3.3 В) | 3.3 В |
| GND | GND | GND |
| CLK | D2 | GPIO18 |
| DIO | D3 | GPIO19 |

CLK and DIO may be any digital pins. The digits of 6-digit modules are wired out of order: create `AlashTM1637 display(CLK, DIO, 6)` and the order is handled. If the digits still look scrambled, run SixDigits and give `setDigitMap()`.

### Quick start
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

### Method reference
| Method | What it does |
|---|---|
| `begin(brightness)` | Set up pins, switch on (false if no answer) |
| `showNumber(n, leadingZeros)` | Integer -999...9999 |
| `showFloat(x, decimals)` | Decimal with a point |
| `showText(s) / scrollText(s, ms)` | Word up to 4 chars / scrolling text |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Custom segments |
| `setBrightness(0..7) / display(on)` | Brightness / on-off |
| `clear()` | Clear |
| `diagnose(Serial)` | Light 8.8.8.8. and give tips |
| `showTime(h, m, colon)` | Time 12:34 |
| `showTimeHMS(h, m, s)` | 6 digits: 12.34.56 |
| `AlashTM1637(clk, dio, digits)` | Digit count 4 or 6 |
| `setDigitMap(map)` | Your own digit layout |

### If something does not work
**The display does not light**

Check VCC and GND and that CLK and DIO are not swapped. Run the Diagnose example: it lights all segments. If it answers but is dark, use `setBrightness(7)`.

**The colon does not light**

Not every module has a colon. Clock modules wire it to the dot of the second digit; others have none.

**Digits appear in an odd order (3 2 1 6 5 4)**

It is a 6-digit module but the object was made for 4. Write `AlashTM1637 display(CLK, DIO, 6)`. If still scrambled, run SixDigits and give `setDigitMap()`.

**The number does not fit, ---- is shown**

4 digits: -999...9999; 6 digits: -99999...999999. For a 6-digit module create the object with six digits.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`, `SixDigits`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashTM1637
