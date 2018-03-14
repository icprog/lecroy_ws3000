
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
GetWaveParameter(EWaveParameter_t type)
{
  std::string result;

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
GetCmdSymbol (ECmdSymbol_t type)
{
  std::string result;
  if      (type==kCmdSymbolColon)    result = ":";
  else if (type==kCmdSymbolQuestion) result = "?";
  else if (type==kCmdSymbolBlank)    result = " ";
  else if (type==kCmdSymbolComma)    result = ",";
  return result;
}
