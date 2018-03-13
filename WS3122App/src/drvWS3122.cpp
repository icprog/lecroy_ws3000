

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
		   asynInt32Mask  | asynFloat64Mask | asynOctetMask |  asynDrvUserMask, 
		   asynInt32Mask  | asynFloat64Mask | asynOctetMask,
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
  delete basicWave; basicWave = NULL;
};

asynStatus
drvWS3122::Init()
{
  basicWave = new BasicWave();
  
  asynStatus status = asynSuccess;
  
  createParam(DevIDNString,            asynParamOctet,  &devIdentification_);
  createParam(DevManufacturerString,   asynParamOctet,  &devManufacturer_  );
  createParam(DevModelString,          asynParamOctet,  &devModel_         );
  createParam(DevSerialNumberString,   asynParamOctet,  &devSerialNumber_  );

  createParam(ParBasicWaveTypeSelecString, asynParamInt32,   &parBasicWaveTypeSelect_);
  createParam(ParHeaderPathString,         asynParamInt32,   &parHeaderPath_    );
  createParam(ParWaveFrequencyString,      asynParamFloat64, &parWaveFrequency_);
  createParam(ParWaveAmplifierString,      asynParamFloat64, &parWaveAmplifier_);
  createParam(ParWaveOffsetString,         asynParamFloat64, &parWaveOffset_);
  createParam(ParWavePhaseString,          asynParamFloat64, &parWavePhase_);
  createParam(ParWaveWidthString,          asynParamFloat64, &parWaveWidth_);
  createParam(ParWaveRiseString,           asynParamFloat64, &parWaveRise_);
  createParam(ParWaveFallString,           asynParamFloat64, &parWaveFall_);
  createParam(ParWaveDelayString,          asynParamFloat64, &parWaveDelay_);
  createParam(ParWaveSymmetryString,       asynParamFloat64, &parWaveSymmetry_);
  createParam(ParWaveStdDevString,         asynParamFloat64, &parWaveStdDev_);
  createParam(ParWaveMeanString,           asynParamFloat64, &parWaveMean_);
  createParam(ParWaveDutyCycleString,      asynParamFloat64, &parWaveDutyCycle_);
  
  
  createParam(CmdSetWaveTypeString,        asynParamInt32,   &cmdSetWaveType_ );
  createParam(CmdPhaseInvertString,        asynParamInt32,   &cmdPhaseInvert_ );
  createParam(CmdScreenSaveString,         asynParamInt32,   &cmdScreenSave_  );
  createParam(CmdClockSourceString,        asynParamInt32,   &cmdClockSource_ );
  
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
  
  showCount(fp, "Connection", pasynDrvPvt->connectionCount);
  showCount(fp, "Interrupt",  pasynDrvPvt->interruptCount);
  showCount(fp, "Send",       pasynDrvPvt->bytesSentCount);
  showCount(fp, "Receive",    pasynDrvPvt->bytesReceivedCount);

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


// asynStatus
// drvWS3122::set_wave_type()
// {
//   asynStatus status = asynSuccess;
//   int bWaveType;
//   int header_path;
  
//   getIntegerParam(parHeaderPath_,          &header_path);
//   setIntegerParam(parHeaderPath_,           header_path);
//   getIntegerParam(parBasicWaveTypeSelect_, &bWaveType);
//   setIntegerParam(parBasicWaveTypeSelect_,  bWaveType);
  
//   basicWave -> setHeaderPath((EHeaderPath_t)    header_path);
//   basicWave -> setWaveTypeID((EBasicWaveType_t) bWaveType);
//   //  basicWave -> Print();
//   return status;
// }


// asynStatus
// drvWS3122::set_wave_freq()
// {
//   asynStatus status = asynSuccess;
//   double   wave_freq = 0.0;
//   getDoubleParam(parWaveFrequency_,        &wave_freq);
//   setDoubleParam(parWaveFrequency_,        wave_freq);
//   basicWave -> setFrequencyVal(wave_freq);

//   //  basicWave -> Print();
  
//   return status;
  
// }


// asynStatus
// drvWS3122::set_wave_ampl()
// {
//   asynStatus status = asynSuccess;
//   double   wave_val = 0.0;
//   getDoubleParam(parWaveAmplifier_,        &wave_val);
//   setDoubleParam(parWaveAmplifier_,        wave_val);
//   basicWave -> setAmplifierVal(wave_val);

//   //  basicWave -> Print();
  
//   return status;
  
// }



asynStatus
drvWS3122::set_wave_parameters(int function, epicsInt32 iValue, epicsFloat64 fValue, std::string value_s)
{
  asynStatus status = asynSuccess;
  int par    = 0;
  double val = 0.0;
  double val_reset = -0.1;

  if ( function == parBasicWaveTypeSelect_ ) {
    getIntegerParam(parHeaderPath_,           &par);
    basicWave -> setHeaderPath((EHeaderPath_t) par);
    setIntegerParam(parHeaderPath_,            par);
    par = 0;
    getIntegerParam(parBasicWaveTypeSelect_,  &par);
    basicWave -> setWaveTypeID((EBasicWaveType_t) par);
    setIntegerParam(parBasicWaveTypeSelect_,   par);
  }
  else if (function ==  parWaveFrequency_ ) {
    val = 0.0;
    getDoubleParam(parWaveFrequency_,        &val);
    if( basicWave->IsFrequency() ) basicWave -> setFrequencyVal(val);
    else val = val_reset; // Ignore user input
    setDoubleParam(parWaveFrequency_,         val);
  } else if (function ==  parWaveAmplifier_ ) {
    val = 0.0;
    getDoubleParam(parWaveAmplifier_,        &val);
    if( basicWave->IsAmplifier() ) basicWave -> setAmplifierVal(val);
    else val = val_reset; // Ignore user input
    setDoubleParam(parWaveAmplifier_,         val);
  }
  else if (function ==  parWaveOffset_ ) {
    val = 0.0;
    getDoubleParam(parWaveOffset_, &val);
    if( basicWave->IsOffset() ) basicWave -> setOffsetVal(val);
    else val = val_reset;    // Ignore user input
    setDoubleParam(parWaveOffset_, val);
  }
  else if (function ==  parWavePhase_ ) {
    val = 0.0;
    getDoubleParam(parWavePhase_, &val);
    if( basicWave->IsPhase() ) basicWave -> setPhaseVal(val);
    else val = val_reset;
    setDoubleParam(parWavePhase_, val);
  }
  else if (function ==  parWaveWidth_ ) {
    val = 0.0;
    getDoubleParam(parWaveWidth_, &val);
    if( basicWave->IsWidth() ) basicWave -> setWidthVal(val);
    else val = val_reset;
    setDoubleParam(parWaveWidth_, val);
  }
  else if (function ==  parWaveRise_ ) {
    val = 0.0;
    getDoubleParam(parWaveRise_, &val);
    if( basicWave->IsRise() ) basicWave -> setRiseVal(val);
    else  val = val_reset;
    setDoubleParam(parWaveRise_, val);
  }
  else if (function ==  parWaveFall_ ) {
    val = 0.0;
    getDoubleParam(parWaveFall_, &val);
    if( basicWave->IsFall() ) basicWave -> setFallVal(val);
    else val = val_reset;
    setDoubleParam(parWaveFall_, val);
  }
  else if (function ==   parWaveDelay_) {
    val = 0.0;
    getDoubleParam(parWaveDelay_, &val);
    if( basicWave->IsDelay() ) basicWave -> setDelayVal(val);
    else val = val_reset;
    setDoubleParam(parWaveDelay_, val);
  }
  else if (function ==  parWaveSymmetry_ ) {
    val = 0.0;
    getDoubleParam(parWaveSymmetry_, &val);
    if( basicWave->IsSymmetry() ) basicWave -> setSymmetryVal(val);
    else val = val_reset;
    setDoubleParam(parWaveSymmetry_, val);
  }
  else if (function ==  parWaveStdDev_ ) {
    val = 0.0;
    getDoubleParam(parWaveStdDev_, &val);
    if( basicWave->IsStdDev() ) basicWave -> setStdDevVal(val);
    else val = val_reset;
    setDoubleParam(parWaveStdDev_, val);
  }
  else if (function ==  parWaveMean_ ) {
    val = 0.0;
    getDoubleParam(parWaveMean_, &val);
    if( basicWave->IsMean() ) basicWave -> setMeanVal(val);
    else val = val_reset;
    setDoubleParam(parWaveMean_, val);
  }
  else if (function == parWaveDutyCycle_ ) {
    val = 0.0;
    getDoubleParam(parWaveDutyCycle_, &val);
    if( basicWave->IsDutyCycle() ) basicWave -> setDutyCycleVal(val);
    else val=val_reset;
    setDoubleParam(parWaveDutyCycle_, val);
  }


  
  // basicWave -> Print( value_s );
  
  return status;
  
}

  
// asynStatus drvWS3122::set_wavetype_parameters()
// {
//   asynStatus status = asynSuccess;

//   int bWaveType;
//   std::string wave_type;
//   std::string header_path;
//   //  double   wave_freq;
  
//   getIntegerParam(parBasicWaveTypeSelect_, &bWaveType);
//   getStringParam(parHeaderPath_,           header_path);
//   setIntegerParam(parBasicWaveTypeSelect_, bWaveType);
//   //  getDoubleParam(parWaveFrequency_,        &wave_freq);
//   //  setDoubleParam(parWaveFrequency_,        wave_freq);
  
//   basicWave -> setWaveTypeID(( EBasicWaveType_t) bWaveType);
//   basicWave -> setHeaderPath(header_path);
//   //  basicWave -> setFrequencyVal(wave_freq);


//   basicWave-> Print();
    
//   std::cout << " set_wavetype_paramter " << header_path << " " << wave_type << std::endl;
//   //  std::cout << header_path << std::endl;
//   //  std::cout << wave_type   << std::endl;
   
//   return status;
  
// };




asynStatus
drvWS3122::usbTmcWrite(std::string sendBuffer, double timeout)
{
  size_t nbytesOut       = 0;
 /* 
    asynUser   *pasynUser,
    const char *write_buffer, 
    size_t      write_buffer_len,
    double      timeout,
    size_t     *nbytesOut
 */

 
  asynStatus status = pasynOctetSyncIO->write(this->usbTmcAsynUser,
					      sendBuffer.c_str(),
					      sendBuffer.size(),
					      timeout,
					      &nbytesOut);
    
  callParamCallbacks();
 
  return status;

}




asynStatus
drvWS3122::usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout)
{
  size_t nbytesOut = 0;
  size_t nbytesIn  = 0;
  int    eomReason;
  size_t send_buffer_len = strlen(sendBuffer);
  size_t read_buffer_len = MAX_BUF_SIZE;
  char   *pReadBuffer    = recvBuffer;

 printf("%s :: sendBuffer %s, len %zd, recvBuffer %s, len %zd, nbytesOut %zd, nbytesIn %zd, eomReason %d\n",
	 __func__,
	 sendBuffer,
	 send_buffer_len,
	 pReadBuffer,
	 read_buffer_len,
	 nbytesOut,
	 nbytesIn,
	 eomReason);

 /* 
    asynUser   *pasynUser,
    const char *write_buffer, 
    size_t      write_buffer_len,
    char       *read_buffer, 
    size_t      read_buffer_len,
    double      timeout,
    size_t     *nbytesOut, 
    size_t     *nbytesIn, 
    int        *eomReason
 */
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


asynStatus
drvWS3122::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
  int function = pasynUser->reason;
  asynStatus status = asynSuccess;
  const char* functionName = "writeInt32";

  setIntegerParam(function, value);

  std::string value_s;
  std::string string_in = functionName;
  
  if ( function == cmdClockSource_ ) {
    status = this-> SetClockSource(value);
  } else if (function == cmdPhaseInvert_ ) {
    status = this-> SetPhaseInvert(value);
  } else if (function == cmdScreenSave_ ) {
    status = this-> SetScreenSave(value);
  } else if (function == cmdSetWaveType_ ) {
    status = this-> SetWaveTypeCmds(value);
  }
  status = this->set_wave_parameters( function, value, 0.0, functionName);
  
  callParamCallbacks();
  
  if (status == 0) {
    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
	      "%s:%s, port %s, wrote %d \n",
	      driverName, functionName, this->portName, value);
  } else {
    asynPrint(pasynUser, ASYN_TRACE_ERROR, 
	      "%s:%s, port %s, ERROR writing %d to  status=%d\n",
	      driverName, functionName, this->portName, value, status);
  }
  return (status==0) ? asynSuccess : asynError;
}


