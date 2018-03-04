#include "PanelIncl.hpp"
#include "SerialController.hpp"



CSerialController controller;

long CSerialController::ActivepaPage = eDoor; //todo

CDoorPanelController *m_panelDoor = 0;


void setup()
{
  controller.Init();

  m_panelDoor = new CDoorPanelController(&controller.cmdArduino);
  m_panelDoor->Init();

  SMES("Free memory after init:");
  SMESN(memoryFree());
  
}


void loop() 
{ 
  m_panelDoor->Process();
	controller.ProcessSerial(); //in the begin all command from 1 controller, then second
}
