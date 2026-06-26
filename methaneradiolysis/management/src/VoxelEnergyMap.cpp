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
    }
}

void VoxelEnergyMap::SaveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Cannot open " + filename);

    out << "# VoxelEnergyMap  Nx=" << fNx
        << " Ny=" << fNy << " Nz=" << fNz << "\n";
    out << "# Occupied voxels: " << fMap.size() << "\n";
    out << "x_mm,y_mm,z_mm,energyPE_eV,energyKE_eV\n";
    out << std::fixed << std::setprecision(6);

    for (const auto& [idx, data] : fMap) {
        G4double x = (idx.ix * 2.0 - 1000.0) * mm;  // centre of voxel
        G4double y = (idx.iy * 2.0 - 1000.0) * mm;
        G4double z = (idx.iz * 2.0 - 1200.0) * mm;

        out << x / mm << ",  " << y / mm << ",  " << z / mm << ",  "
            << data.energyPE / eV << ",  "
            << data.energyKE / eV << "\n";
    }
}