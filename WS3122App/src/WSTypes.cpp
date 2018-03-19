
#include <algorithm>

#include "WSTypes.hh"

EBasicWaveType_t
GetBasicWaveType(std::string name)
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  EBasicWaveType_t result;
  if      (name.compare("sine")  ==0) result = kWaveTypeSine;
  else if (name.compare("square")==0) result = kWaveTypeSquare;
  else if (name.compare("ramp")  ==0) result = kWaveTypeRamp;
  else if (name.compare("pulse") ==0) result = kWaveTypePulse;
  else if (name.compare("noise") ==0) result = kWaveTypeNoise;
  else if (name.compare("arb")   ==0) result = kWaveTypeArb;
  else if (name.compare("db")    ==0) result = kWaveTypeDc;
  else                                result = kWaveTypeUnknown;
  return result;
};


EWaveParameter_t
GetWaveParmeter(std::string name)
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  EWaveParameter_t result;
  if      (name.compare("frequency")==0) result = kWaveFreq;
  else if (name.compare("amplifier")==0) result = kWaveAmpl;
  else if (name.compare("offset")   ==0) result = kWaveOffset;
  else if (name.compare("phase")    ==0) result = kWavePhase;
  else if (name.compare("width")    ==0) result = kWaveWidth;
  else if (name.compare("rise")     ==0) result = kWaveRise;
  else if (name.compare("fall")     ==0) result = kWaveFall;
  else if (name.compare("delay")    ==0) result = kWaveDelay;
  else if (name.compare("symmetry") ==0) result = kWaveSymm;
  else if (name.compare("stddev")   ==0) result = kWaveStdDev;
  else if (name.compare("mean")     ==0) result = kWaveMean;
  else if (name.compare("dutycycle")==0) result = kWaveDuty;
  else                                   result = kWaveUnknown;
  return result;
};


std::string
GetHeaderType   (EHeaderType_t type)
{
  std::string result;
  if      (type==kHeaderARWV) result = "ARWV";
  else if (type==kHeaderBSWV) result = "BSWV";
  else if (type==kHeaderBTWV) result = "BTWV";
  else if (type==kHeaderMDWV) result = "MDWV";
  else if (type==kHeaderSWWV) result = "SWWV";
  //  else if (type==kHeaderOUTP) result = "OUTP";
  //  else if (type==kHeaderSYNC) result = "SYNC";
  else                        result = "UNKNOWN";
  
  return result;
};




std::string
GetBasicWaveType(EBasicWaveType_t type)
{
  std::string result;
  if      (type==kWaveTypeSine)   result = "SINE";
  else if (type==kWaveTypeSquare) result = "SQUARE";
  else if (type==kWaveTypeRamp)   result = "RAMP";
  else if (type==kWaveTypePulse)  result = "PULSE";
  else if (type==kWaveTypeNoise)  result = "NOISE";
  else if (type==kWaveTypeArb)    result = "ARB";
  else if (type==kWaveTypeDc)     result = "DC";
  else                            result = "UNKNOWN";

  return result;
};


std::string
GetCmdSymbol (ECmdSymbol_t type)
{
  std::string result;
  if      (type==kCmdSymbolColon)    result = ":";
  else if (type==kCmdSymbolQuestion) result = "?";
  else if (type==kCmdSymbolBlank)    result = " ";
  else if (type==kCmdSymbolComma)    result = ",";
  else                               result = "";
  return result;
}


std::string
GetWaveState (EWaveState_t type)
{
  std::string result;
  if      (type==kWaveStateOn)  result = "ON";
  else if (type==kWaveStateOff) result = "OFF";
  else                          result = "";
  return result;
};




std::string
GetBurstMode (EBurstMode_t type)
{
  std::string result;
  if      (type==kBurstModeGate)   result = "GATE";
  else if (type==kBurstModeNCycle) result = "NCYC";
  else                             result = "";
  return result;
};



std::string
GetTriggerSrc (ETriggerSrc_t type)
{
  std::string result;
  if      (type==kTriggerSrcExternal) result = "EXT";
  else if (type==kTriggerSrcInternal) result = "INT";
  else if (type==kTriggerSrcManual)   result = "MAN";
  else                                result = "";
  return result;
};


std::string
GetTriggerMode   (ETriggerMode_t    type)
{
  std::string result;
  if      (type==kTriggerModeRise) result = "RISE";
  else if (type==kTriggerModeFall) result = "FALL";
  else if (type==kTriggerModeOff)  result = "OFF";
  else                             result = "";
  
  return result;
};

std::string
GetEdgeMode (EEdgeModeMap_t    type)
{
  std::string result;
  if      (type==kEdgeModeRise) result = "RISE";
  else if (type==kEdgeModeFall) result = "FALL";
  else                          result = "";

  return result;
};


std::string
GetPolarity      (EPolarityMap_t    type)
{
  std::string result;
  if      (type==kPolarityNegative) result = "NEG";
  else if (type==kPolarityPositive) result = "POS";
  else                              result = "";

  return result;
};





std::string
GetBurstParameter(EBurstParameter_t type)
{
  std::string result;
  if      (type==kBurstParPeriod)      result = "PRD";
  else if (type==kBurstParStartPhase)  result = "STPS";
  else if (type==kBurstParGateNcyc)    result = "GATE_NCYC";
  else if (type==kBurstParTriggerSrc)  result = "TRSR";
  else if (type==kBurstParDelay)       result = "DLAY";
  else if (type==kBurstParPolarity)    result = "PLRT";
  else if (type==kBurstParTriggerMode) result = "TRMD";
  else if (type==kBurstParEdge)        result = "EDGE";
  else if (type==kBurstParTime)        result = "TIME";
  else if (type==kBurstParMtrig)       result = "MTRIG";
  else                                 result = "";
  return result;
};

