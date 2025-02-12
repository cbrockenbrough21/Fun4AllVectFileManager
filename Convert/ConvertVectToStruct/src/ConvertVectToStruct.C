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

    int run_id, spill_id, event_id, fpga_bits[5], nim_bits[5];
    std::vector<int>* detector_id = nullptr, *element_id = nullptr;
    std::vector<double>* tdc_time = nullptr, *drift_distance = nullptr;
    std::vector<int>* trigger_detector_id = nullptr, *trigger_element_id = nullptr;
    std::vector<double>* trigger_tdc_time = nullptr, *trigger_drift_distance = nullptr;
    std::vector<bool>* hits_in_time = nullptr, *trigger_in_time = nullptr;

    tree->SetBranchAddress("runID", &run_id);
    tree->SetBranchAddress("spillID", &spill_id);
    tree->SetBranchAddress("eventID", &event_id);
    tree->SetBranchAddress("fpgaTriggers", fpga_bits);
    tree->SetBranchAddress("nimTriggers", nim_bits);
    tree->SetBranchAddress("detectorIDs", &detector_id);
    tree->SetBranchAddress("elementIDs", &element_id);
    tree->SetBranchAddress("tdcTimes", &tdc_time);
    tree->SetBranchAddress("driftDistances", &drift_distance);
    tree->SetBranchAddress("hitsInTime", &hits_in_time);
    tree->SetBranchAddress("triggerDetectorIDs", &trigger_detector_id);
    tree->SetBranchAddress("triggerElementIDs", &trigger_element_id);
    tree->SetBranchAddress("triggerTdcTimes", &trigger_tdc_time);
    tree->SetBranchAddress("triggerDriftDistances", &trigger_drift_distance);
    tree->SetBranchAddress("triggerHitsInTime", &trigger_in_time);

    Long64_t nentries = tree->GetEntries();

    TStopwatch timer;
    timer.Start();

    for (Long64_t i = 0; i < nentries; i++) {
    tree->GetEntry(i);

    writer.evt.run_id = run_id;
    writer.evt.spill_id = spill_id;
    writer.evt.event_id = event_id;

    for (int ii = 0; ii < 5; ii++) {
    writer.evt.fpga_bits[ii] = fpga_bits[ii];
    writer.evt.nim_bits[ii] = nim_bits[ii];
    }

    writer.list_hit.clear();
    for (size_t j = 0; j < tdc_time->size(); j++) {
    HitData hit;
    hit.detector_id = detector_id->at(j);
    hit.element_id = element_id->at(j);
    hit.tdc_time = tdc_time->at(j);
    hit.drift_distance = drift_distance->at(j);
    writer.list_hit.push_back(hit);
    }

    // Populate trigger hit data
    writer.list_trigger_hit.clear();
    for (size_t j = 0; j < trigger_tdc_time->size(); ++j) {
    TriggerHitData trigger_hit;
    trigger_hit.triggerDetectorID = trigger_detector_id->at(j);
    trigger_hit.triggerElementID = trigger_element_id->at(j);
    trigger_hit.triggerTdcTime = trigger_tdc_time->at(j);
    trigger_hit.triggerDriftDistance = trigger_drift_distance->at(j);
    trigger_hit.triggerInTime = trigger_in_time->at(j);
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
