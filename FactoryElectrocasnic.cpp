#include "FactoryElectrocasnic.h"
#include "Frigider.h"
#include "MasinaSpalat.h"
#include "Televizor.h"

shared_ptr<Electrocasnic> FactoryElectrocasnic::create_electrocasnic(
    string tip, string marca, string model, int an, double pret, string atribut_extra) {
    
    if (tip == "Frigider") {
        bool are_congelator = (atribut_extra == "Da" || atribut_extra == "1");
        return make_unique<Frigider>(marca, model, an, pret, are_congelator);
    }
    else if (tip == "MasinaSpalat") {
        double capacitate = stod(atribut_extra);
        return make_unique<MasinaSpalat>(marca, model, an, pret, capacitate);
    }
    else if (tip == "Televizor") {
        double diagonala = stod(atribut_extra);
        return make_unique<Televizor>(marca, model, an, pret, diagonala);
    }
    
    return nullptr; 
}
