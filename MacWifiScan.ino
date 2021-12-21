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
/*black list start*/
const char *ssidBlackProc[] = {
  "_nomap"      //BMac 0
  "android",    //BMac 1
  "phone",      //BMac 2
  "samsung",    //BMac 3
  "huawei",     //BMac 4
  "sony",       //BMac 5

};
uint8_t BlackMacMax=5;
/*black list start*/

/*white list start*/
static const char *ssidWhiteProc[] = {
    "powernetwork",    //WMac 0
    "actiontec",      //WMac 1
    "arris",          //WMac 2
    "at&t",           //WMac 3
    "belkin",         //WMac 4
    "buffalo",        //WMac 5
    "calix",          //WMac 6
    "card-king",      //WMac 7
    "centurylink",    //WMac 8
    "cisco",          //WMac 9
    "corega",         //WMac 10
    "d-link",         //WMac 11
    "dynalink",       //WMac 12
    "elecom",         //WMac 13
    "fritz",          //WMac 14
    "fujitsu",        //WMac 15
    "hootoo",         //WMac 16
    "linksys",        //WMac 17
    "iodata",         //WMac 18
    "logitech",       //WMac 19
    "medialink",      //WMac 20
    "mikrotik",       //WMac 21
    "mitsubishi",     //WMac 22
    "netcomm",        //WMac 23
    "netgear",        //WMac 24
    "ntt",            //WMac 25
    "planex",         //WMac 26
    "sagemcom",       //WMac 27
    "synclayer",      //WMac 28
    "synology",       //WMac 29
    "technicolor",    //WMac 30
    "tenda",          //WMac 31
    "thomson",        //WMac 32
    "tp-link",        //WMac 33
    "ubee",           //WMac 34
    "ubiquiti",       //WMac 35
    "zyxel",          //WMac 36

};
 uint8_t WhiteMacMax=36;
 
 char Payloadbuf[10][64]={""};
 char RxMACPayLoad[10][12]={""}; 
 uint8_t RxIndex=0;
 uint8_t MacIndex=0;
 uint8_t WhiteMac1=99;
 uint8_t WhiteMac2=99;
 
//   uint8_t PayloadSend[12]={0};
void MacPayLoad()
{
  memset(MacPayload,0,sizeof(MacPayload));
  uint8_t starttime=millis();
  WIFI_ON();
  WIFI_RESET();
//  WIFI_SCAN();
//  delay(100);

//  PayLoad_Prosse();  
//    bool WhiteMacFlag=PayLoad_Prosse();
//    if(WhiteMacFlag){
//      Serial.print("WHITE LIST");
//      delay(100);
//    }
      
 for(uint8_t i=0; i<3;i++)
 {
//    WIFI_RESET();
    WIFI_SCAN();
    delay(100);
    bool WhiteMacFlag= false;
    WhiteMacFlag=PayLoad_Prosse();
    Serial.print(WhiteMacFlag);
    delay(100);
    if(WhiteMacFlag){
      break;
    }
 }
  WIFI_OFF();
  delay(500);
  Serial.print("StartSet");
  delay(100);
  uint8_t MSB_Bit=0;
  bool flagBit=true;
  uint8_t MacByte=0;
  uint8_t buff=0;//=(uint8_t *)calloc(12, sizeof(uint8_t));
  for(uint8_t i=0; i<2;i++)
  {
    int WhiteMac=0;;
    if(WhiteMac1<10&&i==0)
    {
      WhiteMac=WhiteMac1;
    }
    else if(WhiteMac2<10&&i==1)
    {
      WhiteMac=WhiteMac2;
    }
    for(uint8_t j=0; j<12;j++)
    {
      buff=ChartoInt(RxMACPayLoad[WhiteMac][j]);
      if(flagBit)
      {
        MSB_Bit=buff<<4&0xF0;
        flagBit=false;
      }
      else
      {
        MacPayload[MacByte]=buff|MSB_Bit&0xF0;
        flagBit=true;
        MacByte++;
      }
      if(MacByte>12)
      {
        break;
      }
    }
  }
  delay(200);
//  Serial.print(MacPayload);
  for(int i=0;i<12;i++)
  {
    Serial.print(MacPayload[i],HEX);
    delay(1);
  }
  delay(200);
  dPrintln("StopSet");
  WhiteMac1=99;
  WhiteMac2=99;
  myWorkFlag.ModeSet=2;

}



