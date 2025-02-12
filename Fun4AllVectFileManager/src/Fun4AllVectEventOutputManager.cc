#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <phool/phool.h>
#include <phool/getClass.h>
#include <phool/PHNode.h>
#include <phool/PHNodeIOManager.h>
#include <fun4all/Fun4AllServer.h> 
#include <interface_main/SQEvent.h>
#include <interface_main/SQRun.h>
#include <interface_main/SQSpillMap.h>
#include <interface_main/SQHitVector.h>
#include <interface_main/SQDimuonVector.h>
#include <ktracker/SRecEvent.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include "Fun4AllVectEventOutputManager.h"
#include <phool/PHNodeIterator.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>
#include <phool/getClass.h>
#include <interface_main/SQRun_v1.h>

using namespace std;

Fun4AllVectEventOutputManager::Fun4AllVectEventOutputManager(const std::string &myname)
  : Fun4AllOutputManager(myname),
    m_file(nullptr),
    m_tree(nullptr),
    m_tree_name("tree"),
    m_file_name("output.root"),
    m_evt(nullptr),
    m_sp_map(nullptr),
    m_hit_vec(nullptr),
    m_run(nullptr)  // Initialize SQRun pointer
{
    // Get the top node from Fun4AllServer
    Fun4AllServer* se = Fun4AllServer::instance();
    PHCompositeNode* topNode = se->topNode("TOP");

    // Create an iterator to search for the "RUN" node
    PHNodeIterator iter(topNode);

    // Try to find or create the "RUN" node
    PHCompositeNode* runNode = static_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "RUN"));
    if (!runNode) {
        std::cout << "[Fun4AllVectEventOutputManager] RUN node not found. Creating a new RUN node." << std::endl;
        runNode = new PHCompositeNode("RUN");
        topNode->addNode(runNode);
    }

    // Try to find SQRun in the node tree
    m_run = findNode::getClass<SQRun>(runNode, "SQRun");
    if (!m_run) {
        std::cout << "[Fun4AllVectEventOutputManager] SQRun not found! Creating a new SQRun_v1 object." << std::endl;
        m_run = new SQRun_v1();

        // Attach it to the RUN node
        PHIODataNode<PHObject>* runHeaderNode = new PHIODataNode<PHObject>(m_run, "SQRun", "PHObject");
        runNode->addNode(runHeaderNode);
    }
}

Fun4AllVectEventOutputManager::~Fun4AllVectEventOutputManager() {
    CloseFile();
}

