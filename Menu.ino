////////////////////////////////////////////////////////////////////////////////////////////////
/**
 @file    Menu.c 
 @author  Green House Si
 @date    V1.0.0      2021/07/26
 @note    CPU         ESP32_C3

 @author  Green House Tan
 @date    V2.0.0      2021/07/26
 @note    CPU         Attiny1626
 
          COMPILER    Arduino IDE
          DETAIL      Menu setting
*/
/////////////////////////////////////////////////////////////////////////////////////////////////


/********************************************************************************/
/*  Name:setting_Menu                                                           */
/*  Detail: Top setting menu                                                    */
/*  Argument:None                                                               */
/*  Return: None                                                                */
/********************************************************************************/
void setting_Menu(){  
  SEEPROM_saved *pCache;
  pCache = getCache();
  byte inByte;
  bool breakLoop = false;
  char buf[1];
    while(1){
      Serial.println("");
      Serial.printf("*GH ASSET TRACKING Ver %s*\r\n",DVERSION);
      Serial.printf("*SIGFOX ID: %s*\r\n",pCache->sigfox_ID);
      Serial.printf("*SIGFOX PAC: %s*\r\n",pCache->sigfox_PAC);
      Serial.printf(" TOP MENU \r\n");
      Serial.printf("1: Device Settings\r\n");
      Serial.printf("2: Factory Reset\r\n");
      Serial.printf("3: Get Sigfox Info\r\n");
      Serial.printf("0: Start\r\n");
      Serial.printf("\r\n%S", "Input Number: ");
      while(1){
            memset(buf, 0, sizeof(buf));
            getSerialData(buf, sizeof(buf),10);
            switch (atoi(buf)){
            case 1:
              Serial.print("\r\n\r\n Device Settings\r\n");
              device_Menu();
              breakLoop = true;
              break;
            case 2:
              Serial.print("\r\n\r\n Factory Reset\r\n");
              factory_Reset();
              breakLoop = true;
              break;
            case 3:
              Serial.print("\r\n\r\n Get Sigfox Info\r\n");
              sigfox_Get_ID(pCache->sigfox_ID);
              saveToEEPROM();
              sigfox_Get_PAC(pCache->sigfox_PAC);
              saveToEEPROM();
              break;
            case 0:
              Serial.println("\r\nSetting Finish!\r\n");
              breakLoop = true;
              break;
            default:
              Serial.print("Input Number: ");
              break;
            }
           if(breakLoop){
            if(atoi(buf) == 0){
              breakLoop = true;
            }else{
              breakLoop = false;
            }
             break;
           }
          delay(10);
      }
      if(breakLoop){
        break;
      }
      delay(10);
    }
}

/********************************************************************************/
/*  Name:device_Menu                                                            */
/*  Detail: Device setting menu                                                 */
/*  Argument:None                                                               */
/*  Return: None                                                                */
/********************************************************************************/

void device_Menu(){
  bool breakLoop = false;
  char buf[1];
  char buf5[5];
  char buf1[1];
  uint8_t waittime=10;
  SEEPROM_saved *pCache;
  pCache = getCache();
  while(1){
      Serial.println("\r\n");
      Serial.printf("1: Inactive Time: %d (Min)\r\n",pCache->timer_Time);
      Serial.printf("2: Reed Switch Time:%d (Min)\r\n",pCache->reedSwitch_Time);
      Serial.printf("3: Active Time: %d (Min)\r\n",pCache->active_Time);
      Serial.print("4: Motion Interrupt Setting\r\n");
      Serial.print("0: Back\r\n");
      Serial.print("Input Number: ");
      while(1){
              memset(buf, 0, sizeof(buf));
              getSerialData(buf, sizeof(buf),waittime);
              switch (atoi(buf)){
              case 1:
                Serial.println("Interval Timer Wake up Time (Minute: 0-43200)");
                Serial.print("Input Number: ");  
                memset(buf5, 0, sizeof(buf5));             
                getSerialData(buf5,sizeof(buf5),waittime);
                if(atoi(buf5)>43200){
                  Serial.println("Invalid Number");
                }else{
                  pCache->timer_Time = atoi(buf5);
                  saveToEEPROM();
                }
                breakLoop = true;
                break;
              case 2:
                Serial.println("Interval Reed Switch Wake up Time (Minute: 0-43200)");
                Serial.print("Input Number: ");
                memset(buf5, 0, sizeof(buf5)); 
                getSerialData(buf5,sizeof(buf5),waittime);
                if(atoi(buf5)>43200){
                  Serial.println("Invalid Number");
                }else{
                  pCache->reedSwitch_Time = atoi(buf5);
                  saveToEEPROM();
                }
                breakLoop = true;
                break;
              case 3:
                Serial.println("Interval Active Interrupt Wake up Time (Minute: 0-43200)");
                Serial.print("Input Number: ");
                memset(buf5, 0, sizeof(buf5)); 
                getSerialData(buf5,sizeof(buf5),waittime);
                if(atoi(buf5)>43200){
                  Serial.println("Invalid Number");
                }else{
                  pCache->active_Time = atoi(buf5);
                  saveToEEPROM();
                }
                breakLoop = true;
                break;
             case 4:
                Serial.print("\r\n\r\n significant motion Interrupt Setting\r\n");
                sigmot_Menu();
                breakLoop = true;
                break;
              case 0:
                breakLoop = true;
                break;
              default:
                 Serial.print("Input Number: ");
                break;
              }
          if(breakLoop){
            if(atoi(buf) == 0){
              breakLoop = true;
            }else{
              breakLoop = false;
            }
             break;
         }
         delay(10);
        }
    if(breakLoop){
      break;
    }
    delay(10);
  }
}

