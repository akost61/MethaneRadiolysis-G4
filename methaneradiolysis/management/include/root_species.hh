#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <numbers>
#include "TFile.h"
#include "TTree.h"

void root_species(int N_sims, float height, float radius) {
    float r = radius;
    float h = height;
    float volume_cm3 = (std::numbers::pi) * r* r * h;
    float electrons_second = 1.602e-19 / 200; // charge (electron = 1.602e-19 coulombs) divided by current (200 micron Amps)
    float rate = 0;
    float A = 6.022e23;


    TFile* f = TFile::Open("output.root");
    if (!f || f->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    TTree* t = (TTree*)f->Get("particles;1");
    if (!t) {
        std::cerr << "Tree not found!" << std::endl;
        return;
    }

    std::cout << "Total entries: " << t->GetEntries() << std::endl;

    std::vector<std::string> species = { "CH4+", "CH3+", "CH2+", "CH+", "C+", "H*", "H2", "H2+", "H+", "CH3*", "CH2*","CH*", "H-" };

    std::cout << "\n=========== Species Counts and Moles per Second per cm^3===========" << std::endl;
    std::cout <<"\nVolume = "<< volume_cm3 <<"cm^3"<<std::endl;
    std::cout << "electrons per second = " << electrons_second << std::endl;
    std::cout << "Number of Simulations= " << N_sims << std::endl;
    std::cout << std::endl;
    Long64_t total = 0;
    for (const auto& s : species) {
        Long64_t n = t->GetEntries(("particleName == \"" + s + "\"").c_str());
        std::cout << s << ": " << n << std::endl;
        rate = (n/N_sims) * electrons_second;
        std::cout << "Moles per second per cm3 of " << s << " :" <<  rate / (A * volume_cm3) << std::endl;
        total += n;
    }
    std::cout << "\nSum of total species: " << total << std::endl;

    f->Close();
}