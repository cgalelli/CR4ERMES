#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <iomanip>

RunAction::RunAction() : G4UserRunAction(), fOutputFile(nullptr) {}

RunAction::~RunAction() {
    if (fOutputFile && fOutputFile->is_open()) {
        fOutputFile->close();
        delete fOutputFile;
    }
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    G4int id = aRun->GetRunID();
    G4cout << "### Run " << id << " start." << G4endl;

    // Generate a new filename based on Run ID
    std::string fileName = "./output/muon_track_" + std::to_string(id) + ".txt";

    fOutputFile = new std::ofstream(fileName);
    
if (!fOutputFile->is_open()) {
        G4cerr << "!!! ERROR: Could not open output file: " << fileName << " !!!" << G4endl;
    } else {
        G4cout << "### Run " << id << " start. Writing to: " << fileName << G4endl;
        *fOutputFile << "LayerID\tX(mm)\tY(mm)\tZ_Plane(mm)\tKinE(MeV)" << std::endl;
    }
}

void RunAction::EndOfRunAction(const G4Run*) {
    if (fOutputFile && fOutputFile->is_open()) {
        fOutputFile->close();
    }
}

std::ofstream* RunAction::GetOutputFile() const {
    return fOutputFile;
}