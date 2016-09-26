#ifndef FORWARDEULER_H
#define FORWARDEULER_H

#include "integrator.h"

class ForwardEuler : public Integrator
{
public:
    ForwardEuler(class Grid *previous, class Grid *current);

    // Integrator interface
    virtual void tick(double dt) override;

private:
    class Grid *m_previous;
    class Grid *m_current;
};

#endif // FORWARDEULER_H
