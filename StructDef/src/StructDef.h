#ifndef _TREE_DATA__H_
#define _TREE_DATA__H_

#include <vector>
#include <map>

// -------------------------------
// SQRun Data Structure
// -------------------------------
struct RunData {
    int runID;
    int n_spill;
    int n_evt_all;
    int n_evt_dec;
    int n_phys_evt;
    int n_phys_evt_bad;
    int n_flush_evt;
    int n_flush_evt_bad;
    int n_hit;
    int n_t_hit;

    RunData();  // Constructor
    virtual ~RunData();  // Virtual destructor
};

// -------------------------------
// SQEvent Data Structure
// -------------------------------
struct EventData {
    int spillID;
    int eventID;
    int rfID;
    int turnID;
    int fpgaTriggers[5];
    int nimTriggers[5];
    int rfIntensities[33];
    int trigger_input;

    EventData();  // Constructor
    virtual ~EventData();  // Virtual destructor
};

// -------------------------------
// SQHit Data Structure
// -------------------------------
struct HitData {
    int hitID;
    int trackID;
    int detectorID;
    int elementID;
    double tdcTime;
    double driftDistance;
    bool hitsInTime;

    bool hodo_mask;
    bool trigger_mask;
    double truth_x;
    double truth_y;
    double truth_z;
    double truth_px;
    double truth_py;
    double truth_pz;
    double pos;  // get_pos() equivalent

    //Not including now
    //std::map<short, float> cell;  // get_cell() equivalent

    HitData();  // Constructor
    virtual ~HitData();  // Virtual destructor
};

// -------------------------------
// SQTriggerHit Data Structure
// -------------------------------
struct TriggerHitData {
    int triggerDetectorID;          // Trigger detector ID
    int triggerElementID;           // Trigger element ID
    double triggerTdcTime;          // Trigger TDC time
    double triggerDriftDistance;    // Trigger drift distance
    bool triggerInTime;             // Trigger hit in time flag

    TriggerHitData();  // Constructor
    virtual ~TriggerHitData();  // Virtual destructor
};

// -------------------------------
// Data Storage Vectors
// -------------------------------
typedef std::vector<HitData> HitList;
typedef std::vector<TriggerHitData> TriggerList;

#endif /* _TREE_DATA__H_ */

