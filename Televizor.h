#pragma once
#include "Electrocasnic.h"
#include <string>

class Televizor : public Electrocasnic {
private:
    double diagonala_cm;

public:
    Televizor(string, string, int, double, double);
    
    void afisare() const override;
    string get_detalii_specifice() const override;
};