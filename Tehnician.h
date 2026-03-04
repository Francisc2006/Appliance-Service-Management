#pragma once
#include "Angajat.h"

class Tehnician: public Angajat {
private:
    vector<string> electronice;
    double val_reparatii_luna; 
    int nr_cereri_active;

public:
    Tehnician(int, string, string , string, string, string);
    
    void adauga_competenta(const string&);
    bool verif_competenta(const string&) const;

    void final_rep(double);
    bool disponibil() const;
    void preia_cerere(int);
    
    double calc_sal() const override;
    void afisare() const override;

};