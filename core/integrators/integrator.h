#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <functional>
#include <vector>

class Integrator
{
public:
    Integrator();
    virtual void tick(double dt) = 0;
    virtual void addModifier(std::shared_ptr<class Modifier> modifier);

protected:
     std::vector<std::shared_ptr<class Modifier>> m_modifiers;
    // TODO: add modifiers befor/after timestep?
};

#endif // INTEGRATOR_H
