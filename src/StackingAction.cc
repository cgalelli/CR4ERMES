#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"

StackingAction::StackingAction()
  : G4UserStackingAction()
{}

StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack) {
    
    // If it is the primary particle (ParentID == 0), process it urgently
    if (aTrack->GetParentID() == 0) {
        return fUrgent;
    }

    return fKill; 
}