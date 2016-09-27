#include "integrator.h"
#include "modifiers/modifier.h"
#include "../system.h"
#include <iostream>
using namespace std;
Integrator::Integrator() : m_initialized(false)
{

}

void Integrator::tick(shared_ptr<System>, real dt)
{
    if(!m_initialized) {
        cerr << "Error, integrator is not initialized." << endl;
        terminate();
    }
}

void Integrator::addModifier(std::shared_ptr<Modifier> modifier)
{
    m_modifiers.push_back(modifier);
}
