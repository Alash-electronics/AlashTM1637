// MIT License. Alash Electronics.
#include "AlashTM1637.h"

#define T(ru, kk, en) (_lang == LANG_KK ? F(kk) : _lang == LANG_EN ? F(en) : F(ru))

static const uint8_t CMD_DATA_AUTO = 0x40;       // запись данных с автоувеличением адреса
static const uint8_t CMD_ADDR = 0xC0;            // адрес первого разряда
static const uint8_t CMD_DISPLAY = 0x80;         // управление дисплеем: 0x88 включить + яркость 0..7

// Цифры 0-9 и A-F (биты: a b c d e f g dp = 0..7)
static const uint8_t DIGITS[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

// ---- Нижний уровень: двухпроводной протокол TM1637 (похож на I2C без адресов) -----------------

void AlashTM1637::dioLow() { digitalWrite(_dio, LOW); pinMode(_dio, OUTPUT); }
void AlashTM1637::dioRelease() { pinMode(_dio, INPUT_PULLUP); }     // линия отпущена: подтяжка на модуле поднимает её в HIGH
void AlashTM1637::clkLow() { digitalWrite(_clk, LOW); pinMode(_clk, OUTPUT); }
void AlashTM1637::clkHigh() { pinMode(_clk, INPUT_PULLUP); }

void AlashTM1637::start() {
  clkHigh(); dioRelease(); delayMicroseconds(_delay);
  dioLow(); delayMicroseconds(_delay);
  clkLow(); delayMicroseconds(_delay);
}

void AlashTM1637::stop() {
  clkLow(); dioLow(); delayMicroseconds(_delay);
  clkHigh(); delayMicroseconds(_delay);
  dioRelease(); delayMicroseconds(_delay);
}

// Байт младшим битом вперёд, потом девятый такт: дисплей подтверждает приём, прижимая DIO к земле
bool AlashTM1637::writeByte(uint8_t b) {
  for (uint8_t i = 0; i < 8; i++) {
    clkLow(); delayMicroseconds(_delay);
    if (b & 1) dioRelease(); else dioLow();
    delayMicroseconds(_delay);
    clkHigh(); delayMicroseconds(_delay);
    b >>= 1;
  }
  clkLow(); dioRelease(); delayMicroseconds(_delay * 2);
  clkHigh(); delayMicroseconds(_delay);
  bool ack = digitalRead(_dio) == LOW;
  clkLow(); delayMicroseconds(_delay);
  return ack;
}

// ---- Вывод ------------------------------------------------------------------------------------

bool AlashTM1637::begin(uint8_t brightness) {
  _bright = brightness > 7 ? 7 : brightness;
  _started = true;
  _on = true;
  digitalWrite(_clk, LOW);
  digitalWrite(_dio, LOW);
  clkHigh();
  dioRelease();
  memset(_seg, 0, sizeof _seg);
  update();
  return _err == ERR_OK;
}

// Три посылки: команда записи, адрес с четырьмя разрядами, управление яркостью
void AlashTM1637::update() {
  if (!_started) begin(_bright);
  bool ok = true;
  start(); ok &= writeByte(CMD_DATA_AUTO); stop();
  start();
  ok &= writeByte(CMD_ADDR);
  for (uint8_t i = 0; i < 4; i++) ok &= writeByte(_seg[i] | ((i == 1 && _colon) ? 0x80 : 0));
  stop();
  start(); ok &= writeByte(CMD_DISPLAY | (_on ? 0x08 : 0x00) | _bright); stop();
  _err = ok ? ERR_OK : ERR_NO_ACK;
}

uint8_t AlashTM1637::charToSegments(char c) {
  if (c >= '0' && c <= '9') return DIGITS[c - '0'];
  switch (c) {
    case 'A': case 'a': return 0x77;
    case 'b': case 'B': return 0x7C;
    case 'C': return 0x39;
    case 'c': return 0x58;
    case 'd': case 'D': return 0x5E;
    case 'E': case 'e': return 0x79;
    case 'F': case 'f': return 0x71;
    case 'G': case 'g': return 0x3D;
    case 'H': return 0x76;
    case 'h': return 0x74;
    case 'I': case 'i': return 0x06;
    case 'J': case 'j': return 0x1E;
    case 'L': case 'l': return 0x38;
    case 'n': case 'N': return 0x54;
    case 'O': return 0x3F;
    case 'o': return 0x5C;
    case 'P': case 'p': return 0x73;
    case 'q': case 'Q': return 0x67;
    case 'r': case 'R': return 0x50;
    case 'S': case 's': return 0x6D;
    case 't': case 'T': return 0x78;
    case 'U': return 0x3E;
    case 'u': return 0x1C;
    case 'y': case 'Y': return 0x6E;
    case 'Z': case 'z': return 0x5B;
    case '-': return 0x40;
    case '_': return 0x08;
    case '=': return 0x48;
    case '*': return 0x63;                // знак градуса
    case '\'': return 0x02;
    case ' ': return 0x00;
    default: return 0x00;
  }
}

void AlashTM1637::clear() {
  memset(_seg, 0, sizeof _seg);
  _colon = false;
  update();
}

void AlashTM1637::setDigit(uint8_t position, uint8_t segments) {
  if (position > 3) return;
  _seg[position] = segments;
  update();
}

void AlashTM1637::setColon(bool on) {
  _colon = on;
  update();
}

void AlashTM1637::showRaw(const uint8_t *s) {
  memcpy(_seg, s, 4);
  update();
}

static uint8_t digitCount(uint32_t n) { uint8_t c = 1; while (n >= 10) { n /= 10; c++; } return c; }

// Раскладка числа по четырём разрядам справа налево: width цифр (с ведущими нулями), точка у разряда dp (считая справа), минус сразу левее
void AlashTM1637::render(uint32_t n, uint8_t width, int8_t dp, bool neg) {
  memset(_seg, 0, 4);
  for (uint8_t k = 0; k < 4; k++) {
    uint8_t pos = 3 - k;
    if (k < width) {
      _seg[pos] = DIGITS[n % 10];
      n /= 10;
    } else if (neg && k == width) {
      _seg[pos] = 0x40;
    }
  }
  if (dp > 0 && dp < 4) _seg[3 - dp] |= 0x80;
  _colon = false;
  update();
}

void AlashTM1637::dashes() {
  memset(_seg, 0x40, 4);
  _colon = false;
  update();
  _err = ERR_OVERFLOW;
}

void AlashTM1637::showNumber(int32_t value, bool leadingZeros) {
  if (value < -999 || value > 9999) {
    dashes();
    return;
  }
  bool neg = value < 0;
  uint32_t v = neg ? (uint32_t)(-value) : (uint32_t)value;
  uint8_t width = leadingZeros ? (neg ? 3 : 4) : digitCount(v);
  render(v, width, 0, neg);
}

void AlashTM1637::showFloat(float value, uint8_t decimals) {
  if (isnan(value)) {
    memset(_seg, 0x40, 4);                         // прочерки "----" для NAN (например, датчик не ответил), ошибкой не считаем
    _colon = false;
    update();
    return;
  }
  bool neg = value < 0;
  float a = neg ? -value : value;
  if (decimals > 3) decimals = 3;
  const uint8_t cols = neg ? 3 : 4;                 // разрядов под цифры (один занимает минус)
  if (a >= 10000.0f) { dashes(); return; }
  int8_t dec = decimals;
  uint32_t n = 0;
  while (true) {
    uint32_t scale = 1;
    for (int8_t i = 0; i < dec; i++) scale *= 10;
    n = (uint32_t)(a * scale + 0.5f);
    uint8_t width = digitCount(n);
    if (dec > 0 && width < (uint8_t)dec + 1) width = dec + 1;       // 0.5 это "0.5": нужна цифра перед точкой
    if (width <= cols) { render(n, width, dec, neg); return; }
    if (dec == 0) { dashes(); return; }
    dec--;                                           // не поместилось: меньше знаков после запятой
  }
}

void AlashTM1637::showTime(uint8_t h, uint8_t m, bool colon) {
  h %= 100; m %= 100;
  _seg[0] = DIGITS[h / 10]; _seg[1] = DIGITS[h % 10];
  _seg[2] = DIGITS[m / 10]; _seg[3] = DIGITS[m % 10];
  _colon = colon;
  update();
}

void AlashTM1637::showText(const char *text) {
  memset(_seg, 0, 4);
  for (uint8_t i = 0; i < 4 && text[i]; i++) _seg[i] = charToSegments(text[i]);
  _colon = false;
  update();
}

void AlashTM1637::scrollText(const char *text, uint16_t stepMs) {
  uint8_t n = strlen(text);
  for (int16_t shift = -3; shift <= (int16_t)n; shift++) {     // слева направо: сначала строка въезжает, потом уезжает
    memset(_seg, 0, 4);
    for (uint8_t i = 0; i < 4; i++) {
      int16_t idx = shift + i;
      if (idx >= 0 && idx < n) _seg[i] = charToSegments(text[idx]);
    }
    _colon = false;
    update();
    delay(stepMs);
  }
}

void AlashTM1637::setBrightness(uint8_t level) {
  _bright = level > 7 ? 7 : level;
  update();
}

void AlashTM1637::display(bool on) {
  _on = on;
  update();
}

// ---- Диагностика ------------------------------------------------------------------------------

const __FlashStringHelper *AlashTM1637::errorText() {
  switch (_err) {
    case ERR_OK: return F("OK");
    case ERR_NO_ACK:
      return T("Дисплей не отвечает: проверьте провода CLK и DIO (не перепутаны ли), питание VCC 3.3-5 В и GND. Если провода длинные, вызовите setBitDelayUs(10)",
               "Дисплей жауап бермейді: CLK және DIO сымдарын (шатыстырмағаныңызды), VCC 3.3-5 В қуатын және GND тексеріңіз. Сымдар ұзын болса, setBitDelayUs(10) шақырыңыз",
               "The display does not answer: check the CLK and DIO wires (not swapped), VCC 3.3-5 V power and GND. With long wires call setBitDelayUs(10)");
    case ERR_OVERFLOW:
      return T("Число не помещается в 4 разряда (допустимо -999...9999)", "Сан 4 разрядқа сыймайды (-999...9999 рұқсат)", "The number does not fit in 4 digits (allowed -999...9999)");
  }
  return F("");
}

void AlashTM1637::diagnose(Print &out) {
  out.println(T("AlashTM1637: диагностика", "AlashTM1637: диагностика", "AlashTM1637: diagnostics"));
  out.print(T("Пины CLK / DIO: ", "CLK / DIO пиндері: ", "Pins CLK / DIO: "));
  out.print((int)_clk);
  out.print(F(" / "));
  out.println((int)_dio);
  static const uint8_t all[4] = {0xFF, 0xFF, 0xFF, 0xFF};      // все сегменты
  _bright = 7;
  showRaw(all);
  if (_err == ERR_OK) {
    out.println(T("Дисплей ответил. Сейчас на нём должно гореть 8.8.8.8. с точками максимальной яркости.",
                  "Дисплей жауап берді. Қазір онда 8.8.8.8. толық жарықпен жануы керек.",
                  "The display answered. It should now show 8.8.8.8. at full brightness."));
    out.println(T("Если горят не все сегменты, проверьте пайку модуля. Если ничего не видно, но ответ есть, поднимите яркость setBrightness(7).",
                  "Барлық сегмент жанбаса, модульдің дәнекерін тексеріңіз.",
                  "If some segments stay dark, check the module soldering. If nothing is visible but it answered, raise setBrightness(7)."));
  } else {
    out.println(errorText());
  }
}
