#ifndef MOTIONSENDER_HPP
#define MOTIONSENDER_HPP

#include "BasesClasses.hpp"

namespace
{
    #define DETECTED ON
    #define CLEAR     OFF
}

class CMotionSensor: public CDigitalSensor
{
    
public:
    CMotionSensor(short pin) 
        : CDigitalSensor(pin)
    {
        m_ForcedSignal = false;
    }

    void SetForceSignal(bool IsForce = true) { m_ForcedSignal = IsForce; }
    
    int ReadData()
    {     
        if(m_ForcedSignal)
        {
            m_ForcedSignal = false;
            m_value = 1;
        }
        else
            m_value = CDigitalSensor::ReadData();

        
#ifdef WIN32
        if(IsDetectedMotion())
        {
            char buff[100];
            sprintf(buff,"Motion detected %d", m_PIN);
            SMESN(buff);
        }
#endif
        return m_value; 
    } 
    bool IsDetectedMotion()
    {
        return m_value == DETECTED ? true : false; 
    }

private:
    bool m_ForcedSignal;
};

class CMicroWaveSensor: public CMotionSensor
{
public:
    CMicroWaveSensor(short pin) 
        : CMotionSensor(pin)
    {
        m_IsAllowlight = false;
    }

    void process()
    {
        ReadData();
        if(IsDetectedMotion())
            m_IsAllowlight = true;
        else
            m_IsAllowlight = false;
    }
    
    bool m_IsAllowlight;

};
#endif