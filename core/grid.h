#ifndef GRID_H
#define GRID_H
#include "common.h"
#include "cell.h"
#include <functional>
#include <string>

class Grid
{
public:
    Grid(int nx, int ny, int nz, int numProperties);
    std::vector<Cell> &cells();
    int nx() const { return m_nx; }
    int ny() const { return m_ny; }
    int nz() const { return m_nz; }
    int index(const int i, const int j, const int k) { return i*m_ny*m_nz+j*m_nz+k; }
    int indexPeriodic(const int i, const int j, const int k) { return ( (i+m_nx) % m_nx)*m_ny*m_nz + ( (j+m_ny) % m_ny)*m_nz + ( (k+m_nz) % m_nz); }
    int indexVector(const int index, int &i, int &j, int &k) { i = index/(m_ny*m_nz); j = (index / m_nz) % m_ny; k = index % m_nz; }
    void iterate(std::function<void(Cell &cell)> action);
    void iterate(std::function<void(Cell &cell, int i, int j, int k)> action);
    void writeVTK(std::string filename, int propertyIndex);
    Cell &operator()(const int i, const int j, const int k) { return m_cells[index(i,j,k)]; }
    Cell &operator[](const int index) { return m_cells[index]; }
private:
    std::vector<Cell> m_cells;
    int m_nx;
    int m_ny;
    int m_nz;
};

#endif // GRID_H
