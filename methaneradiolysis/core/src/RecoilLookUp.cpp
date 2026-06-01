#include "RecoilLookUp.hh"
#include "globals.hh"

void RecoilLookUp::LoadCSV(const std::string& filename)
{
    fRecoil.clear();
    fAngles.clear();
    fEnergies.clear();
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open " + filename);
    
    std::string line;

    if (std::getline(file, line)){
        std::stringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        while (std::getline(ss, token, ',')){
            fEnergies.push_back(std::log(std::stod(token)));
        }
    } 

    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        std::vector<G4double> row;
        while (std::getline(ss, token, ','))
            row.push_back(std::stod(token));
        
        fAngles.push_back(row[0] * CLHEP::deg);
        fRecoil.push_back(std::vector<G4double>(row.begin()+1, row.end()));
    }

}


std::vector<G4double> RecoilLookUp::SelectDistribution(G4double kineticEnergy) const
{
    int rightIndex;
    int leftIndex;
    std::vector<G4double> leftRecoil(fAngles.size());
    std::vector<G4double> rightRecoil(fAngles.size());
    std::vector<G4double> centerRecoil(fAngles.size());
    G4double lnEnergy = std::log(kineticEnergy / eV);

    if (lnEnergy <= fEnergies.front()){
        rightIndex = 0;
        for (size_t i=0; i < fRecoil.size(); i++){
            centerRecoil[i] = fRecoil[i][rightIndex];
        }
        return centerRecoil;
    }
    if (lnEnergy >= fEnergies.back()){
        rightIndex = fRecoil[0].size() - 1;
        for (size_t i=0; i < fRecoil.size(); i++) {
            centerRecoil[i] = fRecoil[i][rightIndex];
        }
        return centerRecoil;
    }

    for (size_t i=0; i < fEnergies.size(); i++ ){
        if (lnEnergy >= fEnergies[i] && lnEnergy < fEnergies[i+1]) {
            leftIndex = i;
            rightIndex = i+1;
        }
    }
    
    G4double weight = (lnEnergy - fEnergies[leftIndex]) / (fEnergies[rightIndex] - fEnergies[leftIndex]);

    for (size_t i=0; i < fRecoil.size(); i++){
        leftRecoil[i] = fRecoil[i][leftIndex] * (1-weight);
        rightRecoil[i] = fRecoil[i][rightIndex] * (weight);
    }
    for (size_t i=0; i < fRecoil.size(); i++) {
        centerRecoil[i] = leftRecoil[i] + rightRecoil[i];
    }
    return centerRecoil;
}

G4double RecoilLookUp::SelectRecoil(G4double kineticEnergy, G4double betaAngle) const
{

    int leftIndex;
    int rightIndex;
    std::vector<G4double> centerRecoil = SelectDistribution(kineticEnergy);

    if (betaAngle <= fAngles.front()){
        rightIndex = 0;
        return centerRecoil[rightIndex] * eV;
    }
    if (betaAngle >= fAngles.back()){
        rightIndex = fRecoil[0].size() - 1;
        return centerRecoil[rightIndex] * eV;
    }

    for (size_t i=0; i < fAngles.size(); i++ ){
        if (betaAngle >= fAngles[i] && betaAngle < fAngles[i+1]) {
            leftIndex = i;
            rightIndex = i+1;
        }
    }
    
    G4double weight = (betaAngle - fAngles[leftIndex]) / (fAngles[rightIndex] - fAngles[leftIndex]);

    return 0 * eV;
}