#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>   // for atoi(), atoll()

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

    // 1. Open the input ROOT file
    TFile* input_file = TFile::Open(input_file_path.c_str(), "READ");
    if (!input_file || input_file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << input_file_path << std::endl;
        return 1;
    }

    // 2. Get the TTree named "tree"
    TTree* input_tree = dynamic_cast<TTree*>(input_file->Get("tree"));
    if (!input_tree) {
        std::cerr << "Error: Could not find TTree 'tree' in file " 
                  << input_file_path << std::endl;
        input_file->Close();
        return 1;
    }

    // 3. Prepare variables/pointers for all branches in the input TTree
    int runID;
    int spillID;
    int eventID;
    int rfID;
    int turnID;

    // Arrays
    int rfIntensities[33];
    int fpgaTriggers[5];
    int nimTriggers[5];

    // Vector branches
    std::vector<int>* detectorIDs          = nullptr;
    std::vector<int>* elementIDs           = nullptr;
    std::vector<double>* tdcTimes          = nullptr;
    std::vector<double>* driftDistances    = nullptr;
    std::vector<bool>* hitsInTime          = nullptr;

    std::vector<int>* triggerDetectorIDs      = nullptr;
    std::vector<int>* triggerElementIDs       = nullptr;
    std::vector<double>* triggerTdcTimes      = nullptr;
    std::vector<double>* triggerDriftDistances = nullptr;
    std::vector<bool>* triggerHitsInTime      = nullptr;

    // 4. Set up branch addresses for reading
    input_tree->SetBranchAddress("runID",           &runID);
    input_tree->SetBranchAddress("spillID",         &spillID);
    input_tree->SetBranchAddress("eventID",         &eventID);
    input_tree->SetBranchAddress("rfID",            &rfID);
    input_tree->SetBranchAddress("turnID",          &turnID);
    input_tree->SetBranchAddress("rfIntensities",   rfIntensities);
    input_tree->SetBranchAddress("fpgaTriggers",    fpgaTriggers);
    input_tree->SetBranchAddress("nimTriggers",     nimTriggers);

    input_tree->SetBranchAddress("detectorIDs",     &detectorIDs);
    input_tree->SetBranchAddress("elementIDs",      &elementIDs);
    input_tree->SetBranchAddress("tdcTimes",        &tdcTimes);
    input_tree->SetBranchAddress("driftDistances",  &driftDistances);
    input_tree->SetBranchAddress("hitsInTime",      &hitsInTime);

    input_tree->SetBranchAddress("triggerDetectorIDs",     &triggerDetectorIDs);
    input_tree->SetBranchAddress("triggerElementIDs",      &triggerElementIDs);
    input_tree->SetBranchAddress("triggerTdcTimes",        &triggerTdcTimes);
    input_tree->SetBranchAddress("triggerDriftDistances",  &triggerDriftDistances);
    input_tree->SetBranchAddress("triggerHitsInTime",      &triggerHitsInTime);

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

    // If you want to apply basket size and autoflush:
    if (autoflush != 0) {
        output_tree->SetAutoFlush(autoflush);
    }
    // Setting basket size for all branches requires looping over them 
    // after they are created. But we'll do it by name (or wildcard approach won't work):
    // (We'll do it after we create them)

    // Basic scalar branches
    output_tree->Branch("runID",         &runID,       "runID/I");
    output_tree->Branch("spillID",       &spillID,     "spillID/I");
    output_tree->Branch("eventID",       &eventID,     "eventID/I");
    output_tree->Branch("rfID",          &rfID,        "rfID/I");
    output_tree->Branch("turnID",        &turnID,      "turnID/I");

    // Arrays
    output_tree->Branch("rfIntensities",  rfIntensities, "rfIntensities[33]/I");
    output_tree->Branch("fpgaTriggers",   fpgaTriggers,  "fpgaTriggers[5]/I");
    output_tree->Branch("nimTriggers",    nimTriggers,   "nimTriggers[5]/I");

    // Vector branches
    output_tree->Branch("detectorIDs",     &detectorIDs);
    output_tree->Branch("elementIDs",      &elementIDs);
    output_tree->Branch("tdcTimes",        &tdcTimes);
    output_tree->Branch("driftDistances",  &driftDistances);
    output_tree->Branch("hitsInTime",      &hitsInTime);

    output_tree->Branch("triggerDetectorIDs",    &triggerDetectorIDs);
    output_tree->Branch("triggerElementIDs",     &triggerElementIDs);
    output_tree->Branch("triggerTdcTimes",       &triggerTdcTimes);
    output_tree->Branch("triggerDriftDistances", &triggerDriftDistances);
    output_tree->Branch("triggerHitsInTime",     &triggerHitsInTime);

    // Optionally set basket size for each branch by name:
    // output_tree->SetBasketSize("runID", basket_size);
    // output_tree->SetBasketSize("rfIntensities", basket_size);
    // etc...
    // Or loop over all branches after they are created:
    //   TObjArray* branchList = output_tree->GetListOfBranches();
    //   for (int iBr = 0; iBr < branchList->GetEntries(); iBr++) {
    //       TBranch* br = (TBranch*) branchList->At(iBr);
    //       br->SetBasketSize(basket_size);
    //   }

    // 7. Loop over the input TTree entries and fill the output TTree
    Long64_t nEntries = input_tree->GetEntries();
    TStopwatch timer;
    timer.Start();

    for (Long64_t i = 0; i < nEntries; i++) {
        input_tree->GetEntry(i);
        // The branch data is now in your local variables/pointers
        output_tree->Fill();
    }

    // 8. Write and close everything
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
