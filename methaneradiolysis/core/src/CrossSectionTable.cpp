#include "CrossSectionTable.hh"

#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>

CrossSectionTable::CrossSectionTable() {}

void CrossSectionTable::LoadFromCSV(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("CrossSectionTable: cannot open " + filename);

    // ---- Parse header ----
    std::string headerLine;
    if (!std::getline(file, headerLine))
        throw std::runtime_error("CrossSectionTable: empty file " + filename);
    if (!headerLine.empty() && headerLine.back() == '\r')
        headerLine.pop_back();

    auto splitCSV = [](const std::string& line) {
        std::vector<std::string> tokens;
        std::istringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            tok.erase(0, tok.find_first_not_of(" \t"));
            auto last = tok.find_last_not_of(" \t\r");
            if (last != std::string::npos) tok = tok.substr(0, last + 1);
            tokens.push_back(tok);
        }
        return tokens;
    };

    auto headers = splitCSV(headerLine);

    int colEnergy  = -1, colElastic = -1, colIon1 = -1, colIon2 = -1, colIon3 = -1, colIon4 = -1, colIon5 = -1, colIon6 = -1, colIon7 = -1;
    int colND1     = -1, colND2 = -1, colND3=-1, colEA      = -1;
    int colNu1  = -1, colNu2=-1, colNu3  = -1, colNu4=-1, colJ3  = -1, colJ4=-1;
    int colLya = -1, colLyb=-1, colLyg=-1, colHa=-1, colHb=-1, colHg=-1, colHd=-1, colCHG=-1, colC3=-1, colC1=-1, colC4=-1;
    int colTotal   = -1;

    for (int i = 0; i < (int)headers.size(); ++i) {
        if (headers[i] == "Energy")  colEnergy  = i;
        if (headers[i] == "Elastic") colElastic = i;
        if (headers[i] == "Ion_1")   colIon1    = i;
        if (headers[i] == "Ion_2")   colIon2    = i;
        if (headers[i] == "Ion_3")   colIon3    = i;
        if (headers[i] == "Ion_4")   colIon4    = i;
        if (headers[i] == "Ion_5")   colIon5    = i;
        if (headers[i] == "Ion_6")   colIon6    = i;
        if (headers[i] == "Ion_7")   colIon7    = i;
        if (headers[i] == "ND_1")    colND1     = i;
        if (headers[i] == "ND_2")    colND2     = i;
        if (headers[i] == "ND_3")    colND3     = i;
        if (headers[i] == "EA")      colEA      = i;
        if (headers[i] == "Nu_1")    colNu1     = i;
        if (headers[i] == "Nu_2")    colNu2     = i;
        if (headers[i] == "Nu_3")    colNu3     = i;
        if (headers[i] == "Nu_4")    colNu4     = i;        
        if (headers[i] == "J3")    colJ3     = i;
        if (headers[i] == "J4")    colJ4     = i;
        if (headers[i] == "Total")   colTotal   = i;
        if (headers[i] == "Lya")   colLya   = i;
        if (headers[i] == "Lyb")   colLyb   = i;
        if (headers[i] == "Lyg")   colLyg   = i;
        if (headers[i] == "Ha")   colHa   = i;
        if (headers[i] == "Hb")   colHb  = i;
        if (headers[i] == "Hg")   colHg   = i;
        if (headers[i] == "Hd")   colHd   = i;
        if (headers[i] == "CHG")   colCHG   = i;
        if (headers[i] == "CIII")   colC3   = i;
        if (headers[i] == "CIV")   colC4   = i;
        if (headers[i] == "CI")   colC1   = i;

    }

    auto check = [&](int col, const std::string& name) {
        if (col < 0) throw std::runtime_error(
            "CrossSectionTable: missing column '" + name + "'");
    };
    check(colEnergy,  "Energy");
    check(colElastic, "Elastic");
    check(colIon1,    "Ion_1");
    check(colIon2,    "Ion_2");
    check(colIon3,    "Ion_3");
    check(colIon4,    "Ion_4");
    check(colIon5,    "Ion_5");
    check(colIon6,    "Ion_6");
    check(colIon7,    "Ion_7");

    check(colND1,     "ND_1");
    check(colND2,     "ND_2");
    check(colND3,     "ND_3");
    check(colEA,      "EA");

    check(colNu1,     "Nu_1");
    check(colNu2,     "Nu_2");
    check(colNu3,     "Nu_3");
    check(colNu4,     "Nu_4");
    check(colJ3,     "J3");
    check(colJ4,     "J4");

    check(colLya,     "Lya");
    check(colLyb,     "Lyb");
    check(colLyg,     "Lyg");
    check(colHa,     "Ha");
    check(colHb,     "Hb");
    check(colHg,     "Hg");
    check(colHd,     "Hd");
    check(colCHG,     "CHG");
    check(colC3,     "CIII");
    check(colC4,     "CIV");
    check(colC1,     "CI");


    check(colTotal,   "Total");

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        auto vals = splitCSV(line);
        int maxCol = std::max({colTotal});
        if ((int)vals.size() <= maxCol) continue;

        try {
            fEnergy .push_back(std::stod(vals[colEnergy]));
            fElastic.push_back(std::stod(vals[colElastic]) * mm2);
            fIon1   .push_back(std::stod(vals[colIon1])    * mm2);
            fIon2   .push_back(std::stod(vals[colIon2])    * mm2);
            fIon3   .push_back(std::stod(vals[colIon3])    * mm2);
            fIon4   .push_back(std::stod(vals[colIon4])    * mm2);
            fIon5   .push_back(std::stod(vals[colIon5])    * mm2);
            fIon6   .push_back(std::stod(vals[colIon6])    * mm2);
            fIon7   .push_back(std::stod(vals[colIon7])    * mm2);
            fND1    .push_back(std::stod(vals[colND1])     * mm2);
            fND2    .push_back(std::stod(vals[colND2])     * mm2);
            fND3    .push_back(std::stod(vals[colND3])     * mm2);
            fEA     .push_back(std::stod(vals[colEA])      * mm2);
            fNu1    .push_back(std::stod(vals[colNu1])     * mm2);
            fNu2    .push_back(std::stod(vals[colNu2])     * mm2);
            fNu3    .push_back(std::stod(vals[colNu3])     * mm2);
            fNu4    .push_back(std::stod(vals[colNu4])     * mm2);
            fJ3    .push_back(std::stod(vals[colJ3])     * mm2);
            fJ4    .push_back(std::stod(vals[colJ4])     * mm2);
            fLya    .push_back(std::stod(vals[colLya])     * mm2);
            fLyb    .push_back(std::stod(vals[colLyb])     * mm2);
            fLyg    .push_back(std::stod(vals[colLyg])     * mm2);
            fHa    .push_back(std::stod(vals[colHa])     * mm2);
            fHb    .push_back(std::stod(vals[colHb])     * mm2);
            fHg    .push_back(std::stod(vals[colHg])     * mm2);
            fHd    .push_back(std::stod(vals[colHd])     * mm2);
            fCHG   .push_back(std::stod(vals[colCHG])     * mm2);
            fC3     .push_back(std::stod(vals[colC3])      * mm2);
            fC4     .push_back(std::stod(vals[colC4])      * mm2);
            fC1     .push_back(std::stod(vals[colC1])      * mm2);
            fTotal  .push_back(std::stod(vals[colTotal])   * mm2);
        } catch (...) { continue; }
    }

    if (fEnergy.empty())
        throw std::runtime_error("CrossSectionTable: no data loaded from " + filename);

    std::cout << "[CrossSectionTable] Loaded " << fEnergy.size()
              << " points from " << filename
              << "  E = [" << fEnergy.front() << ", " << fEnergy.back() << "] eV"
              << std::endl;

}

