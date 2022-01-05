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

void GpioInit()
{
  pinMode(ExtSensorPw, OUTPUT);
  pinMode(BAT_MONI, OUTPUT);
  pinMode(WIFI_Pw, OUTPUT);
  pinMode(Sigfox_Pw, OUTPUT);
//  pinMode(TXD_Sigfox, OUTPUT);
//  pinMode(TXD_WIFI, OUTPUT);
  
  pinMode(BAT_V_CHK, INPUT);
  pinMode(ReedSwInt, INPUT);
  pinMode(STK8323_INT1, INPUT);
  pinMode(STK8323_INT2, INPUT);
          
  digitalWrite(ExtSensorPw, LOW);
  digitalWrite(BAT_MONI, LOW);
  digitalWrite(WIFI_Pw, LOW);
  digitalWrite(Sigfox_Pw, LOW);
//  digitalWrite(STK8323_INT1, LOW);
//  digitalWrite(STK8323_INT2, LOW);
}

enum
{
  DEEP_SLEEP,
  SCAN_WIFI,
  SIGFOX_SEND
}WorkSet;
