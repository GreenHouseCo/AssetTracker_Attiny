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

bool sigfox_Send_PayLoad(uint8_t* PayloadSend)
{
  Serial1.begin(9600); 
  delay(100);
  Serial.println("Sigfox start");
  digitalWrite(Sigfox_Pw, HIGH);
  delay(2000);
//  char sigfoxBuf[36]={""};
  set_Payload(PayloadSend);
//  sprintf(sigfoxBuf ,"AT$SF=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r",
//                      PayloadSend[0],
//                      PayloadSend[1],
//                      PayloadSend[2],
//                      PayloadSend[3],
//                      PayloadSend[4],
//                      PayloadSend[5],
//                      PayloadSend[6],
//                      PayloadSend[7],
//                      PayloadSend[8],
//                      PayloadSend[9],
 //                     PayloadSend[10],
 //                     PayloadSend[11]);
//  Serial.print(sigfoxBuf);
//  delay(1000);

  Serial1.printf("AT$SF=%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
                     PayloadSend[0],
                     PayloadSend[1],
                     PayloadSend[2],
                     PayloadSend[3],
                     PayloadSend[4],
                     PayloadSend[5],
                     PayloadSend[6],
                     PayloadSend[7],
                     PayloadSend[8],
                     PayloadSend[9],
                     PayloadSend[10],
                      PayloadSend[11]);//sigfoxBuf); 

  bool Sendflag=false;
  uint32_t TimeStart=NowTime();
  bool delaytime=true;                       
  while(delaytime)
  {
    if (Serial1.available()) 
    { /* check RX */
        char RxReadC = Serial1.read();
        if(RxReadC=='O')
        {
            Serial.print(RxReadC);
            Sendflag=true;
        }
        else if((Sendflag=true)&&(RxReadC=='K'))
        {
            Serial.print(RxReadC);
            dPrintln("Send SigFox OK");
            break;
        }
        Serial.print("!");
    }
    delaytime=RtcDelay(20,TimeStart);
    Serial.print(".");
    delay(100);
  }
  
  digitalWrite(Sigfox_Pw, LOW);
  delay(200);
  Serial1.end();
  delay(200);
  myWorkFlag.ModeSet=3;

  return true;
}

void set_Payload(uint8_t *payload){
  uint8_t byte0 = payload[0]>>4;
  payload[0] = payload[0]<<4;
  payload[0] = payload[0]>>6;
  uint8_t byte1 = payload[6] | payload[0];
  payload[6] = payload[5];
  payload[5] = payload[4];
  payload[4] = payload[3];
  payload[3] = payload[2];
  payload[2] = payload[1];
  payload[1] = byte1;
  payload[0] = byte0;
//  myWorkFlag.bat = BatMeas();
  payload[0] = byte0 | (ReedSW()<<7) | (BatMeas() << 4);
}

void sigfox_Get_ID(char *sigfoxID){
  Serial1.begin(9600);
  digitalWrite(Sigfox_Pw, HIGH);
  delay(1000);
  Serial1.print("AT$I=10\r");
  sigfox_Get_Data(sigfoxID, 8, 5);
  Serial.print(sigfoxID);
  delay(100);
  digitalWrite(Sigfox_Pw, LOW);
  // delay(200);
  Serial1.end();
  delay(200); 
  // delay(100);
}

void sigfox_Get_PAC(char *sigfoxPAC){
  Serial1.begin(9600);
  digitalWrite(Sigfox_Pw, HIGH);
  delay(1000);
  Serial1.print("AT$I=11\r");
  sigfox_Get_Data(sigfoxPAC, 16, 5);
  Serial.print(sigfoxPAC); 
  delay(100);
  digitalWrite(Sigfox_Pw, LOW);
  Serial1.end();
  delay(200);
  // delay(100);
}

/********************************************************************************/
/*  Name: sigfox_Get_Data                                                       */
/*  Detail: get data from sigfox via serial0                                    */
/*  Argument:                                                                   */
/*           *data_Buf: pointer address buffer                                  */
/*           bufLength: buffer length                                           */
/*           waitingTime: communication waiting time                            */
/*  Return: None                                                                */
/********************************************************************************/
void sigfox_Get_Data(char *data_Buf, uint8_t bufLength, uint8_t waitingTime){
  
  int inByte;
  uint8_t count = 0;
  uint32_t TimeStart=NowTime();
  bool delaytime=true;
  while(delaytime){
    uint8_t Buffnow=Serial1.available();
    if (Buffnow) { /* check RX */
        inByte = Serial1.read();
//        Serial.print(inByte);
        if((inByte>=48&&inByte<=57)||(inByte>=65&&inByte<=70)||(inByte>=97&&inByte<=102)||inByte==0x4F||inByte==0x4B){
          data_Buf[count] = char(inByte);
//          Serial.print(data_Buf[count]);
          count++;
        } 
   }
   if(count >= bufLength){
      break;
   }
   else
    {
      delaytime=RtcDelay(waitingTime,TimeStart);
    }
  }
}

/********************************************************************************/
/*  Name: test_Sigfox                                                           */
/*  Detail: test Command sigfox                                                  */
/*  Argument: None                                                              */
/*  Return: None                                                                */
/********************************************************************************/
void test_Sigfox(){
  Serial1.begin(9600);
  digitalWrite(Sigfox_Pw, HIGH);
  delay(2000);
  Serial1.print("AT$CW=923200000,1,14\r\n");
  delay(1000);
  digitalWrite(Sigfox_Pw, LOW);
  Serial1.end();
  delay(2000);
}
