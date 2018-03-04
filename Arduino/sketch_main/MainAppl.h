
#include "ObjectConstructor.hpp"

#ifdef WIN32
#include "..\Nextion.h"
#else
    #include "Nextion.h"
#endif
#include "SerialCmd.h"
#include "BasesClasses.hpp"
#include "OneWireExt.hpp"
#ifndef WIN32
    #include "EmonLib.h" 
#endif // WIN32


class EnergyMonitor;

const int contMainItems = 7;

class CMainAppl : public CNexPageEx 
{
public:
      CMainAppl(const char* namePage);
    virtual ~CMainAppl(void);

    virtual void Process();
    virtual void ProcessCMD(sCmdWord & cmd);
	virtual void ProcessInvalidateCMD(sCmdInvalidate& cmd);


	sCmdWord m_incomingCmd[50];
	short m_currCMD;
	void ExecCMD();

	sCmdInvalidate m_incomingInvCmd[15];
	short m_currInvCMD;
	void ExecInvCMD();
	

    void ProcessSerial(CSendCmd &cmdArduino);
    
    virtual void Init();
    virtual void Show();
    virtual void ProcessInterface() {   nexLoop(this->m_nex_listen_list); } 
    
    
    ePageID ActivePage;
    void ShowByButton();
    static void ShowLight(void*);
    static void ShowPressAir(void*);
    static void ShowKlimate(void*);
    static void ShowElectric(void*);
    static void ShowSettings(void*);
    static void ShowDoor(void*);

      uint32_t getBrightness();
      void setBrightness(uint32_t);
      uint32_t m_curBrightness;    
      


    CObjectConstructor m_Constructor;
protected:
    NexButton *btnLight;
    NexButton *btnPressAir;
    NexButton *btnKlimate;
    NexButton *btnElectronikNetwork;
    NexButton *btnDoor;
    NexButton *btnSettings;

    NexTouch* m_nex_listen_list[contMainItems];
    short m_currListenItem;
        
    CSendCmd cmdArduino;
    CSendCmd cmdArduino2;
      
    void InvalidateAllSensors(NexText* pDate, NexText* pTime, NexText* pTempIn, NexText* pTempOut, 
                              NexText* pPress, NexText* pPower, NexText* pVoltage, NexText* pAmpers);
    
private:
    CDigitalSensor *m_screenSensor;
    bool               m_IsSleep;

    void ReadData(bool Force = false);

    iarduino_RTC m_DateTime; 
    void ReadAndSetDateAndTime();
    char m_date[12];
    char m_timer[6];

    CTempSensor_DS18B20 *m_TempSensor;
    DeviceAddress ThermometerIN;  
    DeviceAddress ThermometerOUT;
    
    bool ReadTempIn();
    char m_TempIn[9];

    bool ReadTempOut();
    char m_TempOut[9];

    bool ReadAQI(int value = -1);
    char m_AirIndex[8];

    //curr press
    char m_Press[8];

    EnergyMonitor m_voltSensor;
    bool ReadVoltageAC();
    char m_VoltageAC[5];

    float readVcc();
    bool ReadVoltageDC();
    char m_VoltageDC[5];

    void CMDExtFilter(sCmdWord &cmd);

	void ResetNextion();
	void InitNextionSerialSpeed();
};




