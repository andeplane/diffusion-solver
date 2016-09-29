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