/********************************************************************************/
/*  Name:Asigmot_Menu                                                           */
/*  Detail: Acceleration sensor setting menu                                    */
/*  Argument:None                                                               */
/*  Return: None                                                                */
/********************************************************************************/
void sigmot_Menu(){
  
  bool breakLoop = false;
  char buf[1];
  char buf3[3];
  char buf1[1];
  uint8_t waittime=10;
  SEEPROM_saved *pCache;
  pCache = getCache();
  while(1){
      Serial.println("\r\n");
      Serial.printf("1: Slope Sensitivity:%d\r\n",pCache->sigmot.slope_Sensitivity);
      Serial.printf("2: Slope Samples Number:%d\r\n",pCache->sigmot.slope_number);
      Serial.print("0: Back\r\n");
      Serial.printf("Input Number: ");
      while(1){
              memset(buf, 0, sizeof(buf));
              getSerialData(buf, sizeof(buf),waittime);
              switch (atoi(buf)){
              case 1:
                Serial.println("Slope Sensitivity (0-255)");
                Serial.print("Input Number: ");
                memset(buf3, 0, sizeof(buf3)); 
                getSerialData(buf3,sizeof(buf3),waittime);
                if(atoi(buf3)>255){
                  Serial.println("Invalid Number");
                }else{
                  pCache->sigmot.slope_Sensitivity = atoi(buf3);
                  saveToEEPROM();
                }
                breakLoop = true;
                break;
              case 2:
                Serial.println("Slope Samples Number (0-3)");
                Serial.print("Input Number: ");
                memset(buf1, 0, sizeof(buf1)); 
                getSerialData(buf1,sizeof(buf1),waittime);
                if(atoi(buf1)>3){
                  Serial.println("Invalid Number");
                }else{
                  pCache->sigmot.slope_number = atoi(buf1);
                  saveToEEPROM();
                }
                breakLoop = true;
                break;
              case 0:
                breakLoop = true;
                break;
              default:
                 Serial.print("Input Number: ");
                break;
              }
          if(breakLoop){
            if(atoi(buf) == 0){
              breakLoop = true;
            }else{
              breakLoop = false;
            }
             break;
         }
         delay(10);
        }
    if(breakLoop){
      break;
    }
    delay(10);
  }
}

/********************************************************************************/
/*  Name: getSerialData                                                         */
/*  Detail: Get data from serial0                                               */
/*  Argument:                                                                   */
/*           *buf: pornter address buffer                                       */
/*           bufLength: buffer data length                                      */
/*  Return: buffer data                                                         */
/********************************************************************************/

void getSerialData(char *buf, uint8_t bufLength,uint16_t gettime){
  uint8_t count = 0;
  uint32_t result = 0;
  int inByte;
  memset(buf, 0, sizeof(buf));
  uint32_t TimeStart=NowTime();
  bool delaytime=true;
  while(delaytime){
    if (Serial.available()) { /* check RX */
        inByte = Serial.read();
        if(inByte>=48&&inByte<=57){
          if(count < bufLength){
            Serial.write(char(inByte));
            buf[count] = inByte;
            count++;
          }
        }
        if(count !=0 && char(inByte) == 0x08){
          Serial.write(char(inByte));
          count--;
          buf[count] = 0;
        }
        if(count!=0&&char(inByte) == 0x0D){
          Serial.println();
          break;
        }  
   }else 
   {
    delaytime=RtcDelay(gettime,TimeStart);
   }
  }
}
