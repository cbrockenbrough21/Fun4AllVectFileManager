#include "VectorRewrite.h"
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>  
#include <iomanip>

// Function to get file size
std::uintmax_t getFileSize(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0]
                  << " <input_file> <output_file> <compression_algo>"
                  << " <compression_level> <basket_size> <autoflush>" 
                  << std::endl;
        return 1;
    }

    // Parse command-line arguments
    std::string input_file_path  = argv[1];
    std::string output_file_path = argv[2];
    int compression_algo         = std::atoi(argv[3]);
    int compression_level        = std::atoi(argv[4]);
    int basket_size              = std::atoi(argv[5]);
    Long64_t autoflush           = std::atoll(argv[6]);

    // Open the input ROOT file
    TFile* input_file = TFile::Open(input_file_path.c_str(), "READ");
    if (!input_file || input_file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << input_file_path << std::endl;
        return 1;
    }

    // Get the TTree named "tree"
    TTree* input_tree = dynamic_cast<TTree*>(input_file->Get("tree"));
    if (!input_tree) {
        std::cerr << "Error: Could not find TTree 'tree' in file " 
                  << input_file_path << std::endl;
        input_file->Close();
        return 1;
    }

    // SQRun Variables
    int runID, n_spill, n_evt_all, n_evt_dec, n_phys_evt, n_phys_evt_bad;
    int n_flush_evt, n_flush_evt_bad, n_hit, n_t_hit;

    // SQEvent Variables
    int spillID, eventID, rfID, turnID, trigger_input;
    int fpgaTriggers[5], nimTriggers[5], rfIntensities[33];

    // SQHit Variables
    std::vector<int> *hitIDs = nullptr, *trackIDs = nullptr;
    std::vector<int> *detectorIDs = nullptr, *elementIDs = nullptr;
    std::vector<double> *tdcTimes = nullptr, *driftDistances = nullptr;
    std::vector<bool> *hitsInTime = nullptr, *hodo_mask = nullptr, *trigger_mask = nullptr;
    std::vector<double> *truth_x = nullptr, *truth_y = nullptr, *truth_z = nullptr;
    std::vector<double> *truth_px = nullptr, *truth_py = nullptr, *truth_pz = nullptr;
    std::vector<double> *pos = nullptr;  

    // SQTriggerHit Variables
    std::vector<int> *triggerDetectorIDs = nullptr, *triggerElementIDs = nullptr;
    std::vector<double> *triggerTdcTimes = nullptr, *triggerDriftDistances = nullptr;
    std::vector<bool> *triggerHitsInTime = nullptr;

    // 4. Set up branch addresses for reading
    input_tree->SetBranchAddress("runID", &runID);
    input_tree->SetBranchAddress("n_spill", &n_spill);
    input_tree->SetBranchAddress("n_evt_all", &n_evt_all);
    input_tree->SetBranchAddress("n_evt_dec", &n_evt_dec);
    input_tree->SetBranchAddress("n_phys_evt", &n_phys_evt);
    input_tree->SetBranchAddress("n_phys_evt_bad", &n_phys_evt_bad);
    input_tree->SetBranchAddress("n_flush_evt", &n_flush_evt);
    input_tree->SetBranchAddress("n_flush_evt_bad", &n_flush_evt_bad);
    input_tree->SetBranchAddress("n_hit", &n_hit);
    input_tree->SetBranchAddress("n_t_hit", &n_t_hit);

    input_tree->SetBranchAddress("spillID", &spillID);
    input_tree->SetBranchAddress("eventID", &eventID);
    input_tree->SetBranchAddress("rfID", &rfID);
    input_tree->SetBranchAddress("turnID", &turnID);
    input_tree->SetBranchAddress("fpgaTriggers", fpgaTriggers);
    input_tree->SetBranchAddress("nimTriggers", nimTriggers);
    input_tree->SetBranchAddress("rfIntensities", rfIntensities);
    input_tree->SetBranchAddress("trigger_input", &trigger_input);

    input_tree->SetBranchAddress("hitIDs", &hitIDs);
    input_tree->SetBranchAddress("trackIDs", &trackIDs);
    input_tree->SetBranchAddress("detectorIDs", &detectorIDs);
    input_tree->SetBranchAddress("elementIDs", &elementIDs);
    input_tree->SetBranchAddress("tdcTimes", &tdcTimes);
    input_tree->SetBranchAddress("driftDistances", &driftDistances);
    input_tree->SetBranchAddress("pos", &pos);
    input_tree->SetBranchAddress("hitsInTime", &hitsInTime);
    input_tree->SetBranchAddress("hodo_mask", &hodo_mask);
    input_tree->SetBranchAddress("trigger_mask", &trigger_mask);
    input_tree->SetBranchAddress("truth_x", &truth_x);
    input_tree->SetBranchAddress("truth_y", &truth_y);
    input_tree->SetBranchAddress("truth_z", &truth_z);
    input_tree->SetBranchAddress("truth_px", &truth_px);
    input_tree->SetBranchAddress("truth_py", &truth_py);
    input_tree->SetBranchAddress("truth_pz", &truth_pz);

    input_tree->SetBranchAddress("triggerDetectorIDs", &triggerDetectorIDs);
    input_tree->SetBranchAddress("triggerElementIDs", &triggerElementIDs);
    input_tree->SetBranchAddress("triggerTdcTimes", &triggerTdcTimes);
    input_tree->SetBranchAddress("triggerDriftDistances", &triggerDriftDistances);
    input_tree->SetBranchAddress("triggerHitsInTime", &triggerHitsInTime);
    // 5. Open/create the output file
    TFile* output_file = new TFile(output_file_path.c_str(), "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Could not create output file " 
                  << output_file_path << std::endl;
        input_file->Close();
        return 1;
    }

    // Set compression parameters on the output file
    output_file->SetCompressionAlgorithm(compression_algo);
    output_file->SetCompressionLevel(compression_level);

    // 6. Create an output TTree with the same branch structure
    TTree* output_tree = new TTree("tree", "Rewritten TTree");
    
    // Basic scalar branches
    output_tree->Branch("runID",         &runID,       "runID/I");
    output_tree->Branch("n_spill",       &n_spill,     "n_spill/I");
    output_tree->Branch("n_evt_all",     &n_evt_all,   "n_evt_all/I");
    output_tree->Branch("n_evt_dec",     &n_evt_dec,   "n_evt_dec/I");
    output_tree->Branch("n_phys_evt",    &n_phys_evt,  "n_phys_evt/I");
    output_tree->Branch("n_phys_evt_bad",&n_phys_evt_bad, "n_phys_evt_bad/I");
    output_tree->Branch("n_flush_evt",   &n_flush_evt, "n_flush_evt/I");
    output_tree->Branch("n_flush_evt_bad",&n_flush_evt_bad, "n_flush_evt_bad/I");
    output_tree->Branch("n_hit",         &n_hit,       "n_hit/I");
    output_tree->Branch("n_t_hit",       &n_t_hit,     "n_t_hit/I");

    // SQEvent Variables (Event-Level Scalars)
    output_tree->Branch("spillID",       &spillID,     "spillID/I");
    output_tree->Branch("eventID",       &eventID,     "eventID/I");
    output_tree->Branch("rfID",          &rfID,        "rfID/I");
    output_tree->Branch("turnID",        &turnID,      "turnID/I");
    output_tree->Branch("trigger_input", &trigger_input, "trigger_input/I");

    // Arrays (Fixed-Size Arrays)
    output_tree->Branch("rfIntensities",  rfIntensities, "rfIntensities[33]/I");
    output_tree->Branch("fpgaTriggers",   fpgaTriggers,  "fpgaTriggers[5]/I");
    output_tree->Branch("nimTriggers",    nimTriggers,   "nimTriggers[5]/I");

    // SQHit Variables (Vectors)
    output_tree->Branch("hitIDs",          &hitIDs);
    output_tree->Branch("trackIDs",        &trackIDs);
    output_tree->Branch("detectorIDs",     &detectorIDs);
    output_tree->Branch("elementIDs",      &elementIDs);
    output_tree->Branch("tdcTimes",        &tdcTimes);
    output_tree->Branch("driftDistances",  &driftDistances);
    output_tree->Branch("pos",             &pos);
    output_tree->Branch("hitsInTime",      &hitsInTime);
    output_tree->Branch("hodo_mask",       &hodo_mask);
    output_tree->Branch("trigger_mask",    &trigger_mask);
    output_tree->Branch("truth_x",         &truth_x);
    output_tree->Branch("truth_y",         &truth_y);
    output_tree->Branch("truth_z",         &truth_z);
    output_tree->Branch("truth_px",        &truth_px);
    output_tree->Branch("truth_py",        &truth_py);
    output_tree->Branch("truth_pz",        &truth_pz);

    // SQTriggerHit Variables (Vectors)
    output_tree->Branch("triggerDetectorIDs",    &triggerDetectorIDs);
    output_tree->Branch("triggerElementIDs",     &triggerElementIDs);
    output_tree->Branch("triggerTdcTimes",       &triggerTdcTimes);
    output_tree->Branch("triggerDriftDistances", &triggerDriftDistances);
    output_tree->Branch("triggerHitsInTime",     &triggerHitsInTime);

    if (autoflush != 0) {
        output_tree->SetAutoFlush(autoflush);
    }
    output_tree->SetBasketSize("*", basket_size);

    // 7. Loop over the input TTree entries and fill the output TTree
    Long64_t nEntries = input_tree->GetEntries();
    TStopwatch timer;
    timer.Start();

    for (Long64_t i = 0; i < nEntries; i++) {
        input_tree->GetEntry(i);
        output_tree->Fill();
    }

    // Write and close
    output_file->Write("", TObject::kOverwrite);
    output_file->Close();
    input_file->Close();

    timer.Stop();
    double real_time = timer.RealTime();

    // Get the size of the output file
    std::uintmax_t file_size = getFileSize(output_file_path);

    std::cout << "WRITE_TIME=" << real_time 
              << " FILE_SIZE=" << std::fixed << std::setprecision(3)
              << (static_cast<double>(file_size) / (1024.0 * 1024.0)) << "MB" 
              << std::endl;

    delete output_file;
    delete input_file;

    return 0;
}
