
#include <algorithm>

#include "WSTypes.hh"

EBasicWaveType_t GetBasicWaveType(std::string name)
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

std::string GetBasicWaveType(EBasicWaveType_t type)
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
}
