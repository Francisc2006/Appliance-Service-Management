#include <iostream>
#include <string>
#include "Service.h"

using namespace std;

void meniu_gestionare_angajati() {
    Service& app = Service::getInstance();
    char opt {' '};

    while (opt != '0') { 
        cout << "\n========================================" << endl;
        cout << "      GESTIONARE ANGAJATI" << endl;
        cout << "========================================" << endl;
        cout << "1. Afiseaza lista completa angajati" << endl;
        cout << "2. Adauga angajat nou" << endl;
        cout << "3. Modifica angajat (Nume/Prenume)" << endl;
        cout << "4. Sterge angajat" << endl;
        cout << "5. Cauta angajat dupa CNP" << endl;
        cout << "0. Inapoi la meniul principal" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Optiune: ";
        cin >> opt;

        switch (opt) {
            case '1': app.afiseaza_angajati(); break;
            case '2': app.adauga_angajat(); break;
            case '3': app.modifica_angajat(); break;
            case '4': app.sterge_angajat(); break;
            case '5': app.cauta_angajat_cnp(); break;
            case '0': break; 
            default: cout << "Optiune invalida!" << endl;
        }
    }
}

void meniu_gestionare_electrocasnice() {
    Service& app = Service::getInstance();
    char opt {' '};

    while (opt != '0') {
        cout << "\n========================================" << endl;
        cout << "    GESTIONARE ELECTROCASNICE" << endl;
        cout << "========================================" << endl;
        cout << "1. Adauga model nou in catalog" << endl;
        cout << "2. Sterge model din catalog" << endl;
        cout << "3. Afiseaza modele reparate" << endl;
        cout << "4. Afiseaza aparate nereparate (din cereri)" << endl;
        cout << "0. Inapoi la meniul principal" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Optiune: ";
        cin >> opt;

        switch (opt) {
            case '1': app.adauga_model_suportat(); break;
            case '2': app.sterge_model_suportat(); break;
            case '3': app.afiseaza_aparate_reparate(); break;
            case '4': app.afiseaza_aparate_nereparate(); break;
            case '0': break;
            default: cout << "Optiune invalida!" << endl;
        }
    }
}

void meniu_procesare_cereri() {
    Service& app = Service::getInstance();
    char opt = ' ';

    while (opt != '0') {
        cout << "\n========================================" << endl;
        cout << "      PROCESARE CERERI & SIMULARE" << endl;
        cout << "========================================" << endl;
        cout << "1. Ruleaza Simulare" << endl;
        cout << "0. Inapoi la meniul principal" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Optiune: ";
        cin >> opt;

        switch (opt) {
            case '1': {
                int t;
                cout << "Numar ticuri (secunde): ";
                cin >> t;
                app.ruleaza_simulare(t);
                break;
            }
            case '0': break;
            default: cout << "Optiune invalida!" << endl;
        }
    }
}

void meniu_rapoarte() {
    Service& app = Service::getInstance();
    char opt = ' ';

    while (opt != '0') {
        cout << "\n========================================" << endl;
        cout << "          GENERARE RAPOARTE" << endl;
        cout << "========================================" << endl;
        cout << "1. Top 3 Angajati (Salariu)" << endl;
        cout << "2. Tehnicianul cu reparatia maxima" << endl;
        cout << "3. Cereri in asteptare" << endl;
        cout << "0. Inapoi la meniul principal" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Optiune: ";
        cin >> opt;

        switch (opt) {
            case '1': app.raport_top_salarii(); break;
            case '2': app.raport_tehnician_top_durata(); break;
            case '3': app.raport_cereri_asteptare(); break;
            case '0': break;
            default: cout << "Optiune invalida!" << endl;
        }
    }
}

int main() {
    Service& app = Service::getInstance();
    char optiune {' '}; 

    do {
        cout << "\n########################################" << endl;
        cout << "###           FIX IT NOW             ###" << endl;
        cout << "########################################" << endl;
        cout << "1. Incarcare Scenariu (Fisier)" << endl;
        cout << "2. Gestionare Angajati" << endl;
        cout << "3. Gestionare Electrocasnice" << endl;
        cout << "4. Procesare Cereri (Simulare)" << endl;
        cout << "5. Raportari" << endl;
        cout << "0. Oprire" << endl;
        cout << "########################################" << endl;
        cout << "Selectati optiunea: ";
        
        cin >> optiune; 

        switch (optiune) {
        case '1': { 
            string path;
            cout << "Fisier (ex: tests/test2.txt): ";
            cin >> path;
            app.incarca_scenariu(path);
            break;
        }
        case '2': meniu_gestionare_angajati(); break;
        case '3': meniu_gestionare_electrocasnice(); break;
        case '4': meniu_procesare_cereri(); break;
        case '5': meniu_rapoarte(); break;
        case '0': 
            cout << "La revedere!" << endl; 
            break;
        default:
            cout << "Optiune invalida!" << endl;
        }
    } while (optiune != '0');

    return 0;
}