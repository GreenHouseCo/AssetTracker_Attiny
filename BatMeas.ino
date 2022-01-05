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
uint8_t BatMeas()
{
  digitalWrite(BAT_MONI,HIGH);
  delay(100);
  uint16_t VoltBuff=analogRead(BAT_V_CHK); //MAX 565(VBat=3.65V) MIN 50\\5(VBat=3.0V Normal Run)
  Serial.println("Battery:");
  Serial.println(VoltBuff);
  uint8_t PayLoadVal=0;
  digitalWrite(BAT_MONI,LOW);
  delay(1);  
  uint8_t VoltVal=0;
  if(VoltBuff>=505)
  {
    VoltVal=(VoltBuff-505)*100/(565-500);
  }
  else
  {
    VoltVal=0;
  }
  if(VoltVal>90)PayLoadVal=7;
  else if(VoltVal>70)PayLoadVal=6;
  else if(VoltVal>50)PayLoadVal=5;
  else if(VoltVal>40)PayLoadVal=4;
  else if(VoltVal>30)PayLoadVal=3;
  else if(VoltVal>20)PayLoadVal=2;
  else if(VoltVal>10)PayLoadVal=1;
  else PayLoadVal=0;
  return PayLoadVal;
}
