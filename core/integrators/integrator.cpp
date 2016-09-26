#include "integrator.h"
#include "modifiers/modifier.h"

Integrator::Integrator()
{

}

void Integrator::addModifier(std::shared_ptr<Modifier> modifier)
{
    m_modifiers.push_back(modifier);
}

void Integrator::tick(double dt)
{

}
