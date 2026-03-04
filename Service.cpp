#include "Service.h"
#include "FactoryAngajat.h"
#include "FactoryElectrocasnic.h"
#include "Frigider.h"
#include "Televizor.h"
#include "MasinaSpalat.h"
#include "Receptioner.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <thread> 
#include <chrono> 

// Functie pentru a curata spatiile si \r de la final
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) {
        return ""; // Returnam string gol daca e doar spatiu
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool este_in_catalog(const shared_ptr<Electrocasnic>& aparat_cerere, const vector<shared_ptr<Electrocasnic>>& catalog) {
    for (const auto& item_catalog : catalog) {
        if (item_catalog->get_tip() == aparat_cerere->get_tip() && item_catalog->get_marca() == aparat_cerere->get_marca() &&
            item_catalog->get_model() == aparat_cerere->get_model()) {
            return true;
        }
    }
    return false;
}

Service& Service::getInstance() {
    static Service instance; 
    return instance;
}

int Service::get_incarcare_tehnician(int id_tehnician) {
    int total = 0;
    for (auto it = cereri_active.begin(); it != cereri_active.end(); it++) { 
        if ((*it)->get_id_tehnician() == id_tehnician) {
            total += (*it)->get_durata();
        }
    }
    return total;
}


void Service::proceseaza_linie_angajat(stringstream& ss, int nr_linie) {
    // Format: ANGAJAT,tip,nume,prenume,cnp,data,oras,[competente...]
    string tip, nume, prenume, cnp, data, oras;
    getline(ss, tip, ',');
    getline(ss, nume, ',');
    getline(ss, prenume, ',');
    getline(ss, cnp, ',');
    getline(ss, data, ',');
    getline(ss, oras, ',');

    tip = trim(tip);
    nume = trim(nume);

    // id=0 este un placeholder, constructorul Angajat foloseste static int
    shared_ptr<Angajat> ang = AngajatFactory::create_angajat(tip, 0, nume, prenume, cnp, data, oras);
    
    if (ang) {
        angajati.push_back(ang);
        if (tip == "tehnician") {
            auto tech = dynamic_pointer_cast<Tehnician>(ang);
            
            if (tech) { 
                string comp;
                while (getline(ss, comp, ',')) {
                    comp = trim(comp); 
                    if (!comp.empty()) {
                        tech->adauga_competenta(comp);
                    }
                }
            }

        }
    } else {
        cout << "Eroare (Linia " << nr_linie << "): Tip angajat invalid." << endl;
    }
}

void Service::proceseaza_linie_model(stringstream& ss, int nr_linie) {
    // Format: MODEL,Tip,Marca,Model,An,Pret,Extra
    string tip, marca, model, s_an, s_pret, extra;

    getline(ss, tip, ',');
    getline(ss, marca, ',');
    getline(ss, model, ',');
    getline(ss, s_an, ',');
    getline(ss, s_pret, ',');
    getline(ss, extra, ',');

    marca = trim(marca);
    model = trim(model);
    tip = trim(tip);
    s_an = trim(s_an);
    s_pret = trim(s_pret);
    extra = trim(extra);

    int an = stoi(s_an);
    double pret = stod(s_pret);
    shared_ptr<Electrocasnic> obiect_nou = FactoryElectrocasnic::create_electrocasnic(tip, marca, model, an, pret, extra);

    catalog_modele.push_back(obiect_nou);

}

void Service::proceseaza_linie_cerere(stringstream& ss, int nr_linie, int& id_counter) {
    // Format: CERERE,Tip,Marca,Model,An,Pret,Extra,Complexitate,Timestamp
    string tip, marca, model, s_an, s_pret, extra, s_compl, timestamp;
    
    getline(ss, tip, ',');
    getline(ss, marca, ',');
    getline(ss, model, ',');
    getline(ss, s_an, ',');
    getline(ss, s_pret, ',');
    getline(ss, extra, ',');
    getline(ss, s_compl, ',');
    getline(ss, timestamp, ',');

    //curatare stringuri
    marca = trim(marca);
    model = trim(model);
    tip = trim(tip);
    s_an = trim(s_an);
    s_pret = trim(s_pret);
    extra = trim(extra);
    s_compl = trim(s_compl);
    timestamp = trim(timestamp);

    string denumire_aparat = marca + " " + model;

    //Verificare nivel complexitate
    if (s_compl == "0") {
        statistica_nereparate[denumire_aparat]++;
        return;
    }

    if (s_compl != "1" && s_compl != "2" && s_compl != "3" && s_compl != "4" && s_compl != "5") {
        cout << "Eroare (Linia " << nr_linie << "): Cerere respinsa. Nivel de complexitate invalid." << endl;
        statistica_nereparate[denumire_aparat]++;
        return;
    }

    try {
        int an = stoi(s_an);
        double pret = stod(s_pret);
        int complexitate = stoi(s_compl);

        auto aparat = FactoryElectrocasnic::create_electrocasnic(tip, marca, model, an, pret, extra);

        if (aparat) {
            if (!este_in_catalog(aparat, catalog_modele)) {
                cout << "Eroare (Linia " << nr_linie << "): Cerere respinsa. Modelul " << denumire_aparat << " nu este in catalogul nostru." << endl;
                statistica_nereparate[denumire_aparat]++;
                return; 
            }
            
            auto cerere = make_shared<Cerere>(id_counter++, move(aparat), timestamp, complexitate);
            
            coada_asteptare.push_back(cerere); 
            istoric_cereri.push_back(cerere);

            //distribuire egala a cererilor catre receptioneri
            vector<shared_ptr<Receptioner>> lista_receptioneri;
            for (size_t i = 0; i < angajati.size(); i++) {
                auto recep = dynamic_pointer_cast<Receptioner>(angajati[i]);
                if (recep) {
                    lista_receptioneri.push_back(recep);
                }
            }

            if (!lista_receptioneri.empty()) {
                size_t index = cerere->get_id() % lista_receptioneri.size();
                lista_receptioneri[index]->adauga_cereri(cerere->get_id());
            } 
            else {
                cout << "EROARE LOGICA (Linia " << nr_linie << "): Nu exista receptioneri incarcati ANTERIOR acestei cereri! Cererea ramane neasignata." << endl;
            }
        
        } 
        else {
            cout << "Eroare (Linia " << nr_linie << "): Tip electrocasnic necunoscut." << endl;
        }
    }
    catch (...) {
        cout << "Eroare CRITICA (Linia " << nr_linie << "): Date numerice invalide (an, pret, complexitate) sau format data gresit." << endl;
    }
}

//Inacrcare scenarii
void Service::incarca_scenariu(const string& fis) {
    ifstream f(fis);
    if (!f.is_open()) {
        cout << "Eroare: Nu pot deschide fisierul " << fis << endl;
        return;
    }

    string linie;
    int nr_linie = 0;
    int id_cerere_next = 1; 

    cout << ">>> Se incarca scenariul din " << fis << "..." << endl;

    while (getline(f, linie)) {
        nr_linie++;
        if (linie.empty()) continue;
        stringstream ss(linie);
        string tag;
        getline(ss, tag, ','); 
        try {
            if (tag == "ANGAJAT") {
                proceseaza_linie_angajat(ss, nr_linie);
            }
            else if (tag == "MODEL") {
                proceseaza_linie_model(ss, nr_linie);
            }
            else if (tag == "CERERE") {
                proceseaza_linie_cerere(ss, nr_linie, id_cerere_next);
            }
        }
        catch (const string& e) {
            cout << "Eroare logica (Linia " << nr_linie << "): " << e << endl;
        }
    }
    f.close();
    cout << ">>> Incarcare finalizata.\n" << endl;
}

//Procesare cereri
void Service::ruleaza_simulare(int unitati_timp) {
    // Verificare personal minim
    int t = 0, r = 0, s = 0;
    for (const auto& a : angajati) {
        if (dynamic_pointer_cast<Tehnician>(a)) t++;
        if (dynamic_pointer_cast<Receptioner>(a)) r++;
        if (dynamic_pointer_cast<Supervizor>(a)) s++;
    }

    if (t < 3 || r < 1 || s < 1) {
        cout << "Simularea nu poate incepe! Lipsesc angajati (Minim: 3 Tehnicieni, 1 Receptioner, 1 Supervizor)." << endl;
        return;
    }

    cout << "=== START SIMULARE (" << unitati_timp << " ticuri) ===" << endl;

    for (int timp = 1; timp <= unitati_timp; timp++) {
        cout << "\n[TIC " << timp << "]" << endl;

        //Procesare cereri active (scade timp, finalizeaza)
        auto it = cereri_active.begin();
        while (it != cereri_active.end()) {
            auto& c = *it;
            c->scade_timp_ramas();

            cout << "   Tehnician " << c->get_id_tehnician() << " lucreaza la cererea " << c->get_id() << " (Ramas: " << c->get_durata() << ")" << endl;

            if (c->get_status() == Status_cerere::finalizata) {
                cout << "   -> Cererea " << c->get_id() << " FINALIZATA!" << endl;
                
                // tehnicianul este anuntat ca a terminat
                for (size_t i = 0; i < angajati.size(); i++) {
                    if (angajati[i]->get_id() == c->get_id_tehnician()) {
                        auto tech = dynamic_pointer_cast<Tehnician>(angajati[i]);
                        if (tech) {
                            tech->final_rep(c->get_pret());
                        }
                    }
                }

                it = cereri_active.erase(it); 
            } else {
                ++it;
            }
        }

        //Alocare cereri din asteptare
        auto it_coada = coada_asteptare.begin();
        while (it_coada != coada_asteptare.end()) {
            auto cerere = *it_coada;

            //Verifica daca modelul nu a fost sters din lista
            if (!este_in_catalog(cerere->get_electrocasnic(), catalog_modele)) {
                string nume_aparat = cerere->get_electrocasnic()->get_marca() + " " + cerere->get_electrocasnic()->get_model();
                cout << " ANULAT: Cererea " << cerere->get_id() << " (" << nume_aparat << ") respinsa. Modelul a fost scos din catalog." << endl;
                statistica_nereparate[nume_aparat]++;
                it_coada = coada_asteptare.erase(it_coada);
                continue; 
            }


            string marca = cerere->get_electrocasnic()->get_marca();
            Tip_electrocasnic tip_enum = cerere->get_electrocasnic()->get_tip();
            string tip_str {""};

            switch(tip_enum) {
                case Tip_electrocasnic::frigider: tip_str = "Frigider"; break;
                case Tip_electrocasnic::masina_spalat: tip_str = "MasinaSpalat"; break;
                case Tip_electrocasnic::televizor: tip_str = "Televizor"; break;
                default: tip_str = "Necunoscut"; break;
            }

            string competenta_necesara = tip_str + " " + marca;

            // Cautam cel mai bun tehnician
            shared_ptr<Tehnician> best_tech = nullptr;
            int min_load = 999999;

            for (size_t i = 0; i < angajati.size(); i++) {
                auto tech = dynamic_pointer_cast<Tehnician>(angajati[i]);
                if (tech && tech->disponibil() && tech->verif_competenta(competenta_necesara)) {
                    int load = get_incarcare_tehnician(tech->get_id());
                    if (load < min_load) {
                        min_load = load;
                        best_tech = tech;
                    }
                }
            }

            if (best_tech) {
                best_tech->preia_cerere(cerere->get_id());
                cerere->asigneaza_tehnician(best_tech->get_id());
                cereri_active.push_back(cerere);
                
                it_coada = coada_asteptare.erase(it_coada);
            } else {
                ++it_coada;
            }
        }

        if (!coada_asteptare.empty()) {
            cout << "   Cereri in asteptare: ";
            for (size_t i = 0; i < coada_asteptare.size(); i++) {
                cout << coada_asteptare[i]->get_id() << " ";
            }
            cout << endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));//asteapta o secunda
    }
}

