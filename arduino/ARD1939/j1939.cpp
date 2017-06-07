#include <stdlib.h>
#include <inttypes.h>
#include <SPI.h>

#include "ARD1939.h"

#define d49                             10

#define d50                              8

#define d31               	0x00EA00
#define d32         	0x00EAFF
#define d46                   0x00
#define d47                   0xEA
#define d48                   0x00

#define d29               	0x00EE00
#define d30         	0x00EEFF
#define d25                   0x00
#define d26            	0xEE
#define d27                   0x00
#define d28            	6

#define d33                  	0x00EE00
#define d34             	0x00FED8

#define d35                              0x00ECFF
#define d36				0x00EC00
#define d37                           0xEC
#define d38                          7

#define d39				0x00EB00
#define d40                          7

#define d41			32
#define d42			16
#define d43			17
#define d44			19
#define d45			255

#define d51			255

struct v01
{
  bool bActive;
  long v25;
};
v01 v02[MSGFILTERS];

unsigned char v03[] =
{
  (byte)(NAME_IDENTITY_NUMBER & 0xFF),
  (byte)((NAME_IDENTITY_NUMBER >> 8) & 0xFF),
  (byte)(((NAME_MANUFACTURER_CODE << 5) & 0xFF) | (NAME_IDENTITY_NUMBER >> 16)),
  (byte)(NAME_MANUFACTURER_CODE >> 3),
  (byte)((NAME_FUNCTION_INSTANCE << 3) | NAME_ECU_INSTANCE),
  (byte)(NAME_FUNCTION),
  (byte)(NAME_VEHICLE_SYSTEM << 1),
  (byte)((NAME_ARBITRARY_ADDRESS_CAPABLE << 7) | (NAME_INDUSTRY_GROUP << 4) | (NAME_VEHICLE_SYSTEM_INSTANCE))
};

#define v04                     4
long v05[] =
{
  d29,
  d34,
  d36,
  d39
};

struct v06
{
  bool v07;
  byte v08;
  bool v09;
  bool v10;
  bool v11;
  byte v12;
  byte v13;
  byte v14;
  bool v15;
  byte v16;
  byte v17;
};
struct v06 v18;


#if TRANSPORT_PROTOCOL == 1
  #define d01        0
  #define d02          1
  #define d03          2
  
  struct v19
  {
    byte v20;
    bool v21;
    bool v22;
    bool v23;
    bool v24;
    long v25;
    byte v26;
    byte v27;
    byte v28;
    byte v29;
    byte v30[J1939_MSGLEN];
    int v31;
    byte v32;
  };
  v19 v33;
  v19 v34;
  
  #define d04            32
  #define d05            16
  #define d06            17
  #define d07    19
  #define d08        255
  
  #define d09        1
  #define d10    2
  #define d11     3
  
  #define d12	       0
  #define d13      1

  byte v62[] = {0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00};
  byte v63[8];
#endif

struct v35 v38;
struct v35 v39;

#if TRANSPORT_PROTOCOL == 1
  struct v35 v40;
  struct v35 v41;
  
  struct v35 v42;
  struct v35 v43;
  struct v35 v44;
  struct v35 v45;
  struct v35 v46;
  struct v35 v47;
  struct v35 v48;
#endif

int v49;
int v50;

#if TRANSPORT_PROTOCOL == 1
  int v51;
  int v52;
  
  int v53;
  int v54;
  int v55;
  int v56;
  int v57;
  int v58;
  int v59;
#endif

#define d14      	250
#define d15       	100

#if TRANSPORT_PROTOCOL == 1
  #define d16	50
  #define d17	750
  
  #define d18     50      
  #define d19		200
  #define d20		500
  #define d21	750
  #define d22		1250
  #define d23		1250
  #define d24	1050
#endif

int v60[] = {8, 9, 10, 12, 15};
byte v61;

byte v64;
extern byte canInit(void);
extern byte canCheckError(void);
extern byte canTransmit(long, unsigned char*, int);
extern byte canReceive(long*, unsigned char*, int*);

