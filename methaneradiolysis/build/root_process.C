
void root_process() {

    TFile* f = TFile::Open("output.root");
    if (!f || f->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    TTree* t = (TTree*)f->Get("electrons;1");
    if (!t) {
        std::cerr << "Tree not found!" << std::endl;
        return;
    }


    t->Draw("proc", "", "goff");

    std::cout << "\nTotal entries: " << t->GetEntries() << std::endl;

    std::vector<std::string> procs = {"EAProcess",  "Ion1Process", "Ion2Process", "Ion3Process", "Ion4Process"
                                , "Ion5Process", "Ion6Process", "Ion7Process","ND1Process" ,"ND2Process" ,"ND3Process",
                                "J3Process", "J4Process", "Nu1Process", "Nu2Process", "Nu3Process", "Nu4Process", 
                                "CI", "CIII", "CIV", "CH G-Band","H-alpha","H-beta", "H-delta", "H-gamma","Ly-alpha", "Ly-beta", "Ly-gamma"}; 
    std::cout << "\n=== Process Counts ===" << std::endl;
    for (const auto& proc : procs) {
        Long64_t n = t->GetEntries(("proc == \"" + proc + "\"").c_str());
        std::cout << proc << ": " << n << std::endl;
    }

    f->Close();
}