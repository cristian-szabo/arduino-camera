#include "ov7670.hpp"

OV7670::OV7670()
{
}

OV7670::~OV7670()
{
}

void OV7670::begin()
{
  OV7670::ErrorType error;
  String message;

  Serial.println("Initialise OV7670 Camera ...");
  
  error = setValue(COM7, COM7_VALUE_RESET); 
  message = to_string(error);
  Serial.println("Camera reset COM7: " + message);
}

OV7670::ErrorType OV7670::setValue(int reg, byte data)
{
  return writeData(reg, &data, 1);
}

OV7670::ErrorType OV7670::getValue(int reg, byte* data)
{
  return readData(reg, data, 1);
}

OV7670::ErrorType OV7670::writeData(int start, const byte* data, int size)
{
  int n, error;

  Wire.beginTransmission(OV7670_I2C_ADDRESS);

  n = Wire.write(start);
  
  if (n != 1)
  {
    return ErrorType::I2C_WRITE_START_ADDRESS;
  }
  
  n = Wire.write(data, size);

  if (n != size)
  {
    return ErrorType::I2C_WRITE_DATA;
  }
  
  error = Wire.endTransmission(true);

  if (error == 1) {
    return ErrorType::I2C_DATA_TOO_LONG;
  } else if (error == 2) {
    return ErrorType::I2C_NACK_TRANSMIT_ADDRESS;
  } else if (error == 3) {
    return ErrorType::I2C_NACK_TRANSMIT_DATA;
  } else if (error == 4) {
    return ErrorType::I2C_OTHER;
  }
  
  return ErrorType::I2C_OK;
}

OV7670::ErrorType OV7670::readData(int start, byte* buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  
  n = Wire.write(start);
  
  if (n != 1)
  {
    return ErrorType::I2C_READ_START_ADDRESS;
  }
  
  error = Wire.endTransmission(false);
  
  if (error == 1) {
    return ErrorType::I2C_DATA_TOO_LONG;
  } else if (error == 2) {
    return ErrorType::I2C_NACK_TRANSMIT_ADDRESS;
  } else if (error == 3) {
    return ErrorType::I2C_NACK_TRANSMIT_DATA;
  } else if (error == 4) {
    return ErrorType::I2C_OTHER;
  }
  
  Wire.requestFrom(OV7670_I2C_ADDRESS, size, true);
  
  i = 0;
  
  while(Wire.available() && i < size)
  {
    buffer[i++] = Wire.read();
  }
  
  if (i != size)
  {
    return ErrorType::I2C_READ_DATA_SIZE_MISMATCH;
  }
  
  return ErrorType::I2C_OK;
}

String OV7670::to_string(OV7670::ErrorType error)
{
  String result;
  
  switch(error)
  {
    case OV7670::ErrorType::I2C_OK:
      result = "I2C_OK";
    break;
    
    case OV7670::ErrorType::I2C_WRITE_START_ADDRESS:
      result = "I2C_ERROR_WRITING_START_ADDRESS";
    break;
    
    case OV7670::ErrorType::I2C_WRITE_DATA:
      result = "I2C_ERROR_WRITING_DATA";
    break;
    
    case OV7670::ErrorType::I2C_NACK_TRANSMIT_ADDRESS:
      result = "NACK_ON_TRANSMIT_OF_ADDRESS";
    break;
    
    case OV7670::ErrorType::I2C_NACK_TRANSMIT_DATA:
      result = "NACK_ON_TRANSMIT_OF_DATA";
    break;

    case OV7670::ErrorType::I2C_READ_START_ADDRESS:
      result = "I2C_READ_START_ADDRESS";
    break;
    
    case OV7670::ErrorType::I2C_READ_DATA_SIZE_MISMATCH:
      result = "I2C_READ_DATA_SIZE_MISMATCH";
    break;
    
    case OV7670::ErrorType::I2C_DATA_TOO_LONG:
      result = "I2C_DATA_TOO_LONG";
    break;
    
    case OV7670::ErrorType::I2C_OTHER:
      result = "I2C_OTHER_ERROR";
    break;
       
    default:
      result = "I2C_UNKNOWN_ERROR";
    break;
  }
 
  return result;  
}

