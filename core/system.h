#ifndef SYSTEM_H
#define SYSTEM_H
#include <functional>
#include "common.h"

class System
{
public:
    System();

    std::shared_ptr<class Grid> grid() const;
    void setGrid(const std::shared_ptr<class Grid> &grid);
    void setLength(real lx, real ly, real lz);
    real length(int index) { return m_length[index]; }
    real lx() { return m_length[0]; }
    real ly() { return m_length[1]; }
    real lz() { return m_length[2]; }

    real fluxX0() const;
    void setFluxX0(const real &fluxX0);

    real fluxX1() const;
    void setFluxX1(const real &fluxX1);

private:
    std::shared_ptr<class Grid> m_grid;
    real m_length[3];
    real m_fluxX0;
    real m_fluxX1;
};

#endif // SYSTEM_H
