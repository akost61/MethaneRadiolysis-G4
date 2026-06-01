void root_species() {

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

    std::cout << "\n=== Species Counts ===" << std::endl;
    Long64_t total = 0;
    for (const auto& s : species) {
        Long64_t n = t->GetEntries(("particleName == \"" + s + "\"").c_str());
        std::cout << s << ": " << n << std::endl;
        total += n;
    }
    std::cout << "\nSum of listed species: " << total << std::endl;

    f->Close();
}