bool PayLoad_Prosse()
{

  // Data process
  char RxSSID[10][32]={""};
  char RxRSSI[10][4]={""};
  char RxMAC[10][17]={""};
  uint8_t BuffIndex=0;
  uint8_t WriteIndex=0;
  uint8_t MacWriteIndex=0;
  for(uint8_t i=0; i<=MacIndex;i++)
  {
    for(uint8_t j=0; j<=sizeof(Payloadbuf[i]);j++)
    {
      if(Payloadbuf[i][j]=='('&&Payloadbuf[i][j+1]=='"')
      {
        dPrintln("GET MAC START");
        dPrintln("SSID: ");
        BuffIndex=1;
        WriteIndex=0;
        j++;
        continue;
      }else if(Payloadbuf[i][j]=='"'&&Payloadbuf[i][j+1]==',')
      {
        dPrintln("");
        dPrintln("RSSI: ");
        BuffIndex=2;
        WriteIndex=0;
        j++;
        continue;
      }else if(Payloadbuf[i][j]==','&&Payloadbuf[i][j+1]=='"')
      {
        dPrintln("");
        dPrintln("MAC ADD: ");
        BuffIndex=3;
        WriteIndex=0;
        MacWriteIndex=0;
        j++;
        continue;
      }else if(Payloadbuf[i][j]=='"'&&Payloadbuf[i][j+1]==')')
      {
        dPrintln("");
        dPrintln("GET MAC END");
        BuffIndex=0;
        WriteIndex=0;
        MacWriteIndex=0;
        break;
      }
      switch(BuffIndex)
      {
          case 0:
            break;
          case 1:
            RxSSID[i][WriteIndex]=Payloadbuf[i][j];
            dPrint(RxSSID[i][WriteIndex]);
            WriteIndex++;
            break;
          case 2:
            RxRSSI[i][WriteIndex]=Payloadbuf[i][j];
            dPrint(RxRSSI[i][WriteIndex]);
            WriteIndex++;
            break;
          case 3:
            RxMAC[i][WriteIndex]=Payloadbuf[i][j];
            if(Payloadbuf[i][j]!=':')
            {
              RxMACPayLoad[i][MacWriteIndex]=Payloadbuf[i][j];
              dPrint(RxMACPayLoad[i][MacWriteIndex]);
              MacWriteIndex++;
            }
 //           dPrint(RxMAC[i][WriteIndex]);
            WriteIndex++;
            break;
          default :
            dPrintln("GET MAC ERROR");
            break;            
      }
    }
    BuffIndex=0;
    delay(100);
      
  }
  RxIndex=0;
  bool flagRssiWhite = false;
  bool flagRssiBlack = true;
  uint8_t MacRandom1=88;
  uint8_t MacRandom2=88;
  for(uint8_t i=0;i<MacIndex;i++)
  {
    for (uint8_t key_idx = 0; key_idx <=WhiteMacMax; key_idx++) {
      if((char *)RxSSID[i]=="0")
      {
        dPrintln("RxSSID Empty");
        break;
      }
      if (check_ssid_name(((char *)RxSSID[i]),(char *)ssidWhiteProc[key_idx])==false) {
        flagRssiBlack = false;
        if(WhiteMac1==99)
        {
          WhiteMac1=i;
          dPrintln("WhiteMac1"); 
          break;
        }
        else
        {
          int IntRSSI1=atoi(RxRSSI[WhiteMac1]);
//          Serial.print("IntRSSI1");
//          Serial.println(IntRSSI1);
          int8_t IntRSSI2=atoi(RxRSSI[i]);
//          Serial.print("IntRSSI2");
//          Serial.println(IntRSSI2);
          if(IntRSSI1<IntRSSI2)
          {
            WhiteMac2=WhiteMac1;
            WhiteMac1=i;
          }
          else
          {
            WhiteMac2=i;
          }
          dPrintln("WhiteMac2"); 
          break;
        }
      }
//                  Delay(10);
    }
    if(flagRssiBlack)
    {
       bool BlackListFlag=false;
       for (uint8_t key_idx = 0; key_idx <= BlackMacMax; key_idx++) {
        if (check_ssid_name(((char *)RxSSID[i]),(char *)ssidBlackProc[key_idx])==false) {
           BlackListFlag=true;
        }
        if(BlackListFlag)
        {
          dPrintln("BLACK LIST DELETE");
          continue;
        }
        else if(MacRandom1==88)
        {
          MacRandom1=i;
        }
        else
        {
          MacRandom2=i;
        }
        
     }
      
    }
    if(WhiteMac1!=99&&WhiteMac2!=99&&i>=(MacIndex-1))
    {
      dPrintln("WHITE LIST OK");
      flagRssiWhite=true;
      MacIndex=0;
      return true;
    }
    else if(i>(MacIndex))
    {
      if(WhiteMac1==99)
      {
        WhiteMac1=MacRandom1;        
      }
      if(WhiteMac2==99)
      {
        WhiteMac2=MacRandom2;
      }
      MacRandom1=88;
      MacRandom2=88;
    }
  }
  MacIndex=0;
  dPrintln("Pro END");
  delay(100);
  return false;

}

