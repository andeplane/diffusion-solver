#include "system.h"
#include "integrators/integrator.h"
#include "grid.h"

System::System()
{

}

std::shared_ptr<Grid> System::grid() const
{
    return m_grid;
}

void System::setGrid(const std::shared_ptr<Grid> &grid)
{
    m_grid = grid;
}

void System::setLength(real lx, real ly, real lz)
{
    m_length[0] = lx;
    m_length[1] = ly;
    m_length[2] = lz;
}

real System::fluxX0() const
{
    return m_fluxX0;
}

void System::setFluxX0(const real &fluxX0)
{
    m_fluxX0 = fluxX0;
}

real System::fluxX1() const
{
    return m_fluxX1;
}

void System::setFluxX1(const real &fluxX1)
{
    m_fluxX1 = fluxX1;
}
