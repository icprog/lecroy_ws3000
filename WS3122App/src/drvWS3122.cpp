
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <string>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsExit.h>
#include <iocsh.h>



#include <epicsExport.h>
#include <asynDriver.h>

#define epicsExportSharedSymbols
#include <shareLib.h>

//#include "WS3122Def.hh"
#include "drvWS3122.hh"


static const char *driverName="drvWS3122";

static void exitHandler(void *drvPvt) {
	drvWS3122 *pPvt = (drvWS3122 *)drvPvt;
	delete pPvt;
}


drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   MAX_ASYN_ADDRESS, 
		   NUM_PARAMS,
		   asynInt32Mask | asynOctetMask |  asynDrvUserMask, 
		   asynInt32Mask | asynOctetMask,
		   0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
		   1,    /* Autoconnect */
		   0, 0) /* Default priority and stack size*/    
{
  
  const char *functionName = "WS3122";
  asynStatus status;
  
    /* Create an EPICS exit handler */
  epicsAtExit(exitHandler, this);
  
  
  createParam(DevIDNString,                    asynParamOctet, &devIdentification_);
  createParam(WS3122ManufacturerString,        asynParamOctet, &devManufacturer_);
  createParam(WS3122ModelString,               asynParamOctet, &devModel_);
  createParam(WS3122SerialNumberString,        asynParamOctet, &devSerialNumber_);
  createParam(WS3122FirmwareVersionString,     asynParamOctet, &devFirmwareVersion_);
  
  status = pasynOctetSyncIO->connect(asynUSBTMCPortName, 0, &usbTmcAsynUser, NULL);
  
  if (status) {
    printf("%s: Create failure\n", functionName);  
  }
 
};

drvWS3122::~drvWS3122()
{
  pasynOctetSyncIO->disconnect(usbTmcAsynUser);
};


asynStatus
drvWS3122::usbTmcWriteRead(double timeout)
{
  int        eomReason     = 0;
  asynStatus status        = asynSuccess;
  const char *functionName = "usbTmcWriteRead";

  status = pasynOctetSyncIO->writeRead(usbTmcAsynUser,
				       sendBuffer,
				       sendBufferSize,
				       recvBuffer,
				       recvBufferSize,
				       timeout,
				       &sendBufferActualSize,
				       &recvBufferActualSize,
				       &eomReason);

  
  callParamCallbacks();

  printf("sendBuffer %s sbz %zd, recvBuffer %s, rbs %zd, %d, %d, %d, %d\n",
	 sendBuffer, sendBufferSize, recvBuffer, recvBufferSize,
	 timeout,
	 sendBufferActualSize,
	 recvBufferActualSize,
	 eomReason);


  if (status) {
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
	      "%s:%s, status=%d, eomReason %d\n", driverName, functionName, status, eomReason);
  }
  
  return status;
}

asynStatus
drvWS3122::usbTmcWrite(double timeout)
{
  asynStatus status        = asynSuccess;
  const char *functionName = "usbTmcWrite";
  
  status = pasynOctetSyncIO->write(usbTmcAsynUser,
				   sendBuffer,
				   sendBufferSize,
				   timeout,
				   &sendBufferActualSize);

  callParamCallbacks();

  if (status) {
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
	      "%s:%s, status=%d\n", driverName, functionName, status);
  }
  
  return status;
}


asynStatus
drvWS3122::usbTmcRead(double timeout)
{
  int        eomReason     = 0;
  asynStatus status        = asynSuccess;
  const char *functionName = "usbTmcRead";
  
  status = pasynOctetSyncIO->read(usbTmcAsynUser,
				  recvBuffer,
				  recvBufferSize,
				  timeout,
				  &recvBufferActualSize,
				  &eomReason);

  callParamCallbacks();
  
  if (status) {
    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR,
	      "%s:%s, status=%d, eomReason %d\n", driverName, functionName, status, eomReason);
  }
  
  return status;
}


// asynStatus
// drvWS3122::buildSendBuffer()
// {
//   asynStatus status = asynSuccess;

//   memset(sendBuffer, 0, MAX_BUFFER_SIZE);
//   memset(recvBuffer, 0, MAX_BUFFER_SIZE);
//   sendBufferActualSize = 0;
//   recvBufferActualSize = 0;
  
//   sendBufferSize = strlen(sendBuffer);

//   memcpy(sendBuffer, msg, sendBufferSize);

//   return status;
// }



asynStatus
drvWS3122::writeOctet(asynUser *pasynUser, const char *value, size_t nChars, size_t *nActual)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *paramName;
  const char* functionName = "writeOctet";


  printf("string %s, %zd\n", value, nChars);
  /* Set the parameter in the parameter library. */
  status = (asynStatus) setStringParam(function, value);

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  std::string value_s(value, nChars);

  try
    {
      if(function == devIdentification_) {
	//   	value_s = "*IDN?";
	sendBuffer = (char*) value_s.c_str();
	sendBufferSize = value_s.size();
	status = this->usbTmcWriteRead() ;
      }
	
      callParamCallbacks();
      
      if (status == 0) {
	asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
		  "%s:%s: function=%d, name=%s, value=%s\n", 
		  driverName, functionName, function, paramName, value_s.c_str());
	*nActual = value_s.size();
      }
      else {
	asynPrint(pasynUser, ASYN_TRACE_ERROR,
		  "%s:%s, function=%d,  name=%s, value=%s, status=%d\n",
		  driverName, functionName, function, paramName, value_s.c_str(), status);
      }

      return (status==0) ? asynSuccess : asynError;
    }
  catch(const std::exception& ex)
    {
      epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
  		    "%s:%s: status=%d, function=%d, name=%s, value=%s, error=%s", 
  		    driverName, functionName, status, function, paramName, value_s.c_str(), ex.what());
      *nActual = 0;
      return asynError;
    }
};



