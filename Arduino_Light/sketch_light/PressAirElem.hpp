#ifndef PRESSAIR_ELEM_HPP
#define PRESSAIR_ELEM_HPP

#include "GlobalDef.h"
#include "NextionObject.h"

#ifdef WIN32
    #include "../stdafx.h"
#endif

#ifdef WIN32
	#define CLOSE_SINK_TIME 5
#else
	#define CLOSE_SINK_TIME 5000
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
		_needSinkReset = false;
    }

    CCompressor(short pin, const char *cmdName, const char* cmd) 
        : CDigitalOnOff(pin),
        CNextionDSObject(cmdName,cmd, "")
    { 
        m_status = INACTIVE; 
        Off();
		_needSinkReset = false;
    }

    enum
    {
        INACTIVE = 0,
        ACTIVE = 1,        
    } m_status;
	
	bool _needSinkReset;

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
		if(m_status != ACTIVE)
		{
			Off();
			return;
		}

        if(curPress >= maxPress)
		{
			Off();
		}
		else
		if(curPress <= maxPress - 1) //if max press -1 bar, begin pumping
		{
			On();
		}
		else
		if(curPress <= maxPress - 0.3) //if max press -1 bar, begin pumping
		{
			//todo постоянно открыт

			_needSinkReset = true;
		}
		else
		{
			/*
			SMES("Cur: ");
			SMES(curPress);
			SMES(" Max: ");
			SMESN(maxPress);
			*/
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
			SMESN("Open SINK");
			On();
            m_IsOpening = true;
            m_IsNeedOpen = false;
			openTime = millis();
            return;
        }
        else
        if(m_IsOpening)
        {
            if(millis() - openTime > CLOSE_SINK_TIME)
            {
				SMESN("Close SINK");

                Off();
                m_IsOpening = false;
            }
        }
    }

    void Execute()
    {
        m_IsNeedOpen = 1;
    }

    bool SetAuto(bool isOn)
    {
        if(m_IsAuto == isOn)
            return false;

        m_IsAuto = isOn;

        return true;
    }
	bool IsAuto() { return m_IsAuto; }

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
