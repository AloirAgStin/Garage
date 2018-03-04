
#include "arduino.h"

#include "LightPanelController.h"
#include "SerialController.h"

CSerialController controller;

CLightPanelController *m_panelLight = 0;
CElecPanelController *m_panelElect = 0;
CPressAirPanelController *m_panelPressAir = 0;

void setup()
{
	controller.Init();

	m_panelLight = new CLightPanelController(&controller.cmdArduino);
	m_panelLight->Init();
	
	m_panelElect = new CElecPanelController(&controller.cmdArduino);
	m_panelElect->Init();

	m_panelPressAir = new CPressAirPanelController(&controller.cmdArduino);
	m_panelPressAir->Init();

	SMES("Free memory after init:");
	SMESN(memoryFree());
}


void loop() 
{ 
  delay(50);
	m_panelLight->Process();
	m_panelElect->Process();

	if(!m_panelElect->IsAnySocketON())//todo add contorl for pechka 
		m_panelElect->TurnOFFCooler();

	m_panelPressAir->Process();

	controller.ProcessSerial(); //in the begin all command from 1 controller, then second
}
