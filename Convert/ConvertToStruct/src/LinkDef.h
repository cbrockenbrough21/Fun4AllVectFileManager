#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


// Link your custom structs
#pragma link C++ struct EventData+;
#pragma link C++ struct HitData+;
#pragma link C++ struct TriggerHitData+;

// Link the typedefs for vectors
#pragma link C++ class std::vector<HitData>+;
#pragma link C++ class std::vector<TriggerHitData>+;


#endif

