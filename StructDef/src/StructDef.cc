#include "StructDef.h"


// ----------------------
// RunData Constructor
// ----------------------
RunData::RunData()
  : runID(0)
  , n_spill(0)
  , n_evt_all(0)
  , n_evt_dec(0)
  , n_phys_evt(0)
  , n_phys_evt_bad(0)
  , n_flush_evt(0)
  , n_flush_evt_bad(0)
  , n_hit(0)
  , n_t_hit(0)
{
}

// Destructor
RunData::~RunData() {}

// ----------------------
// EventData Constructor
// ----------------------
EventData::EventData()
  : spillID(0)
  , eventID(0)
  , rfID(0)
  , turnID(0)
  , trigger_input(0)
{
  for (int ii = 0; ii < 5; ii++) {
    fpgaTriggers[ii] = 0;
    nimTriggers[ii] = 0;
  }
  for (int ii = 0; ii < 33; ii++) {
    rfIntensities[ii] = 0;
  }
}

// Destructor
EventData::~EventData() {}

// ----------------------
// HitData Constructor
// ----------------------
HitData::HitData()
  : hitID(0)
  , trackID(0)
  , detectorID(0)
  , elementID(0)
  , tdcTime(0)
  , driftDistance(0)
  , hitsInTime(false)
  , hodo_mask(false)
  , trigger_mask(false)
  , truth_x(0)
  , truth_y(0)
  , truth_z(0)
  , truth_px(0)
  , truth_py(0)
  , truth_pz(0)
  , pos(0)  // get_pos() equivalent
{
}

// Destructor
HitData::~HitData() {}

// ----------------------
// TriggerHitData Constructor
// ----------------------
TriggerHitData::TriggerHitData()
    : triggerDetectorID(0),
      triggerElementID(0),
      triggerTdcTime(0.0),
      triggerDriftDistance(0.0),
      triggerInTime(false) 
{
}

// Destructor
TriggerHitData::~TriggerHitData() {}
