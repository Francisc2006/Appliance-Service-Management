#pragma once
#include "Electrocasnic.h"

class Frigider : public Electrocasnic{
private:
    bool congelator;

public:
    Frigider(string, string, int, double, bool);
    
    void afisare() const override;
    string get_detalii_specifice() const override;
};