byte ARD1939::Init(int v80)
{
  int v65;
  f06(&v38);
  f06(&v39);
  v49 = d14 / v80;
  v50 = d15 / v80;
  v61 = 0;
  v60[0] = (int)8 / v80;
  v60[1] = (int)9 / v80;
  v60[2] = (int)10 / v80;
  v60[3] = (int)12 / v80;
  v60[4] = (int)15 / v80;
  v18.v07 = false;
  v18.v08 = false;
  v18.v09 = false;
  v18.v10 = false;
  v18.v11 = false;
  v18.v12 = SA_PREFERRED;
  v18.v13 = NULLADDRESS;
  v18.v14 = NULLADDRESS;
  v18.v15 = false;
  v18.v16 = NULLADDRESS;
  v18.v17 = NULLADDRESS;
  for(v65 = 0; v65 < MSGFILTERS; v65++)
  {
    v02[v65].bActive = false;
    v02[v65].v25 = 0;    
  }
  
#if TRANSPORT_PROTOCOL == 1
  v51 = d16 / v80;
  v52 = d17 / v80;

  v53 = d18 / v80;
  v56 = d21 / v80;
  v54 = d19 / v80;
  v55 = d20 / v80;
  v56 = d21 / v80;
  v57 = d22 / v80;
  v58 = d23 / v80;
  v59 = d24 / v80;
  f11(d12);
  f12(d12);
#endif
    
  v64 = 0;
  return canInit();
}

void ARD1939::Terminate(void)
{
  Init(SYSTEM_TIME);
}

byte ARD1939::Operate(byte* v70, long* v25, byte* v71, int* v31, byte* v67, byte* v66, byte* v76)
{
  byte v68;
  byte v82;
  int v65;
  f05();
  *v70 = f04(v25, &v71[0], v31, v67, v66, v76);
  if(*v70 == J1939_MSG_APP)
  {
    if(*v67 != v18.v17
    && *v67 != GLOBALADDRESS)
      *v70 = J1939_MSG_NETWORKDATA;
  }
  if(v18.v08 == true)
  {
    v68 = NORMALDATATRAFFIC;
    switch(*v25)
    {
      case d31:
        if(*v70 == J1939_MSG_PROTOCOL)
        {
          if(v71[0] == d27
          && v71[1] == d26
          && v71[2] == d25)
          {
            if(*v67 == GLOBALADDRESS)
            {
              Transmit(d28, d29, v18.v17, GLOBALADDRESS,
                            &v03[0], 8);
            }
            else if(*v67 == v18.v17)
            {
              Transmit(d28, d29, v18.v17, *v66,
                            &v03[0], 8);
            }
          }
        }
        break;
      
      case d29:
        if(*v66 == v18.v17)
        {
          v82 = f03(&v71[0], &v03[0]);
          switch(v82)
          {
            case 0:      
#if TRANSPORT_PROTOCOL == 1          
              f11(d12);
              f12(d12);
#endif           
              Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
              v18.v08 = false;
              v18.v11 = true;
              v68 = ADDRESSCLAIM_FAILED;
              break;             
            case 1:     
#if TRANSPORT_PROTOCOL == 1           
              f11(d12);
              f12(d12);
#endif           
              v18.v07 = false;
              v18.v08 = false;
              f01(*v66, &v03[0]);
              break;          
            case 2:  
              Transmit(d28, d29, v18.v17,
                            GLOBALADDRESS, &v03[0], 8);
              break;
          }
        }
        break;
      case d34:
        break;
    }
    
#if TRANSPORT_PROTOCOL == 1
    f13(*v25, v71, *v31, *v67, *v66, *v76);
    if(*v70 == J1939_MSG_NONE)
    {
    	  if(v33.v32 == true)
    	  {
    		  *v25 = v33.v25;
    		  *v31 = v33.v31;
    		  *v67 = v33.v27;
    		  *v66 = v33.v26;
    		  *v76 = d51;
    		  for(v65 = 0; v65 < v33.v31; v65++)
    			  v71[v65] = v33.v30[v65];
    		  f11(d12);
                  *v70 = J1939_MSG_APP;
    	  }
    	  else if(v34.v32 == true)
    	  {
    		  *v25 = v34.v25;
    		  *v31 = v34.v31;
    		  *v67 = v34.v27;
    		  *v66 = v34.v26;
    		  *v76 = d51;
    		  for(v65 = 0; v65 < v34.v31; v65++)
    			  v71[v65] = v34.v30[v65];
    		  f12(d12);
                  *v70 = J1939_MSG_APP;
    	  }
    }
#endif
  }
  else if(v18.v11 == true)
  {
    v68 = ADDRESSCLAIM_FAILED;
    switch(*v25)
    {
      case d31:
        if(*v70 == J1939_MSG_PROTOCOL)
        {
          if(v71[0] == d27
          && v71[1] == d26
          && v71[2] == d25)
          {
            if(*v67 == GLOBALADDRESS)
            {
              v39.v36 = v60[v61++];
              v39.v21 = true;
              if(v61 > 4)
                v61 = 0;
            }
          }
        }
        break;
      case d29:
        break;
      case d34:
        break;
    }
    if(v39.v37 == true)
    {
      f06(&v39);
       Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS,
                    &v03[0], 8);
    }
  }
  else
  {
    v68 = f01(*v66, &v71[0]);
  }
  return v68;
}

