#pragma once
#include "Electrocasnic.h"
#include <iostream>

class MasinaSpalat : public Electrocasnic {
private:
    double capacitate_kg;

public:
    MasinaSpalat(string, string, int, double, double);
    
    void afisare() const override;
    string get_detalii_specifice() const override;
};