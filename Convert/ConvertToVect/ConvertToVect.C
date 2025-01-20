#include <string>
#include <iostream>
#include <TStopwatch.h>
R__LOAD_LIBRARY(libfun4all_vect_file_manager)
R__LOAD_LIBRARY(libcalibrator)
R__LOAD_LIBRARY(libktracker)

std::uintmax_t getFileSize(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); // Returns the file size
}

int ConvertToVect (){
	const char* fn_udst= "/project/ptgroup/E1039_data/dst/run_002281/run_002281_spill_000000000_spin.root";
	Fun4AllServer* se = Fun4AllServer::instance();
	Fun4AllInputManager *in = new Fun4AllDstInputManager("DUMMY");
	se->registerInputManager(in);
	in->fileopen(fn_udst);
	Fun4AllVectEventOutputManager* tree = new Fun4AllVectEventOutputManager();
	tree->Verbosity(100);
	tree->SetTreeName("tree");
	tree->SetFileName("Vector-In.root");
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

    // Get the file size
    std::uintmax_t file_size = getFileSize("Vector-In.root");

    // Output the results
    std::cout << "WRITE_TIME=" << real_time << " FILE_SIZE=" 
              << std::fixed << std::setprecision(3)
              << static_cast<double>(file_size) / (1024 * 1024) << "MB" << std::endl;

    return 0;
}