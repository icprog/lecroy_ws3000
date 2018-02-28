
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>

#include "drvWS3122.hh"
#include <epicsExport.h>

static const char *driverName="drvWS3122";

// void readTask(void *drvPvt)
// {
//   drvWS3122 *pPvt = (drvWS3122 *)drvPvt;

//   pPvt->readTask();
// }


// #define WS3122_IPN_Str 		"WS3122_IPN"            /* asynOctet      r  */
// #define WS3122_FwVer_Str        "WS3122_FWVER"
// #define WS3122_SwVer_Str        "WS3122_SWVER"

//   /** Values used for pasynUser->reason, and indexes into the parameter library. */
//   int  WS3122_IPN;
//   int  WS3122_FwVer;
//   int  WS3122_SwVer;


drvWS3122::drvWS3122(const char *portName, const char *asynUSBTMCPortName) 
  : asynPortDriver(portName, 
		   0, /* maxAddr */ 
		   NUM_WS3122_PARAMS,
		   asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
		   asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
		   0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
		   1, /* Autoconnect */
		   0, /* Default priority */
		   0) /* Default stack size*/    
{
  
  // int i;
  const char *functionName = "WS3122";
  asynStatus status;

  status = pasynOctetSyncIO->connect(asynUSBTMCPortName, 0, &(this->pasynUserDriver), NULL);
   
  eventId_ = epicsEventCreate(epicsEventEmpty);
    
  createParam(WS3122_IPN_Str,           asynParamInt32,         &WS3122_IPN);
  createParam(WS3122_FwVer_Str,         asynParamInt32,         &WS3122_FwVer);
  createParam(WS3122_SwVer_Str,         asynParamInt32,         &WS3122_SwVer);

    
  // /* Create the thread that computes the waveforms in the background */
  // status = (asynStatus)(epicsThreadCreate("drvWS3122AsynPortDriverTask",
  // 					  epicsThreadPriorityMedium,
  // 					  epicsThreadGetStackSize(epicsThreadStackMedium),
  // 					  (EPICSTHREADFUNC)::readTask,
  // 					  this) == NULL);
  if (status) {
    printf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
  }
}
asynStatus
drvWS3122::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *paramName;
  const char* functionName = "readFloat64";
  // int ch = 0;

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  char tmpParamName[256];
  memset(tmpParamName, 0x00, sizeof(tmpParamName));
  strcpy(tmpParamName, paramName);
  
  if((function ==  WS3122_IPN)) {
    printf("hello function \n");
  }
 
  /* Do callbacks so higher layers see any changes */
  status = (asynStatus) callParamCallbacks();

  if (status)
    epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                  "%s:%s: status=%d, function=%d, name=%s, value=%f",
                  driverName, functionName, status, function, paramName, *value);
  else
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
              "%s:%s: function=%d, name=%s, value=%f\n",
              driverName, functionName, function, paramName, *value);
  return status;
}


asynStatus
drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char *paramName;
  const char* functionName = "writeInt32";
  /* Set the parameter in the parameter library. */
  status = (asynStatus) setIntegerParam(function, value);
  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);
  if(function == WS3122_IPN) {
    printf("hello\n");
  }
  /* Do callbacks so higher layers see any changes */
  status = (asynStatus) callParamCallbacks();
  if (status)
    epicsSnprintf(pasynUser-> errorMessage, pasynUser->errorMessageSize,
		  "%s:%s: status=%d, function=%d, name=%s, value=%d",
		  driverName, functionName, status, function, paramName, value);
  else
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
	      "%s:%s: function=%d, name=%s, value=%d\n",
	      driverName, functionName, function, paramName, value);
  return status;
}


asynStatus
drvWS3122::usbTMCPortWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nActual = 0;
  size_t nRead = 0;
  int    eomReason;
  char   *pValue      = sendBuffer;
  size_t nChars       = strlen(sendBuffer);
  char   *pReadBuffer = recvBuffer;
  asynStatus status = pasynOctetSyncIO->writeRead(pasynUserDriver, pValue, nChars,
						  pReadBuffer, MAX_BUF_SIZE, timeout,
						  &nActual, &nRead, &eomReason);
  status = (asynStatus) callParamCallbacks();
  recvBufSize = nRead;
  return status;
}


asynStatus
drvWS3122::usbTMCPortRead(char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nRead = 0;
  int    eomReason;
  char   *pReadBuffer = recvBuffer;
  //  memset(pReadBuffer, 0x00, sizeof(pReadBuffer));
  asynStatus status = pasynOctetSyncIO->read(pasynUserDriver, pReadBuffer,
					     MAX_BUF_SIZE, timeout,
					     &nRead, &eomReason);
  status = (asynStatus) callParamCallbacks();
  return status;
}


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


