
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


/*********************************************************************************/
/*  Name: check_ssid_name                                                        */
/*  Detail: Check ssid name with black list or white list                        */
/*  Argument:                                                                    */
/*           *ssid: pointer scanned mac address ssid                             */
/*           *name: pointer black list or white list ssid                        */
/*  Return: false(ssid don't have name)| true(ssid have name)                    */
/*********************************************************************************/
 static bool check_ssid_name(char * ssid, char * name) {
  bool bret = false;
  delay(1);
  Serial.print("ssidTest");
  Serial.println(ssid);
  Serial.print("nameTest");
  Serial.println(name);
  delay(1);
  for(int i = 0; i<strlen(ssid);i++) {
    if (ssid[i] >='A' && ssid[i] <= 'Z') {
      ssid[i] += 32;
    }
  }
  for(int i = 0; i<strlen(name);i++) {
    if (name[i] >='A' && name[i] <= 'Z') {
      name[i] += 32;
    }
  }
  if(isSubstring(name, ssid) == true){
    bret = false;
    Serial.print("return");
    Serial.println(bret);
    delay(10);
  }else{
    bret = true;
  }

  return bret;
}

/*********************************************************************************/
/*  Name: isSubstring                                                            */
/*  Detail: Compare 2 string s1 and s2                                           */
/*  Argument:                                                                    */
/*           *s1: String 1                                                       */
/*           *s2: String 2                                                       */
/*  Return: false(s2 don't have s1)| true(s2 have s1)                            */
/*********************************************************************************/
bool isSubstring(char * s1, char * s2) 
{ 
    int M = strlen(s1); 
    int N = strlen(s2); 
    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) {
        int j; 
   
        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++){
            if (s2[i + j] != s1[j]){ 
                break; 
            }
        }
        if (j == M) 
            return true; 
    } 
    return false; 
} 

bool userDelay(uint8_t timecount)

{
  if (timecount<=0)
  {
    return true;
  }
  else
  {
    delay(1);
    timecount--;
    return false;
  }
}

template <typename Stype>
void dPrint(Stype Comment)
{
#if DebugP
    Serial.print(Comment);
#endif
}

template <typename Stypeln>
void dPrintln(Stypeln Comment)
{
#if DebugP
    Serial.println(Comment);
#endif
}


//template <typename Sstypeln>
bool CheckDataSize(char* DataC,int LengthC)
{
  char* Buf;
  sprintf(Buf,"Data Length = %d ",sizeof(*DataC));
  dPrint(Buf);
  if(sizeof(*DataC)==LengthC)
  {
    dPrintln("Length Check = True");
    return true;
  }
  else
  {
    dPrintln("Length Check = False");
    return false;
  }
}

uint8_t ChartoInt(char CharVal)
{
  uint8_t IntVal;
  if(CharVal>='a'&&CharVal<='z')
    {
      IntVal=CharVal-'W';
    }
    else if(CharVal>='A'&&CharVal<='Z')
    {
      IntVal=CharVal-55;
    }
    else if(CharVal>='0'&&CharVal<='9')
    {
      IntVal=CharVal-'0';
    }
  return IntVal;
}
