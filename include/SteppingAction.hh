#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include <vector>

struct LayerRecord {
    bool hit = false;
    double x = 0, y = 0, z = 0, e = 0;
};

class RunAction;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(RunAction* runAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    RunAction* fRunAction;
    std::vector<LayerRecord> fLayerResults;
};

#endif