#ifndef GRID_H
#define GRID_H
#include "common.h"
#include <functional>
#include <string>

class Grid
{
public:
    Grid(int nx, int ny, int nz);
    int nx() const { return m_nx; }
    int ny() const { return m_ny; }
    int nz() const { return m_nz; }
    inline int index(const int i, const int j, const int k) { return i*m_ny*m_nz+j*m_nz+k; }
    inline int indexPeriodic(const int i, const int j, const int k) { return ( (i+m_nx) % m_nx)*m_ny*m_nz + ( (j+m_ny) % m_ny)*m_nz + ( (k+m_nz) % m_nz); }
    inline void indexVector(const int index, int &i, int &j, int &k) { i = index/(m_ny*m_nz); j = (index / m_nz) % m_ny; k = index % m_nz; }
#ifdef DEBUG
    inline real &operator()(const int i, const int j, const int k) { return m_grid.at(index(i,j,k)); }
    inline real &operator[](const int index) { return m_grid.at(index); }
#else
    inline real &operator()(const int i, const int j, const int k) { return m_grid[index(i,j,k)]; }
    inline real &operator[](const int index) { return m_grid[index]; }
#endif
    void iterate(std::function<void (real &, short &, int, int, int)> action);
    void writeConcentrationVTK(std::string filename);
    void writeGeometryVTK(std::string filename);
    void writeCSV(std::string filename);
    std::vector<real> &grid();
    std::vector<short> &poreSizes();
    std::vector<bool> &isWall() { return m_isWall; }
    // bool &isWall(int index) { return m_isWall[index]; }
    // bool &isWall(int i, int j, int k) { return m_isWall[ index(i,j,k) ]; }

    short &poreSize(int index) { return m_poreSizes[index]; }
    short &poreSize(int i, int j, int k) { return m_poreSizes[index(i,j,k)]; }

    void writeFugacityVTK(string filename);
    void writePoresVTK(string filename);
private:
    std::vector<real>  m_grid;
    std::vector<short> m_poreSizes;
    std::vector<bool>  m_isWall;
    int m_nx;
    int m_ny;
    int m_nz;
};

#endif // GRID_H
