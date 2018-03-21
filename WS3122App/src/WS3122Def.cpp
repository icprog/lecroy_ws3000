
#include "WS3122Def.hh"
#include "WSTypes.hh"


BasicWave::BasicWave()
{
  Init();
  
};


// BasicWave::BasicWave(const BasicWave &bwobj)
// {

   
//   headerPath           = bwobj.headerPath;
//   waveTypeID           = bwobj.waveTypeID;
//   frequencyVal         = bwobj.frequencyVal;
//   amplifierVal         = bwobj.amplifierVal;
//   offsetVal            = bwobj.offsetVal;
//   dutyCycleVal         = bwobj.dutyCycleVal;
//   phaseVal             = bwobj.phaseVal;
//   stdDevVal            = bwobj.stdDevVal ;
//   meanVal              = bwobj.meanVal;
//   widthVal             = bwobj.widthVal;
//   riseVal              = bwobj.riseVal;
//   fallVal              = bwobj.fallVal;
//   delayVal             = bwobj.delayVal;

//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;

//   waveTypeString       = bwobj.waveTypeString;
//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;
//   fullCommandStream    << bwobj.fullCommandStream;

//   waveParameter
// };


BasicWave::~BasicWave()
{
};


void 
BasicWave::Init()
{

  waveTypeID   = kWaveTypeSquare;
  frequencyVal = 0.0;
  amplifierVal = 0.0;
  offsetVal    = 0;
  dutyCycleVal = 0.0;
  phaseVal     = 0;
  stdDevVal    = 0.0;
  meanVal      = 0.0;
  widthVal     = 0.0;
  riseVal      = 0.0;
  fallVal      = 0.0;
  delayVal     = 0.0;


  frequency_flag  = true;
  amplifier_flag  = true;
  offset_flag     = true;
  symmetry_flag   = true;
  duty_cycle_flag = true;
  
  width_flag      = true;
  rise_flag       = true;
  fall_flag       = true;
  delay_flag      = true;

  phase_flag      = true;
  std_dev_flag    = true;
  mean_flag       = true;

  
  
  waveTypeString.clear();
  frequencyUnit = "HZ";
  amplifierUnit = "V";
  offsetUnit    = "V";
  dutyCycleUnit = "";
  delayUnit     = "S";
  
  clearCommand();

  waveTypeMap     = CreateBasicWaveMap();
  waveParamterMap = CreateWaveParameterMap();
  headerPathMap   = CreateHeaderPathMap();
  //  cmdSymbolMap    = CreateCmdSymbolMap();
  
};

void
BasicWave::setWaveTypeID(EBasicWaveType_t id)
{
  waveTypeID = id;
  waveTypeString = waveTypeMap[id];
  this -> set_flags(id);
};

void
BasicWave::set_flags(EBasicWaveType_t id)
{
  switch (id)
    {
    case kWaveTypeSine:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeSquare:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeRamp:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = true;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypePulse:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = true;    rise_flag       = true;
      fall_flag       = true;    delay_flag      = true;
      symmetry_flag   = false;
      std_dev_flag    = false;   mean_flag       = false;
      break;
    case kWaveTypeNoise:
      frequency_flag  = false;   amplifier_flag  = false;
      offset_flag     = false;   phase_flag      = false;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = true;     mean_flag      = true;
      break;
    case kWaveTypeArb:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    case kWaveTypeDc:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
    default:
      frequency_flag  = true;    amplifier_flag  = true;
      offset_flag     = true;    phase_flag      = true;
      duty_cycle_flag = true;
      width_flag      = false;    rise_flag      = false;
      fall_flag       = false;    delay_flag     = false;
      symmetry_flag   = false;
      std_dev_flag    = false;    mean_flag      = false;
      break;
      
    }

};

// BasicWave & BasicWave::operator=(const BasicWave &bwobj)
// {
//   if (this == &bwobj)
//     return *this;
  
