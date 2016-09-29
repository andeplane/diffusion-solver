#include "grid.h"
#include <iostream>
#include <fstream>
using namespace std;

Grid::Grid(int nx, int ny, int nz) :
    m_grid(nx*ny*nz, 0), m_poreSizes(nx*ny*nz, 0),
    m_nx(nx), m_ny(ny), m_nz(nz)
{

}

void Grid::iterate(function<void (real &gridPoint, short &poreSize, int i, int j, int k)> action)
{
    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                action(m_grid[index(i,j,k)], m_poreSizes[index(i,j,k)], i, j, k);
            }
        }
    }
}

void Grid::writeCSV(string filename)
{
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    // column-major ordering...
    file << "x coord, y coord, z coord, scalar" << endl;
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                const real &value = m_grid[index(i,j,k)];
                file << i << "," << j << "," << k << "," << value << endl;
            }
        }
    }

    file.close();
}

std::vector<short> &Grid::poreSizes()
{
    return m_poreSizes;
}

std::vector<real> &Grid::grid()
{
    return m_grid;
}

void Grid::writeVTK(string filename)
{
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    int numVoxels = m_nx*m_ny*m_nz;

    file << "# vtk DataFile Version 2.0" << endl;
    file << "structured point" << endl;
    file << "ASCII" << endl;
    file << endl;
    file << "DATASET STRUCTURED_POINTS" << endl;
    file << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << endl;
    file << "ORIGIN 0.0 0.0 0.0" << endl;
    file << "SPACING " << 1.0/double(m_nx) << " " << 1.0/double(m_ny) << " " << 1.0/double(m_nz) << endl;
    file << "POINT_DATA " << numVoxels << endl;
    file << "SCALARS atomdist double" << endl;
    file << "LOOKUP_TABLE default" << endl;
    file << endl;

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                const real &value = m_grid[index(i,j,k)];
                file << value << endl;
            }
        }
    }

    file.close();
}
