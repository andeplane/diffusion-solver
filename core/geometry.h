#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "grid.h"

namespace Geometry {
    Grid voidGrid(int nx, int ny, int nz, int numProperties);
    Grid cylinderGrid(int nx, int ny, int nz, int numProperties, float radius);
    Grid boxGrid(int nx, int ny, int nz, int numProperties);
}

#endif // GEOMETRY_H
