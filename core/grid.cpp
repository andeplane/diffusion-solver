#include "grid.h"

using namespace std;
Grid::Grid(int nx, int ny, int nz, int numProperties) :
    m_cells(nx*ny*nz, Cell(numProperties)),
    m_nx(nx), m_ny(ny), m_nz(nz)
{

}

vector<Cell> &Grid::cells()
{
    return m_cells;
}

void Grid::iterate(function<void (Cell &cell)> action)
{
    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                Cell &cell = m_cells[index(i,j,k)];
                action(cell);
            }
        }
    }
}

void Grid::iterate(function<void (Cell &cell, int i, int j, int k)> action)
{
    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                Cell &cell = m_cells[index(i,j,k)];
                action(cell, i, j, k);
            }
        }
    }
}
