/// Fun4SQHitDst.C: Fun4All macro to extract SQHitVector from DST files.
R__LOAD_LIBRARY(libcalibrator)
R__LOAD_LIBRARY(libktracker)

#include <sys/stat.h>

bool file_exists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

int Fun4SQHitDst(const char* fn_dst="input.dst.root",
                 const char* fn_udst="SQHit_only.root",
                 const int n_events=1000)  // Limit events
{
  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(1);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("SimDst");

  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", fn_udst);
  se->registerOutputManager(out);
  out->AddNode("SQHitVector");

  in->fileopen(fn_dst);
  
  se->run(n_events);

  se->End();

  delete se;
  
  return 0;
}