byte ARD1939::f01(byte v16, byte* v91)
{
  byte v68;
  byte v82;
  v68 = ADDRESSCLAIM_INPROGRESS;
  if(v18.v11 == true)
    v68 = ADDRESSCLAIM_FAILED;
  else if(v18.v08 == true)
    v68 = ADDRESSCLAIM_FINISHED;
  else if(v18.v10 == true)
  {
    if(v39.v37 == true)
    {
        f06(&v39);
        Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                      &v03[0], 8);
        v38.v36 = v49;
        v38.v21 = true;
        v18.v10 = false;
    }
  }
  else
  {
    if(v18.v07 == false)
    {
      if(f02() == true)
      {
        Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                      &v03[0], 8);
        v38.v36 = v49;
        v38.v21 = true;
        v18.v07 = true;
      }
      else
      {
        Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
        v18.v08 = false;
        v18.v11 = true;
        v68 = ADDRESSCLAIM_FAILED;
      }
    }
    else
    {
      if(v38.v37 == true)
      {
        f06(&v38);
        v18.v17 = v18.v16;
        v18.v08 = true;
        v68 = ADDRESSCLAIM_FINISHED;
      }
      else
      {
        if(canCheckError() == 1)
        {
          f06(&v38);
          if(++v64 == d49)
          {
            v18.v08 = false;
            v18.v11 = true;
            v68 = ADDRESSCLAIM_FAILED;            
          }
          else
          {
            canInit();
            v39.v36 = v60[v61++];
            v39.v21 = true;
            if(v61 > 4)
              v61 = 0;
            v18.v10 = true;
          }
        }
        else
          v64 = 0;
        if(v16 == v18.v16)
        {
          v82 = f03(&v91[0], &v03[0]);
          switch(v82)
          {
            case 0:
#if TRANSPORT_PROTOCOL == 1            
              f11(d12);
              f12(d12);
#endif           
              Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS, &v03[0], 8);
              v18.v08 = false;
              v18.v11 = true;
              v68 = ADDRESSCLAIM_FAILED;
              break;       
            
              case 1:
#if TRANSPORT_PROTOCOL == 1         
              f11(d12);
              f12(d12);
#endif           
              if(f02() == true)
              {
                Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                              &v03[0], 8);
                v38.v36 = v49;
                v38.v21 = true;
              }
              else
              {
                Transmit(d28, d29, NULLADDRESS, GLOBALADDRESS,
                              &v03[0], 8);
                v18.v08 = false;
                v18.v11 = true;
                v68 = ADDRESSCLAIM_FAILED;
              }
              break;

            case 2:
              Transmit(d28, d29, v18.v16, GLOBALADDRESS,
                            &v03[0], 8);
              v38.v36 = v49;
              v38.v21 = true;
              break;
          }
        }
      }
    }
  }
  return v68;
}

