
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

#include <avr/interrupt.h>
#include <avr/sleep.h>

bool ReedSW()
{
  return digitalRead(ReedSwInt);  // Debug 
//  return !digitalRead(ReedSwInt); //Final 
  
}
