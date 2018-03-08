#ifndef INC_DRVAsynUSBTMC_HH
#define INC_DRVAsynUSBTMC_HH

#include <string.h>
#include <errlog.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsEvent.h>
#include <epicsMessageQueue.h>
#include <epicsMutex.h>
#include <epicsThread.h>
#include <epicsExport.h>

#include <asynDriver.h>


#define IDSTRING_CAPACITY        100


typedef struct drvPvt {

  /*
   * Used to find matching device
   */
  int                    vendorId;
  int                    productId;
  const char            *serialNumber;

  /*
   * Matched device
   */
  int                    deviceVendorId;
  int                    deviceProductId;
  char                   deviceVendorString[IDSTRING_CAPACITY];
  char                   deviceProductString[IDSTRING_CAPACITY];
  char                   deviceSerialString[IDSTRING_CAPACITY];

  /*
   * Asyn interfaces we provide
   */
  char                  *portName;
  asynInterface          asynCommon;
  asynInterface          asynOctet;
  asynInterface          asynInt32;
  void                  *asynInt32InterruptPvt;
  asynInterface          asynDrvUser;

  /*
   * Libusb hooks
   */
  /* 
  libusb_context        *usb;
  libusb_device_handle  *handle;
  */
  int                    bInterfaceNumber;
  int                    bInterfaceProtocol;
  int                    isConnected;
  int                    termChar;
  unsigned char          bTag;
  int                    bulkOutEndpointAddress;
  int                    bulkInEndpointAddress;
  int                    interruptEndpointAddress;
  
  
  /*
   * Interrupt endpoint handling
   */
  
   
  int                    enableInterruptEndpoint;
  char                  *interruptThreadName;
  epicsThreadId          interruptTid;
  epicsMutexId           interruptTidMutex;
  epicsEventId           pleaseTerminate;
  epicsEventId           didTerminate;
  epicsMessageQueueId    statusByteMessageQueue;
  
  
  /*
   * Device capabilities
   */
  unsigned char          tmcInterfaceCapabilities;
  unsigned char          tmcDeviceCapabilities;
  unsigned char          usb488InterfaceCapabilities;
  unsigned char          usb488DeviceCapabilities;
  
  /*
   * I/O buffer
   */
  /*
    unsigned char          buf[BULK_IO_HEADER_SIZE+BULK_IO_PAYLOAD_CAPACITY];
    int                    bufCount;
    const unsigned char   *bufp;
    unsigned char          bulkInPacketFlags;
  */
  
  /*
   * Statistics
   */
  
  size_t                 connectionCount;
  size_t                 interruptCount;
  size_t                 bytesSentCount;
  size_t                 bytesReceivedCount;

  
  
} drvPvt;


#endif
