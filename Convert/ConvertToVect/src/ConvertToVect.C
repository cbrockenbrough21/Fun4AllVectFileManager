#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>

// Fun4All-specific headers
#include "Fun4AllServer.h"
#include "Fun4AllDstInputManager.h"
#include "Fun4AllVectEventOutputManager.h"
#include "Fun4AllVectEventInputManager.h"


std::uintmax_t getFileSize(const std::string &filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); // Returns the file size
}

int ConvertToVect(const std::string& input_file, const std::string& output_file) {
    Fun4AllServer* se = Fun4AllServer::instance();
    Fun4AllInputManager* in = new Fun4AllDstInputManager("DUMMY");
    se->registerInputManager(in);
    in->fileopen(input_file.c_str());

    Fun4AllVectEventOutputManager* tree = new Fun4AllVectEventOutputManager();
    tree->SetTreeName("tree");
    tree->SetFileName(output_file.c_str());
    se->registerOutputManager(tree);

    // TStopwatch timer;
    // timer.Start();
    se->run(0);
    se->End();
    // timer.Stop();

    // double real_time = timer.RealTime();
    // std::uintmax_t file_size = getFileSize(output_file);

    // std::cout << "WRITE_TIME=" << real_time << " FILE_SIZE="
    //           << std::fixed << std::setprecision(3)
    //           << static_cast<double>(file_size) / (1024 * 1024) << "MB" << std::endl;
    return 0;
}