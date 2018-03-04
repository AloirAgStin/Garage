// Arduino.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <ctime>
#include <Windows.h>
void loop();
void setup();
#include <string>
#include <vector>

#include ".\sketch_light\SerialCmd.h"

void main() 
{
    setup();
    while(1)
    {
        loop();
        std::cout<< "--------------------------------------------" << std::endl;

        Sleep(1000);
    }
}


#include "sketch_light/PanelIncl.hpp"

#include "sketch_light/SerialController.h"


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
}

void loop() 
{ 
    m_panelLight->Process();
    m_panelElect->Process();

    if(!m_panelElect->IsAnySocketON())
        m_panelElect->TurnOFFCooler();

    m_panelPressAir->Process();

    controller.ProcessSerial(); //in the begin all command from 1 controller, then second
}