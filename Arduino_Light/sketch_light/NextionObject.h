#pragma once

#include "SerialCmd.h"

#ifdef WIN32
  typedef int uint8_t;
#endif

class CNextCMD
{
public:
    CNextCMD()
    {

    }

    sCmdWord m_cmdOn;
    sCmdWord* m_cmdOff;

    sCmdWord* GetCmdByValue(uint32_t val);
};

class CNextionDSObject : public CNextCMD
{
public:
    CNextionDSObject(const char *Name, const char *cmdOn, const char *cmdOff);
    virtual ~CNextionDSObject(void){}

    char m_Name[20];
};
