#include "StructRewrite.h"
#include "StructDef.h"
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TStopwatch.h>
#include <fstream>

// For file size
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

    // Parse command line
    std::string input_file_path  = argv[1];
    std::string output_file_path = argv[2];
    int compression_algo  = std::atoi(argv[3]);
    int compression_level = std::atoi(argv[4]);
    int basket_size       = std::atoi(argv[5]);
    Long64_t autoflush    = std::atoll(argv[6]);

    // Open input
    TFile *input_file = TFile::Open(input_file_path.c_str());
    if (!input_file || input_file->IsZombie()) {
        std::cerr << "Error: Could not open input file " << input_file_path << std::endl;
        return 1;
    }
    TTree *input_tree = (TTree*) input_file->Get("tree");
    if (!input_tree) {
        std::cerr << "Error: Could not find tree in input file " << input_file_path << std::endl;
        input_file->Close();
        return 1;
    }

    // Pointers to read from the input tree
    EventData *evt_ptr = nullptr;
    std::vector<HitData> *list_hit_ptr = nullptr;
    std::vector<TriggerHitData> *list_trigger_hit_ptr = nullptr;

    input_tree->SetBranchAddress("evt", &evt_ptr);
    input_tree->SetBranchAddress("list_hit", &list_hit_ptr);
    input_tree->SetBranchAddress("list_trigger_hit", &list_trigger_hit_ptr);

    // Create the output
    TFile *output_file = new TFile(output_file_path.c_str(), "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Could not create output file " << output_file_path << std::endl;
        input_file->Close();
        return 1;
    }
    TTree *output_tree = new TTree("tree", "Tree for storing events");
    output_file->SetCompressionAlgorithm(compression_algo);
    output_file->SetCompressionLevel(compression_level);
    // output_tree->SetAutoFlush(autoflush);
    // output_tree->SetBasketSize("*", basket_size);

    // THIS is the StructRewrite object (Option B)
    StructRewrite writer;

    // Create branches in the output TTree using writer's data members
    output_tree->Branch("evt", &writer.evt);
    output_tree->Branch("list_hit", &writer.list_hit);
    output_tree->Branch("list_trigger_hit", &writer.list_trigger_hit);

    // Loop over input entries
    Long64_t nentries = input_tree->GetEntries();
    TStopwatch timer;
    timer.Start();

    for (Long64_t i = 0; i < nentries; i++) {
        input_tree->GetEntry(i);

        // Copy data from pointers -> writer's data
        writer.evt = *evt_ptr;
        writer.list_hit = *list_hit_ptr;
        writer.list_trigger_hit = *list_trigger_hit_ptr;

        // Fill the output tree
        output_tree->Fill();
    }

    // Write/close
    output_file->Write("", TObject::kOverwrite);
    output_file->Close();
    input_file->Close();

    timer.Stop();
    double real_time = timer.RealTime();
    std::uintmax_t file_size = getFileSize(output_file_path);
    std::cout << "WRITE_TIME=" << real_time 
              << " FILE_SIZE=" << std::fixed << std::setprecision(3)
              << (static_cast<double>(file_size) / (1024 * 1024)) << "MB" 
              << std::endl;

    return 0;
}
