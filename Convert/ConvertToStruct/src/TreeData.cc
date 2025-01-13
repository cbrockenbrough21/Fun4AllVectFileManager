#include "TreeData.h"
using namespace std;

EventData::EventData()
  : run_id(0)
  , spill_id(0)
  , event_id(0)
{
 for (int ii = 0; ii < 5; ii++) {
    fpga_bits [ii] = 0;
    nim_bits [ii] = 0;
  }
}

 
HitData::HitData()
  : detector_id(0)
  , element_id(0)
  , tdc_time(0)
  , drift_distance(0)
{
  ;
}

TriggerHitData::TriggerHitData()
    : triggerDetectorID(0),
      triggerElementID(0),
      triggerTdcTime(0.0),
      triggerDriftDistance(0.0),
      triggerInTime(false) 
{
  ;
}

// EventData destructor
EventData::~EventData() {
    // No dynamically allocated members, so nothing to clean up
}

// HitData destructor
HitData::~HitData() {
    // No dynamically allocated members, so nothing to clean up
}

// TriggerHitData destructor
TriggerHitData::~TriggerHitData() {
    // No dynamically allocated members, so nothing to clean up
}
