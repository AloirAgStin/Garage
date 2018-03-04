#ifndef LIGHT_ITEMS_H
#define LIGHT_ITEMS_H

#include "GlobalDef.h"
#include "Timer.hpp"
#include "MotionSensor.hpp"
#include "NextionObject.h"


#ifdef WIN32
    #include "../stdafx.h"
#endif

class CLightSwitch;
class CLightSwitchSensor;

//bulb
class CLightBulb :public CInverseDigitalOnOff
{
    friend class CLightSwitch;
    friend class CLightSwitchSensor;
public:
    CLightBulb():CInverseDigitalOnOff(0){}
    CLightBulb(short pin):CInverseDigitalOnOff(pin) { }

};

//switch with bulb
class CLightSwitch: 
    public IOnOffNofity,
    public CNotifyObjectList
{
    friend class CLightSwitchSensor;

public:
    CLightSwitch() : m_Lamp(0), m_nextionButton("","","")
    {

    }

    CLightSwitch(short pin) : 
      m_Lamp(pin),
      m_nextionButton("","","")
      {
    
      }

      CLightSwitch(short pin, const char*Name, const char* cmdOn, const char* cmdOff) : 
      m_Lamp(pin),
      m_nextionButton(Name, cmdOn, cmdOff)
      {
    
      }

      virtual ~CLightSwitch() {}
        
      int GetID()  { return m_Lamp.GetID();}
      int GetPin() { return m_Lamp.m_PIN;}
     
      void OnOFF(short IsOn)
      {
          if(IsOn == 1)
              On();
          else
              Off();
      }

      bool On(bool IsNotify = true)      
      {
          #ifdef WIN32
              char buff[256];
              sprintf(buff,"Lamp %d is on", m_Lamp.m_PIN);    
              SMESN(buff);
          #endif
          delay(30); //todo lamp deleay
          if(IsNotify)
              NotifyObjects(eNotyfyAction::ACTION_ON);
          return m_Lamp.On();
      }

      bool Off(bool IsNotify = true)
      {
            #ifdef WIN32
                char buff[256];
                sprintf(buff,"Lamp %d is off", m_Lamp.m_PIN);    
                SMESN(buff);
            #endif
                
          delay(30); //todo lamp deleay
          bool Ret = m_Lamp.Off();
          if(IsNotify)
              NotifyObjects(eNotyfyAction::ACTION_OFF);

          return Ret;          
      }
      
      eStatusOnOFF GetObjectPinStatus()
      {
          return m_Lamp.GetStatus();
      }

      CNextionDSObject m_nextionButton;

      sCmdWord* GetCmdForInvalidate()
      {
          int act = m_Lamp.GetStatus() == ST_ON ? 1 : 0;
          return m_nextionButton.GetCmdByValue(act);
      }


protected:
    CLightBulb m_Lamp;
    
};


class CLightSwitchSensor:  
    protected CLightSwitch, 
    public INotifyEvent
{
    friend class CConverter2;
public:
    CNextionDSObject m_btnBulb;
    CNextionDSObject m_btnSensors;
    
    enum eStatusSwitcher
    {
        ST_OFF = 0,
        ST_ON = 1,
        ST_AUTO = 2
    };

    enum eIllumination
    {
        UNKNOW,
        LIGHT,
        DARK,
    };

    CLightSwitchSensor() : 
        m_btnBulb("","",""),
        m_btnSensors("","",""),
        m_sensor(0)
    {
        m_timer = 0;
        m_status = ST_OFF;
        m_IllStatus = CLightSwitchSensor::UNKNOW;
    }

    CLightSwitchSensor(short pinLight, short SensorPin, unsigned int Time, 
        const char* LampName, const char* LampCmdOn, const char* LampCmdOff, const char* sensorName, const char* sensorCmdOn, const char* sensorCmdOff ) : 
        CLightSwitch(pinLight),
        m_sensor(SensorPin),
        m_btnBulb(LampName, LampCmdOn, LampCmdOff),
        m_btnSensors(sensorName, sensorCmdOn, sensorCmdOff)
    {
        m_timer = new CTimer(Time);
        m_status = ST_OFF;
        m_IllStatus = CLightSwitchSensor::UNKNOW;
    }
    
    int GetID()  { return m_Lamp.GetID();}
    eStatusOnOFF GetPinStatus() { return GetObjectPinStatus(); }

    sCmdWord* GetCmdForInvalidateBtn()
    {
      int act = m_Lamp.GetStatus() == eStatusOnOFF::ST_ON ? 1 : 0;
      return m_btnBulb.GetCmdByValue(act);
    }
    
    sCmdWord* GetCmdForInvalidateSens()
    {
      int act = GetStatus() == eStatusSwitcher::ST_AUTO ? 1 : 0;
      return m_btnSensors.GetCmdByValue(act);
    }
    

    void ForceSensorSignal() 
    {
        m_sensor.SetForceSignal();
    }

    void OnOFF(short IsOn)
    {
        if(IsOn == 1)
            On();
        else
            Off();
    }

    bool On(bool IsNotify = true)      
    {
        m_timer->KillTimer();
        m_status = ST_ON;
        return CLightSwitch::On(IsNotify);
    }

    bool Off(bool IsNotify = true)
    {
        m_timer->KillTimer();
        m_status = ST_OFF;
        return CLightSwitch::Off(IsNotify);
    }

    void SetAuto()
    {
        m_status = ST_AUTO;
    }

    void Update()
    {
        m_sensor.ReadData();
        
        if(GetStatus() == ST_AUTO)
        {
            if(m_IllStatus == CLightSwitchSensor::LIGHT)
            {
                m_timer->KillTimer();
                CLightSwitch::Off(true);
                return;
            }

            if(m_sensor.IsDetectedMotion())
            {
                m_timer->StartTimer();
                CLightSwitch::On();        
            }
            m_timer->Update();    
        }
    }

    eStatusSwitcher GetStatus()
    {
        return m_status;
    }


#ifdef WIN32
    void AskStatusForLog()
    {
#ifdef WIN32
        char buff[30];
        sprintf(buff,"Lamp status %d is %s", m_Lamp.m_PIN, m_Lamp.GetStatus() == ST_ON ? "On" : "Off");
        SMESN(buff);
#endif 
    }
#endif

    void Notify(eNotyfyAction val)
    {
        switch(val)
        {
        case ACTION_TIMER_TICK:
            {
                if(GetStatus() != ST_AUTO) return;
                SMES("Shut down Light ");
                SMES(" ");                    
                SMESN(m_Lamp.m_PIN);
                    
                CLightSwitch::Off();
                m_timer->KillTimer();
                break;
            }
        }
    }

    void AddNotifyToObject(INotifyEvent *obj)
    {
        CLightSwitch::AddNotifyObject(obj);    
    }

    bool IsMotionDeteted()
    {
        return m_sensor.IsDetectedMotion();
    }

    eIllumination m_IllStatus;
    CTimer * m_timer;
private:
    eStatusSwitcher m_status;
    CMotionSensor m_sensor;

};


    
#endif
