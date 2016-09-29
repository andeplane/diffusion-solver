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
private:
    std::shared_ptr<class Grid> m_grid;
    real m_length[3];
};

#endif // SYSTEM_H
