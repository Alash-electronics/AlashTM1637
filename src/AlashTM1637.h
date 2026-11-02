// AlashTM1637: 4- и 6-разрядный семисегментный дисплей на TM1637 (часы, счётчики, термометр): числа, дробные, время, буквы, яркость.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashTM1637
#ifndef ALASH_TM1637_H
#define ALASH_TM1637_H

#include <Arduino.h>

class AlashTM1637 {
 public:
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_NO_ACK,       // дисплей не отвечает: провода CLK / DIO, питание
    ERR_OVERFLOW      // число не помещается в разряды дисплея
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  // digits: сколько разрядов у модуля (4 или 6; микросхема умеет до 6). У 6-разрядных модулей разряды подключены не по порядку:
  // по умолчанию ставится самая распространённая раскладка (адреса 2 1 0 5 4 3 слева направо). Другая раскладка: setDigitMap().
  AlashTM1637(uint8_t clkPin, uint8_t dioPin, uint8_t digits = 4);

  // begin() настраивает пины и включает дисплей. Возвращает false, если дисплей не ответил (ERR_NO_ACK).
  bool begin(uint8_t brightness = 4);

  // ---- Вывод --------------------------------------------------------------
  void showNumber(int32_t value, bool leadingZeros = false);   // целое (4 разряда: -999...9999, 6 разрядов: -99999...999999), вправо
  void showFloat(float value, uint8_t decimals = 1);           // 12.5, -3.07: точка на нужном разряде
  void showTime(uint8_t hours, uint8_t minutes, bool colon = true);   // 12:34 (двоеточие есть на модулях для часов)
  void showTimeHMS(uint8_t hours, uint8_t minutes, uint8_t seconds);   // 6 разрядов: 12.34.56 (точки после 2-го и 4-го разрядов)
  void showText(const char *text);                             // по числу разрядов: цифры, буквы A b C d E F H L P S U и другие, '-', '_'
  void scrollText(const char *text, uint16_t stepMs = 300);    // бегущая строка (блокирует на время прокрутки)
  void showRaw(const uint8_t *segments);                       // свои сегменты по числу разрядов: биты a b c d e f g dp = 0..7
  void setDigit(uint8_t position, uint8_t segments);           // один разряд, слева направо, с 0
  void setColon(bool on);                                      // двоеточие (точка после второго разряда)
  void clear();
  static uint8_t charToSegments(char c);                       // сегменты символа (для showRaw)

  // ---- Яркость --------------------------------------------------------------------
  void setBrightness(uint8_t level);                           // 0 (еле видно) ... 7 (максимум)
  void display(bool on);                                       // выключить / включить, не стирая содержимое

  // ---- Диагностика -----------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);                                   // выведет тест 8888 и подскажет, что проверить

  uint8_t digits() const { return _digits; }
  // Раскладка, если у вашего модуля разряды подключены иначе: map[i] = адрес микросхемы (0..5) для i-го разряда слева.
  // Проверка: запустите пример SixDigits, цифры должны идти 1 2 3 4 5 6 слева направо.
  void setDigitMap(const uint8_t *map);

  void setBitDelayUs(uint8_t us) { _delay = us; }              // пауза между фронтами (по умолчанию 3 мкс; увеличьте при длинных проводах)

 private:
  uint8_t _clk, _dio;
  uint8_t _digits = 4;
  uint8_t _seg[6] = {0, 0, 0, 0, 0, 0};
  uint8_t _map[6] = {0, 1, 2, 3, 4, 5};
  uint8_t _dots = 0;
  uint8_t _bright = 4;
  bool _on = true, _colon = false, _started = false;
  uint8_t _delay = 3;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;

  void update();
  void render(uint32_t n, uint8_t width, int8_t dp, bool neg);
  int32_t maxValue() const;
  void dashes();
  void start();
  void stop();
  bool writeByte(uint8_t b);
  void dioLow();
  void dioRelease();
  void clkLow();
  void clkHigh();
};

#endif  // ALASH_TM1637_H
