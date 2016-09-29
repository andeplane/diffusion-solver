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
            value = fugacity(value, cellPoreSize);
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

}
