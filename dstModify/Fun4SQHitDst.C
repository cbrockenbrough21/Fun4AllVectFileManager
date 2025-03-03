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

    cerr << "Deleting Fun4AllServer..." << endl;
    delete se;
  
    cerr << "Process completed!" << endl;
    return 0;
}
