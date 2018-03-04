#pragma once
#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H

#include "SerialCommand.h"
#include "GlobalDef.h"
#ifndef WIN32
#include "Arduino.h"
#endif


#ifdef WIN32

class CSerial
    {
    public:    
        CSerial(){}
        void begin(int){}
        void write(const char* ptr, int len) { SMES("("); SMES(len); SMES(") ");  SMES(ptr); }
        void println(const char* str)        { SMESN(str); }
        void println(int val)                { SMESN(val); }
        void print(const char* str)          { SMES(str);  }
        void print(int val)                  { SMES(val);  }
        uint8_t read(){ return 0;}
        int available(){ return 1;}
        void flush(){}
    };

    static CSerial Serial, Serial1, Serial2, Serial3;
#endif

/*
    command word stuct
    $P........CM~
    $ - begin cmd
    P - page 
    .... - from messge
    CM - command
    ~ - end message
*/
#define BEGIN_CMD 0x24
#define END_CMD      0x7E
#define CMD_WORD_LEN 18
#define COUNT_SYMB_IN_FLOAT_VALUE 7

struct sCmdWord 
{
    sCmdWord()
    {
        zm(buff, 0);
    }
    sCmdWord(const char* cmd)
    {
        strcpy(buff,cmd);
        buff[sizeof(buff) - 1 ] = '\0';
    }
	
    sCmdWord(const char* cmd, float value, short decimalPart = 1)
    {
        strcpy(buff,cmd);
        char buffNum[15]={0};

        if(decimalPart == 1)
            sprintf(buffNum, "%7.1f", value);
        else
        if(decimalPart == 2)
            sprintf(buffNum, "%7.2f", value);
        else
        if(decimalPart == 0)
            sprintf(buffNum, "%7d", value);
        
        buffNum[COUNT_SYMB_IN_FLOAT_VALUE + 1]= 0x0;

        strncpy(buff + (CMD_WORD_LEN  - COUNT_SYMB_IN_FLOAT_VALUE - 2), buffNum, COUNT_SYMB_IN_FLOAT_VALUE);
        buff[sizeof(buff) - 1 ] = '\0';
    }

    sCmdWord(const char* cmd, uint32_t value, bool istrue)
    {
        strcpy(buff,cmd);
        char buffNum[15]={0};

        sprintf(buffNum, "%7d", value);

        buffNum[COUNT_SYMB_IN_FLOAT_VALUE + 1]= 0x0;

        strncpy(buff + (CMD_WORD_LEN  - COUNT_SYMB_IN_FLOAT_VALUE - 3), buffNum, COUNT_SYMB_IN_FLOAT_VALUE);
        buff[sizeof(buff) - 1 ] = '\0';
    }

    char buff[CMD_WORD_LEN];
    bool IsEmpty() { return buff[0] == 0x00; }

    char GetReciver() {  return buff[1]; }

    short GetAction() 
    { 
        if(buff[CMD_WORD_LEN - 3    ] == '0') return 0;
        if(buff[CMD_WORD_LEN - 3    ] == '1') return 1;
        return 0;
    }

    void GetValue(char *buffOut, short len)
    {
        buffOut[len - 1] = 0x0;
        strncpy(buffOut, buff + (CMD_WORD_LEN  - COUNT_SYMB_IN_FLOAT_VALUE - 2), COUNT_SYMB_IN_FLOAT_VALUE);
        tsLeft(buffOut, len);
    }
    
    uint32_t GetColor()
    {
        char buffOut[15]={0};
        strncpy(buffOut, buff + (CMD_WORD_LEN  - COUNT_SYMB_IN_FLOAT_VALUE - 3), COUNT_SYMB_IN_FLOAT_VALUE);
        return atoi(buffOut);
    }

    bool IsCommand(const char* cmd) {return !strcmp(cmd, buff); }

    void SetAction(int act) { buff[CMD_WORD_LEN - 3] = act == 0 ? '0' : '1'; }
};


class CSendCmd
{
public:
    enum Port
    {
        eSerial0,
        eSerial1,
        eSerial2,
        eSerial3
    };

    CSendCmd(Port port, bool _isdebug = false)
    {
        m_IsDebug = _isdebug;
        m_port = port;
    }

    void Init()
    {        
        if(m_port == eSerial0)
            Serial.begin(9600);
        else
        if(m_port == eSerial1)
            Serial1.begin(9600);
        else
        if(m_port == eSerial2)
            Serial2.begin(9600);
        else
        if(m_port == eSerial3)
            Serial3.begin(9600);
        
        if(m_IsDebug)
        {
            Serial.begin(9600);
            Serial.println("Debug enable");
        }
    }

    void SendCommand(const char* mes, int len)
    {
        if(len == 0) return;

        if(m_port == eSerial0)
            Serial.write(mes, len);
        else
        if(m_port == eSerial1)
            Serial1.write(mes, len);
        else
        if(m_port == eSerial2)
            Serial2.write(mes, len);
        else
        if(m_port == eSerial3)
            Serial3.write(mes, len);

        if(m_port == eSerial0) SMES("[S0]");
        else if(m_port == eSerial1) SMES("[S1]");
        else if(m_port == eSerial2) SMES("[S2]");
        else if(m_port == eSerial3) SMES("[S3]");
             
        SMES("send msg: ");
        SMESN(mes);      
     }

  int flush()
  {
        if(m_port == eSerial0)
             Serial.flush();
        else
        if(m_port == eSerial1)
             Serial1.flush();
        else
        if(m_port == eSerial2)
             Serial2.flush();
        else
        if(m_port == eSerial3)
             Serial3.flush();
        return 0;
  }
  
  void SendCommand(const char* mes)
  {
      SendCommand(mes, strlen(mes));
  }
 
  void SendCommand(sCmdWord* mes)
  {
      SendCommand(mes->buff, strlen(mes->buff));
  }
 
 int available()
 {
        if(m_port == eSerial0)
            return Serial.available();
        else
        if(m_port == eSerial1)
            return Serial1.available();
        else
        if(m_port == eSerial2)
            return Serial2.available();
        else
        if(m_port == eSerial3)
            return Serial3.available();
        return 0;
    }
  
    int Read() 
    {
        if(m_port == eSerial0)
            return Serial.read();
        else
        if(m_port == eSerial1)
            return Serial1.read();
        else
        if(m_port == eSerial2)
            return Serial2.read();
        else
        if(m_port == eSerial3)
            return Serial3.read();
        return 0;
    }

    bool m_IsDebug;
    Port m_port;
};

#endif



