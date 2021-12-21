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

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

void SleepMode(uint8_t Settime)
{
  delay(Settime);
}
void sleepMode(){
  digitalWrite(ExtSensorPw, LOW);
  digitalWrite(BAT_MONI, LOW);
  digitalWrite(WIFI_Pw, LOW);
  digitalWrite(Sigfox_Pw, LOW);
  delay(100);
  Accel_Init();  
  delay(100);

  SEEPROM_saved* pCache;
  pCache = getCache();
  uint16_t WakeUpT =pCache->timer_Time*(60);
  double Timebuf = 1.2; // Before Time up watchdog time count reset

  if(ReedSW()==true)
  {
    WakeUpT=pCache->reedSwitch_Time*(60);
  }
  else if(myWorkFlag.wakeupAct==true)
  {
    WakeUpT=pCache->active_Time*(60);
  }
       
  Serial.printf("SLEEP START: %dMin",pCache->timer_Time*60);
  delay(100); 
  
//  myWorkFlag.InterruptFlag=false;   //check intterrupt
  
  do
  {
    if(myWorkFlag.wakeupAct==true|myWorkFlag.wakeupSW==true)
    {
      myWorkFlag.InterruptFlag=true;
      break;
    }
    if(WakeUpT > 11)
    {
      WakeUpT -= (8*Timebuf);
      DeepSleepStart(11);
    }
    else if(WakeUpT>=2)
    {
      WakeUpT -= Timebuf;
      DeepSleepStart(8);
    }
    else
    {
      myWorkFlag.InterruptFlag=false;
      WakeUpT -= Timebuf;
      DeepSleepStart(8);
    }
  }while(WakeUpT >0);
  
  myWorkFlag.wakeupAct==false;
  myWorkFlag.wakeupSW==false;
      
  delay(100); 
  Serial.println(F("\r\nWAKE UP!\r\n"));
  delay(10);
  myWorkFlag.ModeSet=1;
}

void DeepSleepStart(unsigned long t) 
{
    if(myWorkFlag.InterruptFlag==false)
    {
        attachInterrupt(digitalPinToInterrupt(STK8323_INT1), waking_upAcc, RISING);  
        attachInterrupt(digitalPinToInterrupt(ReedSwInt), waking_upSW, FALLING); 
    }
     
 //   delayWDT_setup(t); //reset mode ????  

 
 //   CPU_CCP = (0xD8&0xFF);
 //   USART0.CTRLA=0x00;
 //   CPU_CCP = (0xD8&0xFF);
 //   USART1.CTRLA=0x00;
 //   CPU_CCP = (0xD8&0xFF);
 //   TCA0.SPLIT.CTRLA&=0x00;
//    CPU_CCP = (0xD8&0xFF);
//    TCB0.CTRLA&=0x00;
      
    CPU_CCP = (0xD8&0xFF); //reg Write Enable
    ADC0.CTRLA &=0xFE;    //ADC OFF
    CPU_CCP = (0xD8&0xFF);
    SLPCTRL.CTRLA = 0x02<<1;  //SLEEP LOW POWR MODE 
    CPU_CCP = (0xD8&0xFF);  
    SLPCTRL.CTRLA|=0x01;      //SLEEP MODE Enable
    CPU_CCP = 0x00; 
    
    sleep_cpu();                       // goto Sleep 
  //program takes back from here after wake up
    sleep_disable();  //disable sleep
    CPU_CCP = (0xD8&0xFF); //reg Write Enable
    ADC0.CTRLA |=0x01;    //ADC ON
    CPU_CCP = 0x00;
            
}
void delayWDT_setup(unsigned int ii) {
    // Set Period
  // 0=0ms, 1=8ms, 2=16ms, 3=31ms, 4=63ms, 5=125ms, 6=250ms
  // 7=500ms, 8=1sec, 9=2sec, 10=4sec, 11=8sec
  
  if (ii > 11 ) {                        // Fix bug
    ii = 11;
  }
  ii &= 0x0F; 

   CPU_CCP = (0xD8&0xFF); //reg Write Enable
   WDT.STATUS =(0x00&0x0F); //watchdog time Write Enable
   CPU_CCP = (0xD8&0xFF);
   WDT.CTRLA = ii;  //watchdog time period set
   CPU_CCP = 0x00;

  }
ISR(WDT_vect) {                         // WDT TIME UP RUN 
}

void waking_upAcc(){ 
  detachInterrupt(digitalPinToInterrupt(STK8323_INT1));   //detach interrupt
  myWorkFlag.wakeupAct=true;
  Serial.print("Wakeup Active");
}


void waking_upSW(){ 
  detachInterrupt(digitalPinToInterrupt(ReedSwInt));   //detach interrupt
  myWorkFlag.wakeupSW=true;
  Serial.print("Wakeup Reed Switch");
}