bool ARD1939::f02(void)
{
  bool v72;
  v72 = true;
  if(v18.v12 == NULLADDRESS)
    v18.v09 = true;
  if(v18.v09 == false)
  {
    v18.v16 = v18.v12;
    v18.v09 = true;
  }
  else
  {
    if(v18.v13 == NULLADDRESS || v18.v14 == NULLADDRESS)
    {
      v72 = false;
    }
    else
    {
      if(v18.v16 == NULLADDRESS || v18.v15 == false)
      {
        v18.v16 = v18.v13;
        v18.v15 = true;
      }
      else
      {
        if(v18.v16 < v18.v14)
          v18.v16++;
        else
          v72 = false;
      }
      if(v18.v16 == v18.v12)
      {
        if(v18.v16 < v18.v14)
          v18.v16++;
        else
          v72 = false;
      }
    }
  }
  return v72;
}

byte ARD1939::f03(byte* v92, byte* v93)
{
  byte v65;
  for(v65 = 8; v65 > 0; v65--)
  {
    if(v92[v65-1] != v93[v65-1])
    {      
      if(v92[v65-1] < v93[v65-1])
        return 1;
      else
        return 2;
    }
  }
  return 0;
}

byte ARD1939::f04(long* v25, byte* v71, int* v31, byte* v67, byte* v66, byte* v76)
{
  long v78;
  long v74;
  *v25 = 0;
  *v31 = 0;
  *v67 = NULLADDRESS;
  *v66 = NULLADDRESS;
  *v76 = 0;
  if(canReceive(&v74, &v71[0], v31) == 0)
  {
    v78 = v74 & 0x1C000000;
    *v76 = (byte)(v78 >> 26);
    *v25 = v74 & 0x01FFFF00;
    *v25 = *v25 >> 8;
    *v66 = (byte)(v74 & 0x000000FF);
    *v67 = GLOBALADDRESS;
    if(f08(*v25) == true)
    {
      *v67 = (byte)(*v25 & 0xFF);
      *v25 = *v25 & 0x01FF00;
    }
    if(f07(v25, &v71[0]) == true)
      return J1939_MSG_PROTOCOL;
    else
      return J1939_MSG_APP;
  }
  else
    return J1939_MSG_NONE;
}

byte ARD1939::Transmit(byte v76, long v25, byte v17, byte v67ess, byte* v79, int v73)
{
  long v74;
  if(v73 > J1939_MSGLEN)
    return ERR;
  v74 = ((long)v76 << 26) + (v25 << 8) + (long)v17;
  if(f08(v25) == true)
    v74 = v74 | ((long)v67ess << 8);
  if(v73 > 8)
#if TRANSPORT_PROTOCOL == 1
    return f10(v76, v25, v17, v67ess, v79, v73);
#else
    return ERR;
#endif
  else
    return canTransmit(v74, v79, v73);
}

void ARD1939::f05(void)
{
  if(v38.v21 == true && v38.v37 == false)
  {
    if(--v38.v36 == 0)
      v38.v37 = true;
  }
  if(v39.v21 == true && v39.v37 == false)
    if(--v39.v36 == 0)
      v39.v37 = true;
#if TRANSPORT_PROTOCOL == 1
  if(v40.v21 == true && v40.v37 == false)
    if(--v40.v36 == 0)
    	v40.v37 = true;
  if(v41.v21 == true && v41.v37 == false)
    if(--v41.v36 == 0)
    	v41.v37 = true;
  if(v42.v21 == true && v42.v37 == false)
    if(--v42.v36 == 0)
    	v42.v37 = true;
  if(v43.v21 == true && v43.v37 == false)
    if(--v43.v36 == 0)
    	v43.v37 = true;
  if(v44.v21 == true && v44.v37 == false)
    if(--v44.v36 == 0)
    	v44.v37 = true;
  if(v45.v21 == true && v45.v37 == false)
    if(--v45.v36 == 0)
    	v45.v37 = true;
  if(v46.v21 == true && v46.v37 == false)
    if(--v46.v36 == 0)
    	v46.v37 = true;
  if(v47.v21 == true && v47.v37 == false)
    if(--v47.v36 == 0)
    	v47.v37 = true;
  if(v48.v21 == true && v48.v37 == false)
    if(--v48.v36 == 0)
    	v48.v37 = true;
#endif
}

