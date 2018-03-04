#ifndef OBJECT_CONSTRUCTOR_HPP
#define OBJECT_CONSTRUCTOR_HPP

#include "NextionObject.h"
#include "SerialCmd.h"
#include "LightPanelController.h"
#include "ElecLightPanelController.h"
#include "PressairPanelController.h"
#include "KlimatePanelController.h"
#include "DoorPanelController.h"
#include "SettingsPanelController.h"

class CObjectConstructor
{
public:
    CSendCmd * m_SerialCmd;
    CSendCmd * m_SerialCmd2;
    CNexPageEx* m_currObject;
    CNexPageEx* m_MainPage;

    ePageID currType;

    CObjectConstructor(CSendCmd * _SerialCmd, CSendCmd * _SerialCmd2):
        m_SerialCmd(_SerialCmd),
        m_SerialCmd2(_SerialCmd2)
    {
        m_MainPage = nullptr;
        m_currObject = 0;
        currType = eMAIN;
    }    

    void SetParent(CNexPageEx *parent)
    {
        m_MainPage = parent;
    }
    ~CObjectConstructor()
    {
        delete_if_exists(m_currObject);
        m_currObject = 0;
        currType  = eMAIN;
    }
    
    CNexPageEx* GetObject(ePageID pageType)
    {
        if(pageType == currType && pageType != eMAIN && m_currObject != 0)
        {
           return m_currObject;
        }
        
        if(pageType != eMAIN )
        {           
            SMES("Curr memory:");
            SMESN(memoryFree());

            delete_if_exists(m_currObject);
            m_currObject = 0;

            SMES("Curr memory after clear:");
            SMESN(memoryFree());

        }
        
        switch(pageType)
        {
            case eLight:
                {
                    SMESN("BEGIN BUILD LIGHT");
                    m_currObject = new CLightPanelController(1, 0, "pageLight", m_SerialCmd);
                    m_currObject->Init();
                    currType = pageType;
                    break;                    
                }
            case ePressair:
                {  
                    SMESN("BEGIN BUILD PRESSIR");
                    m_currObject = new CPressairPanelController(2, 0, "pagePAir", m_SerialCmd);
                    m_currObject->Init();
                    currType = pageType;
                    break;
                }
            case eKlimate:
                {
                    SMESN("BEGIN BUILD KLIMATE");
                    m_currObject = new CKlimatePanelController(3, 0, "pageKlim", m_SerialCmd2);
                    m_currObject->Init();
                    currType = pageType;
                    break;
                }
            case eElectNetWork:
                {
                    SMESN("BEGIN BUILD ENETWORK");
                    m_currObject = new CElecLightPanelController(4, 0, "pageElec", m_SerialCmd);
                    m_currObject->Init();
                    currType = pageType;
                    break;
                }
            case eDoor:
                {
                    SMESN("BEGIN BUILD DOOR");
                    m_currObject = new CDoorPanelController(5, 0, "pageDoor", m_SerialCmd2);
                    m_currObject->Init();
                    currType = pageType;
                    break;
                }
            case eSettings:
                {
                    SMESN("BEGIN BUILD SETTINGS");
                    m_currObject = new CSettingsPanelController(6, 0, "pageSett", m_SerialCmd);
                    m_currObject->Init();
                    currType = pageType;
                    break;
                }
            case eMAIN:
            default:
                {
                    return m_MainPage;
                    break;
                }
        }

        SMES("Memory after build:");
        SMESN(memoryFree());

        return m_currObject;
    }


};

#endif
