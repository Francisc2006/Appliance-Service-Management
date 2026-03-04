#include "Electrocasnic.h"


Electrocasnic::Electrocasnic(string marca, string model, int an, double pret, Tip_electrocasnic tip): marca(marca), model(model), an_fabricatie(an), pret_catalog(pret), tip(tip) {
}

string Electrocasnic::get_marca() const { 
    return marca; 
}
string Electrocasnic::get_model() const { 
    return model; 
}
double Electrocasnic::get_pret() const { 
    return pret_catalog; 
}
int Electrocasnic::get_an_fabricatie() const { 
    return an_fabricatie; 
}
Tip_electrocasnic Electrocasnic::get_tip() const { 
    return tip; 
}

int Electrocasnic::get_vechime() const {
    Data azi = get_data_curenta(); 
    int vechime = azi.an - an_fabricatie;

    return vechime;
}



