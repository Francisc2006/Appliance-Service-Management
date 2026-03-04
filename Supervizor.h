#pragma once
#include "Angajat.h"

class Supervizor: public Angajat {
public:
    Supervizor(int, string, string , string, string, string);

    double calc_sal() const override; 
    void afisare() const override;
};



