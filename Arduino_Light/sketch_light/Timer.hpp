#ifndef TIMER_H
#define TIMER_H

#include "IInterface.h"
#include "GlobalDef.h"




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

    void SetParent(INotifyEvent *pParent)
    {
        m_Parent = pParent;
    }

    void StartTimer()     {     
        m_IsEnable = true;  
        m_lastEventTime = AppStartTime;
    }

    void KillTimer() { m_IsEnable = false; m_lastEventTime = 0; }

    void Update()
    {
        if(!m_IsEnable) return;

        uint32_t now = AppStartTime;
        Update(now);
    }

private:
    INotifyEvent *m_Parent;
    unsigned long m_period;
    unsigned long m_lastEventTime;
    bool m_IsEnable;

    void Update(unsigned long now)
    {
        if(!m_IsEnable) return;

        if (now - m_lastEventTime >= m_period)
        {
            #ifdef SHOW_TIMER
                        time_t seconds = time(NULL);
                        tm* timeinfo = localtime(&seconds);
                        cout<<"Notify time:"<<asctime(timeinfo);    
            #endif // WIN32    

            m_Parent->Notify(eNotyfyAction::ACTION_TIMER_TICK);
            m_lastEventTime = now;
        }

    }
};


#endif