//   headerPath           = bwobj.headerPath;
//   waveTypeID           = bwobj.waveTypeID;
//   frequencyVal         = bwobj.frequencyVal;
//   amplifierVal         = bwobj.amplifierVal;
//   offsetVal            = bwobj.offsetVal;
//   dutyCycleVal         = bwobj.dutyCycleVal;
//   phaseVal             = bwobj.phaseVal;
//   stdDevVal            = bwobj.stdDevVal ;
//   meanVal              = bwobj.meanVal;
//   widthVal             = bwobj.widthVal;
//   riseVal              = bwobj.riseVal;
//   fallVal              = bwobj.fallVal;
//   delayVal             = bwobj.delayVal;

//   frequencyUnit        = bwobj.frequencyUnit;
//   amplifierUnit        = bwobj.amplifierUnit;
//   offsetUnit           = bwobj.offsetUnit;
//   dutyCycleUnit        = bwobj.dutyCycleUnit;

//   waveTypeString       = bwobj.waveTypeString;
//   fullCommandStream    << bwobj.fullCommandStream;
  

//   return *this;
  
// };




std::ostream& operator<<(std::ostream& os, const BasicWave &bwobj)
{
  int width = 30;
  os << std::setiosflags(std::ios::right);
  os << "\n HeaderPath    : ";
  os << std::setw(width) << bwobj.headerPath;
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
  os << std::setw(width) << bwobj.stdDevVal ;
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



void
BasicWave::Print(std::string in)
{
  std::cout << " >>> " << in << std::endl;
  int width = 40;
  std::cout
    <<
    std::setiosflags(std::ios::right) <<
    "\n HeaderPath    : " <<
    std::setw(width) << headerPath <<
    "\n WaveTypeID    : " <<
    std::setw(width) << waveTypeID <<
    "\n WaveTypeString: " <<
    std::setw(width) << waveTypeString <<
    "\n frequencyVal  : " <<
    std::setw(width) << frequencyVal <<
    "\n amplifierVal  : " <<
    std::setw(width) << amplifierVal <<
    "\n offsetVal     : " <<
    std::setw(width) << offsetVal <<
    "\n dutyCycleVal  : " <<
    std::setw(width) << dutyCycleVal <<
    "\n phaseVal      : " <<
    std::setw(width) << phaseVal <<
    "\n StandardDeVal : " <<
    std::setw(width) << stdDevVal  <<
    "\n meanVal       : " <<
    std::setw(width) << meanVal <<
    "\n widthVal      : " <<
    std::setw(width) << widthVal <<
    "\n riseVal       : " <<
    std::setw(width) << riseVal <<
    "\n fallVal       : " <<
    std::setw(width) << fallVal <<
    "\n delayVal      : " <<
    std::setw(width) << delayVal <<
    "\n frequencyUnit : " <<
    std::setw(width) << frequencyUnit <<
    "\n amplifierUnit : " <<
    std::setw(width) << amplifierUnit <<
    "\n offsetUnit    : " <<
    std::setw(width) << offsetUnit <<
    "\n dutyCycleUnit : " <<
    std::setw(width) << dutyCycleUnit <<
    "\n delayUnit     : " <<
    std::setw(width) << delayUnit <<
    "\n Full Cmd      : " <<
    std::setw(width) << fullCommandStream.str() <<std::endl;
}



void
BasicWave::buildCommand()
{

  bool front_comma = true;
  
  if ( ! carrier_flag ) fullCommandStream << headerPathMap[headerPathID];
  fullCommandStream << getBasicWaveCmdSnip();
  fullCommandStream << getWaveCmdSnip(frequency_flag,  kWaveFreq,    frequencyVal, front_comma);
  fullCommandStream << getWaveCmdSnip(amplifier_flag,  kWaveAmpl,    amplifierVal, front_comma);
  fullCommandStream << getWaveCmdSnip(offset_flag,     kWaveOffset,  offsetVal,    front_comma);
  fullCommandStream << getWaveCmdSnip(phase_flag,      kWavePhase,   phaseVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(duty_cycle_flag, kWaveDuty,    dutyCycleVal, front_comma);
  fullCommandStream << getWaveCmdSnip(width_flag,      kWaveWidth,   widthVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(rise_flag,       kWaveRise,    riseVal,      front_comma);
  fullCommandStream << getWaveCmdSnip(fall_flag,       kWaveFall,    fallVal,      front_comma);
  fullCommandStream << getWaveCmdSnip(delay_flag,      kWaveDelay,   delayVal,     front_comma);
  fullCommandStream << getWaveCmdSnip(symmetry_flag,   kWaveSymm,    symmetryVal,  front_comma);
  fullCommandStream << getWaveCmdSnip(std_dev_flag,    kWaveStdDev,  stdDevVal,    front_comma);
  fullCommandStream << getWaveCmdSnip(mean_flag,       kWaveMean,    meanVal,      front_comma);
 
};

void
BasicWave::clearCommand()
{
  fullCommandStream.str("");
  fullCommandStream.clear();
  //  std::cout << "Is it clean? " << fullCommandStream.str() << std::endl;
}

const std::string
BasicWave::getBasicWaveCmdSnip()
{
  std::string result;  result.clear();
  result = "";
  
  if ( IsCarrier() ) {
    result += GetCmdSymbol(kCmdSymbolComma);
    result += "CARR";
    result += GetCmdSymbol(kCmdSymbolComma);
    result += "WVTP";
  }
  else {
    result += GetCmdSymbol(kCmdSymbolColon);
    result += GetHeaderType(kHeaderBSWV);
    result += GetCmdSymbol(kCmdSymbolBlank);
    result += "WVTP";
  }
  result += GetCmdSymbol(kCmdSymbolComma);
  result += waveTypeMap[waveTypeID];
  
  return result;
  
};

const std::string
BasicWave::getWaveCmdSnip(bool cmd_flag, EWaveParameter_t id, double value, bool front_prefix)
{
  std::ostringstream result;
  std::string        unit;

  if(cmd_flag) {
    if (front_prefix) result << GetCmdSymbol(kCmdSymbolComma);
    // if ( IsCarrier() ) {
    //   result << "CARR";
    //   result << GetCmdSymbol(kCmdSymbolComma);
    // }
    result << waveParamterMap[id];
    result << GetCmdSymbol(kCmdSymbolComma);
    result << value;
    switch (id)
      {
      case kWaveFreq:   unit=frequencyUnit; break;
      case kWaveAmpl:   unit=amplifierUnit; break;
      case kWaveOffset: unit=offsetUnit;    break;
      case kWaveDuty:   unit=dutyCycleUnit; break;
      case kWaveDelay:  unit=delayUnit;     break;
      default: unit.clear(); break;
	
      }
    result << unit;
  }
  else {
    result.str("");
    result.clear();
  }

  return result.str();
};





BurstWave::BurstWave()
{
  Init();
};



BurstWave::~BurstWave()
{
};


void
BurstWave::Init()
{
  enable_flag      = false;

  period_flag      = true;
  period_flag      = true;
  delay_flag       = true;
  cycleTime_flag   = true;

  period_flag      = true; // PRD
  startPhase_flag  = true; // STPS
  gateNcyc_flag    = true; // GATE_NSCYC
  triggerSrc_flag  = true; // TRSR
  delay_flag       = true; // DLAY
  polarity_flag    = true; // PLRT // Always vaild
  triggerMode_flag = true; // TRMD
  edge_flag        = true; // EDGE
  cycleTime_flag   = true; // TIME
  mtrig_flag       = true; // MAN
  
  
  periodVal      = 0.0;
  startPhaseVal  = 0.0;
  delayVal       = 0.0;
  cycleTimeVal   = 0.0;

  periodUnit     = "S";
  delayUnit      = "S";
  
  clearCommand();

  headerPathID       = kHeaderPathUnknown;
  carrierWaveTypeID  = kWaveTypeUnknown;
  //burstMode          = kBurstModeUnknown;
  // triggerSrc         = kTriggerSrcUnknown;
  
  headerPathMap      = CreateHeaderPathMap();
  carrierWaveTypeMap = CreateBasicWaveMap();
  // burstModeMap       = CreateBurstModeMap();
  //  triggerSrcMap      = CreateTriggerSrcMap();

  burstParameterMap  = CreateBurstParameterMap();
  
};


void
BurstWave::clearCommand()
{
  fullCmdStream.str("");
  fullCmdStream.clear();
  carrierCmdString.clear();

  fullCmdStream_flag = false;
  carrierCmd_flag    = false;
  
};



void
BurstWave::buildCommand()
{

  
  bool front_comma = true;

  if ( IsEnable() ) {
    fullCmdStream << this->getBurstCmdSnip();

    // GATE_NCYC
    fullCmdStream << this->getCmdSnip(gateNcyc_flag,      kBurstParGateNcyc,    front_comma);
    // TRSR
    fullCmdStream << this->getCmdSnip(triggerSrc_flag,    kBurstParTriggerSrc,  front_comma);
    // TRMD
    fullCmdStream << this->getCmdSnip(triggerMode_flag,   kBurstParTriggerMode, front_comma);
    // EDGE
    fullCmdStream << this->getCmdSnip(edge_flag,          kBurstParEdge,        front_comma);
    // PLRT
    fullCmdStream << this->getCmdSnip(polarity_flag,      kBurstParPolarity,    front_comma);
    
    
    // PRD
    fullCmdStream << this->getCmdSnip(period_flag,        kBurstParPeriod,      front_comma, periodVal);
    // STPS
    fullCmdStream << this->getCmdSnip(startPhase_flag,    kBurstParStartPhase,  front_comma, startPhaseVal);
    // DLAY
    fullCmdStream << this->getCmdSnip(delay_flag,         kBurstParDelay,       front_comma, delayVal);
    // TIME
    fullCmdStream << this->getCmdSnip(cycleTime_flag,     kBurstParTime,        front_comma, cycleTimeVal);
  }
  
  fullCmdStream << this->getCarrierCmd();
  
  fullCmdStream_flag = true;

};


void
BurstWave::Print(std::string in)
{
  std::cout << " >>> " << in << std::endl;
  int width = 40;
  std::cout
    <<
    std::setiosflags(std::ios::right) <<
    // "\n HeaderPath    : " <<
    // std::setw(width) << headerPath <<
    // "\n WaveTypeID    : " <<
    // std::setw(width) << waveTypeID <<
    // "\n WaveTypeString: " <<
    // std::setw(width) << waveTypeString <<
    "\n peroidVal : " << 
    std::setw(width) << periodVal <<
    "\n startPhaseVal : " << 
    std::setw(width) << startPhaseVal <<
    "\n delayVal      : " << 
    std::setw(width) << delayVal <<
    "\n cycleTimeVal       : " << 
    std::setw(width) << cycleTimeVal <<
    "\n Full Cmd      : " <<
    std::setw(width) << fullCmdStream.str() <<
    std::endl;
};



void
BurstWave::setCarrierWaveTypeID(EBasicWaveType_t id)
{
  carrierWaveTypeID     = id;
  carrierWaveTypeString = carrierWaveTypeMap[id];
   
};

void
BurstWave::setCarrierCmd(std::string in)
{
  carrierCmdString = in;
  carrierCmd_flag = true; 
};

void
BurstWave::setBurstMode(EBurstMode_t id)
{
  burstModeID     = id;
  burstModeString = GetBurstMode(id);
 
};


void
BurstWave::setTriggerSrc(ETriggerSrc_t id)
{
  triggerSrcID = id;
  triggerSrcString = GetTriggerSrc(id);
};


void
BurstWave::setTriggerMode(ETriggerMode_t id)
{
  triggerModeID = id;
  triggerModeString = GetTriggerMode(id);
};

void
BurstWave::setEdge(EEdgeModeMap_t id)
{
  edgeModeID = id;
  edgeModeString = GetEdgeMode(id);
  
};


void
BurstWave::setPolarity(EPolarityMap_t id)
{
  polarityID = id;
  polarityString = GetPolarity(id);
};


void
BurstWave::setWaveState(EWaveState_t id)
{
  waveStateID = id;
  waveStateString = GetWaveState(id);
};





//  WS3122           , Commands    , Screen
//  period_flag      , PRD         , Burst Period
//  startPhase_flag  , STPS        , Start Phase
//  gateNcyc_flag    , GATE_NSCYC  , NCycle / Gated
//  triggerSrc_flag  , TRSR        , Source (Internal, External, Manual, Cancel)
//  delay_flag       , DLAY        , Delay
//  polarity_flag    , PLRT        , 
//  triggerMode_flag , TRMD        , Trig Out (Down, Up, Off)
//  edge_flag        , EDGE        , Edge  (Down, Up)
//  cycleTime_flag   , TIME        , Cycles / Infinites
//  mtrig_flag       , MAN

int 
BurstWave::set_flags(EBasicWaveType_t wave_type_id, EBurstMode_t gate_ncyc_id, ETriggerSrc_t trigger_src_id)
{
  std::cout
  << "WaveType     : " << GetBasicWaveType (wave_type_id)
  << "\tBurstMode  : " << GetBurstMode (gate_ncyc_id)
  << "\tTriggerSrc : " << GetTriggerSrc (trigger_src_id)
  << std::endl;

  if (wave_type_id == kWaveTypeDc || wave_type_id == kWaveTypeUnknown) {
    // DC cannot be used BURST mode
    this -> setEnable(false);
    return 0;
  }
  
  gateNcyc_flag    = true;    // GATE_NSCYC
  
  if (gate_ncyc_id == kBurstModeGate) {
    
    polarity_flag    = true;    // PLRT
    period_flag      = false;   // PRD
    triggerSrc_flag  = false;   // TRSR
    delay_flag       = false;   // DLAY
    triggerMode_flag = false;   // TRMD
    edge_flag        = false;   // EDGE
    cycleTime_flag   = false;   // TIME
    mtrig_flag       = false;   // MAN
    
    if( wave_type_id == kWaveTypeNoise || wave_type_id == kWaveTypePulse ) {
      startPhase_flag  = false;   // STPS
    }
    else {
      startPhase_flag  = true;    // STPS
    }
  }
  else if (gate_ncyc_id == kBurstModeNCycle ) {

    polarity_flag = false;
    
    if      ( wave_type_id == kWaveTypeNoise ) {
      this-> setEnable(false);
    }

    else if ( wave_type_id == kWaveTypePulse ) {
      
      startPhase_flag  = false;     // STPS
      delay_flag       = true;      // DLAY
      cycleTime_flag   = true;      // TIME
      triggerSrc_flag  = true;      // TRSR
      
      if      ( trigger_src_id == kTriggerSrcExternal ) {
	edge_flag        = true;    // EDG
	period_flag      = false;   // PRD
	triggerMode_flag = false;   // TRMD
	mtrig_flag       = false;   // MAN
      }
      else if ( trigger_src_id == kTriggerSrcInternal ) {
	edge_flag        = false;   // EDG
	period_flag      = true;    // PRD
	triggerMode_flag = true;    // TRMD
	mtrig_flag       = false;   // MAN
      }
      else if ( trigger_src_id == kTriggerSrcManual ) {
	edge_flag        = false;   // EDG
	period_flag      = false;   // PRD
	triggerMode_flag = true;    // TRMD
	mtrig_flag       = true;    // MAN
      }
      
    } // kWaveTypePulse
    else { // kWaveTypeSine, kWaveTypeSquare, kWaveTypeSquare, kWaveTypeArb
      startPhase_flag  = true;      // STPS
      delay_flag       = true;      // DLAY
      cycleTime_flag   = true;      // TIME
      triggerSrc_flag  = true;      // TRSR
      
      if      ( trigger_src_id == kTriggerSrcExternal ) {
	edge_flag        = true;    // EDG
	period_flag      = false;   // PRD
	triggerMode_flag = false;   // TRMD
	mtrig_flag       = false;   // MAN
      }
      else if ( trigger_src_id == kTriggerSrcInternal ) {
	edge_flag        = false;   // EDG
	period_flag      = true;    // PRD
	triggerMode_flag = true;    // TRMD
	mtrig_flag       = false;   // MAN
      }
      else if ( trigger_src_id == kTriggerSrcManual ) {
	edge_flag        = false;   // EDG
	period_flag      = false;   // PRD
	triggerMode_flag = true;    // TRMD
	mtrig_flag       = true;    // MAN
      }
      
    }  // kWaveTypeSine, kWaveTypeSquare, kWaveTypeSquare, kWaveTypeArb
  } // kBurstModeNcycle

  return 0;
    
}


const std::string
BurstWave::getCmdSnip(bool cmd_flag, EBurstParameter_t id, bool front_prefix, double value)
{
  std::ostringstream  result; 
  std::string  unit; unit.clear();
  
  
  if(cmd_flag) {
    if (front_prefix) result << GetCmdSymbol(kCmdSymbolComma);
    result << burstParameterMap[id];
    result << GetCmdSymbol(kCmdSymbolComma);
    result << value;
    switch (id)
      {
      case kBurstParPeriod:   unit=periodUnit; break;
      case kBurstParDelay:    unit=delayUnit;  break;
      default: unit.clear(); break;
      }
    result << unit;
  }
  else {
    result.str("");
    result.clear();
  }
  

  return result.str();
};

const std::string
BurstWave::getCmdSnip(bool cmd_flag, EBurstParameter_t id, bool front_prefix)
{
  std::ostringstream  result; 
  
  
  if(cmd_flag) {
    if (front_prefix) result << GetCmdSymbol(kCmdSymbolComma);
    result << burstParameterMap[id];
    result << GetCmdSymbol(kCmdSymbolComma);
    switch (id)
      {
      case kBurstParGateNcyc:
	result << burstModeString;
	break;
      case kBurstParTriggerSrc:
	result << triggerSrcString;
	break;
      case kBurstParTriggerMode:
	result << triggerModeString;
	break;
      case kBurstParEdge:
	result << edgeModeString;
	break;
      case kBurstParPolarity:
	result << polarityString;
	break;
      default:
	result.str("");
	break;
      }
  }
  else {
    result.str("");
  }
  

  return result.str();
};



const std::string
BurstWave::getBurstCmdSnip()
{
  std::string result; result.clear();
  result = "";
  result += headerPathMap[headerPathID];
  result += GetCmdSymbol(kCmdSymbolColon);
  result += GetHeaderType(kHeaderBTWV);
  result += GetCmdSymbol(kCmdSymbolBlank);
  result += "STATE";
  result += GetCmdSymbol(kCmdSymbolComma);
  result += waveStateString;
  return result;
};



void
BurstWave::setEnable(bool in)
{
  if (in) {
    // true, is it safe to set all true?
    enable_flag      = true;
    // period_flag      = true;   // PRD
    // startPhase_flag  = true;   // STPS
    // gateNcyc_flag    = true;   // GATE_NSCYC
    // triggerSrc_flag  = true;   // TRSR
    // delay_flag       = true;   // DLAY
    // polarity_flag    = true;   // PLRT
    // triggerMode_flag = true;   // TRMD
    // edge_flag        = true;   // EDGE
    // cycleTime_flag   = true;   // TIME
    // mtrig_flag       = true;   // MAN
  }
  else {
    // false, all other have no meanings
    enable_flag      = false;
    period_flag      = false;   // PRD
    startPhase_flag  = false;   // STPS
    gateNcyc_flag    = false;   // GATE_NSCYC
    triggerSrc_flag  = false;   // TRSR
    delay_flag       = false;   // DLAY
    polarity_flag    = false;   // PLRT
    triggerMode_flag = false;   // TRMD
    edge_flag        = false;   // EDGE
    cycleTime_flag   = false;   // TIME
    mtrig_flag       = false;   // MAN
  }
}
