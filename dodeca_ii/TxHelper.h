/*
 * TELEXo Eurorack Module
 * (c) 2016 Brendon Cassidy
 * MIT License
 */
 
#ifndef TxHelper_h
#define TxHelper_h

// i2c
#include <i2c_t3.h>

#include "Arduino.h"

struct TxResponse {
  byte Command;
  byte Output;
  int Value;
};

struct TxIO {
  short Port;
  short Mode;
};

class TxHelper
{
  public:

    static TxResponse Parse(size_t len);
    static TxIO DecodeIO(int io);
    static float VOct2Frequency(int value);
    static unsigned long ConvertMs(unsigned long ms, short format);

  protected:
    
  private:
 

};

#endif