void WIFI_ON()
{
  dPrintln("WIFI ON");
  delay(1000);
  digitalWrite(WIFI_Pw,HIGH);
  delay(5000);
}

void WIFI_OFF()
{
  delay(1000);
  digitalWrite(WIFI_Pw,LOW);
  delay(100);
  dPrintln("WIFI OFF");
}

void WIFI_RESET()
{
  Serial.println("AT+RST");
  delay(100);
  Serial.println("AT+CWMODE=1");
  delay(1000);
  Serial.println("AT+CWLAPOPT=1,14"); //SSID,RSSI,MAC取得モード設定
  delay(1000);
}

void WIFI_SCAN()
  {
  bool PayloadFlag=false;
  Serial.println("AT+CWLAP"); //SSID,RSSI,MAC取得
  delay(1000);
  RxBufDelete();
  delay(100);
  uint32_t TimeStart=millis();
  bool delaytime=true;
  MacIndex=0;
  while(delaytime)
  {
    uint8_t BuffNow1=(uint8_t)Serial.available();
    if(BuffNow1>0)
    {
      char BuffNow=Serial.read();
 //     softSerial.print(BuffNow);
      if((RxIndex<sizeof(Payloadbuf[MacIndex]))&&!(PayloadFlag==false&&BuffNow=='O')&&MacIndex<10)
      {//        
        if(BuffNow=='+')
        {
          MacIndex++;
          RxIndex=0;
          PayloadFlag=true;
        }
        else if(BuffNow==')')
        {
          PayloadFlag=false;
        }else
        {
          Payloadbuf[MacIndex-1][RxIndex]=BuffNow;
        }
      }
      else
      {
        break;
      }
      RxIndex++;
    }
    else
    {
      delay(1);
      delaytime=RtcDelay(20,TimeStart);
    }
  }
}

void RxBufDelete()
{
  while(1)
  {
    if((int)Serial.available()>0)
    {
//      softSerial.print(Serial.read());
      Serial.read();
    }
    else
    {
      break;
    }
    delay(1);
  }
}
