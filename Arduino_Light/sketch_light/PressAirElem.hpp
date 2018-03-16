#ifndef PRESSAIR_ELEM_HPP
#define PRESSAIR_ELEM_HPP

#include "GlobalDef.h"
#include "NextionObject.h"

#ifdef WIN32
    #include "../stdafx.h"
#endif

class CCompressor 
    : protected CDigitalOnOff,
      public CNextionDSObject
{
public:
    CCompressor() 
            : CDigitalOnOff(0),
            CNextionDSObject("","","")
    { 
        m_status = INACTIVE; 
        Off();
    }

    CCompressor(short pin, const char *cmdName, const char* cmd) 
        : CDigitalOnOff(pin),
        CNextionDSObject(cmdName,cmd, "")
    { 
        m_status = INACTIVE; 
        Off();
    }

    enum
    {
        INACTIVE = 0,
        ACTIVE = 1,        
    } m_status;


    void SetActive(bool IsActive)
    {
        if(IsActive)
        {
			m_status = ACTIVE;
			SMESN("PRESS ON");
		}
		else
        {
			m_status = INACTIVE;
			SMESN("PRESS OFF");
		}
    }
	    

    sCmdWord* GetCmdForInvalidateBtn()
    {
        int act = 0;
        if(m_status == ACTIVE)
		{
            act = 1;
		}

        return GetCmdByValue(act);
    }

		
    void Process(const float &curPress, const float &maxPress)
    {
        if(m_status == ACTIVE)
            On();
        else
            Off();

        if(m_status != ACTIVE)
            return;


        if(curPress >= maxPress)
            Off();
        else
        if(curPress <= maxPress - 1) //if max press -1 bar, begin pumping
		{
            On();
		}
    }
};

class CSinkBtn 
    : public CDigitalOnOff,
      public CNextionDSObject
{
public:
    CSinkBtn() 
        : CDigitalOnOff(0),
        CNextionDSObject("","","")
    { m_IsAuto = 0; }

    CSinkBtn(short pin, const char *cmdName, const char* cmd) 
        : CDigitalOnOff(pin),
        CNextionDSObject(cmdName,cmd, "")
    {
        m_IsAuto = 0;
        m_IsNeedOpen = 0;
        m_IsOpening = 0;
    }

    void Process()
    {
        static unsigned long openTime = 0;
        if(m_IsNeedOpen)
        {
            On();
            m_IsOpening = true;
            m_IsNeedOpen = false;
            return;
        }
        else
        if(m_IsOpening)
        {
            if(millis() - openTime > 5000)
            {
                Off();
                m_IsOpening = false;
            }
        }

        //todo auto mode
    }

    void Execute()
    {
        m_IsNeedOpen = 1;
        Process();
    }

    bool SetAuto(bool isOn)
    {
        if(m_IsAuto == isOn)
            return false;

        m_IsAuto = isOn;

        return true;
    }

    sCmdWord* GetCmdForInv()
    {
        return GetCmdByValue(m_IsAuto == 1 ? 1 : 0);
    }

private:
    bool m_IsAuto;

    bool m_IsNeedOpen;
    bool m_IsOpening;

};

#endif
