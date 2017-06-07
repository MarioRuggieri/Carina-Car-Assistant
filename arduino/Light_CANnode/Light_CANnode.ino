// ------------------------------------------------------------------------
// HeadLight CAN Node Simulator
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

ARD1939 j1939;

// LIGHT ECU NAME IDENTITY NUMBER Field 
#define MY_NAME_IDENTITY_NUMBER   0xFFFFFB

const int FARO_PIN = 2;

// ------------------------------------------------------------------------
//  Setup routine runs on power-up or reset
// ------------------------------------------------------------------------
void setup() 
{
  // Set the serial interface baud rate
  Serial.begin(MONITOR_BAUD_RATE);
  pinMode(FARO_PIN, OUTPUT);
  digitalWrite(FARO_PIN, LOW);
  
  // Initialize the J1939 protocol including CAN settings
  if(j1939.Init(SYSTEM_TIME) == 0)
    Serial.print("CAN Controller Init OK.\n\r\n\r");
  else
    Serial.print("CAN Controller Init Failed.\n\r");
    
 // Set the preferred address and address range
 j1939.SetPreferredAddress(SA_PREFERRED);
 j1939.SetAddressRange(ADDRESSRANGEBOTTOM, ADDRESSRANGETOP);
 
 // Set the message filter
 j1939.SetMessageFilter(59999);
 
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


void loop()
{
  // J1939 Variables
  byte nMsgId;
  byte nDestAddr;
  byte nSrcAddr;
  byte nPriority;
  byte nJ1939Status;
  
  int nMsgLen;
  long lPGN;
  byte pMsg[J1939_MSGLEN];
  
  // Establish the timer base in units of milliseconds
  delay(SYSTEM_TIME);
  
  // Call the J1939 protocol stack
  nJ1939Status = j1939.Operate(&nMsgId, &lPGN, &pMsg[0], &nMsgLen, &nDestAddr, &nSrcAddr, &nPriority);

  // Check for reception of PGNs for our ECU/CA
  if(nMsgId == J1939_MSG_APP)
  {
    // Check J1939 protocol status
    if (nJ1939Status == NORMALDATATRAFFIC)
    {
        // TP PROTOCOL
        for (int i=0; i<nMsgLen; i++)
        {
          Serial.println(pMsg[i]);
        }
        Serial.println("---END PACKET---");

        // Light handler
        if (nMsgLen == 1)
        {
          if( pMsg[0])
            digitalWrite(FARO_PIN, HIGH);
          else if( !pMsg[0])
            digitalWrite(FARO_PIN, LOW);  
        }
    }
    
  }// end if
    
}// end loop


