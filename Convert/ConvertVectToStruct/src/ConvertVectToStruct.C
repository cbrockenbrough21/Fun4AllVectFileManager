#include "ConvertVectToStruct.h"
#include "StructDef.h"
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <fstream>

using namespace std;

std::uintmax_t getFileSize(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); // Returns the file size
}



void StructWrite::OpenFile(const std::string &file_name, TFile *&m_file, TTree *&m_tree)
{
    if (m_file && m_tree) {
        std::cout << "File and tree already opened, skipping." << std::endl;
        return;
    }

    std::cout << "DEBUG: Attempting to open output file: [" << file_name << "]" << std::endl;

    if (file_name.empty()) {
        std::cerr << "Error: Output file name is empty!" << std::endl;
        exit(1);
    }

    m_file = new TFile(file_name.c_str(), "RECREATE");
    if (!m_file || m_file->IsZombie()) {
        std::cerr << "Error: Could not create file " << file_name << std::endl;
        exit(1);
    } else {
        std::cout << "DEBUG: File " << m_file->GetName() << " opened successfully." << std::endl;
    }

    // Enable multithreading
    //ROOT::EnableImplicitMT();

    m_file->SetCompressionAlgorithm(compression_algo);
    m_file->SetCompressionLevel(compression_level);

    m_tree = new TTree("tree", "Tree for storing events");

    if (!m_tree) {
        std::cerr << "Error: Could not create tree" << std::endl;
        exit(1);
    } else {
        std::cout << m_tree->GetName() << " created successfully." << std::endl;
    }

    m_tree->Branch("evt", &evt);
    m_tree->Branch("run", &run);
    m_tree->Branch("list_hit", &list_hit);
    m_tree->Branch("list_trigger_hit", &list_trigger_hit);

    // m_tree->SetAutoFlush(m_auto_flush);
    // m_tree->SetBasketSize("*", m_basket_size);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

   // Get file names from command-line arguments
    string input_file_path = argv[1];
    string output_file_path = argv[2];

    StructWrite writer;

    TFile *m_file = nullptr;
    TTree *m_tree = nullptr;

    
    writer.OpenFile(output_file_path, m_file, m_tree);

    TFile *file = TFile::Open(input_file_path.c_str());
    if (!file || file->IsZombie()) {
        cerr << "Error: Could not open input file " << input_file_path << endl;
        return 1;
    }

    TTree *tree = (TTree *)file->Get("tree");
    if (!tree) {
        cerr << "Error: Could not find TTree in input file " << input_file_path << endl;
        return 1;
    }

    int spillID, eventID, rfID, turnID, trigger_input;
    int fpgaTriggers[5], nimTriggers[5], rfIntensities[33];

    int runID, n_spill, n_evt_all, n_evt_dec, n_phys_evt, n_phys_evt_bad;
    int n_flush_evt, n_flush_evt_bad, n_hit, n_t_hit;

    std::vector<int>* hitID = nullptr;
    std::vector<int>* trackID = nullptr;
    std::vector<int>* detectorID = nullptr, *elementID = nullptr;
    std::vector<double>* tdcTime = nullptr, *driftDistance = nullptr;
    std::vector<bool>* hitsInTime = nullptr;

    std::vector<bool>* hodo_mask = nullptr, *trigger_mask = nullptr;
    std::vector<double>* truth_x = nullptr, *truth_y = nullptr, *truth_z = nullptr;
    std::vector<double>* truth_px = nullptr, *truth_py = nullptr, *truth_pz = nullptr;
    std::vector<double>* pos = nullptr;

    std::vector<int>* triggerDetectorID = nullptr, *triggerElementID = nullptr;
    std::vector<double>* triggerTdcTime = nullptr, *triggerDriftDistance = nullptr;
    std::vector<bool>* triggerInTime = nullptr;

    tree->SetBranchAddress("spillID", &spillID);
    tree->SetBranchAddress("eventID", &eventID);
    tree->SetBranchAddress("rfID", &rfID);
    tree->SetBranchAddress("turnID", &turnID);
    tree->SetBranchAddress("trigger_input", &trigger_input);
    tree->SetBranchAddress("fpgaTriggers", fpgaTriggers);
    tree->SetBranchAddress("nimTriggers", nimTriggers);
    tree->SetBranchAddress("rfIntensities", rfIntensities);

    tree->SetBranchAddress("runID", &runID);
    tree->SetBranchAddress("n_spill", &n_spill);
    tree->SetBranchAddress("n_evt_all", &n_evt_all);
    tree->SetBranchAddress("n_evt_dec", &n_evt_dec);
    tree->SetBranchAddress("n_phys_evt", &n_phys_evt);
    tree->SetBranchAddress("n_phys_evt_bad", &n_phys_evt_bad);
    tree->SetBranchAddress("n_flush_evt", &n_flush_evt);
    tree->SetBranchAddress("n_flush_evt_bad", &n_flush_evt_bad);
    tree->SetBranchAddress("n_hit", &n_hit);
    tree->SetBranchAddress("n_t_hit", &n_t_hit);

    tree->SetBranchAddress("hitIDs", &hitID);
    tree->SetBranchAddress("trackIDs", &trackID);
    tree->SetBranchAddress("detectorIDs", &detectorID);
    tree->SetBranchAddress("elementIDs", &elementID);
    tree->SetBranchAddress("tdcTimes", &tdcTime);
    tree->SetBranchAddress("driftDistances", &driftDistance);
    tree->SetBranchAddress("hitsInTime", &hitsInTime);
    tree->SetBranchAddress("pos", &pos);

    tree->SetBranchAddress("hodo_mask", &hodo_mask);
    tree->SetBranchAddress("trigger_mask", &trigger_mask);
    tree->SetBranchAddress("truth_x", &truth_x);
    tree->SetBranchAddress("truth_y", &truth_y);
    tree->SetBranchAddress("truth_z", &truth_z);
    tree->SetBranchAddress("truth_px", &truth_px);
    tree->SetBranchAddress("truth_py", &truth_py);
    tree->SetBranchAddress("truth_pz", &truth_pz);

    tree->SetBranchAddress("triggerDetectorIDs", &triggerDetectorID);
    tree->SetBranchAddress("triggerElementIDs", &triggerElementID);
    tree->SetBranchAddress("triggerTdcTimes", &triggerTdcTime);
    tree->SetBranchAddress("triggerDriftDistances", &triggerDriftDistance);
    tree->SetBranchAddress("triggerHitsInTime", &triggerInTime);


    Long64_t nentries = tree->GetEntries();

    TStopwatch timer;
    timer.Start();

    for (Long64_t i = 0; i < nentries; i++) {
    tree->GetEntry(i);

    writer.evt.spillID = spillID;
    writer.evt.eventID = eventID;
    writer.evt.rfID = rfID;
    writer.evt.turnID = turnID;
    writer.evt.trigger_input = trigger_input;

    for (int ii = 0; ii < 5; ii++) {
        writer.evt.fpgaTriggers[ii] = fpgaTriggers[ii];
        writer.evt.nimTriggers[ii] = nimTriggers[ii];
    }

    for (int ii = 0; ii < 33; ii++) {
        writer.evt.rfIntensities[ii] = rfIntensities[ii];
    }

    writer.run.runID = runID;
    writer.run.n_spill = n_spill;
    writer.run.n_evt_all = n_evt_all;
    writer.run.n_evt_dec = n_evt_dec;
    writer.run.n_phys_evt = n_phys_evt;
    writer.run.n_phys_evt_bad = n_phys_evt_bad;
    writer.run.n_flush_evt = n_flush_evt;
    writer.run.n_flush_evt_bad = n_flush_evt_bad;
    writer.run.n_hit = n_hit;
    writer.run.n_t_hit = n_t_hit;

    writer.list_hit.clear();
    for (size_t j = 0; j < tdcTime->size(); j++) {
        HitData hit;
        hit.hitID = hitID->at(j);
        hit.trackID = trackID->at(j);
        hit.detectorID = detectorID->at(j);
        hit.elementID = elementID->at(j);
        hit.tdcTime = tdcTime->at(j);
        hit.driftDistance = driftDistance->at(j);
        hit.hitsInTime = hitsInTime->at(j);

        hit.hodo_mask = hodo_mask->at(j);
        hit.trigger_mask = trigger_mask->at(j);
        hit.truth_x = truth_x->at(j);
        hit.truth_y = truth_y->at(j);
        hit.truth_z = truth_z->at(j);
        hit.truth_px = truth_px->at(j);
        hit.truth_py = truth_py->at(j);
        hit.truth_pz = truth_pz->at(j);
        hit.pos = pos->at(j);

        writer.list_hit.push_back(hit);
    }

    // Populate trigger hit data
    writer.list_trigger_hit.clear();
    for (size_t j = 0; j < triggerTdcTime->size(); ++j) {
        TriggerHitData trigger_hit;
        trigger_hit.triggerDetectorID = triggerDetectorID->at(j);
        trigger_hit.triggerElementID = triggerElementID->at(j);
        trigger_hit.triggerTdcTime = triggerTdcTime->at(j);
        trigger_hit.triggerDriftDistance = triggerDriftDistance->at(j);
        trigger_hit.triggerInTime = triggerInTime->at(j);

        writer.list_trigger_hit.push_back(trigger_hit);
    }


    m_tree->Fill();
    }

    m_file->Write("", TObject::kOverwrite);
    m_file->Close();

    // Stop the timer
    timer.Stop();
    double real_time = timer.RealTime();

    // Get the file size
    std::uintmax_t file_size = getFileSize(output_file_path);

    // Print out the file size and write time in the expected format
    cout << "WRITE_TIME=" << real_time << " FILE_SIZE="
         << static_cast<double>(file_size) / (1024 * 1024) << "MB" << endl;

    delete m_file;
    return 0;
}
