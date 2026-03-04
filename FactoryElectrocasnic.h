#pragma once
#include "Electrocasnic.h"
#include <memory>
#include <string>

// Factory Method 
class FactoryElectrocasnic {
public:
    static shared_ptr<Electrocasnic> create_electrocasnic(string, string, string, int, double, string);
};
