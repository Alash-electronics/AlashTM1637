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

AlashTM1637::AlashTM1637(uint8_t clkPin, uint8_t dioPin, uint8_t digits) : _clk(clkPin), _dio(dioPin) {
  _digits = digits < 1 ? 1 : digits > 6 ? 6 : digits;
  static const uint8_t six[6] = {2, 1, 0, 5, 4, 3};   // 6-разрядные модули: слева направо адреса 2 1 0 5 4 3
  for (uint8_t i = 0; i < 6; i++) _map[i] = _digits == 6 ? six[i] : i;
}

void AlashTM1637::setDigitMap(const uint8_t *map) {
  for (uint8_t i = 0; i < _digits; i++) _map[i] = map[i] > 5 ? 5 : map[i];
  update();
}

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

// Три посылки: команда записи, адрес первого разряда и все разряды подряд, управление яркостью.
// Байты идут в порядке адресов микросхемы, поэтому сначала раскладываем разряды по карте _map.
void AlashTM1637::update() {
  if (!_started) begin(_bright);
  bool ok = true;
  uint8_t out[6] = {0, 0, 0, 0, 0, 0};
  uint8_t last = 0;
  for (uint8_t i = 0; i < _digits; i++) {
    uint8_t dot = ((i == 1 && _colon) || (_dots >> i & 1)) ? 0x80 : 0;
    out[_map[i]] = _seg[i] | dot;
    if (_map[i] > last) last = _map[i];
  }
  start(); ok &= writeByte(CMD_DATA_AUTO); stop();
  start();
  ok &= writeByte(CMD_ADDR);
  for (uint8_t i = 0; i <= last; i++) ok &= writeByte(out[i]);
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
  _dots = 0;
  update();
}

void AlashTM1637::setDigit(uint8_t position, uint8_t segments) {
  if (position >= _digits) return;
  _seg[position] = segments;
  update();
}

void AlashTM1637::setColon(bool on) {
  _colon = on;
  update();
}

void AlashTM1637::showRaw(const uint8_t *s) {
  memcpy(_seg, s, _digits);
  _dots = 0;
  update();
}

static uint8_t digitCount(uint32_t n) { uint8_t c = 1; while (n >= 10) { n /= 10; c++; } return c; }

// Раскладка числа по разрядам справа налево: width цифр (с ведущими нулями), точка у разряда dp (считая справа), минус сразу левее
void AlashTM1637::render(uint32_t n, uint8_t width, int8_t dp, bool neg) {
  memset(_seg, 0, _digits);
  for (uint8_t k = 0; k < _digits; k++) {
    uint8_t pos = _digits - 1 - k;
    if (k < width) {
      _seg[pos] = DIGITS[n % 10];
      n /= 10;
    } else if (neg && k == width) {
      _seg[pos] = 0x40;
    }
  }
  if (dp > 0 && dp < _digits) _seg[_digits - 1 - dp] |= 0x80;
  _colon = false;
  _dots = 0;
  update();
}

void AlashTM1637::dashes() {
  memset(_seg, 0x40, _digits);
  _colon = false;
  _dots = 0;
  update();
  _err = ERR_OVERFLOW;
}

int32_t AlashTM1637::maxValue() const {   // 9999 для 4 разрядов, 999999 для 6
  int32_t m = 1;
  for (uint8_t i = 0; i < _digits; i++) m *= 10;
  return m - 1;
}

void AlashTM1637::showNumber(int32_t value, bool leadingZeros) {
  int32_t mx = maxValue();
  if (value < -(mx / 10) || value > mx) {
    dashes();
    return;
  }
  bool neg = value < 0;
  uint32_t v = neg ? (uint32_t)(-value) : (uint32_t)value;
  uint8_t width = leadingZeros ? (neg ? _digits - 1 : _digits) : digitCount(v);
  render(v, width, 0, neg);
}

void AlashTM1637::showFloat(float value, uint8_t decimals) {
  if (isnan(value)) {
    memset(_seg, 0x40, _digits);                   // прочерки "----" для NAN (например, датчик не ответил), ошибкой не считаем
    _colon = false;
    _dots = 0;
    update();
    return;
  }
  bool neg = value < 0;
  float a = neg ? -value : value;
  if (decimals > 3) decimals = 3;
  const uint8_t cols = neg ? _digits - 1 : _digits;   // разрядов под цифры (один занимает минус)
  if (a >= (float)(maxValue() + 1)) { dashes(); return; }
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
  memset(_seg, 0, _digits);
  _seg[0] = DIGITS[h / 10]; _seg[1] = DIGITS[h % 10];
  _seg[2] = DIGITS[m / 10]; _seg[3] = DIGITS[m % 10];
  _colon = colon;
  _dots = 0;
  update();
}

