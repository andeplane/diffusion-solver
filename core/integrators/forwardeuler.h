#ifndef FORWARDEULER_H
#define FORWARDEULER_H
#include <functional>
#include "integrator.h"
#include "common.h"

class ForwardEuler : public Integrator
{
public:
    ForwardEuler(std::shared_ptr<class Grid> previous, std::shared_ptr<class Grid>current, real dr);

    // Integrator interface
    virtual void tick(real dt) override;
    real dr() const;
    void setDr(real dr);

private:
    std::shared_ptr<class Grid> m_previous;
    std::shared_ptr<class Grid> m_current;
    real m_dr;
};

#endif // FORWARDEULER_H
