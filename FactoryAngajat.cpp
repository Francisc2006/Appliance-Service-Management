#include "FactoryAngajat.h"
#include "Tehnician.h"
#include "Receptioner.h"
#include "Supervizor.h"

shared_ptr<Angajat> AngajatFactory::create_angajat(string tip, int id, string nume, string prenume, string cnp, string dataAng, string oras) {
    if (tip == "tehnician") {
        return make_shared<Tehnician>(id, nume, prenume, cnp, dataAng, oras);
    }
    else if (tip == "receptioner") {
        return make_shared<Receptioner>(id, nume, prenume, cnp, dataAng, oras);
    }
    else if (tip == "supervizor") {
        return make_shared<Supervizor>(id, nume, prenume, cnp, dataAng, oras);
    }
    return nullptr; 
}