// asynStatus
// drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
// {
//   int addr;
//   int function = pasynUser->reason;
//   int status=0;
//   static const char *functionName = "writeInt32";
  
//   this->getAddress(pasynUser, &addr);
//   setIntegerParam(addr, function, value);

  
  
//   if(function == devIdentification_) {
//     status = this->CmdsSendRead(pasynUser, "*IDN?");
//   } 

//   /* Do callbacks so higher layers see any changes */

//   callParamCallbacks(addr);

//   if (status == 0) {
//     asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
// 	      "%s:%s, port %s, wrote %d to address %d\n",
// 	      driverName, functionName, this->portName, value, addr);
//   } else {
//     asynPrint(pasynUser, ASYN_TRACE_ERROR,
// 	      "%s:%s, port %s, ERROR writing %d to address %d, status=%d\n",
// 	      driverName, functionName, this->portName, value, addr, status);
//   }
//   return (status==0) ? asynSuccess : asynError;
//  };




// asynStatus
// drvWS3122::writeOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
// {
//   int addr;
//   int function = pasynUser->reason;
//   int status=0;
//   static const char *functionName = "writeOctec";
//   this->getAddress(pasynUser, &addr);
//   setStringParam(addr, function, value);

  
//   if(function == devIdentification_) {
    
//     status = this->SetDataCmd(pasynUser, "*IDN?");
    
//   } 
  
// }


// asynStatus
// drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
// {
//   int addr;
//   int function = pasynUser->reason;
//   int status=0;
//   static const char *functionName = "writeInt32";
  
//   this->getAddress(pasynUser, &addr);
//   setIntegerParam(addr, function, value);

  
  
//   if(function == devIdentification_) {
//     status = this->CmdsSendRead(pasynUser, "*IDN?");
//   } 

//   /* Do callbacks so higher layers see any changes */

//   callParamCallbacks(addr);

//   if (status == 0) {
//     asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
// 	      "%s:%s, port %s, wrote %d to address %d\n",
// 	      driverName, functionName, this->portName, value, addr);
//   } else {
//     asynPrint(pasynUser, ASYN_TRACE_ERROR,
// 	      "%s:%s, port %s, ERROR writing %d to address %d, status=%d\n",
// 	      driverName, functionName, this->portName, value, addr, status);
//   }
//   return (status==0) ? asynSuccess : asynError;
//  };




// asynStatus
// drvWS3122::SetDataCmd(asynUser *pasynUser, char *sendString)
// {
//   asynStatus rtn = asynSuccess;
//   char dataBuf[256];
//   int  dataBufSize;

 
//   pasynOctetSyncIO->setOutputEos(this->pasynUserDriver, "\n", 1);
//   pasynOctetSyncIO->setInputEos(this->pasynUserDriver, "", 0);
   
//   if(this->usbTMCPortWriteRead(cmd, dataBuf, dataBufSize) != asynError)
//     {
//       printf("%s\n", dataBuf);
//     }

//   return rtn;
// }


// asynStatus
// drvWS3122::GetDataCmd(asynUser *pasynUser, char *recvString)
// {
//   asynStatus rtn = asynSuccess;
//   char dataBuf[256];
//   int  dataBufSize;

 
//   pasynOctetSyncIO->setOutputEos(this->pasynUserDriver, "\n", 1);
//   pasynOctetSyncIO->setInputEos(this->pasynUserDriver, "", 0);
   
//   if(this->usbTMCPortWriteRead(cmd, dataBuf, dataBufSize) != asynError)
//     {
//       printf("%s\n", dataBuf);
//     }

//   return rtn;
// }






// asynStatus
// drvWS3122::writeOctet(asynUser *pasynUser, const char *value, size_t Chars, size_t *nActual)
// {
//     asynStatus status = asynSuccess;

//     return status;
// }





/* Configuration routine.  Called directly, or from the iocsh function below */

extern "C" {

  /** EPICS iocsh callable function to call constructor for the WS3122 class.**/
  
  int drvWS3122Configure(const char *portName, const char *asynUSBTMCPortName)
  {
    new drvWS3122(portName, asynUSBTMCPortName);
    return(asynSuccess);
  }


  /* EPICS iocsh shell commands */

  static const iocshArg initArg0 = { "portName",           iocshArgString};
  static const iocshArg initArg1 = { "asynUSBTMCPortName", iocshArgString};
  static const iocshArg * const initArgs[] = {&initArg0,
					      &initArg1};
  static const iocshFuncDef initFuncDef = {"drvWS3122Configure",2,initArgs};
  static void initCallFunc(const iocshArgBuf *args)
  {
    drvWS3122Configure(args[0].sval, args[1].sval);
  }

  void drvWS3122Register(void) {
    iocshRegister(&initFuncDef, initCallFunc);
  }

  epicsExportRegistrar(drvWS3122Register);

}