asynStatus
drvWS3122::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    //    epicsInt32 run;
    const char *paramName;
    const char* functionName = "writeFloat64";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    status = this->set_wave_parameters( function, 0, value, functionName);
  
    // if (function == parWaveFrequency_) {
    //   status = this->set_wave_freq();
    //   //        status = this->set_wave_parameters();
    // } else if (function == parWaveAmplifier_) {
    //   status = this->set_wave_ampl();
    // //     status = this->set_wave_parameters();
    // } else {
    //   //     /* All other parameters just get set in parameter list, no need to
    //   //      * act on them here */
    //   status = this->set_wave_parameters(functionName);
    // }
    
    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%f", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%f\n", 
              driverName, functionName, function, paramName, value);
    return status;
}



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
	status = this->write_read(value_s);
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
drvWS3122::write_read(std::string cmd)
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


asynStatus
drvWS3122::SetWaveTypeCmds(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  std::string value_s;
  value_s.clear();
  basicWave -> buildCommand();
  value_s = basicWave-> getFullCommand();

  std::cout << value_s << std::endl;
  // if (value == 1) {
  status = this->usbTmcWrite(value_s);

  basicWave -> clearCommand();
  return status;
};






asynStatus
drvWS3122::SetClockSource(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  std::string value_s;
  value_s.clear();
  // 0     : INTERNAL CLOCK
  // Other : EXTERNAL CLOCK
  if (value == 0) {
    value_s = "ROSC INT";
  } else {
    value_s = "ROSC EXT";
  }

  status = this->usbTmcWrite(value_s);

  return status;
};


asynStatus
drvWS3122::SetPhaseInvert(epicsInt32 value)
{
  asynStatus status = asynSuccess;
  std::string value_s;
  value_s.clear();
  // 0     : INVERT OFF
  // Other : INVERT ON
  if (value == 0) {
    value_s = "INVT OFF";
  } else {
    value_s = "INVT ON";
  }

  status = this->usbTmcWrite(value_s);

  return status;
};



asynStatus
drvWS3122::SetScreenSave(epicsInt32 value)
{
  asynStatus  status = asynSuccess;
  std::string value_s;
  std::ostringstream toString;   // stream used for the conversion

  toString << value;
  
  value_s += "SCSV";
  value_s += " ";
  value_s += toString.str();
    
  //  std::cout << "0 " << __func__ << " value: " << value << " value_s : " << value_s << std::endl;

  status = this->usbTmcWrite(value_s);

  return status;
};



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


