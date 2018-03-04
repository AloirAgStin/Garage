#ifndef BASES_CLASSES
#define BASES_CLASSES

#include "GlobalDef.h"
#include "IInterface.h"
#include "CIDGen.h"

class CAObject
{
public:
    CAObject()
    {
        m_ID = CIDGen::GetID();
    }
    int GetID()
    {
        return m_ID;
    }

private:
    int m_ID;
};



//class for notify diff object
class CNotifyObjectList
{
    static const short maxNotifyElement = 10;
public:
    CNotifyObjectList()
    {
        m_CountNotObject = 0;
        for(int i = 0; i < CNotifyObjectList::maxNotifyElement; i++)
            m_arr[i] = 0;
    }

    ~CNotifyObjectList()
    {

    }

    void NotifyObjects(eNotyfyAction val)
    {
        for(int i = 0; i < m_CountNotObject; i++)
            m_arr[i]->Notify(val);
    }

    void AddNotifyObject(INotifyEvent *obj)
    {
        if(m_CountNotObject +1 >= CNotifyObjectList::maxNotifyElement)
            return;
        
        m_arr[m_CountNotObject] = obj;
        m_CountNotObject++;
    }

private:
    short m_CountNotObject;
    INotifyEvent* m_arr[CNotifyObjectList::maxNotifyElement];
};

//class for On/Off object PIN/*
class CDigitalOnOff : public IOnOff, public CAObject
{
public:
    CDigitalOnOff(short PIN) 
        : CAObject()
    {
        m_PIN = PIN;
        m_Status = ST_OFF;
        SetPinModeOutput(m_PIN);
        PINOFF(m_PIN);
    }

    bool On()
    {
        if(m_Status == ST_ON) return false;
        
        PINON(m_PIN);
        m_Status =  ST_ON;

        return true;
    }

    bool Off()
    {
        if(m_Status == ST_OFF) return false;

        PINOFF(m_PIN);
        m_Status =  ST_OFF;

        return true;
    }

    short GetPin()
    {
        return m_PIN;
    }

    eStatusOnOFF GetStatus()
    {
        return m_Status;    
    }
    
    eStatusOnOFF m_Status;

    virtual ~CDigitalOnOff(){}
    
protected:
    short m_PIN;
};


class CInverseDigitalOnOff : public IOnOff, public CAObject
{
public:
    CInverseDigitalOnOff (short PIN) 
        : CAObject()
    {
        m_PIN = PIN;
        m_Status = ST_OFF;
        SetPinModeOutput(m_PIN);
        PINON(m_PIN);
    }

    //On PIN
    bool On()
    {
        if(m_Status == ST_ON) return false; 
        PINOFF(m_PIN);
        m_Status =  ST_ON;

        return true;
    }

    //Off PIN
    bool Off()
    {
        if(m_Status == ST_OFF) return false;

        PINON(m_PIN);
        m_Status =  ST_OFF;
        
        return true;
    }

    short GetPin()
    {
        return m_PIN;
    }

    eStatusOnOFF GetStatus()
    {
        return m_Status;    
    }
    
    eStatusOnOFF m_Status;

    virtual ~CInverseDigitalOnOff(){}
    
protected:
    short m_PIN;
};

//class for read analog data from PIN
//return 0 - 1023
class CAnalogReader: public CAObject
{
public:
    CAnalogReader(short PIN)
        : CAObject()
    {
        m_PIN = PIN;
        SetPinModeInput(m_PIN);
    }

    virtual int ReadValue()
    {
        return A_READ(m_PIN);
    }

    virtual ~CAnalogReader(){}
protected:
    short m_PIN;
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
