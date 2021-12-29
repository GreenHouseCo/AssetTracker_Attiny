////////////////////////////////////////////////////////////////////////////////////////////////
/**
 @author  Green House Si
 @date    V1.0.0      2021/07/26
 @note    CPU         ESP32_C3

 @author  Green House Tan
 @date    V2.0.0      2021/12/21
 @note    CPU         Attiny1626
 
          COMPILER    Arduino IDE
*/
/////////////////////////////////////////////////////////////////////////////////////////////////

bool RtcDelay(uint8_t _delay_time,uint32_t _start_time)
{
  uint32_t _now_time = millis();
  if(_delay_time>=(_now_time-_start_time)/1000) //millis 単位 1 mili second
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint32_t NowTime()
{
  return millis();
}
