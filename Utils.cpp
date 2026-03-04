#include "Utils.h"
#include <iostream>
#include <ctime>

//Functie pentrua obtine data de azi
Data get_data_curenta() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    Data d = {0, 0, 0}; 

    if (ltm != nullptr) { 
        d.an = 1900 + ltm->tm_year;
        d.luna = 1 + ltm->tm_mon;
        d.zi = ltm->tm_mday;
    } else {
        // daca localtime eșueaza
        d.an = 2025;
        d.luna = 1;
        d.zi = 1;
    }
    
    return d;
}

Data string_to_data(string dataStr) {
    Data d = {0, 0, 0};
    string temp = "";
    int puncte = 0;

    for (char c : dataStr) {
        if (c == '.') {
            if (puncte == 0) {
                d.zi = stoi(temp);
            }
            else if (puncte == 1) d.luna = stoi(temp);
            temp = "";
            puncte++;
        } 
        else {
            temp = temp + c;
        }
    }

    if (!temp.empty()) d.an = stoi(temp);
    return d;
}

time_t string_to_time_t(const string& dataStr) {
    tm t = {};
    // Presupun formatul din fisier: dd.mm.yyyy HH:MM
    // sscanf pentru a extrage numerele sigur
    int zi, luna, an, ora, minut;
    if (sscanf(dataStr.c_str(), "%d.%d.%d %d:%d", &zi, &luna, &an, &ora, &minut) == 5) {
        t.tm_mday = zi;
        t.tm_mon = luna - 1;    
        t.tm_year = an - 1900;  
        t.tm_hour = ora;
        t.tm_min = minut;
        t.tm_isdst = -1;// sistemul decida ora de vara
        return mktime(&t);// Converteste in timestamp numeric (secunde)
    }
    return 0;
}

string time_t_to_string(time_t timp) {
    char buffer[80];
    tm* t = localtime(&timp);
    // Formatam inapoi in string pentru afisare
    snprintf(buffer, sizeof(buffer), "%02d.%02d.%d %02d:%02d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    return string(buffer);
}