double CrossSectionTable::Interpolate(const std::vector<double>& xs,
                                      double E_eV) const
{
    if (fEnergy.empty()) return 0.0;
    if (E_eV <= fEnergy.front()) return xs.front();
    if (E_eV >= fEnergy.back())  return xs.back();

    auto it = std::lower_bound(fEnergy.begin(), fEnergy.end(), E_eV);
    size_t i = std::distance(fEnergy.begin(), it) - 1;

    double E0 = fEnergy[i], E1 = fEnergy[i + 1];
    double x0 = xs[i],      x1 = xs[i + 1];

    if (x0 <= 0.0 || x1 <= 0.0) {
        double t = (E_eV - E0) / (E1 - E0);
        return x0 + t * (x1 - x0);
    }

    double t = (std::log(E_eV) - std::log(E0)) / (std::log(E1) - std::log(E0));
    return std::exp(std::log(x0) + t * (std::log(x1) - std::log(x0)));
}

double CrossSectionTable::GetElastic (double E_eV) const { return Interpolate(fElastic, E_eV); }
double CrossSectionTable::GetIon1    (double E_eV) const { return Interpolate(fIon1,    E_eV); }
double CrossSectionTable::GetIon2    (double E_eV) const { return Interpolate(fIon2,    E_eV); }
double CrossSectionTable::GetIon3    (double E_eV) const { return Interpolate(fIon3,    E_eV); }
double CrossSectionTable::GetIon4    (double E_eV) const { return Interpolate(fIon4,    E_eV); }
double CrossSectionTable::GetIon5    (double E_eV) const { return Interpolate(fIon5,    E_eV); }
double CrossSectionTable::GetIon6    (double E_eV) const { return Interpolate(fIon6,    E_eV); }
double CrossSectionTable::GetIon7    (double E_eV) const { return Interpolate(fIon7,    E_eV); }

