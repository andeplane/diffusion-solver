#ifndef INTEGRATOR_H
#define INTEGRATOR_H


class Integrator
{
public:
    Integrator();
    virtual void tick(double dt) = 0;
};

#endif // INTEGRATOR_H
