#ifndef WSTYPES_HH
#define WSTYPES_HH

#include <map>
#include <iostream>
#include <string>

#define MAX_ENUM_STRING_SIZE 26

typedef enum {
	      kWaveTypeSine,
	      kWaveTypeSquare,
	      kWaveTypeRamp,
	      kWaveTypePulse,
	      kWaveTypeNoise,
	      kWaveTypeArb,
	      kWaveTypeDc,
	      kWaveTypeUnknown
} EBasicWaveType_t;


typedef std::map < EBasicWaveType_t, std::string > BasicWaveMap;

inline BasicWaveMap CreateBasicWaveMap()
{
  BasicWaveMap map;
  map[kWaveTypeSine]    = "SINE";
  map[kWaveTypeSquare]  = "SQUARE";
  map[kWaveTypeRamp]    = "RAMP";
  map[kWaveTypePulse]   = "PULSE";
  map[kWaveTypeNoise]   = "NOISE";
  map[kWaveTypeArb]     = "ARB";
  map[kWaveTypeDc]      = "DC";
  map[kWaveTypeUnknown] = "UNKNOWN";
  return map;
  
};

static const BasicWaveMap kMapBasicWave = CreateBasicWaveMap();


EBasicWaveType_t GetBasicWaveType(std::string name);
std::string    GetBasicWaveType(EBasicWaveType_t type);



#endif