//Rapoarte
void Service::raport_top_salarii() {
    if (angajati.empty()) {
        cout << "Nu exista angajati." << endl;
        return;
    }

    //sortare descrescatoare dupa salaiu
    vector<shared_ptr<Angajat>> top_angajati = angajati;
    sort(top_angajati.begin(), top_angajati.end(), [](shared_ptr<Angajat> a, shared_ptr<Angajat> b) {
        return a->calc_sal() > b->calc_sal();
    });

    //luam pe primii 3
    if (top_angajati.size() > 3) {
        top_angajati.resize(3);
    }

    //sortare dupa nume, prenume
    sort(top_angajati.begin(), top_angajati.end(), [](shared_ptr<Angajat> a, shared_ptr<Angajat> b) {
        if (a->get_nume() != b->get_nume()) 
            return a->get_nume() < b->get_nume();
        return a->get_prenume() < b->get_prenume();
    });

    ofstream f("raport_top3_salarii.csv");
    f << "Nume, Prenume, Functie, Salariu\n";

    for (int i = 0; i < top_angajati.size() && i < 3; i++) {
        f << top_angajati[i]->get_nume() << "," << top_angajati[i]->get_prenume() << "," << top_angajati[i]->calc_sal() << "\n";
    }
    f.close();
    cout << "Raport generat: raport_top3_salarii.csv" << endl;

}

