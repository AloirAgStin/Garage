#ifndef CONVERTER_HPP
#define CONVERTER_HPP


#include "GlobalDef.h"
#include "LightItems.h"

//преобразователь напряжения на 2 элемента
class CConverter2 : public CInverseDigitalOnOff, public INotifyEvent
{
	public:
		CConverter2(short PIN) : CInverseDigitalOnOff(PIN) {
			m_obj1 = 0;
			m_obj2 = 0;
		}
	  
	void SetSens1(IOnOffNofity * Item) { m_obj1 = Item; }
	void SetSens2(IOnOffNofity* Item) { m_obj2 = Item; }
	
	short SetObjectOwner(IOnOffNofity* Item)
	{
		if(!m_obj1)
			m_obj1 = Item;
		else
		if(!m_obj2)
			m_obj2 = Item;
		else
			return 0;

		return 1;
	}

#ifdef WIN32
	void AskStatusForLog()
	{
#ifdef WIN32
		char buff[30];
		sprintf(buff,"Convertor %d is %s", m_PIN, GetStatus() == ST_ON ? "On" : "Off");
		SMES(buff);
#endif 
	}
#endif

private:
	void Notify(eNotyfyAction val) 
	{
		switch(val)
		{
			case ACTION_ON:
			{
				OnOn();
				break;
			}
			case ACTION_OFF:
				{
					if(m_obj1 && m_obj1->GetObjectPinStatus() == ST_ON)
						return;

					if(m_obj2 && m_obj2->GetObjectPinStatus() == ST_ON)
						return;

					OnOff();
					break;
				}

		}

	}

	void OnOn()
	{
		SMES("Convertor is on");
		if(GetStatus() == ST_OFF)
		{
			On();
			delay(100);
		}
	}

	void OnOff()
	{
		SMES("Convertor is off");
		if(GetStatus() == ST_ON)
		{
			Off();
			delay(100);
		}
	}

	IOnOffNofity * m_obj1;
	IOnOffNofity * m_obj2;
};


#endif