void ARD1939::f06(struct v35* v75)
{
  v75->v36 = 0;
  v75->v21 = false;
  v75->v37 = false;
}

bool ARD1939::f07(long* v25, byte* v83)
{
  bool v69;
  byte v65;
  byte v84;
  byte v85;
  v69 = false;
  v84 = (byte)((*v25 & 0x00FF00) >> 8);
  v85 = (byte)(*v25 & 0x0000FF);
  switch(v84)
  {
    case d47:
      if(v85 == GLOBALADDRESS)
      {
        if(v83[0] == d27
        && v83[1] == d26
        && v83[2] == d25)
          v69 = true;
      }
      else
      {
        *v25 = *v25 & 0x00FF00;
        if(v83[0] == d27
        && v83[1] == d26
        && v83[2] == d25)
          v69 = true;
      }
      break;
  
    default:
      for(v65 = 0; v65 < v04 - 1; v65++)
      {
        if(*v25 == v05[v65])
        {
          v69 = true;
          break;
        }
      }
      break;
  }
  return v69;
}

bool ARD1939::f08(long v25)
{
  if(v25 > 0 && v25 <= 0xEFFF)
    return true;
  if(v25 > 0x10000 && v25 <= 0x1EFFF)
    return true;
  return false;
}

byte ARD1939::GetSourceAddress(void)
{
   return v18.v17;
}

void ARD1939::SetPreferredAddress(byte v86)
{
  v18.v12 = v86;
}

void ARD1939::SetAddressRange(byte v87, byte v88)
{
  v18.v13 = v87;
  v18.v14 = v88;
}

void ARD1939::SetNAME(long v81, int nManufacturerCode, byte nFunctionInstance, byte nECUInstance,
                  byte nFunction, byte nVehicleSystem, byte nVehicleSystemInstance, byte nIndustryGroup, byte nArbitraryAddressCapable)
{
  v03[0] = (byte)(v81 & 0xFF);
  v03[1] = (byte)((v81 >> 8) & 0xFF);
  v03[2] = (byte)(((nManufacturerCode << 5) & 0xFF) | (v81 >> 16));
  v03[3] = (byte)(nManufacturerCode >> 3);
  v03[4] = (byte)((nFunctionInstance << 3) | nECUInstance);
  v03[5] = (byte)(nFunction);
  v03[6] = (byte)(nVehicleSystem << 1);
  v03[7] = (byte)((nArbitraryAddressCapable << 7) | (nIndustryGroup << 4) | (nVehicleSystemInstance));
}

byte ARD1939::SetMessageFilter(long v25)
{
  byte v94;
  int v65;
  v94 = ERR;
  if((v25 & 0x00FF00) == d31)
	  v25 = d31;
  if(f09(v25) == true)
    v94 = OK;
  else
  {
    for(v65 = 0; v65 < MSGFILTERS; v65++)
    {
      if(v02[v65].bActive == false)
      {
        v02[v65].bActive = true;
        v02[v65].v25 = v25;
        v94 = OK;
        break;
      }
    }
  }
  return v94;
}

void ARD1939::DeleteMessageFilter(long v25)
{
  int v65;
  if((v25 & 0x00FF00) == d31)
	  v25 = d31;
  for(v65 = 0; v65 < MSGFILTERS; v65++)
  {
    if(v02[v65].v25 == v25)
    {
      v02[v65].bActive = false;
      v02[v65].v25 = 0;
      break;
    }
  }
}

