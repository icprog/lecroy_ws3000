

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


/*
 * Decode a status byte
 */
static void
showHexval(FILE *fp, const char *name, int val, int bitmask, const char *bitname, ...)
{
    const char *sep = " -- ";
    va_list ap;
    va_start(ap, bitname);
    fprintf(fp, "%28s: ", name);
    if (bitmask)
        fprintf(fp, "%#x", val);
    else
        fprintf(fp, "%#4.4x", val);
    for (;;) {
        if (((bitmask > 0) && ((val & bitmask)) != 0)
         || ((bitmask < 0) && ((val & -bitmask)) == 0)
         || ((bitmask == 0) && (bitname != NULL) && (bitname[0] != '\0'))) {
            fprintf(fp, "%s%s", sep, bitname);
            sep = ", ";
        }
        if (bitmask == 0)
            break;
        bitmask = va_arg(ap, int);
        if (bitmask == 0)
            break;
        bitname = va_arg(ap, char *);
    }
    fprintf(fp, "\n");
    va_end(ap);
}

/*
 * Show a byte number
 */
static void
pcomma(FILE *fp, size_t n)
{
    if (n < 1000) {
        fprintf(fp, "%zu", n);
        return;
    }
    pcomma(fp, n/1000);
    fprintf(fp, ",%03zu", n%1000);
}
    
static void
showCount(FILE *fp, const char *label, size_t count)
{
    fprintf(fp, "%22s Count: ", label);
    pcomma(fp, count);
    fprintf(fp, "\n");
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


  asynUsbTmcPortName = asynUSBTMCPortName;
  /* Create an EPICS exit handler */
  epicsAtExit(exitHandler, this);

  Init();
  
 
};

drvWS3122::~drvWS3122()
{
  pasynOctetSyncIO->disconnect(usbTmcAsynUser);
};

asynStatus
drvWS3122::Init()
{
  asynStatus status = asynSuccess;
  
  createParam(DevIDNString,                 asynParamOctet, &devIdentification_);
  createParam(DevManufacturerString,        asynParamOctet, &devManufacturer_);
  createParam(DevModelString,               asynParamOctet, &devModel_);
  createParam(DevSerialNumberString,        asynParamOctet, &devSerialNumber_);
  
  status = pasynOctetSyncIO->connect(this->getUsbTmcPortName(), 0, &usbTmcAsynUser, NULL);

  pasynInterface = pasynManager->findInterface(usbTmcAsynUser, asynDrvUserType, 1);
  pasynDrvPvt    = (drvPvt *)pasynInterface->drvPvt;

  status = report_device_information(stdout);
  status = set_device_information();

  if (status) {
    printf("Initialization  failure\n");  
  }
  
  return status;
}


asynStatus
drvWS3122::report_device_information(FILE *fp)
{
  asynStatus status = asynSuccess;
  pasynManager->report(stdout, 10, this->getUsbTmcPortName());
  

  fprintf(fp, "\nReport Device Information : %s\n", __func__ );
  fprintf(fp,"        %s pinterface %p drvPvt %p\n",
	  pasynInterface->interfaceType, pasynInterface->pinterface,
	  pasynInterface->drvPvt);
  
  fprintf(fp, "%20sonnected, Interrupt handler thread %sactive\n",
	  pasynDrvPvt->isConnected ? "C" : "Disc",
	  pasynDrvPvt->interruptTid ? "" : "in");
  
  showHexval(fp, "Vendor",  pasynDrvPvt->deviceVendorId,  0, pasynDrvPvt->deviceVendorString);
  showHexval(fp, "Product", pasynDrvPvt->deviceProductId, 0, pasynDrvPvt->deviceProductString);

  if (pasynDrvPvt->deviceSerialString[0]) {
    fprintf(fp, "%28s: \"%s\"\n", "Serial Number", pasynDrvPvt->deviceSerialString);
  }
  
  fprintf(fp, "          Interface Protocol: %x",  pasynDrvPvt->bInterfaceProtocol);

  switch ( pasynDrvPvt->bInterfaceProtocol)
    {
    case 0: fprintf(fp, " -- USBTMC\n");                break;
    case 1: fprintf(fp, " -- USBTMC USB488\n");         break;
    default: fprintf(fp, "\n");                         break;
    }
  
  
  if (pasynDrvPvt->termChar >= 0) {
    fprintf(fp, "%28s: %x\n", "Terminator",  pasynDrvPvt->termChar);
  }

  
  showHexval(fp, "TMC Interface Capabilities",
	     pasynDrvPvt->tmcInterfaceCapabilities,
	     0x4, "Accepts INDICATOR_PULSE",
	     0x2, "Talk-only",
	     0x1, "Listen-only",
	     -0x3, "Talk/Listen",
	     0);
  showHexval(fp, "TMC Device Capabilities",
	     pasynDrvPvt->tmcDeviceCapabilities,
	     0x1, "Supports termChar",
	     0);
  
  if (pasynDrvPvt->bInterfaceProtocol == 1) {
    
    showHexval(fp, "488 Interface Capabilities",
	       pasynDrvPvt->usb488InterfaceCapabilities,
	       0x4, "488.2",
	       0x2, "REN/GTL/LLO",
	       0x1, "TRG",
	       0);
    showHexval(fp, "488 Device Capabilities",
	       pasynDrvPvt->usb488DeviceCapabilities,
	       0x8, "SCPI",
	       0x4, "SR1",
	       -0x4, "SR0",
	       0x2, "RL1",
	       -0x2, "RL0",
	       0x1, "DT1",
	       -0x1, "DT0",
	       0);
  }
  
  showCount(fp, "Connection",  pasynDrvPvt->connectionCount);
  showCount(fp, "Interrupt",  pasynDrvPvt->interruptCount);
  showCount(fp, "Send",  pasynDrvPvt->bytesSentCount);
  showCount(fp, "Receive",  pasynDrvPvt->bytesReceivedCount);

  fprintf(fp, "\n");
  return status;
};
  