double CrossSectionTable::GetND1     (double E_eV) const { return Interpolate(fND1,     E_eV); }
double CrossSectionTable::GetND2     (double E_eV) const { return Interpolate(fND2,     E_eV); }
double CrossSectionTable::GetND3     (double E_eV) const { return Interpolate(fND3,     E_eV); }

double CrossSectionTable::GetEA      (double E_eV) const { return Interpolate(fEA,      E_eV); }
double CrossSectionTable::GetNu1     (double E_eV) const { return Interpolate(fNu1,     E_eV); }
double CrossSectionTable::GetNu2     (double E_eV) const { return Interpolate(fNu2,     E_eV); }
double CrossSectionTable::GetNu3     (double E_eV) const { return Interpolate(fNu3,     E_eV); }
double CrossSectionTable::GetNu4     (double E_eV) const { return Interpolate(fNu4,     E_eV); }
double CrossSectionTable::GetJ3     (double E_eV) const { return Interpolate(fJ3,     E_eV); }
double CrossSectionTable::GetJ4     (double E_eV) const { return Interpolate(fJ4,     E_eV); }

double CrossSectionTable::GetLya      (double E_eV) const { return Interpolate(fLya,      E_eV); }
double CrossSectionTable::GetLyb     (double E_eV) const { return Interpolate(fLyb,     E_eV); }
double CrossSectionTable::GetLyg     (double E_eV) const { return Interpolate(fLyg,     E_eV); }
double CrossSectionTable::GetHa     (double E_eV) const { return Interpolate(fHa,     E_eV); }
double CrossSectionTable::GetHb     (double E_eV) const { return Interpolate(fHb,     E_eV); }
double CrossSectionTable::GetHg    (double E_eV) const { return Interpolate(fHg,     E_eV); }
double CrossSectionTable::GetHd     (double E_eV) const { return Interpolate(fHd,     E_eV); }
double CrossSectionTable::GetCHG     (double E_eV) const { return Interpolate(fCHG,     E_eV); }
double CrossSectionTable::GetC3     (double E_eV) const { return Interpolate(fC3,     E_eV); }
double CrossSectionTable::GetC4    (double E_eV) const { return Interpolate(fC4,     E_eV); }
double CrossSectionTable::GetC1     (double E_eV) const { return Interpolate(fC1,     E_eV); }

double CrossSectionTable::GetTotal   (double E_eV) const { return Interpolate(fTotal,   E_eV); }


