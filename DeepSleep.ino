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
void sleepMode(){

  digitalWrite(ExtSensorPw, LOW);
  digitalWrite(BAT_MONI, LOW);
  digitalWrite(WIFI_Pw, LOW);
  digitalWrite(Sigfox_Pw, LOW);
  delay(100);
  Accel_Init();  
  delay(100);

  loadToCache();
  SEEPROM_saved* pCache;
  pCache = getCache();
  uint32_t WakeUpT =(uint32_t)pCache->timer_Time *(60);
  
//  Serial.println(WakeUpT);
  // double Timebuf = 1.2; // Before Time up watchdog time count reset
  // int counter = 0;
//  Serial.printf("myWorkFlag.wakeupAct0= %d \r\n",myWorkFlag.wakeupAct);
  if(ReedSW()==true)
  {
    WakeUpT=pCache->reedSwitch_Time*(60);
//    Serial.printf("reedSwitch_Time1= %d \r\n",pCache->reedSwitch_Time);
//    Serial.printf("reedSwitch_Time2= %d \r\n",WakeUpT);
  }
  else if(myWorkFlag.wakeupAct==true)
  {
    WakeUpT=pCache->active_Time*(60);
//    Serial.printf("active_Time1= %d \r\n",pCache->active_Time);
//    Serial.printf("active_Time2= %d \r\n",WakeUpT);
  }
  else
  {
    myWorkFlag.InterruptFlag=false;
  }
       
  Serial.printf("SLEEP START: %ld S \r\n",WakeUpT);
  delay(1000);
/*  
if(DebugP!=1)
{
    Serial.end();
    delay(100); 
}
*/
//  Serial.printf("myWorkFlag.wakeupAct1= %d \r\n",myWorkFlag.wakeupAct);
  myWorkFlag.wakeupAct=false;
  myWorkFlag.wakeupSW=false;
    
//  myWorkFlag.InterruptFlag=false;   //check intterrupt
  
  do
  {
    if(myWorkFlag.wakeupAct==true||myWorkFlag.wakeupSW==true)
    {
      myWorkFlag.InterruptFlag=true;
      break;
    }
    if(WakeUpT >= 32){
      dPrint("SLEEP 32s ");
      WakeUpT -= 32;
      dPrintln(WakeUpT);
      DeepSleepStart(RTC_PERIOD_CYC32768_gc);
    }else if(WakeUpT>=16){
      dPrint("SLEEP 16s ");
      WakeUpT -= 16;
      dPrintln(WakeUpT);
      DeepSleepStart(RTC_PERIOD_CYC16384_gc);
    }else if(WakeUpT>=8){
      dPrint("SLEEP 8s ");
      WakeUpT -= 8;
      dPrintln(WakeUpT);
      DeepSleepStart(RTC_PERIOD_CYC8192_gc);
    }else if(WakeUpT>=4){
      dPrint("SLEEP 4s ");
      WakeUpT -= 2;
      dPrintln(WakeUpT);
      DeepSleepStart(RTC_PERIOD_CYC4096_gc);
    }else if(WakeUpT>=2){
      dPrint("SLEEP 2s ");
      WakeUpT -= 2;
      dPrintln(WakeUpT);
      DeepSleepStart(RTC_PERIOD_CYC2048_gc);
    }
    else
    {
      dPrint("SLEEP 1s ");
      myWorkFlag.InterruptFlag=false;
      WakeUpT -= 1;
      DeepSleepStart(RTC_PERIOD_CYC1024_gc);
    }
    delay(1);
  }while(WakeUpT >0);
/*
if(DebugP!=1)
{
  CPU_CCP = (0xD8&0xFF); //reg Write Enable
  ADC0.CTRLA |=0x01;    //ADC ON
  CPU_CCP = 0x00;
}
*/
  delay(1000);
  CPU_CCP = (0xD8&0xFF); //reg Write Enable
  ADC0.CTRLA |=0x01;    //ADC ON
  CPU_CCP = 0x00;
  delay(1000); 
//  Serial.begin(115200);
//  delay(1000);
  Serial.println(F("\r\nWAKE UP!\r\n"));
//  Serial.printf("myWorkFlag.wakeupAct2= %d \r\n",myWorkFlag.wakeupAct);
  delay(1000);
//  myWorkFlag.ModeSet=3;
}

void DeepSleepStart(RTC_PERIOD_t time) 
{
    if(myWorkFlag.InterruptFlag==false)
    {
        attachInterrupt(digitalPinToInterrupt(STK8323_INT1), waking_upAcc, RISING);  
        attachInterrupt(digitalPinToInterrupt(ReedSwInt), waking_upSW, FALLING); 
    }
     
    PIT_INT_setup(time); //reset mode ????  

    delay(10);
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
 //   CPU_CCP = (0xD8&0xFF); //reg Write Enable
 //   ADC0.CTRLA |=0x01;    //ADC ON
 //   CPU_CCP = 0x00;
    Serial.print(""); //WHY????
//  delay(1);
            
}
void PIT_INT_setup(RTC_PERIOD_t ii) {
    // Set Period
  // 0=0ms, 1=8ms, 2=16ms, 3=31ms, 4=63ms, 5=125ms, 6=250ms
  // 7=500ms, 8=1sec, 9=2sec, 10=4sec, 11=8sec
  
  // if (ii > 13 ) {                        // Fix bug
  //   ii = 13;
  // }
  // ii <<= 3; 
  // Serial.println(F("\r\nWAKE UP!\r\n"));
  dPrintln( "-set CLK- PIT int" );
  CPU_CCP = CCP_IOREG_gc;
  CLKCTRL.XOSC32KCTRLA = 0x02; // Open the crystal oscillator 
  // Initialize RTC
  // Serial.print( "-set pit-" );
   /* Internal 1.024 kHz oscillator*/
  RTC.CLKSEL = RTC_CLKSEL_INT1K_gc;
//  RTC.DBGCTRL = RTC_DBGRUN_bm;                        // Run in debug: enabled
  RTC.PITINTCTRL = RTC_PI_bm;                         // Periodic Interrupt: enabled
  // RTC Clock Cycles 32768, enabled ie 1Hz interrupt
  RTC.PITCTRLA = ii | RTC_PITEN_bm; // RTC_PERIOD_CYC32768_gc 32s walk up || RTC_PERIOD_CYC8192_gc 8s

}

// Interrupt Service Routine called every n(s)
ISR(RTC_PIT_vect) {
  RTC.PITINTFLAGS = RTC_PI_bm;                        // Clear interrupt flag
  // counter++;
}

void waking_upAcc(){ 
  detachInterrupt(digitalPinToInterrupt(STK8323_INT1));   //detach interrupt
  myWorkFlag.wakeupAct=true;
  Serial.println("Wakeup Active");
}


void waking_upSW(){ 
  detachInterrupt(digitalPinToInterrupt(ReedSwInt));   //detach interrupt
  myWorkFlag.wakeupSW=true;
  Serial.println("Wakeup Reed Switch");
}
