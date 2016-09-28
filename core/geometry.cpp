#include "geometry.h"
#include <iostream>
using namespace std;

namespace Geometry {
    Grid voidGrid(int nx, int ny, int nz, int numProperties)
    {
        Grid grid(nx, ny, nz, numProperties);
        grid.iterate([&](Cell &cell) {
            cell.setType(0);
        });

        return grid;
    }

    Grid cylinderGrid(int nx, int ny, int nz, int numProperties, float radius)
    {
        Grid grid(nx, ny, nz, numProperties);
        grid.iterate([&](Cell &cell, int i, int j, int) {
            float x = (i - 0.5*grid.nx()) / float(grid.nx());
            float y = (j - 0.5*grid.ny()) / float(grid.ny());
            if(x*x + y*y > radius*radius) {
                cell.setType(1);
            } else {
                cell.setType(0);
            }
        });

        return grid;
    }

    Grid boxGrid(int nx, int ny, int nz, int numProperties)
    {
        Grid grid(nx, ny, nz, numProperties);
        grid.iterate([&](Cell &cell, int i, int j, int k) {
            if(i==0 || j == 0 || k == 0) {
                cell.setType(1);
            } else {
                cell.setType(0);
            }
        });

        return grid;
    }

    shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2) {
        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz, numProperties) );
        Grid &grid = *gridPtr;
        grid.iterate([&](Cell &cell, int i, int , int ) {
            cell.setType(0);
            real factor = float(i) / (grid.nx()-1);
            real value = factor * value2 + (1 - factor) * value1;
            cell[CONCENTRATION] = value;
            cell.setPoreSize(poreSize);
        });

        return gridPtr;
    }

    shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2) {
        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz, numProperties) );
        Grid &grid = *gridPtr;
        grid.iterate([&](Cell &cell, int i, int , int ) {
            cell.setType(0);
            if(i==0) {
                cell[CONCENTRATION] = value1;
            } else {
                cell[CONCENTRATION] = value2;
            }
            cell.setPoreSize(poreSize);
        });

        return gridPtr;
    }

}
