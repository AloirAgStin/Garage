#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H


//#define SHOWPINS
//#define SHOW_TIMER

enum eStatusOnOFF
{
  ST_OFF = 0,
  ST_ON = 1,
};
    
//todo debug
#define DEBUG_INFO

bool IsDoubleEqual4(double x, double y);
bool IsDoubleEqual2(double x, double y);

#ifdef WIN32
    #include "../stdafx.h"
    #include <windows.h>
    #include <iostream>

    #define delay(x) Sleep(x)
    #define AppStartTime  clock() / 1000


    void _pinMode(short, int);
    #define OUTPUT 0
    #define INPUT 1


    #define pinMode(x,y) _pinMode(x,y)

    #define ON 1
    #define OFF 0

    #define SMES(x)  std::cout << x
    #define SMESN(x) std::cout << x << std::endl 

    typedef int uint8_t;
    typedef int uint32_t;

    inline float constrain(int x, int y, int z) {return 1;}

    inline int analogRead(){ return 1; }

    inline bool recvRetNumber(uint32_t *number, uint32_t timeout = 100) {return true;}
    inline void sendCommand(char* t) {}
    inline int memoryFree() { return 9999; }

    inline uint32_t millis() { return time(0);} 
    inline int dtostrf(float, int, int, char*){ return 1;}


    #define A0 0
    #define A1 1
    #define A2 3
    #define A3 2
    #define A4 4
    #define A5 5
    #define A6 6
    #define A7 7
    #define A8 8
    #define A9 9
#else
    #include <arduino.h>
    #define AppStartTime  millis()

    #define ON HIGH
    #define OFF LOW
    
    #ifdef DEBUG_INFO
        #define SMES(x) Serial.print(x)
        #define SMESN(x) Serial.println(x)
    #else
        #define SMES(x)
        #define SMESN(x) 

    #endif


      
  // Переменные, создаваемые процессом сборки,
  // когда компилируется скетч
  extern int __bss_end;
  extern void *__brkval;
   
  // Функция, возвращающая количество свободного ОЗУ (RAM)
  inline int memoryFree()
  {
     int freeValue;
     if((int)__brkval == 0)
        freeValue = ((int)&freeValue) - ((int)&__bss_end);
     else
        freeValue = ((int)&freeValue) - ((int)__brkval);
     return freeValue;
  }
  
#endif

void SetPinModeInput(short pin, bool IsInput);
void SetPinModeInput(short pin);
void SetPinModeOutput(short pin);

void PINON(int pin);
void PINOFF(int pin);

//HIGH and LOW
int D_READ(int pin);
void D_WRITE(int pin, short value);

//0-1023
int A_READ(int pin);
void A_WRITE(int pin, int value);

//trim string left
inline void tsLeft(char* buff, short len)
{
    short i = 0;
    for(i = 0; i < len ; i ++)
    {
        if(buff[i] != ' ')
            break;
    }

    if(i > 0)
    {
        memmove(buff, buff + i, len - i);
        buff[len - i] = 0x0;
    }
}


#define delete_2x_Array(arr, size) for(int i = 0; i < size; i++) delete arr[i]; delete []arr;
#define zm(x,val) memset(x,val, sizeof(x))
#define delete_if_exists(x) if(x) delete x;

#endif
