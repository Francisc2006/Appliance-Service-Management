#include "Frigider.h"

Frigider::Frigider(string marca, string model, int an, double pret, bool are_congelator): Electrocasnic(marca, model, an, pret, Tip_electrocasnic::frigider), congelator(are_congelator) {
}

void Frigider::afisare() const {
    cout << "--- FRIGIDER ---" << endl;
    cout << "Marca: " << marca << endl;
    cout << "Model: " << model << endl;
    cout << "An fabricatie: " << an_fabricatie << endl;
    cout << "Pret catalog: " << pret_catalog << " RON" << endl;
    cout << "Congelator: " << (congelator ? "Da" : "Nu") << endl;
    cout << "----------------" << endl;
}

string Frigider::get_detalii_specifice() const {
    if (congelator == true)
        return "Cu congelator";
    return "Fara congelator";
}



