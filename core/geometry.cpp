#include "geometry.h"

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
}
