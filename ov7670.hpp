#pragma once

#include <Arduino.h>
#include <Wire.h>

#define OV7670_I2C_ADDRESS  0x21

#define COM7                0x12 
#define COM7_VALUE_RESET    0x80

class OV7670 
{
public:

  enum class ErrorType : byte
  {
    I2C_OK = 0,
    I2C_WRITE_START_ADDRESS = 11,
    I2C_WRITE_DATA = 22,
    I2C_NACK_TRANSMIT_ADDRESS = 33,
    I2C_NACK_TRANSMIT_DATA = 44,    
    I2C_READ_START_ADDRESS = 55,
    I2C_READ_DATA_SIZE_MISMATCH = 66,
    I2C_DATA_TOO_LONG = 77,
    I2C_OTHER = 99
  };

  OV7670();

  ~OV7670();

  void begin();

  ErrorType setValue(int reg, byte data);

  ErrorType getValue(int reg, byte* data);

private:

  ErrorType writeData(int start, const byte* data, int size);

  ErrorType readData(int start, byte* buffer, int size);

  String to_string(ErrorType error);

};

