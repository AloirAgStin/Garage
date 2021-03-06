#ifdef WIN32
    #include "../stdafx.h"
#else
    #include "arduino.h"
#endif

#include "NextionObject.h"


sCmdWord* CNextCMD::GetCmdByValue(uint32_t val)
{
    m_cmdOn.GetAction();
    if(val == 0)
    {
      if(!m_cmdOff)
      {
        m_cmdOn.SetAction(0);
        return &m_cmdOn;
      }
      
      return m_cmdOff;
    }
    else
    {
      if(!m_cmdOff)
        m_cmdOn.SetAction(1);
      return &m_cmdOn;
    }
    return 0;
}



CNextionDSObject::CNextionDSObject(const char *Name, const char *cmdOn, const char *cmdOff)
    : CNextCMD()
{    
    m_cmdOff = 0;

    m_cmdOn  = sCmdWord(cmdOn);
    if(strlen(cmdOff) != 0)
        m_cmdOff = new sCmdWord(cmdOff);
    strcpy(m_Name, Name);
}
