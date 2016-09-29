#include "geometry.h"
#include <iostream>
using namespace std;

namespace Geometry {
    shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2) {
        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz, numProperties) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int j, int k) {
            real factor = float(i) / (grid.nx()-1);
            value = factor * value2 + (1 - factor) * value1;
            cellPoreSize = poreSize;
        });

        return gridPtr;
    }

    shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int numProperties, int poreSize, real value1, real value2) {
        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz, numProperties) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int j, int k) {
            if(i==0) {
                value = value1;
            } else {
                value = value2;
            }
            cellPoreSize = poreSize;
        });

        return gridPtr;
    }

}
