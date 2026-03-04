#include "Televizor.h"

Televizor::Televizor(string marca, string model, int an, double pret, double diagonala): Electrocasnic(marca, model, an, pret, Tip_electrocasnic::televizor), diagonala_cm(diagonala){
}

void Televizor::afisare() const {
    cout << "--- TELEVIZOR ---" << endl;
    cout << "Marca: " << marca << endl;
    cout << "Model: " << model << endl;
    cout << "An fabricatie: " << an_fabricatie << endl;
    cout << "Pret catalog: " << pret_catalog << " RON" << endl;
    cout << "Diagonala: " << diagonala_cm << " cm" << endl;
    cout << "-----------------" << endl;
}

string Televizor::get_detalii_specifice() const {
    return "Diagonala: " + to_string(diagonala_cm) + " cm";
}

