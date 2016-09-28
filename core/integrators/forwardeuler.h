#ifndef FORWARDEULER_H
#define FORWARDEULER_H
#include <functional>
#include "integrator.h"
#include "common.h"

class ForwardEuler : public Integrator
{
public:
    ForwardEuler();

    // Integrator interface
    virtual void tick(std::shared_ptr<class System> systemPtr, real dt) override;
private:
    std::shared_ptr<class Grid> m_grid;
};

#endif // FORWARDEULER_H
