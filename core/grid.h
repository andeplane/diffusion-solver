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
    int nx() const { return m_nx; }
    int ny() const { return m_ny; }
    int nz() const { return m_nz; }
    inline int index(const int i, const int j, const int k) { return i*m_ny*m_nz+j*m_nz+k; }
    inline int indexPeriodic(const int i, const int j, const int k) { return ( (i+m_nx) % m_nx)*m_ny*m_nz + ( (j+m_ny) % m_ny)*m_nz + ( (k+m_nz) % m_nz); }
    inline int indexVector(const int index, int &i, int &j, int &k) { i = index/(m_ny*m_nz); j = (index / m_nz) % m_ny; k = index % m_nz; }
#ifdef DEBUG
    inline real &operator()(const int i, const int j, const int k) { return m_grid.at(index(i,j,k)); }
    inline real &operator[](const int index) { return return m_grid.at(index(i,j,k); }
#else
    inline real &operator()(const int i, const int j, const int k) { return m_grid[index(i,j,k)]; }
    inline real &operator[](const int index) { return m_grid[index]; }
#endif
    void iterate(std::function<void (real &, short &, int, int, int)> action);
    void writeVTK(std::string filename);
    void writeCSV(std::string filename);
    std::vector<real> &grid();
    std::vector<short> &poreSizes();
    short poreSize(int index) { return m_poreSizes[index]; }

private:
    std::vector<real>  m_grid;
    std::vector<short> m_poreSizes;
    int m_nx;
    int m_ny;
    int m_nz;
    int m_numProperties;
};

#endif // GRID_H
