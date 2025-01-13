#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_

#include <vector>

// Event-level data
struct EventData {
    int run_id;
    int spill_id;
    int event_id;
    int fpga_bits[5];
    int nim_bits[5];

    EventData();  // Constructor
    virtual ~EventData();  // Virtual destructor
};

// Regular hit data
struct HitData {
    int detector_id;       // Detector ID
    int element_id;        // Element ID
    double tdc_time;       // TDC time
    double drift_distance; // Drift distance

    HitData();  // Constructor
    virtual ~HitData();  // Virtual destructor
};

// Trigger-specific hit data
struct TriggerHitData {
    int triggerDetectorID;          // Trigger detector ID
    int triggerElementID;           // Trigger element ID
    double triggerTdcTime;          // Trigger TDC time
    double triggerDriftDistance;    // Trigger drift distance
    bool triggerInTime;             // Trigger hit in time flag

    TriggerHitData();  // Constructor
    virtual ~TriggerHitData();  // Virtual destructor
};

// Vectors for storing hit and trigger hit data
typedef std::vector<HitData> HitList;
typedef std::vector<TriggerHitData> TriggerList;

#endif /* _TREE_DATA__H_ */
