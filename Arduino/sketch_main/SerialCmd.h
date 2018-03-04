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
    CSerial(){
         memset(buff, 0, sizeof(buff));
#ifdef WIN32
		 strcpy(buff, "L|A0000000000000111L|B00011001Lbs11");
#endif
    }
    void begin(int){}
    void write(const char* ptr, int len) { SMES("("); SMES(len); SMES(") ");  SMES(ptr); }
	void write(int val)					 { SMES(val);  }
    void println(const char* str)        { SMESN(str); }
    void println(int val)                { SMESN(val); }
    void print(const char* str)          { SMES(str);  }
    void print(int val)                  { SMES(val);  }
	void printf(char * str)				 { SMES(str);  }
	void end(){}
    uint8_t read()
    {
        uint8_t v = buff[0];
        if(available() == 1)
            *buff = 0x0;
        else
            memmove(buff, buff + 1, sizeof(buff)-1);
        return v;
    }
    int available()
    { 
        return strlen(buff);
    }
    void flush(){}

    char buff[1000];
};

static CSerial Serial, Serial1, Serial2, Serial3;

#define nexSerial Serial1

#endif


#define CMD_NUM 0x24			// $
#define CMD_FLOAT 0x7e			// ~
#define CMD_INVALIDATE 0x7c		// |


#define CMD_LEN                5 
#define CMD_FLOAT_NUM_LEN    11

#define TYPE_POS 1
#define FLOAT_NUM_POS 5
#define CMD_WORD_LEN 12


struct sCmdWord 
{

    enum eMSGType
    {
        eCMD = 1,
        eCMD_NUMBER5,
        eCMD_FLOAT
    };

    eMSGType Type;  

    char buff[CMD_WORD_LEN];


    sCmdWord()
    {
        clear();
    }

    sCmdWord(const char* cmd)
    {
        clear();
        
        strcpy(buff,cmd);
#ifdef WIN32
        if(buff[1] != CMD_FLOAT && buff[1] != CMD_NUM)
            cout << (strlen(buff) != 5 ? "ERROR" : "");
#endif
        buff[CMD_LEN] = 0x00;
        Type = eCMD;
    }

    sCmdWord(const char* cmd, uint32_t number5)
    {
        clear();
    
        strcpy(buff, cmd);
        for(int j = 0; j < 6; j++)
            buff[j + FLOAT_NUM_POS] = ' ';

        sprintf(buff + FLOAT_NUM_POS, "%6d", number5); // from 5 symb begin number
        buff[TYPE_POS] = CMD_NUM;

#ifdef WIN32
        cout << (strlen(buff) != 11 ? "ERROR" : "");
#endif

        Type = eCMD_NUMBER5;
    }

    sCmdWord(const char* cmd, float num, short decimalCount)
    {
        clear();
        
        strcpy(buff, cmd);
        for(int j = 0; j < 6; j++)
            buff[j + FLOAT_NUM_POS] = ' ';

        char tmpbuff[7]={0};

#ifdef WIN32
        if(decimalCount == 1)
            sprintf(tmpbuff, "%.1f", num);
        if(decimalCount == 2)
            sprintf(tmpbuff, "%.2f", num);
#else    
        if(decimalCount == 1)
            dtostrf(num, 6, 1, tmpbuff);
        if(decimalCount == 2)
            dtostrf(num, 6, 2, tmpbuff);
#endif // WIN32

        
        short copyCount = 6 - strlen(tmpbuff);
        strncpy(buff + FLOAT_NUM_POS + copyCount, tmpbuff, strlen(tmpbuff));
        buff[sizeof(buff) - 1 ] = '\0';

        buff[TYPE_POS] = CMD_FLOAT;
        Type = eCMD_FLOAT;
    }


    void clear()
    {
        memset(buff, 0, sizeof(buff));
        Type = eCMD;    
    }

    void EjectType() 
    {
        Type = eCMD;
        if(buff[1] == CMD_NUM)
            Type = eCMD_NUMBER5;
        else
        if(buff[1] == CMD_FLOAT)
            Type = eCMD_FLOAT;
    }
    

    void SetAction(short value)
    {
        if(value == -1)
            return;

        if(Type == eCMD)
            buff[CMD_LEN - 1] =  value == 0 ? '0' : '1';
    }

    short GetAction() 
    { 

        if(Type == eCMD)
        {
            if(buff[CMD_LEN - 1    ] == '0') return 0;
            if(buff[CMD_LEN - 1    ] == '1') return 1;
        }
        return 0;
    }

    uint32_t GetNumber(char outbuff[7])
    {
        if(Type != eCMD_NUMBER5)
            return 0;
        
        memset(outbuff, 0,0);
        strncpy(outbuff, buff + 6, 6);
        tsLeft(outbuff, strlen(outbuff));

        return atoi(outbuff);
    }
    
    float GetFloatNumber(float& value, char outbuff[7])
    {
        memset(outbuff, 0,0);
        
        strncpy(outbuff, buff + FLOAT_NUM_POS, 6);
        outbuff[6] = 0x0;
        int pos = -1;
        
        tsLeft(outbuff, strlen(outbuff));

        value = atof(outbuff);
        return value;
    }

    char GetReciver() {  return buff[0]; }

    bool IsCommand(const char* cmd, bool IsIgnoreTarget = true) 
    {
        if(Type == eCMD)
            return !strncmp(cmd, buff, 4);
        else
            return !strncmp(cmd +2, buff + 2, 3) && buff[0] == cmd[0];
    }
    
    bool IsName(const char* cmd) 
    {
        if(Type == eCMD)
            return !strncmp(cmd, buff + 1, 3);
        else
            return !strncmp(cmd, buff + 2, 3);
    }
};


struct sCmdInvalidate
{
	void clear()
	{
		curPos = 0;
		memset(buff, 0, sizeof(buff));
	}

	sCmdInvalidate()
	{
		clear();
	}

	sCmdInvalidate(char Target)
	{
		clear();
		sprintf(buff, "%c|", Target);
		curPos = 2;
	}

	sCmdInvalidate(char* szCmd)
	{
		clear();
		strcpy(buff, szCmd);
		curPos = 3;
	}

	char GetReciver() {  return buff[0]; }

	void AddNextSymb(char Symb)
	{
		if(curPos < sizeof(buff))
		{
			buff[curPos] = Symb;
			curPos++;
			buff[curPos] = 0x00;
		}
	}
	char GetMode()
	{
		return buff[2];
	}
	short GetCountItems()
	{
		if(strlen(buff) <= 3)
			return 0;
		return strlen(buff+3);
	}

	char GetItem(short pos)
	{
		return buff[pos + 3];
	}

	char buff[25];
private:
	short curPos;
};



#define SERIAL_SPEED_9600	9600
#define SERIAL_SPEED_14400	14400
#define SERIAL_SPEED_38400	38400
#define SERIAL_SPEED_57600	57600


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
            Serial.begin(SERIAL_SPEED_9600);
        else
        if(m_port == eSerial1)
            Serial1.begin(SERIAL_SPEED_57600);
        else
        if(m_port == eSerial2)
            Serial2.begin(SERIAL_SPEED_57600);
        else
        if(m_port == eSerial3)
            Serial3.begin(SERIAL_SPEED_57600);

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

        flush();
        //todo add delay ? 

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