void Service::raport_tehnician_top_durata() {
    int max_durata = -1;
    int id_tehnician_target = -1;

    for (size_t i = 0; i < istoric_cereri.size(); i++) {
        if (istoric_cereri[i]->get_id_tehnician() != -1) {
            int vechime = istoric_cereri[i]->get_electrocasnic()->get_vechime();
            if (vechime == 0) 
                vechime = 1;
            
            int durata_initiala = vechime * istoric_cereri[i]->get_complexitate();

            if (durata_initiala > max_durata) {
                max_durata = durata_initiala;
                id_tehnician_target = istoric_cereri[i]->get_id_tehnician();
            }
        }
    }

    if (id_tehnician_target != -1) {
        for (size_t i = 0; i < angajati.size(); i++) {
            if (angajati[i]->get_id() == id_tehnician_target) {
                
                ofstream f("raport_tehnician_top.csv");
                f << "ID, Nume, Prenume, CNP, domiciliu, salariu, listaReparatie_Max_Ore\n";
                f << angajati[i]->get_id() << ", " << angajati[i]->get_nume() << ", " << angajati[i]->get_prenume() << ", " 
                    << angajati[i]->get_CNP() << ", " << angajati[i]->get_oras() << ", " << angajati[i]->calc_sal() << ", " << max_durata << "\n";
                f.close();
                
                cout << ">> Raport generat: 'raport_tehnician_top.csv'" << endl;
                return;
            }
        }
    } 
    else {
        cout << "Nu exista reparatii asignate in istoric." << endl;
    }
}


