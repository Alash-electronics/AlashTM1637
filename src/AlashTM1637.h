// AlashTM1637: 4-разрядный семисегментный дисплей на TM1637 (часы, счётчики, термометр): числа, дробные, время, буквы, яркость.
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashTM1637
#ifndef ALASH_TM1637_H
#define ALASH_TM1637_H

#include <Arduino.h>

class AlashTM1637 {
 public:
  enum Error : uint8_t {
    ERR_OK = 0,
    ERR_NO_ACK,       // дисплей не отвечает: провода CLK / DIO, питание
    ERR_OVERFLOW      // число не помещается в 4 разряда
  };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  AlashTM1637(uint8_t clkPin, uint8_t dioPin) : _clk(clkPin), _dio(dioPin) {}

  // begin() настраивает пины и включает дисплей. Возвращает false, если дисплей не ответил (ERR_NO_ACK).
  bool begin(uint8_t brightness = 4);

  // ---- Вывод --------------------------------------------------------------
  void showNumber(int32_t value, bool leadingZeros = false);   // целое -999...9999, выравнивание вправо
  void showFloat(float value, uint8_t decimals = 1);           // 12.5, -3.07: точка на нужном разряде
  void showTime(uint8_t hours, uint8_t minutes, bool colon = true);   // 12:34 (двоеточие есть на модулях для часов)
  void showText(const char *text);                             // до 4 символов: цифры, буквы A b C d E F H L P S U и другие, '-', '_'
  void scrollText(const char *text, uint16_t stepMs = 300);    // бегущая строка (блокирует на время прокрутки)
  void showRaw(const uint8_t *segments4);                      // свои сегменты: биты a b c d e f g dp = 0..7
  void setDigit(uint8_t position, uint8_t segments);           // один разряд 0..3
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

  void setBitDelayUs(uint8_t us) { _delay = us; }              // пауза между фронтами (по умолчанию 3 мкс; увеличьте при длинных проводах)

 private:
  uint8_t _clk, _dio;
  uint8_t _seg[4] = {0, 0, 0, 0};
  uint8_t _bright = 4;
  bool _on = true, _colon = false, _started = false;
  uint8_t _delay = 3;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;

  void update();
  void render(uint32_t n, uint8_t width, int8_t dp, bool neg);
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
