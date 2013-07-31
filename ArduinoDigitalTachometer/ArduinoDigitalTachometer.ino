/*

  Arduino Tachometer

  Coded by Yasuhiro ISHII


  ELM327 Lib : https://code.google.com/p/loguino/source/checkout

*/

#define ELM_BAUD_RATE  38400
#define ELM_PORT Serial1

#include <ELM327.h>
#include <stdio.h>
#include <SPI.h>

byte Tab[]={
  0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};

void printStatus(byte status){
  switch (status)
  {
  case ELM_TIMEOUT: 
    Serial.println("ELM_TIMEOUT");
    break;
  case ELM_SUCCESS:
    Serial.println("ELM_SUCCESS");
    break;
  case ELM_NO_RESPONSE:
    Serial.println("ELM_NO_RESPONSE");
    break;
  case ELM_BUFFER_OVERFLOW:
    Serial.println("ELM_BUFFER_OVERFLOW");
    break;
  case ELM_GARBAGE:
    Serial.println("ELM_GARBAGE");
    break;
  case ELM_UNABLE_TO_CONNECT:
    Serial.println("ELM_UNABLE_TO_CONNECT");
    break;
  case ELM_NO_DATA:
    Serial.println("ELM_NO_DATA");
    break;
  default:
    Serial.print("UNKNOWN: ");
    Serial.println(status);
  }
}

void setup() {
  // put your setup code here, to run once:

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  Serial.begin(115200);

  Serial.println("Start!!");

  SPI.begin();
  resetDisplay();
  update8digit(12345);
  delay(2000);
  resetDisplay();

}

void loop() {
  int i;
  byte status;
  String s;
  
  Elm327 Elm;
  Serial.println("Call begin");
  printStatus(Elm.begin());
  Serial.println("Begin returned!!");

  Serial.print("Elm Version: ");
  status=Elm.getVersion(s) ;
  if (status == ELM_SUCCESS)
  {
    Serial.println ("Pass");
    Serial.print(" Value: ");
    Serial.println(s);
  }
  else{
    printStatus(status);
  }

  while(1){
    status=Elm.engineRPM(i) ;
    printStatus(status);
    if (status == ELM_SUCCESS)
    {
      update8digit(i);
    }
    else{
      update8digit(99);
    }

  }

}

void resetDisplay()
{
  int i;
  digitalWrite(13,LOW);
  for(i=0;i<8;i++){
    transfer1digit(10);
  }
  digitalWrite(13,HIGH);
}

void transfer1digit(int num)
{
  SPI.transfer(Tab[num]);
}

void update8digit(int num)
{
  char str[32];
  int i;
  byte data;
  char tmp;

  sprintf(str,"%08d",num);

  digitalWrite(13,LOW);
  for(i=0;i<8;i++){
    tmp = str[7-i];
    if(tmp == ' '){
      tmp = 10;
    } 
    else {
      tmp -= '0';
    }
    //data = Tab[tmp];
    transfer1digit(tmp);
  }
  digitalWrite(13,HIGH);

}