void Service::raport_cereri_asteptare() {
    if (coada_asteptare.empty()) {
        cout << "Nu sunt cereri in asteptare pentru a genera raportul." << endl;
        return;
    }

    // copiere deque in vector pentru sortare
    vector<shared_ptr<Cerere>> lista_sortata;
    for (size_t i = 0; i < coada_asteptare.size(); i++) {
        lista_sortata.push_back(coada_asteptare[i]);
    }

    // sortare dupa Tip, Marca, Model
    sort(lista_sortata.begin(), lista_sortata.end(), [](shared_ptr<Cerere> a, shared_ptr<Cerere> b) {
        auto get_tip_str = [](Tip_electrocasnic t) {
            if (t == Tip_electrocasnic::frigider) return string("Frigider");
            if (t == Tip_electrocasnic::masina_spalat) return string("MasinaSpalat");
            if (t == Tip_electrocasnic::televizor) return string("Televizor");
            return string("Necunoscut");
        };

        string t1 = get_tip_str(a->get_electrocasnic()->get_tip());
        string t2 = get_tip_str(b->get_electrocasnic()->get_tip());

        if (t1 != t2) 
            return t1 < t2; // Sortare Tip
        
        string m1 = a->get_electrocasnic()->get_marca();
        string m2 = b->get_electrocasnic()->get_marca();
        if (m1 != m2)
            return m1 < m2; // Sortare Marca

        return a->get_electrocasnic()->get_model() < b->get_electrocasnic()->get_model(); // Sortare Model
    });

    ofstream f("raport_asteptare_grupate.csv");
    f << "Tip,Marca,Model,ID_Cerere\n";

    for (size_t i = 0; i < lista_sortata.size(); i++) {
        string tip_str;
        Tip_electrocasnic t = lista_sortata[i]->get_electrocasnic()->get_tip();
        
        if (t == Tip_electrocasnic::frigider) 
            tip_str = "Frigider";
        else if (t == Tip_electrocasnic::masina_spalat) 
            tip_str = "MasinaSpalat";
        else tip_str = "Televizor";

        f << tip_str << "," << lista_sortata[i]->get_electrocasnic()->get_marca() << "," << lista_sortata[i]->get_electrocasnic()->get_model() << "," 
          << lista_sortata[i]->get_id() << "\n";
    }
    f.close();
    cout << ">> Raport generat: 'raport_asteptare_grupate.csv'" << endl;
}

