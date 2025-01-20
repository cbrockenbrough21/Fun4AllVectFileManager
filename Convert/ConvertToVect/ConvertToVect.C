#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <TStopwatch.h>
R__LOAD_LIBRARY(libfun4all_vect_file_manager)
R__LOAD_LIBRARY(libcalibrator)
R__LOAD_LIBRARY(libktracker)

// Function to calculate file size
std::uintmax_t getFileSize(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); // Returns the file size
}

// Main function for conversion
void ConvertToVect() {
    // Hardcoded input file path
    const std::string input_file = "Convert/ConvertToVect/run_002282_spill_000000000_spin.root";

    // Derive the base name and output file name
    std::string base_name = input_file.substr(0, input_file.find_last_of("."));
    std::string output_file = base_name + "_vector.root";

    // Output the input and output file paths
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;

    // Set up Fun4AllServer and input/output managers
    Fun4AllServer* se = Fun4AllServer::instance();
    Fun4AllInputManager* in = new Fun4AllDstInputManager("DUMMY");
    se->registerInputManager(in);
    in->fileopen(input_file.c_str());

    Fun4AllVectEventOutputManager* tree = new Fun4AllVectEventOutputManager();
    tree->Verbosity(100);
    tree->SetTreeName("tree");
    tree->SetFileName(output_file.c_str());
    se->registerOutputManager(tree);

    // Start timer
    TStopwatch timer;
    timer.Start();

    // Run the process
    se->run(0);
    se->End();

    // Stop the timer
    timer.Stop();
    double real_time = timer.RealTime();

    // Get the file size of the output file
    std::uintmax_t file_size = getFileSize(output_file);

    // Output the results
    std::cout << "WRITE_TIME=" << real_time << " FILE_SIZE="
              << std::fixed << std::setprecision(3)
              << static_cast<double>(file_size) / (1024 * 1024) << " MB" << std::endl;
}
