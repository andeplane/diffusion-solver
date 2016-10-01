#include "geometry.h"
#include "common.h"

#include <iostream>
using namespace std;

namespace Geometry {
    shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int poreSize, real value1, real value2) {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int , int ) {
            real factor = float(i) / (grid.nx()-1);
            value = factor * value2 + (1 - factor) * value1;
            cellPoreSize = poreSize;
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

    shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int poreSize, real value1, real value2) {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int , int ) {
            if(i==0) {
                value = value1;
            } else {
                value = value2;
            }
            cellPoreSize = poreSize;
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

    shared_ptr<Grid> cubeGridX(int nx, int ny, int nz, int poreSize1, int poreSize2, real value1, real value2)
    {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;

        vector<bool> wallsX(grid.nx(),false);
        vector<bool> wallsY(grid.nx(),false);
        vector<bool> wallsZ(grid.nx(),false);

        for(int i=0; i<grid.nx(); i++) {
            double rnd = rand() / double(RAND_MAX);
            wallsX[i] = rnd < 0.1;
            wallsY[i] = rnd < 0.1;
            wallsZ[i] = rnd < 0.1;
        }

        grid.iterate([&](real &value, short &cellPoreSize, int i, int j, int k) {
            real factor = float(i) / (grid.nx()-1);
            value = factor * value2 + (1 - factor) * value1;
            // value = 0;

            if(wallsX[i] || wallsY[j] || wallsZ[k]) {
                cellPoreSize = poreSize1;
            } else {
                cellPoreSize = rand() % 18 + 1;
            }
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

}
