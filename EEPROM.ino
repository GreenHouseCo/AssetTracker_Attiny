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


#include "EEPROM.h"
static SEEPROM_saved EEPROM_saved;

SEEPROM_saved* getCache(){
  return &EEPROM_saved;
}
static void loadToCache(){
  uint8_t *p;
  int i;
  p = (uint8_t*)&EEPROM_saved;
  
  for(i=0;i<sizeof(EEPROM_saved);i++){
    *p = EEPROM.read(i);
    p++;
  }
  
}
static void saveToEEPROM(){
#if 1
  uint8_t *p;
  int i;
  p = (uint8_t*)&EEPROM_saved;
  for(i=0;i<sizeof(EEPROM_saved);i++){
    EEPROM.write(i,*p);
    p++;
  }
#endif

}


void factory_Reset(){

      SEEPROM_saved* pCache;
      loadToCache();
      pCache = getCache();
      pCache->timer_Time = 5;
      pCache->reedSwitch_Time = 1;
      pCache->active_Time = 1;
      pCache->sigmot.slope_Sensitivity = 20;
      pCache->sigmot.slope_number = 3;
      pCache->endcode = 0x99;
      memset(pCache->sigfox_ID, 0, sizeof(pCache->sigfox_ID));
      memset(pCache->sigfox_PAC, 0, sizeof(pCache->sigfox_PAC));
      saveToEEPROM();
      myWorkFlag.wakeupWdt=false;
      myWorkFlag.wakeupSW=false;
      myWorkFlag.wakeupAct=false; 
      myWorkFlag.InterruptFlag=false;
      myWorkFlag.ModeSet=0;
}
