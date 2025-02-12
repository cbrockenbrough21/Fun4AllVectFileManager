#ifndef CONVERT_VECT_TO_STRUCT_H
#define CONVERT_VECT_TO_STRUCT_H

#include <vector>
#include <TFile.h>  
#include <TTree.h>
#include "StructDef.h" 
#include <iostream>



// StructWrite class to manage event-level data
class StructWrite {
public:
    StructWrite() {
        std::cout << "DEBUG: StructWrite constructor called!" << std::endl;
    }


    // Event-level data members
    int RunID = 0;
    int SpillID = 0;
    int RFID = 0;
    int EventID = 0;
    int TurnID = 0;
    int trig_bits = 0;
    int Intensity[33] = {0};  // Initialize array with zero values

    // Hit data (list of hits for each event)
    EventData  evt;
    TriggerList list_trigger_hit;
    HitList list_hit;  

    // Compression settings for ROOT output
    int compression_algo = 1;
    int compression_level = 5;
    int m_basket_size = 32000;
    int m_auto_flush = 0;

    // Function to open ROOT file and create TTree
    void OpenFile(const std::string& file_name, TFile*& m_file, TTree*& m_tree);
};

#endif // STRUCT_WRITE_H

