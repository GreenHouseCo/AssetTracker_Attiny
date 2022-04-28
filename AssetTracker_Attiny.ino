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

#include <SoftwareSerial.h>
#include <Wire.h>
#include "EEPROM.h"

//--------Signal---------PIN Define--Device NO******Pin Assign
  #define DVERSION "2.0.2"
  #define ExtSensorPw     12    //    14  外部センサーの電源制御端子
  #define BAT_MONI        1     //    3   Batteryの電圧読み込む端子
  #define BAT_V_CHK       4     //    6   ReedSwitch読み込む端子
  #define ReedSwInt       13    //    15  ReedSwitch読み込む端子 //New board  => Pin 12
  
  #define WIFI_Pw         5     //    7   ESP8266の電源制御端子-ESP82 Pin 1
  #define TXD_WIFI        6     //    8   Uart TX端子-ESP82 Pin 12
  #define RXD_WIFI        7     //    9   Uart RX端子-ESP82 Pin 11
  
  #define Sigfox_Pw       16    //    19  ESP8266の電源制御端子-Sigfox Pin 21
  #define TXD_Sigfox      14    //    17  Uart TX端子-Sigfox Pin 14
  #define RXD_Sigfox      15    //    18  Uart RX端子-Sigfox Pin 15
  
  #define STK8323_INT1    2     //    4   STK8323PwのINTERUPT1端子
  #define STK8323_INT2    3     //    5   STK8323PwのINTERUPT2端子
  
  #define Sensor_SDA      8     //    10  I2C SDA端子
  #define Sensor_SCL      9     //    11  I2C SCL端子
//--------Signal---------PIN Define--Device NO******Pin Assign
  
SoftwareSerial softSerial(RXD_Sigfox, TXD_Sigfox);  // RX, TX

#define DebugP    0   // Debug Print Uart 
uint8_t MacPayload[12]={0};

static struct
{
  bool   wakeupWdt=false;
  bool   wakeupSW=false;
  bool   wakeupAct=false; 
  bool   InterruptFlag=false;
  uint8_t ModeSet=0;
} myWorkFlag;

typedef struct
{
  uint16_t  timer_Time;
  uint16_t  reedSwitch_Time;
  uint16_t  active_Time;
  uint8_t   endcode;
  struct 
  {
    uint8_t   slope_Sensitivity;
    uint8_t   slope_number;
  } sigmot;
  char sigfox_ID[10];
  char sigfox_PAC[20];
} SEEPROM_saved;

void setup() 
{ 
  Serial.begin(115200); //ESP TX,RX    
  SEEPROM_saved* pCache;
  loadToCache();
  pCache = getCache();
  if(pCache->endcode != 0x99){
    factory_Reset();
  }   
  // Serial.print("\r\n SLEEP : (Min) - ");//,pCache->timer_Time*60);
  // Serial.println(pCache->timer_Time);
  // Accel_Init();
  GpioInit();

  Serial.print("\r\n SLEEP : (Min) - ");//,pCache->timer_Time*60);
  Serial.println(pCache->timer_Time);
  Serial.println("----------RESET----------");
  delay(100);
}

void loop()
{
//  SEEPROM_saved* pCache;
//  pCache = getCache();
//  loadToCache();
//  sigfox_Get_Info(pCache->sigfox_ID,pCache->sigfox_PAC);
//  saveToEEPROM();
  switch(myWorkFlag.ModeSet)
  {
    case 0: //DEEP_SLEEP
      Serial.println("Menu Board On/Off Put:1/0 Timeout 10second");
      while(1)
      {
        char MenuFlag;
        getSerialData(&MenuFlag,1,10);
        if(MenuFlag=='1')
        {
          setting_Menu();
        }
        break;
      }
      myWorkFlag.ModeSet=1;
      break;
    case 1:
      MacPayLoad();
      break;
    case 2:
      sigfox_Send_PayLoad(MacPayload);
      break;
    case 3:
      myWorkFlag.ModeSet=1;
      // Serial.print(pCache->timer_Time);
      Serial.println("sleep mode");
      sleepMode();
      break;
    default :
      break;
  }

}
