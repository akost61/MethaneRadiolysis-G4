#include "AngleDistribution.hh"

void AngularDistribution::LoadCSV(const std::string& filename)
{
    std::ifstream file(filename);
    fEnergies.clear();
    fCDF.clear();
    fAngles.clear();
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
        fCDF.push_back(std::vector<G4double>(row.begin() + 1, row.end()));
    }

}

std::vector<G4double> AngularDistribution::SelectDistribution(G4double kineticEnergy) const
{

    int rightIndex;
    int leftIndex;
    std::vector<G4double> leftCDF(fAngles.size());
    std::vector<G4double> rightCDF(fAngles.size());
    std::vector<G4double> cumDF(fAngles.size());
    G4double lnEnergy = std::log(kineticEnergy / eV);


    if (lnEnergy <= fEnergies.front()){
        rightIndex = 0;
        for (size_t i= 0; i < fCDF.size(); i++){
            cumDF[i] = fCDF[i][rightIndex];
        }
        return cumDF;
    }

    if (lnEnergy >= fEnergies.back()){
        rightIndex = fCDF[0].size() - 1;
        for (size_t i= 0; i < fCDF.size(); i++){
            cumDF[i] = fCDF[i][rightIndex];
        }
        return cumDF;
    }

    for (size_t i =0; i < fEnergies.size(); i++ ){
        if (lnEnergy >= fEnergies[i] && lnEnergy < fEnergies[i+1]){
            leftIndex = i;
            rightIndex = i+1;
        }
    }

    
    G4double weight = (lnEnergy - fEnergies[leftIndex]) / (fEnergies[rightIndex] - fEnergies[leftIndex]);



    for (size_t i=0; i < fCDF.size(); i++){
        leftCDF[i] = fCDF[i][leftIndex] * (1-weight);
        rightCDF[i] = fCDF[i][rightIndex] * weight;
    }
    for (size_t i=0; i < fEnergies.size(); i++){
        cumDF[i] = leftCDF[i] + rightCDF[i];
    }

    return cumDF;
}


G4double AngularDistribution::SampleAngle(G4double kineticEnergy) const {
    G4double randomValue = G4UniformRand();

    std::vector<G4double> cumDF = SelectDistribution(kineticEnergy);

    for (size_t i = 0; i< cumDF.size(); i++ ){
        if(randomValue <= cumDF[i]) {
            return fAngles[i];
        }
    }
    return fAngles.back();
}

G4ThreeVector AngularDistribution::RotateDirection(const G4ThreeVector& oldDirection,
                                                    G4double kineticEnergy) const
{

    G4double angle    = SampleAngle(kineticEnergy);

    G4double cosTheta = std::cos(angle);
    G4double sinTheta = std::sin(angle);            
    G4double phi      = CLHEP::twopi * G4UniformRand();

    G4ThreeVector perp1, perp2;

    if (std::abs(oldDirection.x()) < 0.9)
        perp1 = G4ThreeVector(1, 0, 0);
    else
        perp1 = G4ThreeVector(0, 1, 0);

    perp1 = (perp1 - oldDirection * (perp1.dot(oldDirection))).unit();
    perp2 = oldDirection.cross(perp1).unit();

    G4ThreeVector newDir = cosTheta             * oldDirection
                         + sinTheta * std::cos(phi) * perp1
                         + sinTheta * std::sin(phi) * perp2;


    return newDir.unit();
}