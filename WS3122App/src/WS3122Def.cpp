
#include "WS3122Def.hh"



BasicWave::BasicWave()
{
  Init();
  
};


BasicWave::BasicWave(const BasicWave &bwobj)
{

   
  channelID            = bwobj.channelID;
  waveTypeID           = bwobj.waveTypeID;
  frequencyVal         = bwobj.frequencyVal;
  amplifierVal         = bwobj.amplifierVal;
  offsetVal            = bwobj.offsetVal;
  dutyCycleVal         = bwobj.dutyCycleVal;
  phaseVal             = bwobj.phaseVal;
  standardDeviationVal = bwobj.standardDeviationVal ;
  meanVal              = bwobj.meanVal;
  widthVal             = bwobj.widthVal;
  riseVal              = bwobj.riseVal;
  fallVal              = bwobj.fallVal;
  delayVal             = bwobj.delayVal;

  frequencyUnit        = bwobj.frequencyUnit;
  amplifierUnit        = bwobj.amplifierUnit;
  offsetUnit           = bwobj.offsetUnit;
  dutyCycleUnit        = bwobj.dutyCycleUnit;

  waveTypeString       = bwobj.waveTypeString;
  fullCommandStream    << bwobj.fullCommandStream;

};


BasicWave::~BasicWave()
{
};


void 
BasicWave::Init()
{

  channelID  = 0;
  waveTypeID = 0;
  frequencyVal = 0.0;
  amplifierVal = 0.0;
  offsetVal    = 0;
  dutyCycleVal = 0.0;
  phaseVal     = 0;
  standardDeviationVal = 0.0;
  meanVal              = 0.0;
  widthVal             = 0.0;
  riseVal              = 0.0;
  fallVal              = 0.0;
  delayVal             = 0.0;

  frequencyUnit = "HZ";
  amplifierUnit = "V";
  offsetUnit    = "V";
  dutyCycleUnit = "percent";

  waveTypeString.clear();
  fullCommandStream.clear();
  
};



BasicWave & BasicWave::operator=(const BasicWave &bwobj)
{
  if (this == &bwobj)
    return *this;
  
  channelID            = bwobj.channelID;
  waveTypeID           = bwobj.waveTypeID;
  frequencyVal         = bwobj.frequencyVal;
  amplifierVal         = bwobj.amplifierVal;
  offsetVal            = bwobj.offsetVal;
  dutyCycleVal         = bwobj.dutyCycleVal;
  phaseVal             = bwobj.phaseVal;
  standardDeviationVal = bwobj.standardDeviationVal ;
  meanVal              = bwobj.meanVal;
  widthVal             = bwobj.widthVal;
  riseVal              = bwobj.riseVal;
  fallVal              = bwobj.fallVal;
  delayVal             = bwobj.delayVal;

  frequencyUnit        = bwobj.frequencyUnit;
  amplifierUnit        = bwobj.amplifierUnit;
  offsetUnit           = bwobj.offsetUnit;
  dutyCycleUnit        = bwobj.dutyCycleUnit;

  waveTypeString       = bwobj.waveTypeString;
  fullCommandStream    << bwobj.fullCommandStream;
  

  return *this;
  
};



std::ostream& operator<<(std::ostream& os, const BasicWave &bwobj)
{
  int width = 30;
  os << std::setiosflags(std::ios::right);
  os << "\n ChannelID     : ";
  os << std::setw(width) << bwobj.channelID;
  os << "\n WaveTypeID    : ";
  os << std::setw(width) << bwobj.waveTypeID;
  os << "\n frequencyVal  : ";
  os << std::setw(width) << bwobj.frequencyVal;
  os << "\n amplifierVal  : ";
  os << std::setw(width) << bwobj.amplifierVal;
  os << "\n offsetVal     : ";
  os << std::setw(width) << bwobj.offsetVal;
  os << "\n dutyCycleVal  : ";
  os << std::setw(width) << bwobj.dutyCycleVal;
  os << "\n phaseVal      : ";
  os << std::setw(width) << bwobj.phaseVal;
  os << "\n standardDeviationVal : ";
  os << std::setw(width) << bwobj.standardDeviationVal ;
  os << "\n meanVal       : ";
  os << std::setw(width) << bwobj.meanVal;
  os << "\n widthVal      : ";
  os << std::setw(width) << bwobj.widthVal;
  os << "\n riseVal       : ";
  os << std::setw(width) << bwobj.riseVal;
  os << "\n fallVal       : ";
  os << std::setw(width) << bwobj.fallVal;
  os << "\n delayVal      : ";
  os << std::setw(width) << bwobj.delayVal;
  os << "\n frequencyUnit : ";
  os << std::setw(width) << bwobj.frequencyUnit;
  os << "\n amplifierUnit : ";
  os << std::setw(width) << bwobj.amplifierUnit;
  os << "\n offsetUnit    : ";
  os << std::setw(width) << bwobj.offsetUnit;
  os << "\n dutyCycleUnit : ";
  os << std::setw(width) << bwobj.dutyCycleUnit;
  os << "\n Full Cmd      : ";
  os << std::setw(width) << bwobj.fullCommandStream.str();
  return os;
  
}




