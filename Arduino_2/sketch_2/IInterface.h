#ifndef IINTERFACE_H
#define IINTERFACE_H

enum eNotyfyAction 
{
    ACTION_OFF = 0,    
    ACTION_ON = 1,    
    ACTION_TIMER_TICK = 2,

    ACTION_MICROWAVE_ACCEPT,
    ACTION_MICROWAVE_DECLINE
    
};


class INotifyEvent
{
public:
    virtual void Notify(eNotyfyAction) = 0;
};

class IOnOff
{
public:
    //return true if state was chaged
    virtual bool On() = 0; 

    //return true if state was chaged
    virtual bool Off() = 0;
    
    virtual short GetPin() = 0;
};

class IOnOffNofity
{
public:
    virtual bool On(bool) = 0;
    virtual bool Off(bool) = 0;
    virtual eStatusOnOFF GetObjectPinStatus() = 0;
};

#endif