asynStatus drvWS3122::set_device_information()
{
  asynStatus status = asynSuccess;
  setStringParam(devManufacturer_, pasynDrvPvt->deviceVendorString);
  setStringParam(devModel_,        pasynDrvPvt->deviceProductString);
  
  if (pasynDrvPvt->deviceSerialString[0]) {
    setStringParam(devSerialNumber_, pasynDrvPvt->deviceSerialString);
  }
   
  return status;
  
};


asynStatus
drvWS3122::usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, int timeout)
{
  size_t nbytesOut = 0;
  size_t nbytesIn  = 0;
  int    eomReason;
  size_t send_buffer_len = strlen(sendBuffer);
  size_t read_buffer_len = MAX_BUF_SIZE;
  char   *pReadBuffer = recvBuffer;

  //    int                    vendorId;
  //    int                    productId;
  //    const char            *serialNumber;

 printf("%s :: sendBuffer %s, len %zd, recvBuffer %s, len %zd, nbytesOut %zd, nbytesIn %zd, eomReason %d\n",
	 __func__,
	 sendBuffer,
	 send_buffer_len,
	 pReadBuffer,
	 read_buffer_len,
	 nbytesOut,
	 nbytesIn,
	 eomReason);
 
  asynStatus status = pasynOctetSyncIO->writeRead(this->usbTmcAsynUser,
						  sendBuffer,
						  send_buffer_len,
                                                  pReadBuffer,
						  read_buffer_len,
						  timeout,
                                                  &nbytesOut,
						  &nbytesIn,
						  &eomReason);

  callParamCallbacks();
  recvBufSize = nbytesIn;
  
  printf("%s :: sendBuffer %s, len %zd, recvBuffer %s, len %zd, nbytesOut %zd, nbytesIn %zd, eomReason %d\n",
	 __func__,
	 sendBuffer,
	 send_buffer_len,
	 pReadBuffer,
	 read_buffer_len,
	 nbytesOut,
	 nbytesIn,
	 eomReason);
  
  return status;

}

// asynStatus
// drvWS3122::usbTmcWrite(double timeout)
// {
//   asynStatus status        = asynSuccess;
//   const char *functionName = "usbTmcWrite";
  
//   status = pasynOctetSyncIO->write(usbTmcAsynUser,
// 				   sendBuffer.c_str(),
// 				   sendBufferSize,
// 				   timeout,
// 				   &sendBufferActualSize);

//   callParamCallbacks();

//   if (status) {
//     asynPrint(pasynUserSelf, ASYN_TRACE_ERROR,
// 	      "%s:%s, status=%d\n", driverName, functionName, status);
//   }
  
//   return status;
// }



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

 printf("%s :: value %s, nChars %zd, nActual %zd\n",
	 __func__,
	value, nChars, *nActual);
 

  /* Set the parameter in the parameter library. */
  status = (asynStatus) setStringParam(function, value);

  /* Fetch the parameter string name for possible use in debugging */
  getParamName(function, &paramName);

  std::string value_s(value, nChars);

  try
    {
      if(function == devIdentification_) {
	value_s = "*IDN?";
	nChars = value_s.size();
	status = this->SetDataCmd(value_s);
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



asynStatus
drvWS3122::SetDataCmd(std::string cmd)
{
  asynStatus status = asynSuccess;

  char dataBuf[256];
  int  dataBufSize;
 
  printf("%s\n", __func__);
  memset(dataBuf, 0x00, sizeof(dataBuf));

  pasynOctetSyncIO->setOutputEos(this->usbTmcAsynUser, "", 0);
  pasynOctetSyncIO->setInputEos(this->usbTmcAsynUser, "\n", 1);


  if (this->usbTmcWriteRead((char*) cmd.c_str(), dataBuf, dataBufSize) != asynError) {
    asynPortDriver::setStringParam(devIdentification_, dataBuf);
  }

  return status;
}

//
  // std::string sendBuffer;
  // size_t sendBufferSize;
  // size_t sendBufferActualSize;
  // std::string recvBuffer;
  // size_t recvBufferSize;
  // size_t recvBufferActualSize;





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


