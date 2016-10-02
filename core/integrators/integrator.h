#ifndef INTEGRATOR_H
#define INTEGRATOR_H
#include <functional>
#include <vector>
#include "common.h"

class Integrator
{
public:
    Integrator();
    virtual void tick(std::shared_ptr<class System>, real dt) = 0;
    virtual void addModifier(std::shared_ptr<class Modifier> modifier);
    void applyModifiers(class Grid &grid);
    int numThreads() const;
    void setNumThreads(int numThreads);
protected:
    std::vector<std::shared_ptr<class Modifier>> m_modifiers;
     int m_numThreads;
    // TODO: add modifiers befor/after timestep?
};

#endif // INTEGRATOR_H
