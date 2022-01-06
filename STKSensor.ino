////////////////////////////////////////////////////////////////////////////////////////////////
/**
 @file    Menu.c 
 @author  Green House Si
 @date    V1.0.0      2021/07/26
 @note    CPU         ESP32_C3

 @author  Green House Tan
 @date    V2.0.0      2021/12/21
 @note    CPU         Attiny1626
 
          COMPILER    Arduino IDE
          DETAIL      Menu setting
*/
/////////////////////////////////////////////////////////////////////////////////////////////////
#include <Wire.h>

void Accel_Init(){
  SEEPROM_saved *pCache;
  pCache = getCache();
  pCache->sigmot.slope_Sensitivity&=0xFF;
  pCache->sigmot.slope_number&=0xFF;
  Wire.begin();
  delay(3000);
  i2c_write(0x0F,0x14,0xB6);
  delay(100);
  i2c_write(0x0F,0x21,0x0B);
  delay(100); 
  i2c_write(0x0F,0x09,0x0F);
  delay(100);
  i2c_write(0x0F,0x0B,0x77);
  delay(100);
  i2c_write(0x0F,0x27,pCache->sigmot.slope_number);
  delay(100); 
  i2c_write(0x0F,0x28,pCache->sigmot.slope_Sensitivity);
  delay(100); 
//  i2c_write(0x0F,0x2C,pCache->sigmot.slope_number);
//  delay(100); 
//  i2c_write(0x0F,0x2E,pCache->sigmot.slope_Sensitivity);
//  delay(100); 
  i2c_write(0x0F,0x16,0x07);
  delay(100);
  i2c_write(0x0F,0x19,0xF0); // INT1<>INT2 Board wrong => NOW INT1 ENABLE
  delay(100);
//  char buff=i2c_Read(0x0F,0x00);
//  delay(10);
//  Serial.print(buff);
//  delay(100);
  i2c_write(0x0F,0x11,0x5E );
  delay(100);
}

void i2c_write(int sensor_Addr,int register_Addr, int val)
  {
//    Wire.beginTransmission(1);   
 //   Serial.print(1);
//    Serial.print("\r\n");

    Wire.beginTransmission(sensor_Addr);   
//    Serial.print(sensor_Addr);
//    Serial.print("\r\n");
    
    Wire.write(register_Addr);
//    Serial.print(register_Addr);
 //   Serial.print("\r\n");
          
    Wire.write(val);
//    Serial.print(val);
//    Serial.print("\r\n");
    
    Wire.endTransmission();
//    Serial.print("endTransmission");
  }
 char i2c_Read(int sensor_Addr_Read, uint8_t register_Addr){
    Wire.beginTransmission(sensor_Addr_Read);
    Wire.write(register_Addr);
    Wire.endTransmission(); 
    Wire.requestFrom(sensor_Addr_Read,1);
    while(Wire.available())
    {
      return Wire.read();
    }
    return 0;
}
