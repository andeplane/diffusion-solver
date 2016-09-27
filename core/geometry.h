#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "grid.h"

namespace Geometry {
    Grid voidGrid(int nx, int ny, int nz, int numProperties);
    Grid cylinderGrid(int nx, int ny, int nz, int numProperties, float radius);
    Grid boxGrid(int nx, int ny, int nz, int numProperties);
    Grid linearGridX(int nx, int ny, int nz, int numProperties, real value1, real value2);
}

#endif // GEOMETRY_H
