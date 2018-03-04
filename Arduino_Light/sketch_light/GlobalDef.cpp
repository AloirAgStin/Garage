#include "GlobalDef.h"
#include <math.h>

#ifdef WIN32
    void _pinMode(short, int)
    {
    }
#endif

bool IsDoubleEqual(double x, double y)
{
    return fabs(x - y) < 1e-6;
}

void SetPinModeInput(short pin, bool IsInput)
{
    char buff[256] = {0};

    sprintf(buff, "PIN %d is Inited (%s)", pin, IsInput ? "INPUT" : "OUTPUT");
  
    if (IsInput)
      pinMode(pin, INPUT);
    else
      pinMode(pin, OUTPUT);
#ifdef WIN32
    SMESN(buff);
#endif
}


void SetPinModeInput(short pin)
{
  SetPinModeInput(pin, true);
}

void SetPinModeOutput(short pin)
{
  SetPinModeInput(pin, false);
}



void PINON(int pin)
{
#ifndef WIN32
  digitalWrite(pin, ON);
#endif
}

void PINOFF(int pin)
{
#ifndef WIN32
  digitalWrite(pin, OFF);
#endif
}



//HIGH and LOW
int D_READ(int pin)
{
#ifndef WIN32
  return digitalRead(pin);
#else
    char buff[20] = "PIN";
    char tempbuf[5] = {0};
    itoa(pin,tempbuf,10);
    return GetPrivateProfileInt("PINS",strcat(buff,tempbuf) ,0, "d:/ard.ini");
#endif
}

void D_WRITE(int pin, short value)
{
#ifndef WIN32
  digitalWrite(pin, value);
#endif
}


//0-1023
int A_READ(int pin)
{
#ifndef WIN32
  return analogRead(pin);
#else
  char buff[20] = "PIN";
  char tempbuf[5] = {0};
  itoa(pin,tempbuf,10);

  return GetPrivateProfileInt("PINS",strcat(buff,tempbuf) ,0, "d:/ard.ini");

  return 0;
#endif
}

void A_WRITE(int pin, int value)
{
#ifndef WIN32
  analogWrite(pin, value);
#endif
}