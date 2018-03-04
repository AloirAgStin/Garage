// Arduino.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <ctime>
#include <Windows.h>
#include "StdAfx.h"
#include <string>
#include <vector>

void loop();
void setup();

#include <stdio.h>

void main() 
{
    setup();
    while(1)
    {
        loop();
        std::cout<< "--------------------------------------------" << std::endl;

        Sleep(2000);
        //system("pause");
    }
}

#include "Nextion.h"
#include "sketch_main/MainAppl.h"
#include "sketch_main/OneWireExt.hpp"

CMainAppl *app = 0;

void setup()
{
   nexInit();
   app = new CMainAppl("pageMain");  
   app->Init();

   app->ShowLight(app);


   int symbCount = 10;
	int curv= 0;
   while(symbCount > curv)
   {
	   delay(1);
	   curv++;

   }


	return;
	/*
    for(int i = 0; i < data.size(); i++)
    {
        const char * szName = data[i].c_str();
        
        sCmdWord w;
        strcpy(w.buff, szName);
        /*if(w.IsCommand(KL_TEXT_EXHAUST))
            w= sCmdWord(KL_TEXT_EXHAUST, 100);
        else4
        if(w.IsCommand(KL_TEXT_HEATING))
            w= sCmdWord(KL_TEXT_HEATING, 200.9, 1);
    
        
        app->ProcessCMD(w);
   }*/
    
}


void loop() 
{ 
    app->Process();
}
