#pragma once
#include "Angajat.h"
#include "Tehnician.h"
#include "Receptioner.h"
#include "Supervizor.h"
#include <memory>
#include <string>


using namespace std;

class AngajatFactory {
public:
    static shared_ptr<Angajat> create_angajat(string, int, string, string, string, string, string);
};