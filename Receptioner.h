#pragma once
#include "Angajat.h"
#include <vector>

class Receptioner: public Angajat{
private:
    vector<int> id_cereri_inreg;

public:
    Receptioner(int, string, string, string, string, string );

    void adauga_cereri(int);

    void afisare() const override;
    double calc_sal() const override;

};

