#ifndef __PS2_H__
#define __PS2_H__

#include <Arduino.h>

class PS2Mouse {
  public:
    PS2Mouse(int data, int clk);
    void write(uint8_t data);
    uint8_t read(void);
    void writeAndRead(uint8_t data);

    void begin(bool absolute);
    void getPosition(uint8_t &stat, int &x, int &y);
    void getAbsPosition(uint8_t &stat, int &x, int &y, int& z);

  private:
    int _ps2clk;
    int _ps2data;
    void golo(int pin);
    void gohi(int pin);
    int twos(uint8_t value, bool sign);
};

#endif // __PS_2_H__
