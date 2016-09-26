#ifndef SYSTEM_H
#define SYSTEM_H
#include <functional>

class System
{
public:
    System();
    std::shared_ptr<class Integrator> integrator() const;
    void setIntegrator(const std::shared_ptr<class Integrator> &integrator);

    std::shared_ptr<class Grid> grid() const;
    void setGrid(const std::shared_ptr<class Grid> &grid);

private:
    std::shared_ptr<class Integrator> m_integrator;
    std::shared_ptr<class Grid> m_grid;
};

#endif // SYSTEM_H