//Gestionare angajati
void Service::adauga_angajat() {
    string tip, nume, prenume, cnp, data, oras;
    cout << "\n---Angajat Nou---" << endl;
    cout << "Functie (ex: tehnician, receptioner, supervizor): ";
    cin >> tip;
    cout << "Nume: ";
    cin >> nume;
    cout <<"Prenume: ";
    cin >> prenume;
    cout<<"CNP: ";
    cin>> cnp;
    cout << "Data angajare (zz.ll.aaaa): ";
    cin >> data;
    cout << "Oras: ";
    cin >> oras;

    try {
        auto ang = AngajatFactory::create_angajat(tip, 0, nume, prenume, cnp, data, oras);
        
        if (ang) {
            if (tip == "tehnician") {
                auto tech = dynamic_pointer_cast<Tehnician>(ang);
                cout << "Introduceti competente (ex: Frigider Samsung). Scrieti 'STOP' pentru a termina:" << endl;
                string comp;
                cin.ignore(); // curatare buffer
                while (true) {
                    cout << "> ";
                    getline(cin, comp);
                    if (comp == "STOP") break;
                    if (!comp.empty()) tech->adauga_competenta(comp);
                }
            }
            angajati.push_back(ang);
            cout << "Angajat adaugat cu succes!" << endl;
        } else {
            cout << "Tip angajat invalid!" << endl;
        }
    } catch (const string& e) {
        cout << "Eroare la creare: " << e << endl;
    }
}

void Service::sterge_angajat() {
    cout << "\n--- STERGERE ANGAJAT ---" << endl;

    int id_cautat;
    cout << "Introduceti ID-ul angajatului dorit: ";
    cin >> id_cautat;

    bool gasit = false;
    auto it = angajati.begin();
    while (it != angajati.end()) {
        if ((*it)->get_id() == id_cautat) {
            cout << "Angajatul " << (*it)->get_nume() << " a fost concediat." << endl;
            it = angajati.erase(it); 
            gasit = true;
            break;
        } else {
            ++it;
        }
    }
    
    if (!gasit) {
        cout << "Eroare: Nu exista niciun angajat cu ID-ul " << id_cautat << "." << endl;
    }
}
void Service::modifica_angajat() {
    cout << "\n--- MODIFICARE DATE ANGAJAT ---" << endl;

    int id_cautat;
    cout << "Introduceti ID-ul angajatului de modificat: ";
    cin >> id_cautat;

    bool gasit = false;
    auto it = angajati.begin();
    while (it != angajati.end()) {
        if ((*it)->get_id() == id_cautat) {
            string nume, prenume;
            cout << "Nume nou: "; 
            cin >> nume;
            (*it)->set_nume(nume);
            cout << "Prenume nou: ";
            cin >> prenume;
            (*it)->set_prenume(prenume);

            gasit = true;
            break;
        } else {
            ++it;
        }
    }

    if (!gasit) {
        cout << "Eroare: Nu exista niciun angajat cu ID-ul " << id_cautat << "." << endl;
    }

}

void Service::cauta_angajat_cnp(){
    cout << "\n--- CAUTARE ANGAJAT DUPA CNP ---" << endl;

    string cnp_cautat;
    cout << "Introduceti CNP-ul angajatului: ";
    cin >> cnp_cautat;
    cnp_cautat = trim(cnp_cautat);

    bool gasit = false;
    auto it = angajati.begin();
    while (it != angajati.end()) {
        if ((*it)->get_CNP() == cnp_cautat) {
            (*it)->afisare();
            gasit = true;
            break;
        } else {
            ++it;
        }
    }

    if (!gasit) {
        cout << "Eroare: Nu exista niciun angajat cu CNP-ul " << cnp_cautat << "." << endl;
    }

}

void Service::afiseaza_angajati() const {
    cout << "\n--- LISTA ANGAJATI ---" << endl;
    for (const auto& a : angajati) {
        a->afisare();
        cout << endl;
    }
}

