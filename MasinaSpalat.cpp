#include "MasinaSpalat.h"

MasinaSpalat::MasinaSpalat(string marca, string model, int an, double pret, double capacitate): Electrocasnic(marca, model, an, pret, Tip_electrocasnic::masina_spalat), capacitate_kg(capacitate) {
}

void MasinaSpalat::afisare() const {
    cout << "--- MASINA DE SPALAT ---" << endl;
    cout << "Marca: " << marca << endl;
    cout << "Model: " << model << endl;
    cout << "An fabricatie: " << an_fabricatie << endl;
    cout << "Pret catalog: " << pret_catalog << " RON" << endl;
    cout << "Capacitate cuva: " << capacitate_kg << " kg" << endl;
    cout << "------------------------" << endl;
}

string MasinaSpalat::get_detalii_specifice() const {
    return "Capacitate: " + to_string(capacitate_kg) + " kg";
}