bool ARD1939::f09(long v25)
{
  bool v69;
  int v65;
  v69 = false;
  if((v25 & 0x00FF00) == d31)
	  v25 = d31;
  for(v65 = 0; v65 < MSGFILTERS; v65++)
  {
    if(v02[v65].bActive == true
    && v02[v65].v25 == v25)
    {
      v69 = true;
      break;
    }
  }
  return v69;
}

#if TRANSPORT_PROTOCOL == 1
byte ARD1939::f13(long v25, byte* v71, int v31, byte v67, byte v66, byte v76)
{
  byte v94;
  int nPointer;
  int v65;
  v94 = OK;
  if(v33.v20 == d03 && v33.v21 == true)
  {
      if(v33.v22 == false)
      {
        v62[0] = d04;
        v62[1] = (byte)(v33.v31 & 0xFF);
        v62[2] = (byte)(v33.v31 >> 8);
        v62[3] = v33.v28;
        v62[4] = 0xFF;
        v62[5] = (byte)(v33.v25 & 0x0000FF);
        v62[6] = (byte)((v33.v25 & 0x00FF00) >> 8);
        v62[7] = (byte)(v33.v25 >> 16);
        v94 = Transmit(d38, d36, v33.v26, GLOBALADDRESS, &v62[0], 8);
        v40.v36 = v51;
        v40.v21 = true;        
        v33.v22 = true;
      }
      else
      {
        if(v40.v37 == true )
        {
          nPointer = v33.v29 * 7;
          v63[0] = ++v33.v29;
          for(v65 = 0; v65 < 7; v65++)
            v63[v65+1] = v33.v30[nPointer + v65];
          v94 = Transmit(d40, d39, v33.v26, GLOBALADDRESS, &v63[0], 8);
          if(v33.v29 == v33.v28)
          {
            f11(d12);
          }
          else
          {
            v40.v36 = v51;
            v40.v21 = true;      
            v40.v37 = false;  
          }
        }
      }
  }
  if(v25 == d36 && v71[0] == d04 && v33.v20 == d01
  && v33.v32 == false)
  {    
      v33.v25 = (((long)v71[7]) << 16) + (((long)v71[6]) << 8) + (long)v71[5];
      if(f09(v33.v25) == true)
      {
        v33.v31 = (int)v71[1] + ((int)(v71[2]) << 8);
        if(v33.v31 > J1939_MSGLEN)
        {
          f11(d12);
        }
        else
        {
          v33.v20 = d02;
          v33.v21 = true;
          v33.v26 = v66;
          v33.v27 = v67;
          v33.v28 = v71[3];
          v33.v29 = 0;
          v41.v36 = v52;
          v41.v21 = true;
        }
       }
      else 
        v33.v25 = 0;
  }
  if(v33.v20 == d02 && v41.v37 == true)
  {
    f11(d12);
  }

  if(v33.v20 == d02 && v33.v21 == true
  && v25 == d39 && v66 == v33.v26 && v67 == v33.v27)
  {
    nPointer = ((int)v71[0] - 1) * 7;
    for(v65 = 1; v65 < 8; v65++)
      v33.v30[nPointer++] = v71[v65];
    if(++v33.v29 == v33.v28)
    {
	f11(d13);
	v33.v32 = true;
    }
  }
  if(v34.v20 == d03 && v34.v21 == true
  && v25 == d36 && v71[0] == d45)
  {
    f12(d12);
  }
  
  if(v34.v20 == d03 && v34.v21 == true
  && v25 == d36 && v71[0] == d07)
  {
    f12(d12);
  }
  if(v34.v20 == d03 && v34.v21 == true)
  {
      if(v34.v23 == false)
      {
        v62[0] = d05;
        v62[1] = (byte)(v34.v31 & 0xFF);
        v62[2] = (byte)(v34.v31 >> 8);
        v62[3] = v34.v28;
        v62[4] = 0xFF;
        v62[5] = (byte)(v34.v25 & 0x0000FF);
        v62[6] = (byte)((v34.v25 & 0x00FF00) >> 8);
        v62[7] = (byte)(v34.v25 >> 16);
        v94 = Transmit(d38, d36, v34.v26, v34.v27, &v62[0], 8);
        v43.v36 = v54;
        v43.v21 = true;        
        v34.v23 = true;
      }
      else
      {
        if(v43.v37 == true)
        {
          v62[0] = d08;
          v62[1] = d11;
          v62[2] = 0xFF;
          v62[3] = 0xFF;
          v62[4] = 0xFF;
          v62[5] = (byte)(v34.v25 & 0x0000FF);
          v62[6] = (byte)((v34.v25 & 0x00FF00) >> 8);
          v62[7] = (byte)(v34.v25 >> 16);
          v94 = Transmit(d38, d36, v34.v26, v34.v27, &v62[0], 8);
          f12(d12);
        }
        if(v25 == d36 && v67 == v34.v26 && v71[0] == d06)
        {
          f06(&v43);
          v42.v36 = v53;
          v42.v21 = true;        
          v34.v24 = true;
        }
        if(v34.v24 == true && v42.v37 == true)
        {
          nPointer = v34.v29 * 7;
          v63[0] = ++v34.v29;
          for(v65 = 0; v65 < 7; v65++)
            v63[v65+1] = v34.v30[nPointer + v65];
          v94 = Transmit(d40, d39, v34.v26, v34.v27, &v63[0], 8);
          if(v34.v29 == v34.v28)
          {
            f06(&v42);
            v47.v36 = v58;
            v47.v21 = true;
          }
          else
          {
            v42.v36 = v51;
            v42.v21 = true;      
            v42.v37 = false;  
          }
        }
        if(v47.v37 == true)
        {
          v62[0] = d08;
          v62[1] = d11;
          v62[2] = 0xFF;
          v62[3] = 0xFF;
          v62[4] = 0xFF;
          v62[5] = (byte)(v34.v25 & 0x0000FF);
          v62[6] = (byte)((v34.v25 & 0x00FF00) >> 8);
          v62[7] = (byte)(v34.v25 >> 16);
          v94 = Transmit(d38, d36, v34.v26, v34.v27, &v62[0], 8);
          f12(d12);
        }
      }
  }
  if(v25 == d36 && v67 == v18.v17 && v71[0] == d05)
  {
    int v77;
    v77 = (int)v71[1] + ((int)(v71[2]) << 8);
    if(v34.v20 != d01 || v77 > J1939_MSGLEN  || v34.v32 == true)
    {
        v62[0] = d08;
        if(v34.v31 > J1939_MSGLEN)
          v62[1] = d10;
        else
          v62[1] = d09;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = v71[5];
        v62[6] = v71[6];
        v62[7] = v71[7];
        v94 = Transmit(d38, d36, v67, v66, &v62[0], 8);
    }
    else
    {
      v34.v25 = (((long)v71[7]) << 16) + (((long)v71[6]) << 8) + (long)v71[5];
      if(f09(v34.v25) == true)
      {
        v34.v20 = d02;
        v34.v21 = true;
        v34.v24 = true;
        v34.v26 = v66;
        v34.v27 = v67;
        v34.v28 = v71[3];
        v34.v29 = 0;
        v34.v31 = (int)v71[1] + ((int)(v71[2]) << 8);
        for(v65 = 0; v65 < 8; v65++)
          v62[v65] = v71[v65];
        v62[0] = d06;
        v62[1] = v62[3];    
        v62[2] = 1;              
        v62[3] = 0xFF;          
        v94 = Transmit(d38, d36, v67, v66, &v62[0], 8);
        v45.v36 = v56;
        v45.v21 = true;
      }
      else
      {       
        v62[0] = d08;
        v62[1] = d10;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = v71[5];
        v62[6] = v71[6];
        v62[7] = v71[7];
        v94 = Transmit(d38, d36, v67, v66, &v62[0], 8);
      }
    }
  }
  if(v34.v20 == d02 && v34.v21 == true)
  {
      if(v45.v37 == true)
      {
        f12(d12);
        v62[0] = d08;
        v62[1] = d11;
        v62[2] = 0xFF;
        v62[3] = 0xFF;
        v62[4] = 0xFF;
        v62[5] = (byte)(v34.v25 & 0x0000FF);
        v62[6] = (byte)((v34.v25 & 0x00FF00) >> 8);
        v62[7] = (byte)(v34.v25 >> 16);
        v94 = Transmit(d38, d36, v34.v27, v34.v26, &v62[0], 8);
      }
      if(v25 == d39 && v67 == v34.v27 && v66 == v34.v26)
      {
        nPointer = ((int)v71[0] - 1) * 7;
        for(v65 = 1; v65 < 8; v65++)
          v34.v30[nPointer++] = v71[v65];
        if(++v34.v29 == v34.v28)
        {
          v62[0] = d07;
          v62[1] = (byte)(v34.v31 & 0x00FF);         
          v62[2] = (byte)((v34.v31 & 0x00FF) >> 8);   
          v62[3] = v34.v28;
          v62[4] = 0xFF;
          v62[5] = (byte)(v34.v25 & 0x0000FF);
          v62[6] = (byte)((v34.v25 & 0x00FF00) >> 8);
          v62[7] = (byte)(v34.v25 >> 16);
          v94 = Transmit(d38, d36, v34.v27, v34.v26, &v62[0], 8);
          f12(d13);
          v34.v32 = true;
        }
      }
  }
  return v94;
}

