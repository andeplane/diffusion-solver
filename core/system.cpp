#include "system.h"
#include "integrators/integrator.h"
#include "grid.h"

System::System()
{

}

std::shared_ptr<Integrator> System::integrator() const
{
    return m_integrator;
}

void System::setIntegrator(const std::shared_ptr<Integrator> &integrator)
{
    m_integrator = integrator;
}

std::shared_ptr<Grid> System::grid() const
{
    return m_grid;
}

void System::setGrid(const std::shared_ptr<Grid> &grid)
{
    m_grid = grid;
}
