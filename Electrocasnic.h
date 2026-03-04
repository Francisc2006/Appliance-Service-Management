#pragma once
#include <string>
#include <iostream>
#include "Utils.h"

using namespace std;


class Electrocasnic {
protected:
    string marca;
    string model;
    int an_fabricatie;
    double pret_catalog;
    Tip_electrocasnic tip;

public:
    Electrocasnic(string, string , int, double, Tip_electrocasnic);
    virtual ~Electrocasnic() = default;

    string get_marca() const;
    string get_model() const;
    double get_pret() const;
    int get_an_fabricatie() const;
    Tip_electrocasnic get_tip() const;
    int get_vechime() const;

    virtual void afisare() const = 0;
    virtual string get_detalii_specifice() const = 0; 

};