int Fun4AllVectEventOutputManager::OpenFile(PHCompositeNode* startNode) {
    std::cout << "Fun4AllVectEventOutputManager::OpenFile(): Attempting to open file: " << m_file_name << " with tree: " << m_tree_name << std::endl;
    m_file = new TFile(m_file_name.c_str(), "RECREATE");

    if (!m_file || m_file->IsZombie()) {
        std::cerr << "Error: Could not create file " << m_file_name << std::endl;
        exit(1);
    } else {
        std::cout << "File " << m_file->GetName() << " opened successfully." << std::endl;
    }

    m_tree = new TTree(m_tree_name.c_str(), "Tree for storing events");
    if (!m_tree) {
        std::cerr << "Error: Could not create tree " << m_tree_name << std::endl;
        exit(1);
    } else {
        std::cout << "Tree " << m_tree->GetName() << " created successfully." << std::endl;
    }

    m_file->SetCompressionAlgorithm(m_compression_algo);
    m_file->SetCompressionLevel(m_compression_level);
        
   // SQRun Variables
    m_tree->Branch("runID", &runID, "runID/I");
    m_tree->Branch("n_spill", &n_spill, "n_spill/I");
    m_tree->Branch("n_evt_all", &n_evt_all, "n_evt_all/I");
    m_tree->Branch("n_evt_dec", &n_evt_dec, "n_evt_dec/I");
    m_tree->Branch("n_phys_evt", &n_phys_evt, "n_phys_evt/I");
    m_tree->Branch("n_phys_evt_bad", &n_phys_evt_bad, "n_phys_evt_bad/I");
    m_tree->Branch("n_flush_evt", &n_flush_evt, "n_flush_evt/I");
    m_tree->Branch("n_flush_evt_bad", &n_flush_evt_bad, "n_flush_evt_bad/I");
    m_tree->Branch("n_hit", &n_hit, "n_hit/I");
    m_tree->Branch("n_t_hit", &n_t_hit, "n_t_hit/I");

    // SQEvent Variables
    m_tree->Branch("spillID", &spillID, "spillID/I");
    m_tree->Branch("eventID", &eventID, "eventID/I");
    m_tree->Branch("rfID", &rfID, "rfID/I");
    m_tree->Branch("turnID", &turnID, "turnID/I");
    m_tree->Branch("fpgaTriggers", fpgaTriggers, "fpgaTriggers[5]/I");
    m_tree->Branch("nimTriggers", nimTriggers, "nimTriggers[5]/I");
    m_tree->Branch("rfIntensities", rfIntensities, "rfIntensities[33]/I");
    m_tree->Branch("trigger_input", &trigger_input, "trigger_input/I");

    // SQHit Variables
    m_tree->Branch("hitIDs", &hitIDs);
    m_tree->Branch("trackIDs", &trackIDs);
    m_tree->Branch("detectorIDs", &detectorIDs);
    m_tree->Branch("elementIDs", &elementIDs);
    m_tree->Branch("tdcTimes", &tdcTimes);
    m_tree->Branch("driftDistances", &driftDistances);
    m_tree->Branch("pos", &pos);
    m_tree->Branch("hitsInTime", &hitsInTime);
    m_tree->Branch("hodo_mask", &hodo_mask);
    m_tree->Branch("trigger_mask", &trigger_mask);
    m_tree->Branch("truth_x", &truth_x);
    m_tree->Branch("truth_y", &truth_y);
    m_tree->Branch("truth_z", &truth_z);
    m_tree->Branch("truth_px", &truth_px);
    m_tree->Branch("truth_py", &truth_py);
    m_tree->Branch("truth_pz", &truth_pz);
    m_tree->Branch("hit_cells", &hit_cells); //  Added missing `get_cell()`

    m_tree->Branch("triggerDetectorIDs", &triggerDetectorIDs);
    m_tree->Branch("triggerElementIDs", &triggerElementIDs);
    m_tree->Branch("triggerTdcTimes", &triggerTdcTimes);
    m_tree->Branch("triggerDriftDistances", &triggerDriftDistances);
    m_tree->Branch("triggerHitsInTime", &triggerHitsInTime);

    m_evt = findNode::getClass<SQEvent>(startNode, "SQEvent");
    m_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQHitVector");
    m_trig_hit_vec = findNode::getClass<SQHitVector>(startNode, "SQTriggerHitVector");

    if (!m_evt || !m_hit_vec || !m_trig_hit_vec) return Fun4AllReturnCodes::ABORTEVENT;
    return Fun4AllReturnCodes::EVENT_OK;
}
int Fun4AllVectEventOutputManager::Write(PHCompositeNode* startNode) {
        if (!m_file || !m_tree) {
                OpenFile(startNode);
    }
    ResetBranches();
    runID = m_evt->get_run_id();
    spillID = m_evt->get_spill_id();
    rfID = m_evt->get_qie_rf_id();
    eventID = m_evt->get_event_id();
    turnID = m_evt->get_qie_turn_id();
    
    fpgaTriggers[0] = m_evt->get_trigger(SQEvent::MATRIX1);
    fpgaTriggers[1] = m_evt->get_trigger(SQEvent::MATRIX2);
    fpgaTriggers[2] = m_evt->get_trigger(SQEvent::MATRIX3);
    fpgaTriggers[3] = m_evt->get_trigger(SQEvent::MATRIX4);
    fpgaTriggers[4] = m_evt->get_trigger(SQEvent::MATRIX5);
    
    nimTriggers[0] = m_evt->get_trigger(SQEvent::NIM1);
    nimTriggers[1] = m_evt->get_trigger(SQEvent::NIM2);
    nimTriggers[2] = m_evt->get_trigger(SQEvent::NIM3);
    nimTriggers[3] = m_evt->get_trigger(SQEvent::NIM4);
    nimTriggers[4] = m_evt->get_trigger(SQEvent::NIM5);

    if (!m_run) {
        std::cerr << "Error: m_run is nullptr" << std::endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }
    
    n_spill = m_run->get_n_spill();
    n_evt_all = m_run->get_n_evt_all();
    n_evt_dec = m_run->get_n_evt_dec();
    n_phys_evt = m_run->get_n_phys_evt();
    n_phys_evt_bad = m_run->get_n_phys_evt_bad();
    n_flush_evt = m_run->get_n_flush_evt();
    n_flush_evt_bad = m_run->get_n_flush_evt_bad();
    n_hit = m_run->get_n_hit();
    n_t_hit = m_run->get_n_t_hit();

    for (int i = -16; i < 16; ++i) {
        rfIntensities[i + 16] = m_evt->get_qie_rf_intensity(i);
    }

    // SQHit Variables
    if (m_hit_vec) {  
        for (size_t ihit = 0; ihit < m_hit_vec->size(); ++ihit) {
            SQHit* hit = m_hit_vec->at(ihit);
            hitIDs.push_back(hit->get_hit_id());
            trackIDs.push_back(hit->get_track_id());
            detectorIDs.push_back(hit->get_detector_id());
            elementIDs.push_back(hit->get_element_id());
            tdcTimes.push_back(hit->get_tdc_time());
            driftDistances.push_back(hit->get_drift_distance());
            pos.push_back(hit->get_pos()); 

            hitsInTime.push_back(hit->is_in_time());
            hodo_mask.push_back(hit->is_hodo_mask());
            trigger_mask.push_back(hit->is_trigger_mask());
            truth_x.push_back(hit->get_truth_x());
            truth_y.push_back(hit->get_truth_y());
            truth_z.push_back(hit->get_truth_z());
            truth_px.push_back(hit->get_truth_px());
            truth_py.push_back(hit->get_truth_py());
            truth_pz.push_back(hit->get_truth_pz());
        }
    } else {
        std::cerr << "Warning: m_hit_vec is nullptr" << std::endl;
    }

    if (m_trig_hit_vec) {
        for (int ihit = 0; ihit < m_trig_hit_vec->size(); ++ihit) {
            SQHit* hit = m_trig_hit_vec->at(ihit);
            triggerDetectorIDs.push_back(hit->get_detector_id());
            triggerElementIDs.push_back(hit->get_element_id());
            triggerTdcTimes.push_back(hit->get_tdc_time());
            triggerDriftDistances.push_back(hit->get_drift_distance());
            triggerHitsInTime.push_back(hit->is_in_time());
        }
    }

    //std::cout << "Checking m_tree before filling" << std::endl;
    if (!m_tree) {
        std::cerr << "Error: m_tree is nullptr before Fill()" << std::endl;
        return Fun4AllReturnCodes::ABORTEVENT;
    }

    // std::cout << "Filling tree with eventID: " << eventID << std::endl;
    m_tree->Fill();

    //std::cout << "Successfully wrote event: " << eventID << ", spill: " << spillID << std::endl;
    return 0;
}

