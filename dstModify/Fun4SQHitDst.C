/// Fun4SQHitDst.C: Fun4All macro to extract SQHitVector from DST files.
R__LOAD_LIBRARY(libcalibrator)
R__LOAD_LIBRARY(libktracker)
int Fun4SQHitDst(const char* fn_dst="input.dst.root",
                 const char* fn_udst="SQHit_only.root")
{
    Fun4AllServer* se = Fun4AllServer::instance();
    Fun4AllInputManager *in = new Fun4AllDstInputManager("SimDst");
    se->registerInputManager(in);

    // Register Output Manager
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", fn_udst);
    se->registerOutputManager(out);
    out->AddNode("SQHitVector");  // Only keep SQHitVector

    // **Exclude SQRun node from the output DST**
    out->RemoveNode("SQRun");

    cerr << "Opening DST file: " << fn_dst << endl;
    in->fileopen(fn_dst);
    
    se->run();

    cerr << "Ending run..." << endl;
    se->End();

    cerr << "Reopening file to clean up backup cycles..." << endl;

    // ✅ Clean up the output file to remove backup cycles and rename the tree
    TFile *f_in = TFile::Open(fn_udst, "READ");
    TTree *tree_latest = (TTree*)f_in->Get("T");

    TFile *f_out = new TFile("SQHit_clean.root", "RECREATE");
    TTree *new_tree = tree_latest->CloneTree();
    new_tree->SetName("SQHitVector");
    new_tree->Write();

    f_out->Delete("T;*");

    f_out->Close();
    f_in->Close();

    // ✅ Replace the original file with the cleaned-up version
    std::rename("SQHit_clean.root", fn_udst);

    cerr << "Backup cycles removed. Final file saved as: " << fn_udst << endl;

    cerr << "Deleting Fun4AllServer..." << endl;
    delete se;
  
    cerr << "Process completed!" << endl;
    return 0;
}


// TFile *f_in = TFile::Open("SQHit_only.root", "READ");
// TTree *tree_latest = (TTree*)f_in->Get("T");

// TFile *f_out = new TFile("SQHit_clean.root", "RECREATE");

// // Clone and rename the tree
// TTree *new_tree = tree_latest->CloneTree();
// new_tree->SetName("SQHitVector");
// new_tree->Write();

// // Explicitly delete the original tree "T" before closing the file
// f_out->Delete("T;*");

// f_out->Close();
// f_in->Close();

