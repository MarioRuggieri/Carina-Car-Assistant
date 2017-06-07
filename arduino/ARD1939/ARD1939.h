#ifndef ARD1939_H
#define ARD1939_H

// Arduino Definitions
#define MONITOR_BAUD_RATE                      115200
#define CS_PIN                                 10//9     // Use pin 10 for Seeed Studio CAN Shield up to version 1.0
                                                     // Use pin 9 for Seeed Studio CAN Shield version 1.1 and higher

// System Settings
#define SYSTEM_TIME                             1    // Milliseconds

// Program Version
// -----------------------------------------------
// 0 - ARD1939-Uno
// 1 - ARD1939-Uno/TP
// 2 - ARD1939-Mega
#define ARD1939VERSION                          1

// J1939 Settings
#if ARD1939VERSION == 0
  #define TRANSPORT_PROTOCOL                    0
  #define J1939_MSGLEN                          8
  #define MSGFILTERS                            10
#endif

#if ARD1939VERSION == 1
  #define TRANSPORT_PROTOCOL                    1
  #define J1939_MSGLEN                          256
  #define MSGFILTERS                            10
#endif

#if ARD1939VERSION == 2
  #define TRANSPORT_PROTOCOL                    1
  #define J1939_MSGLEN                          1785
  #define MSGFILTERS                            100
#endif

#define SA_PREFERRED                        128
#define ADDRESSRANGEBOTTOM                  129
#define ADDRESSRANGETOP                     247

#define GLOBALADDRESS                    	255
#define NULLADDRESS                      	254

// NAME Fields Default 
#define NAME_IDENTITY_NUMBER              	0xFFFFFF
#define NAME_MANUFACTURER_CODE            	0xFFF
#define NAME_FUNCTION_INSTANCE            	0
#define NAME_ECU_INSTANCE                 	0x01
#define NAME_FUNCTION                     	0xFF
#define NAME_RESERVED                     	0
#define NAME_VEHICLE_SYSTEM               	0x7F
#define NAME_VEHICLE_SYSTEM_INSTANCE      	0
#define NAME_INDUSTRY_GROUP               	0x00
#define NAME_ARBITRARY_ADDRESS_CAPABLE    	0x01

// Return Codes
#define ADDRESSCLAIM_INIT                       0
#define ADDRESSCLAIM_INPROGRESS           	1
#define ADDRESSCLAIM_FINISHED             	2
#define NORMALDATATRAFFIC                 	2
#define ADDRESSCLAIM_FAILED               	3

#define J1939_MSG_NONE                   	0
#define J1939_MSG_PROTOCOL               	1
#define J1939_MSG_NETWORKDATA                   2
#define J1939_MSG_APP                   	3

// Compiler Settings
#define OK                                     0
#define ERR                                    1

// Debugger Settings
#define DEBUG                                  0

#if DEBUG == 1

  #define DEBUG_INIT() char sDebug[128];
  #define DEBUG_PRINTHEX(T, v) Serial.print(T); sprintf(sDebug, "%x\n\r", v); Serial.print(sDebug);
  #define DEBUG_PRINTDEC(T, v) Serial.print(T); sprintf(sDebug, "%d\n\r", v); Serial.print(sDebug);
  #define DEBUG_PRINTARRAYHEX(T, a, l) Serial.print(T); if(l == 0) Serial.print("Empty.\n\r"); else {for(int x=0; x<l; x++){sprintf(sDebug, "%x ", a[x]); Serial.print(sDebug);} Serial.print("\n\r");}
  #define DEBUG_PRINTARRAYDEC(T, a, l) Serial.print(T); if(l == 0) Serial.print("Empty.\n\r"); else {for(int x=0; x<l; x++){sprintf(sDebug, "%d ", a[x]); Serial.print(sDebug);} Serial.print("\n\r");}
  #define DEBUG_HALT() while(Serial.available() == 0); Serial.setTimeout(1); Serial.readBytes(sDebug, 1);

#endif

struct v35
{
  int v36;
  bool v21;
  bool v37;
};

class ARD1939
{
  public: 
    // Initialization
    byte Init(int nSystemTime);
    void SetPreferredAddress(byte nAddr);
    void SetAddressRange(byte nAddrBottom, byte nAddrTop);
    void SetNAME(long lIdentityNumber, int nManufacturerCode, byte nFunctionInstance, byte nECUInstance, 
                           byte nFunction, byte nVehicleSystem, byte nVehicleSystemInstance, byte nIndustryGroup, byte nArbitraryAddressCapable);
    byte SetMessageFilter(long lPGN);
  
    // Read/Write - Check Status
    byte Operate(byte* nMsgId, long* lPGN, byte* pMsg, int* nMsgLen, byte* nDestAddr, byte* nSrcAddr, byte* nPriority);
    byte Transmit(byte nPriority, long lPGN, byte nSourceAddress, byte nDestAddress, byte* pData, int nDataLen);
  
    // Other Application Functions
    void Terminate(void);
    byte GetSourceAddress(void);
    void DeleteMessageFilter(long lPGN);
    
  private:
    byte f01(byte, byte*);
    bool f02(void);
    byte f03(byte*, byte*);
    byte f04(long*, byte*, int*, byte*, byte*, byte*);
    void f05(void);
    void f06(struct v35*);
    bool f07(long*, byte*);
    bool f08(long);
    bool f09(long);

#if TRANSPORT_PROTOCOL == 1
    byte f10(byte, long, byte, byte, byte*, int);
    void f11(byte);
    void f12(byte);
    byte f13(long, byte*, int, byte, byte, byte);
#endif
  
}; // end class ARD1939

#endif
