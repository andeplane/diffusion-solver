#include "grid.h"
#include <iostream>
#include <fstream>
using namespace std;

Grid::Grid(int nx, int ny, int nz) :
    m_grid(nx*ny*nz, 0), m_poreSizes(nx*ny*nz, 0), m_isWall(nx*ny*nz, false),
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
                file << i << "," << j << "," << k << "," << value << "\n";
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

void Grid::writeConcentrationVTK(string filename)
{
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    int numVoxels = m_nx*m_ny*m_nz;

    file << "# vtk DataFile Version 2.0\n";
    file << "structured point\n";
    file << "ASCII\n\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << "\n";
    file << "ORIGIN 0.0 0.0 0.0\n";
    file << "SPACING " << 1.0/double(m_nx) << " " << 1.0/double(m_ny) << " " << 1.0/double(m_nz) << "\n";
    file << "POINT_DATA " << numVoxels << "\n";
    file << "SCALARS concentration double" << "\n";
    file << "LOOKUP_TABLE default" << "\n\n";

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                real value = m_grid[index(i,j,k)];
                if(value < 1e-10) value = 0;

                file << value << "\n";
            }
        }
    }

    file.close();
}

void Grid::writeFugacityVTK(string filename)
{
    createTables();
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    int numVoxels = m_nx*m_ny*m_nz;

    file << "# vtk DataFile Version 2.0\n";
    file << "structured point\n";
    file << "ASCII\n\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << "\n";
    file << "ORIGIN 0.0 0.0 0.0\n";
    file << "SPACING " << 1.0/double(m_nx) << " " << 1.0/double(m_ny) << " " << 1.0/double(m_nz) << "\n";
    file << "POINT_DATA " << numVoxels << "\n";
    file << "SCALARS fugacity double" << "\n";
    file << "LOOKUP_TABLE default" << "\n\n";

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                real value = fugacity(m_grid[index(i,j,k)], m_poreSizes[index(i,j,k)]);
                if(value < 1e-10) value = 0;

                file << value << "\n";
            }
        }
    }

    file.close();
}

void Grid::writeGeometryVTK(string filename)
{
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    int numVoxels = m_nx*m_ny*m_nz;

    file << "# vtk DataFile Version 2.0\n";
    file << "structured point\n";
    file << "ASCII\n\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << "\n";
    file << "ORIGIN 0.0 0.0 0.0\n";
    file << "SPACING " << 1.0/double(m_nx) << " " << 1.0/double(m_ny) << " " << 1.0/double(m_nz) << "\n";
    file << "POINT_DATA " << numVoxels << "\n";
    file << "SCALARS geometry double" << "\n";
    file << "LOOKUP_TABLE default" << "\n\n";

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                file << m_isWall[index(i,j,k)] << "\n";
            }
        }
    }

    file.close();
}

void Grid::writePoresVTK(string filename)
{
    ofstream file(filename.c_str(), ios::out | ios::binary);

    if(!file.is_open()) {
        cerr << "Grid::writeVTK(string filename) error, could not open file " << filename <<  ", aborting!" << endl;
        exit(1);
    }

    int numVoxels = m_nx*m_ny*m_nz;

    file << "# vtk DataFile Version 2.0\n";
    file << "structured point\n";
    file << "ASCII\n\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << "\n";
    file << "ORIGIN 0.0 0.0 0.0\n";
    file << "SPACING " << 1.0/double(m_nx) << " " << 1.0/double(m_ny) << " " << 1.0/double(m_nz) << "\n";
    file << "POINT_DATA " << numVoxels << "\n";
    file << "SCALARS poreSizes double" << "\n";
    file << "LOOKUP_TABLE default" << "\n\n";

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                file << m_poreSizes[index(i,j,k)] << "\n";
            }
        }
    }

    file.close();
}