byte ARD1939::f10(byte v76, long v25, byte v17, byte v67ess, byte* v79, int v77)
{
  byte v68;
  int v65;    
  struct v19* v89;
  v68 = OK;
  if(v67ess != GLOBALADDRESS)
    v89 = &v34;
  else 
    v89 = &v33;
  if(v89->v20 != d01 || v77 > J1939_MSGLEN)
    v68 = ERR;
  else
  {
    for(v65 = 0; v65 < v77; v65++)
      v89->v30[v65] = v79[v65];
    for(v65 = v77; v65 < (v77 + 7); v65++)
    {
      if(v65 >= J1939_MSGLEN) break;
      v89->v30[v65] = 0xFF;
    }
    v89->v25 = v25;
    v89->v31 = v77;
    v89->v26 = v17;
    v89->v27 = v67ess;
    v65 = v77;
    v89->v28 = 0;
    while(v65 > 0)
    {
      v65 = v65 - 7;
      v89->v28++;
    }
    v89->v29 = 0;
    v89->v20 = d03;
    v89->v21 = true;
  }
  return v68;
}

void ARD1939::f11(byte v90)
{
	if(v90 == d12)
	{
		v33.v20 = d01;
		v33.v21 = false;
		v33.v22 = false;
		v33.v25 = 0;
		v33.v26 = GLOBALADDRESS;
		v33.v27 = GLOBALADDRESS;
		v33.v28 = 0;
		v33.v29 = 0;
		v33.v31 = 0;
		v33.v32 = false;
	}
	f06(&v40);
	f06(&v41);
}

void ARD1939::f12(byte v90)
{
	if(v90 == d12)
	{
		v34.v20 = d01;
		v34.v21 = false;
		v34.v23 = false;
		v34.v24 = false;
		v34.v25 = 0;
		v34.v26 = GLOBALADDRESS;
		v34.v27 = GLOBALADDRESS;
		v34.v28 = 0;
		v34.v29 = 0;
		v34.v31 = 0;
		v34.v32 = false;	
	}
	f06(&v42);
	f06(&v43);
	f06(&v44);
	f06(&v45);
	f06(&v46);
	f06(&v47);
	f06(&v48);
}

#endif

