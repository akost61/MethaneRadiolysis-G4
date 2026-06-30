#ifndef VOXEL_ENERGY_MAP_HH
#define VOXEL_ENERGY_MAP_HH


#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "G4Threading.hh"
#include <unordered_map>
#include <string>

struct VoxelData {
    G4double energyPE = 0.0;
    G4double energyKE = 0.0;
    G4int ch4plus = 0;
    G4int ch3plus = 0;
    G4int ch2plus = 0;
    G4int chplus = 0;
    G4int ch3star = 0;
    G4int ch2star = 0;
    G4int chstar = 0;
    G4int hstar = 0;
    G4int cplus = 0;
    G4int h2plus = 0;
    G4int hneg = 0;
    G4int hplus = 0;
    G4int h2 = 0;

};

struct VoxelIndex {
    G4int ix,iy,iz;

    bool operator ==(const VoxelIndex& o) const {
        return ix == o.ix && iy == o.iy && iz == o.iz;
    }

};

struct VoxelHash {
    std::size_t operator()(const VoxelIndex& k) const {
        return  (std::size_t)k.ix
             | ((std::size_t)k.iy << 10)
             | ((std::size_t)k.iz << 20);
    }
};


class VoxelEnergyMap {
public:
    static VoxelEnergyMap* GetInstance();

    void SetDimensions(G4int nx, G4int ny, G4int nz);
    void Deposit(G4int ix, G4int iy, G4int iz, G4double pe, G4double ke);
    void RecordProduct(G4int ix, G4int iy, G4int iz, const std::string& name);

    const VoxelData* GetVoxel(G4int ix, G4int iy, G4int iz) const;
    std::size_t OccupiedVoxels() const { return fMap.size(); }

    void Merge(const VoxelEnergyMap* other);
    void SaveToFile(const std::string& filename) const;

private:
    VoxelEnergyMap() = default;

    G4int fNx = 0, fNy = 0, fNz = 0;

    std::unordered_map<VoxelIndex, VoxelData, VoxelHash> fMap;

    static G4Mutex fMergeMutex;
};


#endif