//Gestionare electrocanice
void Service::adauga_model_suportat() {
    cout << "\n--- ADAUGARE MODEL NOU IN CATALOG ---" << endl;
    
    string tip, marca, model, s_an, s_pret, extra;
    
    
    while (true) {
        cout << "Tip electrocasnic (Frigider/MasinaSpalat/Televizor): ";
        cin >> tip;
        if (tip == "Frigider" || tip == "MasinaSpalat" || tip == "Televizor") break;
        cout << "Eroare: Tip invalid! Folositi exact denumirile din paranteza." << endl;
    }
    
    cout << "Marca: "; 
    cin >> marca;
    cout << "Model: "; 
    cin >> model;
    cout << "An de fabricatie: ";
    cin >> s_an;
    cout << "Pret de catalog: ";
    cin >> s_pret;
    if(tip == "Frigider") {
        cout << "Are congelator? (Da/Nu): ";
        cin >> extra;
    }
    else if (tip == "MasinaSpalat") {
        cout << "Capacitate (in kg): ";
        cin >> extra;
    }
    else if (tip == "Televizor") {
        cout << "Diagonala (in cm): ";
        cin >> extra;
    }

    int an = stoi(s_an);
    double pret = stod(s_pret);
    shared_ptr<Electrocasnic> aparat_nou = FactoryElectrocasnic::create_electrocasnic(tip, marca, model, an, pret, extra);

    string cheie = tip + " " + marca + " " + model;

    if (aparat_nou) {
        if (este_in_catalog(aparat_nou, catalog_modele)) {
            cout << "Atentie: Modelul " << marca << " " << model << " exista deja in catalog!" << endl;
        } else {
            catalog_modele.push_back(aparat_nou);
            cout << "Succes! Modelul a fost adaugat in lista de reparatii suportate." << endl;
        }
    } else {
        cout << "Eroare: Nu s-a putut crea modelul. Verificati datele." << endl;
    }
}

void Service::sterge_model_suportat() {
    cout << "\n--- STERGERE MODEL DIN CATALOG ---" << endl;
    
    if (catalog_modele.empty()) {
        cout << "Lista este goala. Nu aveti ce sterge." << endl;
        return;
    }

   //Afisare lista cu modele
    for (size_t i = 0; i < catalog_modele.size(); i++) {
        string tip_str;
        switch (catalog_modele[i]->get_tip()) {
            case Tip_electrocasnic::frigider: tip_str = "Frigider"; break;
            case Tip_electrocasnic::masina_spalat: tip_str = "MasinaSpalat"; break;
            case Tip_electrocasnic::televizor: tip_str = "Televizor"; break;
            default: tip_str = "Necunoscut";
        }
        cout << (i + 1) << ". " << tip_str << " " << catalog_modele[i]->get_marca() << " " << catalog_modele[i]->get_model() << endl;
    }

    // Citire index
    int optiune;
    cout << "Alegeti numarul modelului de sters: ";
    cin >> optiune;

    if (optiune > 0 && optiune <= catalog_modele.size()) {
        int index_real = optiune - 1;
        cout << "Modelul '" << catalog_modele[index_real]->get_marca() << " " << catalog_modele[index_real]->get_model() << "' a fost sters din catalog." << endl;
        catalog_modele.erase(catalog_modele.begin() + index_real);
    } else {
        cout << "Optiune invalida!" << endl;
    }
}


void Service::afiseaza_aparate_reparate() const {
    cout << "\n--- APARATE REPARATE ---" << endl;
    for (size_t i = 0; i < istoric_cereri.size(); i++) {
        if (istoric_cereri[i]->get_status() == Status_cerere::finalizata) {
            istoric_cereri[i]->get_electrocasnic()->afisare();
            cout << "Reparat de Tehnician ID: " << istoric_cereri[i]->get_id_tehnician() << endl;
            cout << endl;
        }
    }
}

void Service::afiseaza_aparate_nereparate() const {
    cout << "\n--- APARATE NEREPARATE ---" << endl;
    
    if (statistica_nereparate.empty()) {
        cout << "Nu exista inregistrari despre aparate respinse." << endl;
        return;
    }

    // mutare date din map in vector pentru sortare
    vector<pair<string, int>> lista_sortata;
    for (auto it = statistica_nereparate.begin(); it != statistica_nereparate.end(); ++it) {
        lista_sortata.push_back(*it);
    }

    // sortare descrescatoare a vectorului
    sort(lista_sortata.begin(), lista_sortata.end(), [](const pair<string,int>& a, const pair<string,int>& b) {
            return a.second > b.second; 
        }
    );
    
    for (size_t i = 0; i < lista_sortata.size(); i++) {
        cout << lista_sortata[i].first << " -> " << lista_sortata[i].second << " cereri respinse" << endl;
    }
    cout << "-----------------------------" << endl;
}

