#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction(RunAction* runAction)
: G4UserSteppingAction(), fRunAction(runAction)
{
    fLayerResults.resize(10);
}
SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4Track* track = aStep->GetTrack();
    if (track->GetTrackID() != 1) return;

    if (track->GetCurrentStepNumber() == 1) {
        for (int i = 0; i < 10; ++i) {
            fLayerResults[i].hit = false;
            fLayerResults[i].x = 0; fLayerResults[i].y = 0; 
            fLayerResults[i].e = 0;
            fLayerResults[i].z = 100.0 * m - ((i + 1) * 20.0 * m); // Pre-set the Z depth
        }
    }


    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
    G4double zPre = preStepPoint->GetPosition().z();
    G4double zPost = postStepPoint->GetPosition().z();


    G4VPhysicalVolume* volume = track->GetVolume();
    if (volume && volume->GetName() == "Target") {
        for (int i = 0; i < 10; ++i) {
            G4double targetZ = fLayerResults[i].z;

            if (zPre >= targetZ && zPost < targetZ) {
                G4ThreeVector posPre = preStepPoint->GetPosition();
                G4ThreeVector posPost = postStepPoint->GetPosition();
                G4double fraction = (targetZ - zPre) / (zPost - zPre);
                
                fLayerResults[i].hit = true;
                fLayerResults[i].x = posPre.x() + fraction * (posPost.x() - posPre.x());
                fLayerResults[i].y = posPre.y() + fraction * (posPost.y() - posPre.y());
                fLayerResults[i].e = preStepPoint->GetKineticEnergy() + fraction * (postStepPoint->GetKineticEnergy() - preStepPoint->GetKineticEnergy());
            }
        }
    }

    if (track->GetTrackStatus() == fStopAndKill || postStepPoint->GetStepStatus() == fWorldBoundary) {    
        std::ofstream* out = fRunAction->GetOutputFile();
        if (out && out->is_open()) {
            for (int i = 0; i < 10; ++i) {
                if (fLayerResults[i].hit) {
                    *out << i << "\t" 
                         << fLayerResults[i].x / mm << "\t" 
                         << fLayerResults[i].y / mm << "\t" 
                         << fLayerResults[i].z / mm << "\t"
                         << fLayerResults[i].e / MeV << G4endl;
                } else {
                    *out << i << "\t0\t0\t" 
                         << fLayerResults[i].z / mm << "\t0" << G4endl;
                }
            }
        }
    }
}