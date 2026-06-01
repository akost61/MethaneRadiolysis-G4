#ifndef CROSSSECTIONTABLE_HH
#define CROSSSECTIONTABLE_HH

#include <vector>
#include <string>


class CrossSectionTable
{
public:
    CrossSectionTable();
    ~CrossSectionTable() = default;


    void LoadFromCSV(const std::string& filename);

    double GetElastic  (double E_eV) const;
    double GetIon1     (double E_eV) const;
    double GetIon2     (double E_eV) const;
    double GetIon3     (double E_eV) const;
    double GetIon4     (double E_eV) const;
    double GetIon5     (double E_eV) const;
    double GetIon6     (double E_eV) const;
    double GetIon7     (double E_eV) const;
    double GetND1      (double E_eV) const;
    double GetND2      (double E_eV) const;
    double GetND3      (double E_eV) const;
    double GetEA       (double E_eV) const;
    double GetNu1      (double E_eV) const;
    double GetNu2      (double E_eV) const;
    double GetNu3      (double E_eV) const;
    double GetNu4      (double E_eV) const;
    double GetJ3      (double E_eV) const;
    double GetJ4      (double E_eV) const;
    double GetLya       (double E_eV) const;
    double GetLyb       (double E_eV) const;
    double GetLyg       (double E_eV) const;
    double GetHa       (double E_eV) const;
    double GetHb       (double E_eV) const;
    double GetHg       (double E_eV) const;
    double GetHd       (double E_eV) const;
    double GetCHG       (double E_eV) const;
    double GetC3       (double E_eV) const;
    double GetC4       (double E_eV) const;
    double GetC1       (double E_eV) const;

    double GetTotal    (double E_eV) const;


    int SampleProcess(double E_eV) const;

    bool IsLoaded()  const { return !fEnergy.empty(); }
    int  NumPoints() const { return static_cast<int>(fEnergy.size()); }

private:
    double Interpolate(const std::vector<double>& xs, double E_eV) const;

    std::vector<double> fEnergy;   // eV
    std::vector<double> fElastic;  // mm^2
    std::vector<double> fIon1;     // mm^2
    std::vector<double> fIon2;     // mm^2
    std::vector<double> fIon3;     // mm^2
    std::vector<double> fIon4;     // mm^2
    std::vector<double> fIon5;     // mm^2
    std::vector<double> fIon6;     // mm^2
    std::vector<double> fIon7;     // mm^2
    std::vector<double> fND1;      // mm^2
    std::vector<double> fND2;      // mm^2
    std::vector<double> fND3;      // mm^2
    std::vector<double> fEA;       // mm^2
    std::vector<double> fNu1;      // mm^2
    std::vector<double> fNu2;      // mm^2
    std::vector<double> fNu3;      // mm^2
    std::vector<double> fNu4;      // mm^2
    std::vector<double> fJ3;      // mm^2
    std::vector<double> fJ4;      // mm^2

    std::vector<double> fLya;      // mm^2
    std::vector<double> fLyb;       // mm^2
    std::vector<double> fLyg;      // mm^2
    std::vector<double> fHa;      // mm^2
    std::vector<double> fHb;      // mm^2
    std::vector<double> fHg;      // mm^2
    std::vector<double> fHd;      // mm^2
    std::vector<double> fCHG;      // mm^2
    std::vector<double> fC3;      // mm^2
    std::vector<double> fC4;      // mm^2
    std::vector<double> fC1;      // mm^2

    std::vector<double> fTotal;    // mm^2
};

#endif // CROSSSECTIONTABLE_HH
