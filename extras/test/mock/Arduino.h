// Имитация Arduino.h и дисплея TM1637 на уровне двух линий: декодер старта, стопа, байтов и подтверждений.
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#define OUTPUT 1
#define INPUT 0
#define INPUT_PULLUP 2
#define LOW 0
#define HIGH 1
using __FlashStringHelper = char;
#define F(x) ((const __FlashStringHelper *)(x))
inline uint32_t g_ms = 1000;
inline uint32_t millis() { return g_ms; }
inline void delay(uint32_t ms) { g_ms += ms; }
inline void delayMicroseconds(uint32_t) {}
struct Tm1637Sim {
  uint8_t clk = 2, dio = 3;
  uint8_t modeClk = INPUT, modeDio = INPUT, outClk = 0, outDio = 0;
  bool present = true;
  // состояние декодера
  bool inFrame = false; int bits = 0; uint8_t cur = 0; bool ackWindow = false; bool deviceAck = false;
  std::vector<std::vector<uint8_t>> frames; std::vector<uint8_t> frame;
  int lastClk = 1, lastDio = 1; int stops = 0, starts = 0;
  int lineClk() const { return modeClk == OUTPUT ? outClk : 1; }
  int lineDio() const { return (modeDio == OUTPUT ? outDio : 1) & (deviceAck ? 0 : 1); }
  void sync() {
    int c = lineClk(), d = lineDio();
    if (lastClk == 1 && c == 1 && lastDio == 1 && d == 0) { inFrame = true; bits = 0; cur = 0; frame.clear(); starts++; }
    else if (lastClk == 1 && c == 1 && lastDio == 0 && d == 1 && inFrame) { inFrame = false; stops++; frames.push_back(frame); }
    else if (lastClk == 0 && c == 1 && inFrame) {                 // фронт CLK: читаем бит или подтверждение
      if (bits < 8) { cur |= (d ? 1 : 0) << bits; bits++; if (bits == 8) { frame.push_back(cur); } }
      else { bits = 9; }
    } else if (lastClk == 1 && c == 0 && inFrame) {
      if (bits == 8) { deviceAck = present; ackWindow = true; }    // после восьмого бита дисплей прижимает DIO на девятом такте
      else if (bits == 9) { deviceAck = false; bits = 0; cur = 0; ackWindow = false; }
    }
    lastClk = c; lastDio = lineDio();
  }
};
inline Tm1637Sim g_sim;
inline void pinMode(uint8_t p, uint8_t m) { if (p == g_sim.clk) g_sim.modeClk = m; else if (p == g_sim.dio) g_sim.modeDio = m; g_sim.sync(); }
inline void digitalWrite(uint8_t p, uint8_t v) { if (p == g_sim.clk) g_sim.outClk = v; else if (p == g_sim.dio) g_sim.outDio = v; g_sim.sync(); }
inline int digitalRead(uint8_t p) { return p == g_sim.dio ? g_sim.lineDio() : g_sim.lineClk(); }
class Print {
 public:
  void print(const char *s) { printf("%s", s); }
  void print(int v) { printf("%d", v); }
  void println(const char *s = "") { printf("%s\n", s); }
  void println(int v) { printf("%d\n", v); }
};
