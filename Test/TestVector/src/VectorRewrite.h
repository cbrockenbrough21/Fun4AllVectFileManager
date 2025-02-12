#ifndef VECTORREWRITE_H
#define VECTORREWRITE_H

#include <iostream>
#include <vector>
#include <map>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <TMatrixD.h>

class VectorWrite {
public:
    VectorWrite() = default;
    void OpenFile(const std::string& file_name, TFile*& m_file, TTree*& m_tree);

    // SQRun Variables
    int runID, n_spill, n_evt_all, n_evt_dec, n_phys_evt, n_phys_evt_bad;
    int n_flush_evt, n_flush_evt_bad, n_hit, n_t_hit;

    // SQEvent Variables
    int spillID, eventID, rfID, turnID, trigger_input;
    int fpgaTriggers[5];
    int nimTriggers[5];
    int rfIntensities[33];

    // SQHit Variables
    std::vector<int> hitIDs;
    std::vector<int> trackIDs;
    std::vector<int> detectorIDs;
    std::vector<int> elementIDs;
    std::vector<double> tdcTimes;
    std::vector<double> driftDistances;
    std::vector<double> pos;
    std::vector<bool> hitsInTime;
    std::vector<bool> hodo_mask;
    std::vector<bool> trigger_mask;
    std::vector<double> truth_x;
    std::vector<double> truth_y;
    std::vector<double> truth_z;
    std::vector<double> truth_px;
    std::vector<double> truth_py;
    std::vector<double> truth_pz;
    //std::vector<std::map<short, float>> hit_cells;

    // SQTriggerHit Variables
    std::vector<int> triggerDetectorIDs;
    std::vector<int> triggerElementIDs;
    std::vector<double> triggerTdcTimes;
    std::vector<double> triggerDriftDistances;
    std::vector<bool> triggerHitsInTime;

    // Compression settings for ROOT output
    int compression_algo = 1;
    int compression_level = 1;
    int m_basket_size = 32000;
    int m_auto_flush = 1000;
};

#endif // VECTORWRITE_H
