#pragma once
#include <string>
#include <iostream>

using namespace std;

struct Data {
    int zi, luna, an;
};

enum class Tip_electrocasnic { frigider, masina_spalat, televizor, necunoscut};

enum class Status_cerere {in_asteptare, asignata, finalizata, anulata};

const double SALARIU_BAZA {4000.0};
const double BONUS_FIDELITATE {0.05};
const int ANI_FIDELITATE {3};
const double PRIMA_TRANSPORT {400.0};

Data get_data_curenta();
Data string_to_data(string dataStr);

time_t string_to_time_t(const string& dataStr);
string time_t_to_string(time_t timp);