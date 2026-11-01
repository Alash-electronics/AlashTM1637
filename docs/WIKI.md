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

Пины CLK и DIO любые цифровые. Модули с 6 разрядами и с большим числом разрядов нумеруют разряды по-своему, эта версия поддерживает 4 разряда.

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

### Справочник по методам
| Метод | Что делает |
|---|---|
| `begin(brightness)` | Настроить пины, включить дисплей (false, если не отвечает) |
| `showNumber(n, leadingZeros)` | Целое -999...9999 |
| `showFloat(x, decimals)` | Дробное с точкой |
| `showTime(h, m, colon)` | Время 12:34 |
| `showText(s) / scrollText(s, ms)` | Слово до 4 символов / бегущая строка |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Свои сегменты |
| `setBrightness(0..7) / display(on)` | Яркость / вкл-выкл |
| `clear()` | Погасить |
| `diagnose(Serial)` | Зажечь 8.8.8.8. и подсказать |

### Если что-то не работает
**Дисплей не светится**

Проверьте VCC и GND, не перепутаны ли CLK и DIO. Запустите пример Diagnose: он зажжёт все сегменты. Если ответ есть, но темно, поднимите `setBrightness(7)`.

**Двоеточие не горит**

Двоеточие есть не на всех модулях. На модулях для часов оно подключено к точке второго разряда; на остальных его нет.

**Цифры идут в обратном порядке**

Это модуль с другим порядком разрядов. Выведите число функцией showRaw() с перестановкой или напишите нам об этой плате.

### Самопроверка
В библиотеке есть `diagnose(Serial)`: она печатает, что видит плата, и подсказывает, что проверить. Язык подсказок: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Примеры
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`

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

CLK және DIO кез келген цифрлық пин. Бұл нұсқа 4 разрядты қолдайды.

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

### Әдістер анықтамасы
| Әдіс | Не істейді |
|---|---|
| `begin(brightness)` | Пиндерді баптау |
| `showNumber(n, leadingZeros)` | Бүтін сан |
| `showFloat(x, decimals)` | Нүктелі бөлшек |
| `showTime(h, m, colon)` | Уақыт |
| `showText(s) / scrollText(s, ms)` | Сөз / жүгіретін жол |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Өз сегменттері |
| `setBrightness(0..7) / display(on)` | Жарықтық / қосу-өшіру |
| `clear()` | Өшіру |
| `diagnose(Serial)` | 8.8.8.8. жағу |

### Бірдеңе жұмыс істемесе
**Дисплей жанбайды**

VCC мен GND, CLK мен DIO шатыспағанын тексеріңіз. Diagnose мысалын іске қосыңыз.

**Қос нүкте жанбайды**

Қос нүкте барлық модульде жоқ. Сағат модульдерінде ғана бар.

**Сандар кері ретпен**

Бұл разряд реті басқа модуль.

### Өзін-өзі тексеру
Кітапханада `diagnose(Serial)` бар: тақта не көргенін басып шығарады және нені тексеру керегін айтады. Кеңес тілі: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Мысалдар
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`

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

CLK and DIO can be any digital pins. 6-digit modules number their digits differently; this version supports 4 digits.

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

### Method reference
| Method | What it does |
|---|---|
| `begin(brightness)` | Set up pins, switch on (false if no answer) |
| `showNumber(n, leadingZeros)` | Integer -999...9999 |
| `showFloat(x, decimals)` | Decimal with a point |
| `showTime(h, m, colon)` | Time 12:34 |
| `showText(s) / scrollText(s, ms)` | Word up to 4 chars / scrolling text |
| `showRaw(seg[4]) / setDigit(pos, seg) / setColon(on)` | Custom segments |
| `setBrightness(0..7) / display(on)` | Brightness / on-off |
| `clear()` | Clear |
| `diagnose(Serial)` | Light 8.8.8.8. and give tips |

### If something does not work
**The display does not light**

Check VCC and GND and that CLK and DIO are not swapped. Run the Diagnose example: it lights all segments. If it answers but is dark, use `setBrightness(7)`.

**The colon does not light**

Not every module has a colon. Clock modules wire it to the dot of the second digit; others have none.

**Digits are in reverse order**

This is a module with a different digit order. Use showRaw() with a swap or tell us about the board.

### Self-check
The library has `diagnose(Serial)`: it prints what the board sees and tells what to check. Tips language: `setLanguage(LANG_RU / LANG_KK / LANG_EN)`.

### Examples
`Basic`, `Clock`, `Thermometer`, `Counter`, `Text`, `Diagnose`

---

MIT License, Alash Electronics. https://github.com/Alash-electronics/AlashTM1637