// Для 6 разрядов: часы, минуты, секунды с точками после 2-го и 4-го разрядов. При меньшем числе разрядов: часы и минуты
void AlashTM1637::showTimeHMS(uint8_t h, uint8_t m, uint8_t s) {
  if (_digits < 6) { showTime(h, m, true); return; }
  h %= 100; m %= 100; s %= 100;
  _seg[0] = DIGITS[h / 10]; _seg[1] = DIGITS[h % 10];
  _seg[2] = DIGITS[m / 10]; _seg[3] = DIGITS[m % 10];
  _seg[4] = DIGITS[s / 10]; _seg[5] = DIGITS[s % 10];
  _colon = false;
  _dots = 0x02 | 0x08;
  update();
}

void AlashTM1637::showText(const char *text) {
  memset(_seg, 0, _digits);
  for (uint8_t i = 0; i < _digits && text[i]; i++) _seg[i] = charToSegments(text[i]);
  _colon = false;
  _dots = 0;
  update();
}

void AlashTM1637::scrollText(const char *text, uint16_t stepMs) {
  uint8_t n = strlen(text);
  for (int16_t shift = -(int16_t)(_digits - 1); shift <= (int16_t)n; shift++) {     // сначала строка въезжает, потом уезжает
    memset(_seg, 0, _digits);
    for (uint8_t i = 0; i < _digits; i++) {
      int16_t idx = shift + i;
      if (idx >= 0 && idx < n) _seg[i] = charToSegments(text[idx]);
    }
    _colon = false;
    _dots = 0;
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
      return T("Число не помещается в разряды дисплея (4 разряда: -999...9999, 6 разрядов: -99999...999999). Для 6-разрядного модуля создайте AlashTM1637 tm(CLK, DIO, 6)",
               "Сан дисплей разрядтарына сыймайды (4 разряд: -999...9999, 6 разряд: -99999...999999). 6 разрядты модуль үшін AlashTM1637 tm(CLK, DIO, 6) жасаңыз",
               "The number does not fit in the display digits (4 digits: -999...9999, 6 digits: -99999...999999). For a 6-digit module create AlashTM1637 tm(CLK, DIO, 6)");
  }
  return F("");
}

void AlashTM1637::diagnose(Print &out) {
  out.println(T("AlashTM1637: диагностика", "AlashTM1637: диагностика", "AlashTM1637: diagnostics"));
  out.print(T("Пины CLK / DIO: ", "CLK / DIO пиндері: ", "Pins CLK / DIO: "));
  out.print((int)_clk);
  out.print(F(" / "));
  out.println((int)_dio);
  out.print(T("Разрядов: ", "Разряд саны: ", "Digits: "));
  out.println((int)_digits);
  uint8_t all[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};        // все сегменты
  _bright = 7;
  showRaw(all);
  if (_err == ERR_OK) {
    out.println(T("Дисплей ответил. Сейчас на нём должны гореть все сегменты и точки (8.8.8.8.) с максимальной яркостью.",
                  "Дисплей жауап берді. Қазір онда барлық сегменттер мен нүктелер (8.8.8.8.) толық жарықпен жануы керек.",
                  "The display answered. It should now show every segment and dot (8.8.8.8.) at full brightness."));
    if (_digits != 4) out.println(T("Если у 6-разрядного модуля цифры идут не слева направо, запустите пример SixDigits и подберите setDigitMap().",
                                   "6 разрядты модульде сандар солдан оңға жүрмесе, SixDigits мысалын іске қосып, setDigitMap() таңдаңыз.",
                                   "If the digits of a 6-digit module do not run left to right, run the SixDigits example and pick setDigitMap()."));
    out.println(T("Если горят не все сегменты, проверьте пайку модуля. Если ничего не видно, но ответ есть, поднимите яркость setBrightness(7).",
                  "Барлық сегмент жанбаса, модульдің дәнекерін тексеріңіз.",
                  "If some segments stay dark, check the module soldering. If nothing is visible but it answered, raise setBrightness(7)."));
  } else {
    out.println(errorText());
  }
}
