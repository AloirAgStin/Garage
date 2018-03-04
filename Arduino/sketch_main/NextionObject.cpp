#include "NextionObject.h"

CNextionDSObject::CNextionDSObject(uint8_t pid, uint8_t cid, const char *_name, const char* cmdON, const char* cmdOFF) :
    m_button(pid, cid, _name), m_cmdON(0), m_cmdOFF(0)
{
    m_oldValue = 0;
    m_IsTypeOnOff = false;

    strcpy(m_name, _name);
    m_cmdON  = new sCmdWord(cmdON);

    if(strlen(cmdOFF) != 0)
        m_cmdOFF = new sCmdWord(cmdOFF);
}

sCmdWord* CNextionDSObject::GetCmdByValue(uint32_t val)
{
  if(val == 0)
  {
    if(!m_cmdOFF)
    {
      m_cmdON->SetAction(0);
      return m_cmdON;
    }
    else
      return m_cmdOFF;
  }
  else
  if(val == 1)
  {
    if(!m_cmdOFF)
      m_cmdON->SetAction(1);
    return m_cmdON;
  }
}

void CNextionDSObject::SetValue(uint32_t _val)
{
    if(_val == m_oldValue)
        return;

    m_button.setValue(_val);
    m_oldValue = _val;
}

void CNextionDSObject::Invalidate()
{
    m_button.setValue(m_oldValue);
}

CNextionDSObject::~CNextionDSObject(void)
{
    delete_if_exists(m_cmdON);
    delete_if_exists(m_cmdOFF);
}


CNextionSingleObject::CNextionSingleObject(uint8_t pid, uint8_t cid, const char *_name, const char* cmd) :
    m_button(pid, cid, _name)
{
    strcpy(m_name, _name);
    m_cmd  = new sCmdWord(cmd);
}

CNextionSingleObject::~CNextionSingleObject(void)
{
    delete_if_exists(m_cmd);
}





CNexPageEx::CNexPageEx(uint8_t pid, uint8_t cid, const char *name)
    :NexPage(pid, cid, name)
{
    m_pageID = pid;
    m_btnMenu = 0;

    //sensors
    m_pDate    = 0 ;
    m_pTime    = 0;
    m_pTempIn  = 0;
    m_pTempOut = 0;
    m_pPress   = 0;
    m_pPower   = 0;
    m_pVoltage = 0;
    m_pAmpers  = 0;
}

CNexPageEx::~CNexPageEx()
{
    delete_if_exists(m_btnMenu);
    
    delete_if_exists(m_pDate);
    delete_if_exists(m_pTime);
    delete_if_exists(m_pTempIn);
    delete_if_exists(m_pTempOut);
    delete_if_exists(m_pPress);
    delete_if_exists(m_pPower);
    delete_if_exists(m_pVoltage);
    delete_if_exists(m_pAmpers);
}
    



