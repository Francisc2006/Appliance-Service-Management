#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Utils.h"

using namespace std;


class Angajat {
protected:

    const int id;
    string nume;
    string prenume;
    string cnp;
    Data data_angajare;
    string oras_dom;

    static int next_ID; //pentru a atribui ID unic

    int calc_vechime() const;

public:
    Angajat(int, string, string , string, string, string);
    virtual ~Angajat() = default;

    int get_id() const;
    string get_nume() const;
    string get_prenume() const;
    string get_CNP() const;
    string get_oras() const;

    void set_nume(const string&);
    void set_prenume(const string&);

    virtual double calc_sal() const;
    virtual void afisare() const = 0;
};

