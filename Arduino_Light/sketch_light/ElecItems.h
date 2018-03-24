#ifndef ELEC_ITEMS_H
#define ELEC_ITEMS_H

#include "GlobalDef.h"
#include "Timer.hpp"
#include "MotionSensor.hpp"
#include "NextionObject.h"

#ifdef WIN32
    #include "../stdafx.h"
#endif


//socet item in panel
class CSimpleSocet :     
    public CDigitalOnOff,
    public INotifyEvent
{
    CSimpleSocet(short pin) : CDigitalOnOff(pin),
        m_btnOnOff("","",""), m_btnProperty("","","") { m_IsNeedOff = false; m_PropertyOnOFF = ST_OFF; m_currNetworkVolts = false; }

public:
    CSimpleSocet(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff, bool IsNeedInvesrSignals = false) : 
      CDigitalOnOff(pinSoc),
          m_btnOnOff(SocName, SocCmdOn, SocCmdOff),
          m_btnProperty(PropName, PropCmdOn, PropCmdOff)
      {
          m_IsNeedOff = false;
          m_PropertyOnOFF = ST_OFF;
          m_currNetworkVolts = false;
          m_timer = new CTimer(TimeOFF);
          m_IsNeedInvesrSignals = IsNeedInvesrSignals;

          if(m_IsNeedInvesrSignals)
              OffSocet();
      }

    inline void SetTimerParent(INotifyEvent *pParent)
    {
        m_timer->SetParent(pParent);
    }

    static int OffDelay;

    eStatusOnOFF m_PropertyOnOFF;

    /// nextion work
    CNextionDSObject m_btnOnOff;
    CNextionDSObject m_btnProperty;

    sCmdWord* GetCmdForInvalidateBtn()
    {
        int act = GetStatus() == ST_ON ? 1 : 0;
            if(m_IsNeedInvesrSignals)
                 act = act == 1 ? 0 : 1;

        return m_btnOnOff.GetCmdByValue(act);
    }

    sCmdWord* GetCmdForInvalidateProp()
    {
        int act = m_PropertyOnOFF == ST_ON ? 1 : 0;
    
        return m_btnProperty.GetCmdByValue(act);
    }

    /// end nextion work

    virtual bool ActionBeforeOn() { return true; }
    virtual bool ActionAfterOn() { return true; }

    virtual bool ActionBeforeOFF() { return true; }
    virtual bool ActionAfterOFF() { return true; }

    
    virtual bool OnSocet()
    {
        if(!ActionBeforeOn()) 
            return false;


        bool ret = false;
        
        if(m_IsNeedInvesrSignals)
            ret = Off();
        else
            ret = On();
        
        if(!ret) 
            return ret;

        ActionAfterOn();
        delay(CSimpleSocet::OffDelay);

        return ret;
    }

    virtual bool OffSocet()
    {
        if(!ActionBeforeOFF())
            return false;

        StartStopKillTimer(false);
        m_IsNeedOff = false;

        bool ret = !m_IsNeedInvesrSignals ? Off(): On();

        if(ret)
        {
            ActionAfterOFF();
            delay(CSimpleSocet::OffDelay);
        }

        return ret;
    }

    void SetNetworkVoltage(bool val) { m_currNetworkVolts = val; };
    virtual void Update()
    {
        if(m_timer)
            m_timer->Update();    
    }
    
    virtual void StartStopKillTimer(bool isStart)
    {
        if(m_PropertyOnOFF != !m_IsNeedInvesrSignals ? ST_ON : ST_OFF)
            return;

        if(isStart && !m_timer->IsEnable())
        {            
            m_timer->StartTimer();
        }
        else
            m_timer->KillTimer();
    }    
    bool IsNeedOFF(){ return m_IsNeedOff; }
protected:
    //tick timer to off socket
    void Notify(eNotyfyAction val)
    {
        switch(val)
        {
            case ACTION_TIMER_TICK:
            {
                m_IsNeedOff = true;
                m_timer->KillTimer();
                break;
            }
        }
    }

    bool m_IsNeedOff;
    CTimer* m_timer;

    bool m_currNetworkVolts;
    bool m_IsNeedInvesrSignals;
};





class CDisplaySocket :     public CSimpleSocet
{
public:
    virtual ~CDisplaySocket(){}

