#ifndef FORWARDEULER_H
#define FORWARDEULER_H
#include <functional>
#include "integrator.h"
class ForwardEuler : public Integrator
{
public:
    ForwardEuler(std::shared_ptr<class Grid> previous, std::shared_ptr<class Grid>current);

    // Integrator interface
    virtual void tick(double dt) override;

private:
    std::shared_ptr<class Grid> m_previous;
    std::shared_ptr<class Grid> m_current;
};

#endif // FORWARDEULER_H
