#ifndef INC_DRVWS3122_HH
#define INC_DRVWS3122_HH

/*
 * drvWS3122.hh
 *
 * Author: Jeong Han Lee
 *
 * Created Wednesday, February 28 21:20:54 CET 2018
 */


#include "asynPortDriver.h"
#include "asynOctetSyncIO.h"


#include "WS3122Def.hh"

#define MAX_BUF_SIZE            65536
#define TIMEOUT                 5



#define WS3122ManufacturerString        "DEV_MANUFACTURER"          /**< (asynOctet,    r/o) manufacturer name */
#define WS3122ModelString               "DEV_MODEL"                 /**< (asynOctet,    r/o) model name */
#define WS3122SerialNumberString        "DEV_SERIAL_NUMBER"         /**< (asynOctet,    r/o) serial number */
#define WS3122FirmwareVersionString     "DEV_FIRMWARE_VERSION"      /**< (asynOctet,    r/o) firmware version */
#define DevIDNString                 "DEV_GET_IDN"                   /* asynOctet      w  */

class drvWS3122 : public asynPortDriver {
public:
  drvWS3122(const char *portName, const char *asynUSBTMCPortName);

  virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
  //  virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
  // virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t 	nChars, size_t *nActual);

  
protected:
  /** Values used for pasynUser->reason, and indexes into the parameter library. */
  int  Identification;
  #define FIRST_WS3122_PARAM Identification
  int  WS3122Manufacturer;
  int  WS3122Model;
  int  WS3122SerialNumber;
  int  WS3122FirmwareVersion;
#define LAST_WS3122_PARAM WS3122FirmwareVersion

private:

  asynUser	*pasynUserDriver;

  asynStatus usbTMCPortWriteRead(char *sendBuffer, char *recvBuffer, int &recvBufSize, double timeout=TIMEOUT);
  asynStatus CmdsSendRead(asynUser *pasynUser, epicsInt32 value);
};

#define NUM_WS3122_PARAMS ((int)(&LAST_WS3122_PARAM - &FIRST_WS3122_PARAM + 1))

#endif 