    CDisplaySocket(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff) : 
    CSimpleSocet(pinSoc, TimeOFF, SocName, SocCmdOn, SocCmdOff, PropName, PropCmdOn, PropCmdOff, true){}

    virtual bool ActionBeforeOFF() 
    { 
        if(m_PropertyOnOFF == ST_ON)
        {
            if(m_currNetworkVolts == false)
                return true;
            else
               return false;
        }
        
        return true; 
    }
};


class CSocketWithIndicator :     public CSimpleSocet
{
public:
    virtual ~CSocketWithIndicator(){}

    CSocketWithIndicator(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff, short pinIndicator) : 
    CSimpleSocet(pinSoc, TimeOFF, SocName, SocCmdOn, SocCmdOff, PropName, PropCmdOn, PropCmdOff), m_indicator(pinIndicator) {}

    //check indicator On/Off
    virtual bool IsIndicatorOn() 
    { 
        return m_indicator.GetValue() == ON ? true: false; 
    }
    
    virtual void Update()
    {
        UpdateIndicator();
        CSimpleSocet::Update();
    }

    virtual void UpdateIndicator()
    {
        m_indicator.ReadData();
    }

protected:
    CDigitalValue m_indicator;
};


class CMultimediaSystem :     public CSocketWithIndicator
{
public:
    virtual ~CMultimediaSystem(){}

    CMultimediaSystem(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff, short pinIndicator) : 
        CSocketWithIndicator(pinSoc, TimeOFF, SocName, SocCmdOn, SocCmdOff, PropName, PropCmdOn, PropCmdOff, pinIndicator){}

    virtual bool ActionBeforeOn() 
    { 
        if(IsIndicatorOn())
            return false;        

        return true;  
    }
    
    virtual bool ActionBeforeOFF() 
    { 
        m_indicator.ReadData();
    
        if(!IsIndicatorOn())
            return false;        
        
        return true; 
    }

    virtual bool ActionAfterOFF() { 
        int n = 0;
        
        //waiting for system on for 10 sek
        while(n != 20)
        {
            delay(500);
            m_indicator.ReadData();

            if(!IsIndicatorOn())
                return true;
            n++;
        }
        return false;    
    }
};


class CUPSSocket : public CSocketWithIndicator
{
public:
    virtual ~CUPSSocket(){}

    CUPSSocket(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff, short pinIndicator) : 
    CSocketWithIndicator(pinSoc, TimeOFF, SocName, SocCmdOn, SocCmdOff, PropName, PropCmdOn, PropCmdOff, pinIndicator){}

    virtual bool ActionBeforeOn() 
    { 
        if(IsIndicatorOn())
            return true;     // OK, LET ON 
        return false;  
    }

	/*
    virtual bool ActionBeforeOFF() 
    { 
        m_indicator.ReadData();

        if(!IsIndicatorOn())
            return false;        

        return true; 
    }
	*/
    virtual bool ActionAfterOFF() 
	{ 
        int n = 0;

        //waiting for system on for 10 sek
        while(n != 20)
        {
            delay(500);
            m_indicator.ReadData();

            if(!IsIndicatorOn())
                return true;
            n++;
        }

        return false;   
    }
	
};


class CUPSSocket2 : public CSocketWithIndicator
{
public:
	virtual ~CUPSSocket2(){}

	CUPSSocket2(short pinSoc, unsigned int TimeOFF, const char* SocName, const char* SocCmdOn, const char* SocCmdOff, const char* PropName, const char* PropCmdOn, const char* PropCmdOff, short pinIndicator, short pinIndicator2) : 
	CSocketWithIndicator(pinSoc, TimeOFF, SocName, SocCmdOn, SocCmdOff, PropName, PropCmdOn, PropCmdOff, pinIndicator),
	m_indicator2(pinIndicator2) {}

	virtual bool ActionBeforeOn() 
	{ 
		if(IsIndicatorOn() && IsIndicator2On())
			return true;     // OK, LET ON 
		return false;  
	}
	

	virtual void Update()
	{
		UpdateIndicator();
		CSocketWithIndicator::Update();
	}

	virtual void UpdateIndicator()
	{
		m_indicator.ReadData();
		m_indicator2.ReadData();
	}


	//check indicator On/Off
	virtual bool IsIndicator2On() 
	{ 
		return m_indicator2.GetValue() == ON ? true: false; 
	}

protected:
	CDigitalValue m_indicator2;
	
};

#endif
