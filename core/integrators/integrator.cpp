#include "integrator.h"
#include "grid.h"
#include "modifiers/modifier.h"
#include "../system.h"
#include <iostream>
using namespace std;
Integrator::Integrator()
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
