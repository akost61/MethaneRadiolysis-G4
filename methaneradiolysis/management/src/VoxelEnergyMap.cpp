#include "VoxelEnergyMap.hh"
#include "G4Threading.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "G4AutoLock.hh"

G4Mutex VoxelEnergyMap::fMergeMutex = G4MUTEX_INITIALIZER;

VoxelEnergyMap* VoxelEnergyMap::GetInstance() {
    G4ThreadLocal static VoxelEnergyMap instance;
    return &instance;
}

void VoxelEnergyMap::SetDimensions(G4int nx, G4int ny, G4int nz) {
    fNx = nx; fNy = ny; fNz = nz;
}

void VoxelEnergyMap::Deposit(G4int ix, G4int iy, G4int iz,
                              G4double pe, G4double ke) {
    auto& voxel = fMap[{ix, iy, iz}];
    voxel.energyPE += pe;
    voxel.energyKE += ke;
}


void VoxelEnergyMap::RecordProduct(G4int ix, G4int iy, G4int iz, const std::string& name) {
    auto& voxel = fMap[{ix, iy, iz}];
    if      (name == "CH4+")  voxel.ch4plus++;
    else if (name == "CH3+")  voxel.ch3plus++;
    else if (name == "CH2+")  voxel.ch2plus++;
    else if (name == "CH+")   voxel.chplus++;
    else if (name == "CH3*")  voxel.ch3star++;
    else if (name == "CH2*")  voxel.ch2star++;
    else if (name == "CH*")   voxel.chstar++;
    else if (name == "H*")    voxel.hstar++;
    else if (name == "C+")    voxel.cplus++;
    else if (name == "H2+")   voxel.h2plus++;
    else if (name == "H-")    voxel.hneg++;
    else if (name == "H+")    voxel.hplus++;
    else if (name == "H2")    voxel.h2++;
}

const VoxelData* VoxelEnergyMap::GetVoxel(G4int ix, G4int iy, G4int iz) const {
    auto it = fMap.find({ix, iy, iz});
    return (it != fMap.end()) ? &it->second : nullptr;
}

void VoxelEnergyMap::Merge(const VoxelEnergyMap* other) {
    G4AutoLock lock(&fMergeMutex);
    for (const auto& [idx, data] : other->fMap) {
        auto& voxel = fMap[idx];
        voxel.energyPE += data.energyPE;
        voxel.energyKE += data.energyKE;
        voxel.ch4plus += data.ch4plus;
        voxel.ch3plus += data.ch3plus;
        voxel.ch2plus += data.ch2plus;
        voxel.chplus += data.chplus;
        voxel.ch3star += data.ch3star;
        voxel.ch2star += data.ch2star;
        voxel.chstar += data.chstar;
        voxel.hstar += data.hstar;
        voxel.cplus += data.cplus;
        voxel.h2plus += data.h2plus;
        voxel.hneg += data.hneg;
        voxel.hplus += data.hplus;
        voxel.h2 += data.h2;
    }
}

void VoxelEnergyMap::SaveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot open " + filename);

    out << "# VoxelEnergyMap  Nx=" << fNx
        << " Ny=" << fNy << " Nz=" << fNz << "\n";
    out << "# Occupied voxels: " << fMap.size() << "\n";
    out << "x_mm,y_mm,z_mm,energyPE_eV,energyKE_eV, CH4+, CH3+, CH2+, CH+, CH3*, CH2*, CH*, H*, C+, H2+, H-, H+, H2\n";
    out << std::fixed << std::setprecision(6);

    for (const auto& [idx, data] : fMap) {
        G4double x = (idx.ix * 2.0 - 1000.0) * mm;  // centre of voxel
        G4double y = (idx.iy * 2.0 - 1000.0) * mm;
        G4double z = (idx.iz * 2.0 - 1200.0) * mm;

        out << x / mm << ",  " << y / mm << ",  " << z / mm << ",  "
            << data.energyPE / eV << ",  "
            << data.energyKE / eV << ", "
            << data.ch4plus << ", "
            << data.ch3plus << ", "
            << data.ch2plus << ", "
            << data.chplus << ", "
            << data.ch3star<< ", "
            << data.ch2star << ", "
            << data.chstar << ", "
            << data.hstar << ", "
            << data.cplus << ", "
            << data.h2plus << ", "
            << data.hneg << ", "
            << data.hplus << ", "
            << data.h2 << "\n";
    }
}


