// Arduino.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <ctime>
#include <Windows.h>
void loop();
void setup();


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


#include "sketch_2/PanelIncl.hpp"
#include "sketch_2/SerialController.hpp"



CSerialController controller;

long CSerialController::ActivepaPage = eLight; //todo

CDoorPanelController *m_panelLight = 0;


void setup()
{
    controller.Init();

    m_panelLight = new CDoorPanelController(&controller.cmdArduino);
    m_panelLight->Init();

}


void loop() 
{ 
	m_panelLight->Process();
	controller.ProcessSerial(); //in the begin all command from 1 controller, then second
}