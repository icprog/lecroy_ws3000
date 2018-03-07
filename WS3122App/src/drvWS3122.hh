#ifndef INC_DRVWS3122_HH
#define INC_DRVWS3122_HH

/*
 * drvWS3122.hh
 *
 * Author: Jeong Han Lee
 *
 * Created Wednesday, February 28 21:20:54 CET 2018
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>



#include "asynPortDriver.h"
#include "asynOctetSyncIO.h"


//#include "WS3122Def.hh"

#define MAX_BUF_SIZE            65536
#define TIMEOUT                 2

#define MAX_ASYN_ADDRESS        0
#define MAX_BUFFER_SIZE	        512

#define DevIDNString                    "DEV_IDN"                   /* asynOctet      w  */
#define WS3122ManufacturerString        "DEV_MANUFACTURER"          /**< (asynOctet,    r/o) manufacturer name */
#define WS3122ModelString               "DEV_MODEL"                 /**< (asynOctet,    r/o) model name */
#define WS3122SerialNumberString        "DEV_SERIAL_NUMBER"         /**< (asynOctet,    r/o) serial number */
#define WS3122FirmwareVersionString     "DEV_FIRMWARE_VERSION"      /**< (asynOctet,    r/o) firmware version */



class drvWS3122 : public asynPortDriver {
public:
  drvWS3122(const char *portName, const char *asynUSBTMCPortName);
  virtual ~drvWS3122();

  //virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  // //  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  //virtual asynStatus readOctet (asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
  virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t 	nChars, size_t *nActual);
  
  friend std::ostream& operator<<(std::ostream& os, const drvWS3122 &ws);

protected:
  /* Derived classes need access to these members */

  // std::string sendBuffer;
  // size_t sendBufferSize;
  // size_t sendBufferActualSize;
  // std::string recvBuffer;
  // size_t recvBufferSize;
  // size_t recvBufferActualSize;

  /** Values used for pasynUser->reason, and indexes into the parameter library. */
  int  devIdentification_;
  #define FIRST_PARAM devIdentification_
  int  devManufacturer_;
  int  devModel_;
  int  devSerialNumber_;
  int  devFirmwareVersion_;
#define LAST_PARAM devFirmwareVersion_

  
private:

  asynUser	*usbTmcAsynUser;

  //asynStatus WriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, int timeout)
  //  asynStatus usbTmcWriteRead(double timeout=TIMEOUT);
  asynStatus    usbTmcWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, int timeout=TIMEOUT);
  //asynStatus usbTmcRead(double timeout=TIMEOUT);
  //  asynStatus usbTmcWrite(double timeout=TIMEOUT);

  //  asynStatus usbSendBuffer(const char* param, std::string value_s);
  asynStatus SetDataCmd(std::string cmd);

  
  
};

#define NUM_PARAMS ((int)(&LAST_PARAM - &FIRST_PARAM + 1))

#endif 
