// Тесты AlashTM1637 без железа: имитация дисплея читает настоящие фронты CLK / DIO, байты и подтверждения.
//   g++ -std=c++17 -Wall -I mock -I ../../src test_tm1637.cpp ../../src/AlashTM1637.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashTM1637.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static const uint8_t D[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
static void reset() { g_sim = Tm1637Sim(); }
// Последний кадр данных: байт 0xC0 и четыре сегмента
static bool segs(uint8_t out[4]) {
  for (int i = (int)g_sim.frames.size() - 1; i >= 0; i--)
    if (g_sim.frames[i].size() == 5 && g_sim.frames[i][0] == 0xC0) { memcpy(out, &g_sim.frames[i][1], 4); return true; }
  return false;
}
static bool is(uint8_t a, uint8_t b, uint8_t c, uint8_t d) { uint8_t s[4]; return segs(s) && s[0] == a && s[1] == b && s[2] == c && s[3] == d; }

int main() {
  // 1) протокол: команда 0x40, адрес 0xC0 с четырьмя байтами, яркость 0x88 | уровень; каждый байт подтверждён
  reset(); AlashTM1637 t(2, 3);
  CHECK(t.begin(4));
  CHECK(g_sim.frames.size() == 3);
  CHECK(g_sim.frames[0].size() == 1 && g_sim.frames[0][0] == 0x40);
  CHECK(g_sim.frames[1].size() == 5 && g_sim.frames[1][0] == 0xC0);
  CHECK(g_sim.frames[2].size() == 1 && g_sim.frames[2][0] == 0x8C);
  CHECK(g_sim.starts == 3 && g_sim.stops == 3);

  // 2) целые числа: выравнивание вправо, ведущие нули, минус
  t.showNumber(1234); CHECK(is(D[1], D[2], D[3], D[4]));
  t.showNumber(7);    CHECK(is(0, 0, 0, D[7]));
  t.showNumber(0);    CHECK(is(0, 0, 0, D[0]));
  t.showNumber(42);   CHECK(is(0, 0, D[4], D[2]));
  t.showNumber(42, true); CHECK(is(D[0], D[0], D[4], D[2]));
  t.showNumber(9999); CHECK(is(D[9], D[9], D[9], D[9]));
  t.showNumber(-5);   CHECK(is(0, 0, 0x40, D[5]));
  t.showNumber(-42);  CHECK(is(0, 0x40, D[4], D[2]));
  t.showNumber(-999); CHECK(is(0x40, D[9], D[9], D[9]));
  t.showNumber(-7, true); CHECK(is(0x40, D[0], D[0], D[7]));
  t.showNumber(10000); CHECK(is(0x40, 0x40, 0x40, 0x40) && t.error() == AlashTM1637::ERR_OVERFLOW);
  t.showNumber(-1000); CHECK(is(0x40, 0x40, 0x40, 0x40));
  t.showNumber(5);     CHECK(t.error() == AlashTM1637::ERR_OK);

  // 3) дробные: точка (бит 7) на нужном разряде
  t.showFloat(12.5f, 1);   CHECK(is(0, D[1], D[2] | 0x80, D[5]));
  t.showFloat(3.14159f, 2); CHECK(is(0, D[3] | 0x80, D[1], D[4]));
  t.showFloat(0.5f, 1);    CHECK(is(0, 0, D[0] | 0x80, D[5]));
  t.showFloat(25.0f, 0);   CHECK(is(0, 0, D[2], D[5]));
  t.showFloat(-3.07f, 2);  CHECK(is(0x40, D[3] | 0x80, D[0], D[7]));
  t.showFloat(-12.34f, 2); CHECK(is(0x40, D[1], D[2] | 0x80, D[3]));            // -12.3: два знака не помещаются, остаётся один
  t.showFloat(123.456f, 2); CHECK(is(D[1], D[2], D[3] | 0x80, D[5]));            // 123.5
  t.showFloat(9.99f, 1);   CHECK(is(0, D[1], D[0] | 0x80, D[0]));            // 9.99 округляется до 10.0
  t.showFloat(1234.0f, 1); CHECK(is(D[1], D[2], D[3], D[4]));
  t.showFloat(99999.0f, 1); CHECK(is(0x40, 0x40, 0x40, 0x40));
  t.showFloat(NAN);        CHECK(is(0x40, 0x40, 0x40, 0x40));

  // 4) время и двоеточие (бит 7 второго разряда)
  t.showTime(12, 34, true);  CHECK(is(D[1], D[2] | 0x80, D[3], D[4]));
  t.showTime(7, 5, false);   CHECK(is(D[0], D[7], D[0], D[5]));
  t.setColon(true);          CHECK(is(D[0], D[7] | 0x80, D[0], D[5]));

  // 5) текст и свои сегменты
  t.showText("HELP"); CHECK(is(0x76, 0x79, 0x38, 0x73));
  t.showText("Err");  CHECK(is(0x79, 0x50, 0x50, 0));
  t.showText("12-3"); CHECK(is(D[1], D[2], 0x40, D[3]));
  const uint8_t raw[4] = {0x01, 0x02, 0x04, 0x08}; t.showRaw(raw); CHECK(is(1, 2, 4, 8));
  t.setDigit(2, 0x55); CHECK(is(1, 2, 0x55, 8));
  t.clear(); CHECK(is(0, 0, 0, 0));
  CHECK(AlashTM1637::charToSegments('8') == 0x7F && AlashTM1637::charToSegments('a') == 0x77 && AlashTM1637::charToSegments('#') == 0);

  // 6) яркость и включение
  t.setBrightness(0); CHECK(g_sim.frames.back().size() == 1 && g_sim.frames.back()[0] == 0x88);
  t.setBrightness(7); CHECK(g_sim.frames.back()[0] == 0x8F);
  t.setBrightness(99); CHECK(g_sim.frames.back()[0] == 0x8F);
  t.display(false);   CHECK(g_sim.frames.back()[0] == 0x87);          // выключено (бит 3 = 0), яркость сохранена
  t.display(true);    CHECK(g_sim.frames.back()[0] == 0x8F);

  // 7) бегущая строка проходит через все положения
  reset(); AlashTM1637 sc(2, 3); sc.begin(); size_t before = g_sim.frames.size();
  sc.scrollText("AB", 10);
  CHECK(g_sim.frames.size() - before >= 5 * 3);                        // 6 шагов по 3 кадра

  // 8) нет ответа: ERR_NO_ACK; потом дисплей появился
  reset(); g_sim.present = false; AlashTM1637 n(2, 3);
  CHECK(!n.begin() && n.error() == AlashTM1637::ERR_NO_ACK);
  g_sim.present = true; n.showNumber(1); CHECK(n.error() == AlashTM1637::ERR_OK);

  // 9) другие пины
  reset(); g_sim.clk = 7; g_sim.dio = 8; AlashTM1637 o(7, 8); CHECK(o.begin()); o.showNumber(55); CHECK(is(0, 0, D[5], D[5]));

  // 10) диагностика
  reset(); AlashTM1637 g(2, 3); g.begin(); g.setLanguage(AlashTM1637::LANG_KK); const char *kk = (const char *)g.errorText();
  g.setLanguage(AlashTM1637::LANG_EN); g.diagnose(*(new Print));
  CHECK(is(0xFF, 0xFF, 0xFF, 0xFF) && kk != nullptr);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