void Fun4AllVectEventOutputManager::CloseFile() {
    if (!m_file) return;
    std::cout << "Fun4AllVectEventOutputManager::CloseFile(): Closing file: " << m_file_name << std::endl;
    m_file->Write();
    m_file->Close();
    delete m_file;
    m_file = nullptr;
}

void Fun4AllVectEventOutputManager::ResetBranches() {
    // SQRun Variables
    runID = 0;
    n_spill = 0;
    n_evt_all = 0;
    n_evt_dec = 0;
    n_phys_evt = 0;
    n_phys_evt_bad = 0;
    n_flush_evt = 0;
    n_flush_evt_bad = 0;
    n_hit = 0;
    n_t_hit = 0;

    // SQEvent Variables
    spillID = 0;
    eventID = 0;
    rfID = 0;
    turnID = 0;
    trigger_input = 0;

    for (int i = 0; i < 5; i++) {
        fpgaTriggers[i] = 0;
        nimTriggers[i] = 0;
    }
    for (int i = 0; i < 33; i++) {
        rfIntensities[i] = 0;
    }

    // SQHit Variables
    hitIDs.clear();
    trackIDs.clear();
    detectorIDs.clear();
    elementIDs.clear();
    tdcTimes.clear();
    driftDistances.clear();
    pos.clear();
    hitsInTime.clear();
    hodo_mask.clear();
    trigger_mask.clear();
    truth_x.clear();
    truth_y.clear();
    truth_z.clear();
    truth_px.clear();
    truth_py.clear();
    truth_pz.clear();
    hit_cells.clear();

    triggerDetectorIDs.clear();
    triggerElementIDs.clear();
    triggerTdcTimes.clear();
    triggerDriftDistances.clear();
    triggerHitsInTime.clear();
}

