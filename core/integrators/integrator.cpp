#include "integrator.h"
#include "grid.h"
#include "modifiers/modifier.h"
#include "../system.h"
#include <iostream>
using namespace std;
Integrator::Integrator() : m_numThreads(1)
{

}

void Integrator::addModifier(std::shared_ptr<Modifier> modifier)
{
    m_modifiers.push_back(modifier);
}

void Integrator::applyModifiers(Grid &grid)
{
    for(auto modifierPtr : m_modifiers) {
        Modifier &modifier = *modifierPtr;
        modifier.apply(grid);
    }
}

int Integrator::numThreads() const
{
    return m_numThreads;
}

void Integrator::setNumThreads(int numThreads)
{
    m_numThreads = numThreads;
}
