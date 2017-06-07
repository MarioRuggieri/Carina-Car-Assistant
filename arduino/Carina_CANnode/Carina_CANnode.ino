// ------------------------------------------------------------------------
// Carina Assistant Car - CAN Node 
// ------------------------------------------------------------------------

// Made using ARD1939: SAE J1939 Protocol Stack for Arduino

//  This Arduino program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.

//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.

//  Authors: Vincenzo De Angelis, Gennaro Farina, Mario Ruggieri

#include <stdlib.h>
#include <inttypes.h>
#include <SPI.h>

#include "ARD1939.h"

// ECU NAME IDENTITY NUMBER Field 
#define MY_NAME_IDENTITY_NUMBER   0xFFFFFA

ARD1939 j1939;

void setup() 
{
  // Set the serial interface baud rate
  Serial.begin(9600);

  // HeadLight
  pinMode(A0,INPUT);
  
  // Initialize the J1939 protocol including CAN settings
  if(j1939.Init(SYSTEM_TIME) == 0)
    Serial.print("CAN Controller Init OK.\n\r\n\r");
  else
    Serial.print("CAN Controller Init Failed.\n\r");
    
 // Set the preferred address and address range
 j1939.SetPreferredAddress(SA_PREFERRED);
 j1939.SetAddressRange(ADDRESSRANGEBOTTOM, ADDRESSRANGETOP);
 
 // Set the NAME
 j1939.SetNAME(MY_NAME_IDENTITY_NUMBER,
               NAME_MANUFACTURER_CODE,
               NAME_FUNCTION_INSTANCE,
               NAME_ECU_INSTANCE,
               NAME_FUNCTION,
               NAME_VEHICLE_SYSTEM,
               NAME_VEHICLE_SYSTEM_INSTANCE,
               NAME_INDUSTRY_GROUP,
               NAME_ARBITRARY_ADDRESS_CAPABLE);
               
}// end setup

bool off = true, onFromCommand = false;
String incomingString;

// HeadLights handler
void loop()
{
  // J1939 Variables
  byte nMsgId;
  byte nDestAddr;
  byte nSrcAddr;
  byte nPriority;
  byte nJ1939Status;
  int lightVal;
  
  int nMsgLen;
  
  long lPGN;
  
  byte pMsg[J1939_MSGLEN];
  
  // Variables for proof of concept tests
  byte turnLightOn[] = {1};
  byte turnLightOff[] = {0};
  
  // Establish the timer base in units of milliseconds
  delay(SYSTEM_TIME);
  
  // Call the J1939 protocol stack
  nJ1939Status = j1939.Operate(&nMsgId, &lPGN, &pMsg[0], &nMsgLen, &nDestAddr, &nSrcAddr, &nPriority);
  
  // RTS/CTS Session
  if(nJ1939Status == NORMALDATATRAFFIC)
  {
    nSrcAddr = j1939.GetSourceAddress();
    
    if(Serial.available()) //message received from Python
    { 
      incomingString = Serial.readString();
      //Serial.println(incomingString);
      if(!strcmp(incomingString.c_str(), "turnOn"))
      { 
        j1939.Transmit(6, 59999, nSrcAddr, GLOBALADDRESS, turnLightOn, 1);
        onFromCommand = true;
        Serial.println("on_success");  
      }
      else if(!strcmp(incomingString.c_str(), "turnOff"))
      {
        j1939.Transmit(6, 59999, nSrcAddr, GLOBALADDRESS, turnLightOff, 1);
        onFromCommand = false;
        Serial.println("off_success");
      }
    }
    
    lightVal = analogRead(A0);
    if (!onFromCommand)
    {
      if (lightVal < 950 && off)
      {
        j1939.Transmit(6, 59999, nSrcAddr, GLOBALADDRESS, turnLightOn, 1);
        Serial.println("lightOn");
        off = false;
      }
      else if (lightVal > 950 && !off)
      {
        j1939.Transmit(6, 59999, nSrcAddr, GLOBALADDRESS, turnLightOff, 1);  
        Serial.println("lightOff");
        off = true;
      }
    }

  }
    
}


