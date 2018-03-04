#ifndef BASES_CLASSES
#define BASES_CLASSES

#include "GlobalDef.h"

#define SENSOR_DETECTED ON
#define SENSOR_CLEAR OFF


class CTimer 
{
public:
    CTimer(unsigned long period)
    {
        m_period = period;

        m_lastEventTime = 0;
        m_IsEnable = false;
    }
    bool IsEnable() { return m_IsEnable; }

    void KillTimer()
    {
        m_IsEnable = false;
        m_lastEventTime = 0;
    }

    void StartTimer()     
    {     
        m_IsEnable = true;  
        m_lastEventTime = AppStartTime;
    }

    bool Update()
    {
        if(!m_IsEnable) return false;

        unsigned long  now = AppStartTime;
        return Update(now);
    }

private:
    unsigned long  m_period;
    unsigned long  m_lastEventTime;
    bool m_IsEnable;

    bool Update(unsigned long now)
    {
        if(!m_IsEnable) return false;

        if (now - m_lastEventTime >= m_period)
        { 
            m_lastEventTime = now;
            return true;
        }

        return false;
    }
};

class CAObject
{
public:
    CAObject()
    {
    }
private:
};



//class for Read data from PIN
//Result: 0 or 1
class CDigitalReader: public CAObject
{
protected:
    CDigitalReader(short PIN)
         : CAObject()
            
    {
        m_PIN = PIN;
        SetPinModeInput(m_PIN);
    }

    virtual int ReadValue()
    {
        return D_READ(m_PIN);
    }

    virtual ~CDigitalReader(){}
protected:
    short m_PIN;
};

class CDigitalSensor : public CDigitalReader
{
public:
    CDigitalSensor(short PIN) : CDigitalReader(PIN)
    {
        m_value = 0;
    }

    virtual int ReadData()
    {
        m_value = ReadValue();
        return m_value;
    }

    bool IsDetectedMotion()
    {
        return m_value == SENSOR_DETECTED ? true : false; 
    }
protected:
    int m_value;
};


class CDigitalValue : public CDigitalSensor
{
public:
    CDigitalValue(short PIN) : CDigitalSensor(PIN) {}
    int GetValue() { return m_value;}
};

#endif // end BASES_CLASSES
